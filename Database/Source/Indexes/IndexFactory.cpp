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
        bool isHighFreq, std::vector<KmerProperty>& indexes) {

    std::vector<KmerProperty> kmers(KmerUtils::findCodes(region, kmerLen));

    struct KmerInRegion {

        KmerProperty property_;
        unsigned long counter_;
    };

    std::vector<KmerInRegion> sortedKmers;

    for (const auto& it : kmers) {

        if (auto cn = counters.getValue(it.hash)) {
            sortedKmers.emplace_back(KmerInRegion{KmerProperty{it.hash, it.position_ + regionStart}, cn});
        }
    }

    auto sortFunction = [isHighFreq](const KmerInRegion& a, const KmerInRegion& b) {
        return isHighFreq ? a.counter_ > b.counter_ : a.counter_ < b.counter_;
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


    auto find = [&](bool isHigh, int numberOfKmers, int regionSize, int overlapSize) {
        std::vector<std::vector<KmerProperty>> indexes;
        indexes.reserve(sequence.size() / regionSize + 1);
        int stepSize = regionSize - overlapSize;

        for (int i = 0; i < (int) sequence.size(); i += stepSize) {
            std::string_view region(sequence.substr(i,regionSize));
            if (i != 0 && (int) region.size() < 0.5 * overlapSize) break;
            indexes.emplace_back();
            findInRegion(region, cs, options.kmerLen, i,numberOfKmers, isHigh, indexes.back());
        }

        return indexes;
    };

    int regionId = 0;
    for (const auto& its : find(true, options.highFreq.number, options.highFreq.regionSize, options.highFreq.overlapSize)) {
        for (const auto& it : its) {
            data.map_[it.hash].emplace_back(IndexProperty{id, regionId, it.position_, true});
        }
        ++regionId;
    }


    regionId = 0;
    for (const auto& its : find(false, options.lowFreq.number, options.lowFreq.regionSize, options.lowFreq.overlapSize)) {
        for (const auto& it : its) {
            data.map_[it.hash].emplace_back(IndexProperty{id, regionId, it.position_, false});
        }
        ++regionId;
    }
}

static std::optional<Indexes> buildIteratorIndexes(const char* path, const IndexingOptions& options) {

    using QueueType = std::shared_ptr<utils::FastaFileElem>;

    utils::Timer timer;

    auto globalCounter(IndexCounting::countIteratorIndexes(path, options));

    printf("\nFinished with counting %f\n", timer.end());

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
