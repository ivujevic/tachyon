#pragma once

#include <unordered_map>
#include <vector>
#include <map>

#include "NonCopyable.hpp"

namespace tachyon {

struct IndexProperty {
    unsigned int id;
    int region;
    int position;
    bool isHigh;
};

using IndexHashType = unsigned long;
using IndexStruct = std::unordered_map<IndexHashType, std::vector<IndexProperty>>;

struct Indexes : public NonCopyable {

    using NonCopyable::NonCopyable;
    Indexes(IndexStruct map) : map_(std::move(map)) {

    }

    Indexes(Indexes&& o) {
        map_ = std::move(o.map_);
    }

    IndexStruct map_;
};

}
