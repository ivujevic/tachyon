#include "IndexCounting.hpp"

#include "../../../Utils/Source/Seg.hpp"
#include "../../../Utils/Source/FastaFileUtils.hpp"
#include "../../../Utils/Source/ThreadPool.hpp"
#include "KmerUtils.hpp"

namespace tachyon {


namespace {

void sequenceCount(CounterStruct& counters, const std::shared_ptr<utils::FastaFileElem>& seq,
        const SegParam& segParam, int kmerLen) {

    Seg seg(segParam.window, segParam.segLowCutOff, segParam.segHighCutOff);

    ++counters.cn;

    if (seq->id_ % 10000 == 0) {
        printf("\rCounting status: %u", seq->id_);
        fflush(stdout);

    }
    std::string sequenceCopy(seq->sequence_);
    seg.mask(sequenceCopy);

    std::unordered_set<long> localStatus;

    for (int i = 0; i < (int) sequenceCopy.size() - (kmerLen - 1); ++i) {
        unsigned long long hash = 0;
        if (!KmerUtils::hashFunction(sequenceCopy.substr(i, kmerLen), hash)) continue;
        if (localStatus.find(hash) != localStatus.end()) continue;

        localStatus.insert(hash);
        counters.increment(hash);
    }
}

}
CounterStruct IndexCounting::countIteratorIndexes(const char *path, const IndexingOptions &options) {

    using QueueType = std::shared_ptr<utils::FastaFileElem>;

    auto accFun = [](CounterStruct& acc, const CounterStruct& s) {
        acc.cn += s.cn;
        s.addTo(acc);
    };


    return utils::ProducerConsumersPool<QueueType>::run<CounterStruct>(options.numberOfThreads, utils::FastaIterator(path),
                                                                                   [&] (auto& data, const auto& t){
                                                                                     return sequenceCount(data, t, options.segParam, options.kmerLen);},

                                                                                   accFun);
}

CounterStruct IndexCounting::buildFromFile(const char* path) {
    CounterStruct dst;

    std::ifstream infile(path);
    std::string s;
    unsigned int cn;
    while (infile >> s >> cn) {

        unsigned long long code = 0;
        KmerUtils::hashFunction(s, code);
        dst.addToKey(code, cn);
    }

    return dst;
}

}
