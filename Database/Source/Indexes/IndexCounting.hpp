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
    }

    unsigned int getValue(unsigned long long key) const {
        if (const auto p = this->map_.find(key); p != this->map_.end()) {
            return p->second;
        }

        return 0;
    }

    void addToKey(unsigned long long key, unsigned int value) { map_[key] += value; }
    void increment(unsigned long long key) { addToKey(key, 1); }

    void addTo(CounterStruct& acc) const {
        for (const auto& it : map_) {
            if (it.second != 0) {
                acc.addToKey(it.first, it.second);
            }
        }
    }

    int cn = 0;

    std::unordered_map<unsigned long long, unsigned int> map_;
};


struct IndexCounting {

    static CounterStruct countIteratorIndexes(const char* path, const IndexingOptions& options);

    static CounterStruct buildFromFile(const char* path);
};
}

