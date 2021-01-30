#pragma once

#include <vector>
#include "Indexes.hpp"
#include "IndexingOptions.hpp"

namespace tachyon {

//Temporary for testing
struct CounterStruct : public NonCopyable {

    CounterStruct() = default;

    CounterStruct(CounterStruct&& o) {
        map_ = std::move(o.map_);
        cn = o.cn;
    }

    unsigned int getValue(unsigned int key) const { return map_[key]; }

    void addToKey(unsigned int key, unsigned int value) { map_[key] += value; }
    void increment(unsigned int key) { addToKey(key, 1); }

    void addTo(CounterStruct& acc) const {
        unsigned int i = 0;
        for (const auto& it : map_) {
            if (it != 0) {
                acc.addToKey(i, it);
            }
            i++;
        }
    }

    int cn = 0;
    std::vector<unsigned int> map_ = std::vector<unsigned int>(33554432, 0);
};


struct IndexCounting {

    static CounterStruct countIteratorIndexes(const char* path, const IndexingOptions& options);

    static CounterStruct buildFromFile(const char* path);
};
}

