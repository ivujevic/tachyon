#include "HitsFinder.hpp"
#include "../../../../Database/Source/Indexes/KmerUtils.hpp"
#include "../../../../Utils/Source/Visitor.hpp"
#include "../../../../Utils/Source/ThreadPool.hpp"

#include <algorithm>
namespace tachyon {

namespace {

struct KmerPositions {

    int query;
    int target;
    int region;
};


template<class IT>
static int findNumberOfSameDistances(const IT& fst, const IT& last) {

    std::vector<long> distances;
    distances.reserve(std::distance(fst, last));
    std::transform(fst, last, std::back_inserter(distances), [](const auto& a) { return a.second.query - a.second.target;});

    std::sort(distances.begin(), distances.end());

    int maxNumberOfEq = 0;
    int first = distances[0];
    int numberOfEq = 0;
    for (const auto& distance : distances) {
        if (first == distance) {
            numberOfEq++;
        } else {
            first = distance;
            maxNumberOfEq = std::max(maxNumberOfEq, numberOfEq);
            numberOfEq = 1;
        }
    }

    return std::max(maxNumberOfEq, numberOfEq);
}

template<class IT>
bool isValid(int cutOff, const IT& fst, const IT& end) {

    return std::distance(fst, end) < 7;
}


static HitsP filterHits(int highCutOff, int lowCutOff,
        const std::vector<std::pair<unsigned long, KmerPositions>>& highMatches,
        const std::vector<std::pair<unsigned long, KmerPositions>>& lowMatches) {

    std::vector<std::pair<unsigned long, int>> filteredMatches;
    HitsP hittedIds;

    auto fst = lowMatches.begin();
    while (fst != lowMatches.end()) {
        auto end = std::find_if(fst, lowMatches.end(), [&](const auto& it) { return it.first != fst->first; });

        if (isValid(lowCutOff, fst, end)) {
            hittedIds.insert(fst->first);
        }
        fst = end;
    }

    fst = highMatches.begin();

    while (fst != highMatches.end()) {
        auto end = std::find_if(fst, highMatches.end(), [&](const auto& it) { return it.first != fst->first || it.second.region != fst->second.region; });

        if (filteredMatches.empty() || filteredMatches.back().first != fst->first) {
            if (isValid(highCutOff, fst, end)) {
                filteredMatches.emplace_back(fst->first, std::distance(fst, end));
            }
        }

        fst = end;
    }


    std::sort(filteredMatches.begin(), filteredMatches.end(),  [](const auto& a, const auto& b) { return a.second > b.second; });
    for (auto& p : filteredMatches) {
        hittedIds.insert(p.first);
        if (hittedIds.size() > 10000) break;
    }

    return hittedIds;
}


HitsP getHits(const Database& database, const std::shared_ptr<utils::FastaFileElem>& query, const FirstStepOptions& options) {

    std::vector<KmerProperty> codedKmers(KmerUtils::findCodes(query->sequence_, options.kmerLen));

    std::vector<std::pair<unsigned long, KmerPositions>> highMatches, lowMatches;

    highMatches.reserve(codedKmers.size());
    lowMatches.reserve(codedKmers.size());

    for (const auto& kmer : codedKmers) {
        if (auto m = database.indexes_.map_.find(kmer.hash); m != database.indexes_.map_.end()) {
            for (const auto& p : m->second) {
                if (p.isHigh) {
                    highMatches.emplace_back(p.id, KmerPositions{kmer.position_, p.position, p.region});
                } else {
                    lowMatches.emplace_back(p.id, KmerPositions{kmer.position_, p.position, p.region});
                }
            }
        }
    }

    std::sort(lowMatches.begin(), lowMatches.end(), [](const auto& a, const auto& b) {
        if (a.first == b.first) {
            return a.second.region == b.second.region ? a.second.query < b.second.query : a.second.region < b.second.region;
        }

        return a.first < b.first;
    });

    return filterHits(options.highFreqCutOff, options.lowFreqCutOff, highMatches, lowMatches);
}

HitsN getHits(const Database& database, const NucleotideQuery& query, const FirstStepOptions& options) {

   HitsN retResult;

    for (int i = 0; i < 6; ++i) {
        retResult[i] = getHits(database, query.translated[i], options);
    }

    return retResult;
}

DatabaseHits findInDatabase_(const Database& database, std::vector<ProteineQuery>&& query, const FirstStepOptions& options) {
    return utils::ThreadPool::run<HitsP>(options.numberOfThreads, query, [&](const auto& query) {
        return getHits(database, query, options);
    });
}


DatabaseHits findInDatabase_(const Database& database, const std::vector<NucleotideQuery>& query, const FirstStepOptions& options) {

    return utils::ThreadPool::run<HitsN>(options.numberOfThreads, query, [&](const auto& query) {
        return getHits(database, query, options);
    });
}

}

DatabaseHits HitsFinder::findInDatabase(const Database& database, TachyonQuery&& query, const FirstStepOptions& options) {
    return std::visit([&](auto&& q) { return findInDatabase_(database, std::move(q), options); }, query.queries);
}
}
