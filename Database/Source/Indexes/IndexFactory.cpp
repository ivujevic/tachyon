#include "IndexFactory.hpp"

#include "../../../Utils/Source/FastaFileUtils.hpp"
#include "../../../Utils/Source/Logger.hpp"
#include "../../../Utils/Source/Seg.hpp"
#include "../../../Utils/Source/ThreadPool.hpp"
#include "../../../Utils/Source/Timer.hpp"
#include "../../../Utils/Source/BinaryStream.hpp"

#include "KmerUtils.hpp"

#include "IndexCounting.hpp"

#include <deque>
#include <queue>

#include <thread>
#include <memory>
#include <algorithm>

namespace tachyon {

namespace {

struct IndexData {

    IndexData() = default;
    IndexData(IndexData& o) = delete;

    IndexData(IndexData&& o) {
        map_ = std::move(o.map_);
    }

    IndexStruct map_;
};


static void findInRegion(const std::string_view& region, const CounterStruct& counters, int kmerLen, int regionStart, int numberInRegion,
        bool isHighFreq, std::vector<KmerProperty>& indexes, int numberOfSequences) {

    std::vector<KmerProperty> kmers(KmerUtils::findCodes(region, kmerLen));

    std::unordered_map<KmerHashType, int> localKmerCounter;

    for (const auto& it : kmers) {
        if (!localKmerCounter.count(it.hash)) {
            localKmerCounter.insert({it.hash, 1});
            continue;
        }
        localKmerCounter.insert({it.hash, localKmerCounter[it.hash] + 1});
    }

    struct KmerInRegion {

        KmerProperty property_;
        double tfIdf;
    };

    std::vector<KmerInRegion> sortedKmers;

    const int numberOfWords = region.size() - (kmerLen - 1);

    for (const auto& it : kmers) {

        if (auto cn = counters.getValue(it.hash)) {

            const double tf = ((float) localKmerCounter[it.hash]) / numberOfWords;
            const double idf = std::log( ((float) numberOfSequences) / cn);

            sortedKmers.emplace_back(KmerInRegion{KmerProperty{it.hash, it.position_ + regionStart}, tf * idf});
        }
    }

    auto sortFunction = [isHighFreq](const KmerInRegion& a, const KmerInRegion& b) {
        return isHighFreq ? a.tfIdf > b.tfIdf : a.tfIdf < b.tfIdf;
    };

    std::stable_sort(sortedKmers.begin(), sortedKmers.end(), sortFunction);

    std::vector<KmerProperty> added;
    added.reserve(numberInRegion);

    for (const auto& it : sortedKmers) {
        if ((int) added.size() == numberInRegion) break;
        bool isValid = true;
        for (const auto& it1 : added) {
            if (std::abs(it.property_.position_ - it1.position_) < (int) kmerLen) {
                isValid = false;
                break;
            }
        }
        if (isValid) added.push_back(it.property_);
    }

    if (!added.empty()) {
        int diff = numberInRegion - (int) added.size();
        while(diff--) added.push_back(added.front());
    }

    indexes.insert(indexes.end(), added.begin(), added.end());
}

void findIndexes(IndexData& data, const CounterStruct& cs, const std::shared_ptr<utils::FastaFileElem>& elem, const IndexingOptions& options) {

    const std::string_view sequence = std::string_view(elem->sequence_);
    auto id = elem->id_;

    if (id % 10000 == 0) {
        printf("\rIndexing status: %u", id);
        fflush(stdout);

    }

    std::vector<KmerProperty> indexes;

    findInRegion(sequence, cs, options.kmerLen, 0, options.highFreq.number, true, indexes, cs.cn);

    for (const auto& it : indexes) {
        data.map_[it.hash].emplace_back(IndexProperty{id, 0, it.position_, true});
    }
}

static std::optional<Indexes> buildIteratorIndexes(const char* path, const IndexingOptions& options) {

    using QueueType = std::shared_ptr<utils::FastaFileElem>;

    utils::Timer timer;

    auto globalCounter(IndexCounting::countIteratorIndexes(path, options));

    printf("\nFinished with counting %f\n", timer.end());
    printf("\nNumber of sequences %d\n", globalCounter.cn);

    timer.start();

    auto indexStructAccumulate = [](IndexData& acc, const IndexData& data) {
        for (const auto& it : data.map_) {
            auto& v(acc.map_[it.first]);
            v.insert(v.end(), it.second.begin(), it.second.end());
        }
    };

    auto data(utils::ProducerConsumersPool<QueueType>::run<IndexData>(options.numberOfThreads, utils::FastaIterator(path),
            [&] (auto& data, const auto& t) { return findIndexes(data, globalCounter, t, options);},
            indexStructAccumulate));


    printf("\nFinished with indexes %f\n", timer.end());

    return {Indexes(std::move(data.map_))};
}

}

std::optional<Indexes> IndexFactory::buildIndexes(const char* path, const IndexingOptions& options) {

    auto indexes(buildIteratorIndexes(path, options));

    return indexes;
}

}
