
#include <unordered_set>
#include <set>
#include <map>
#include <search.h>

#include "FirstStep.hpp"
#include "../../../../Utils/Source/ThreadPool.hpp"
#include "../../../../Database/Source/Indexes/KmerUtils.hpp"
#include "HitsFinder.hpp"
#include <algorithm>

namespace tachyon {

namespace {

using MatchP = std::vector<std::shared_ptr<utils::FastaFileElem>>;
using MatchN = std::array<MatchP, 6>;

void addIds(const std::vector<HitsP>& ids, std::unordered_set<long>& neededIds) {
    for(auto& it : ids) neededIds.insert(it.begin(), it.end());
}

void addIds(const std::vector<HitsN> ids, std::unordered_set<long>& neededIds) {
    for (const auto& it : ids) {
        for (const auto& it1 : it) {
            neededIds.insert(it1.begin(), it1.end());
        }
    }
}


std::variant<std::vector<MatchP>, std::vector<MatchN>> createMatches(const std::vector<HitsP>& ids, const std::unordered_map<long, std::shared_ptr<utils::FastaFileElem>>& db) {

    std::vector<MatchP> dst;
    dst.reserve(ids.size());

  /*  dst = utils::ThreadPool::run<MatchP>(8, ids, [&](const auto& targets) {
        MatchP dst1;
        dst1.reserve(targets.size());
        for (const auto& id : targets) dst1.emplace_back(db.find(id)->second);

        return dst1;
    }); */

    for (const auto& targets : ids) {
        dst.emplace_back();
        dst.back().reserve(targets.size());
        for (const auto& id : targets) dst.back().emplace_back(db.find(id)->second);
    }

    return dst;
}

std::variant<std::vector<MatchP>, std::vector<MatchN>> createMatches(const std::vector<HitsN>& ids, const std::unordered_map<long, std::shared_ptr<utils::FastaFileElem>>& db) {
    std::vector<MatchN> dst;
    dst.reserve(ids.size());

    for (const auto& targets : ids) {
        auto& v = dst.emplace_back();
        for (int i = 0; i < 6; ++i) {
            v[i].reserve(targets[i].size());
            for (const auto& id : targets[i]) v[i].emplace_back(db.find(id)->second);
        }
    }

    return dst;
}

std::variant<std::vector<MatchP>, std::vector<MatchN>> readMatchesForEachQuery(const DatabaseHits& hits, const Database& database) {

    std::unordered_set<long> neededIds;

    std::visit([&](const auto& it) { return addIds(it, neededIds);  }, hits);

//    std::vector<long> idsVector;
//    idsVector.reserve(neededIds.size());
//    for (const auto& it :neededIds) idsVector.push_back(it);
//    std::sort(idsVector.begin(), idsVector.end());

//    printf("Added %ld\n", neededIds.size());

    std::vector<std::shared_ptr<utils::FastaFileElem>> elements(database.getSequences(neededIds));

    std::unordered_map<long, std::shared_ptr<utils::FastaFileElem>> localDatabase;
    for (const auto& elem : elements) localDatabase.emplace(elem->id_, elem);
    auto matches(std::visit([&](const auto& it){return createMatches(it, localDatabase); }, hits));

    return matches;
}

FirstStepResult::Result createEntries(std::vector<MatchP>&& matches, TachyonQuery&& query) {
    std::vector<FirstStepResultEntry> entries;
    entries.reserve(matches.size());

    auto& qs(std::get<std::vector<ProteineQuery>>(query.queries));

    for (int i = 0; i < (int) matches.size(); ++i) {
        entries.emplace_back(qs[i], std::move(matches[i]));
    }

    return entries;
}


FirstStepResult::Result createEntries(std::vector<MatchN>&& matches, TachyonQuery&& query) {

    std::vector<FirstStepResult::NucleotideEntry> entries;
    entries.reserve(matches.size());

    auto& qs(std::get<std::vector<NucleotideQuery>>(query.queries));

    for (int i = 0; i < (int) matches.size(); ++i) {
        std::array<FirstStepResultEntry, 6> es;
        for (int j = 0; j < 6; ++j) es[j] = { qs[i].translated[j], std::move(matches[i][j]) };
        entries.emplace_back(qs[i].query_, std::move(es));
    }

    return entries;

}
}

FirstStepResult FirstStep::getPotentialMatches(const Database& database, TachyonQuery&& query, const FirstStepOptions& options) {

    utils::Timer timer;
    DatabaseHits matchesIds(HitsFinder::findInDatabase(database, std::move(query), options));
//    printf("\tFind in database %f\n", timer.end());
    timer.start();
    std::variant<std::vector<MatchP>, std::vector<MatchN>> matches(readMatchesForEachQuery(matchesIds, database));
//    printf("\tRead matches %f\n", timer.end());
    return { std::visit([&](auto&& m) { return createEntries(std::move(m), std::move(query)); }, matches) };
}

}
