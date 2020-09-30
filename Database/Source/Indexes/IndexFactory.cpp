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



    auto find = [&](bool isHigh) {
        std::vector<std::vector<KmerProperty>> indexes;
        indexes.reserve(sequence.size() / options.highFreq.regionSize + 1);
        int stepSize = options.highFreq.regionSize - options.highFreq.overlapSize;

        for (int i = 0; i < (int) sequence.size(); i += stepSize) {
            std::string_view region(sequence.substr(i, options.highFreq.regionSize));
            if (i != 0 && (int) region.size() < 0.5 * options.highFreq.overlapSize) break;
            indexes.emplace_back();
            findInRegion(region, cs, options.kmerLen, i, options.highFreq.number, isHigh, indexes.back());
        }

        return indexes;
    };

    int regionId = 0;
    for (const auto& its : find(true)) {
        for (const auto& it : its) {
            data.map_[it.hash].emplace_back(IndexProperty{id, regionId, it.position_, true});
        }
        ++regionId;
    }


    regionId = 0;
    for (const auto& its : find(false)) {
        for (const auto& it : its) {
            data.map_[it.hash].emplace_back(IndexProperty{id, regionId, it.position_, false});
        }
        ++regionId;
    }
}

//std::string convert(unsigned long hash) {
//    std::string seq;
//    for (int i = 0; i < 5; ++i) {
//        char v = (char) (hash & 31);
//        hash >>= 5;
//        char c = v == 27 ? '*' : v + 'A';
//        seq = c + seq;
//    }
//
//    return seq;
//}

//static void writeCn(const CounterStruct& map) {
//
//    std::unique_ptr<FILE, decltype(&fclose)> file_(fopen("/home/ivujevic/NR/counters", "wt"), fclose);
//
//    std::vector<std::pair<unsigned int, unsigned int>> mv;
//    for (int i = 0; i < (int) map.map_.size(); ++i) mv.emplace_back(i, map.map_[i]);
//
////    for (const auto& it : map.map_) mv.push_back(it);
//    std::sort(mv.begin(), mv.end(), [](const auto& p1, const auto& p2) { return p1.first < p2.first; });
//
//    for (const auto& it : mv) {
//        if (it.second == 0) continue;
//        std::string s(convert(it.first));
//        fwrite(s.data(), sizeof(char), s.length(), file_.get());
//        s = " " + std::to_string(it.second) + "\n";
//        fwrite(s.data(), sizeof(char), s.length(), file_.get());
//    }
//}

//static void writeIndexes(const IndexData& in) {
//
//    auto fun = [&](const IndexStruct& s) {
//        std::map<unsigned int, std::vector<std::pair<std::string, unsigned int>>> map;
//        for (const auto&[hash, v] : s) {
//            auto h = convert(hash);
//            for (const auto& it : v) {
//                map[it.id].emplace_back(h, it.position);
//            }
//        }
//        using P1 = std::vector<std::pair<std::string, unsigned int>>;
//        std::vector<std::pair<unsigned int, P1>> pairs;
//        for (auto& it : map) pairs.emplace_back(std::move(it.first), std::move(it.second));
//
//        std::sort(pairs.begin(), pairs.end(), [&](const auto& a, const auto& b) { return a.first < b.first; });
//
//        return pairs;
//    };
//
//    auto h = fun(in.highMap);
//    auto l = fun(in.lowMap);
//    (void) h;
//    (void) l;
//
//    auto w = [&](auto& v, const auto name) {
//        std::unique_ptr<FILE, decltype(&fclose)> fh(fopen(name, "wt"), fclose);
//        for (auto& [id, v] : v) {
//            fprintf(fh.get(), "%u\t", id);
//            std::sort(v.begin(), v.end(), [](auto& p1, auto& p2) { return p1.second < p2.second; });
//
//            for (const auto& it : v) {
//                fprintf(fh.get(), "{%s, %u} ", it.first.c_str(), it.second);
//            }
//            fprintf(fh.get(), "\n");
//        }
//    };
//
//    w(h, "/home/ivujevic/NR/index_high");
//    w(l, "/home/ivujevic/NR/index_low");
//}

static std::optional<Indexes> buildIteratorIndexes(const char* path, const IndexingOptions& options) {

    using QueueType = std::shared_ptr<utils::FastaFileElem>;

    utils::Timer timer;
//    auto globalCounter(IndexCounting::buildFromFile("/home/ivujevic/NR/counters"));

    auto globalCounter(IndexCounting::countIteratorIndexes(path, options));

    printf("\nFinished with counting %f\n", timer.end());
//    writeCn(globalCounter);

    timer.start();

    auto indexStructAccumulate = [](IndexData& acc, const IndexData& data) {
        for (const auto& it : data.map_) {
            auto& v(acc.map_[it.first]);
            v.insert(v.end(), it.second.begin(), it.second.end());
        }
    };

    IndexData data(utils::ProducerConsumersPool<QueueType>::run<IndexData>(options.numberOfThreads, utils::FastaIterator(path),
            [&] (auto& data, const auto& t) { return findIndexes(data, globalCounter, t, options);},
            indexStructAccumulate));


    printf("\nFinished with indexes %f\n", timer.end());

//    writeIndexes(data);
    return {Indexes(std::move(data.map_))};
}

}

std::optional<Indexes> IndexFactory::buildIndexes(const char* path, const IndexingOptions& options) {

    auto indexes(buildIteratorIndexes(path, options));

    return indexes;
}

}
