#pragma once

#include <memory>
#include <variant>
#include "../../Utils/Source/FastaFileUtils.hpp"
#include "Query.hpp"

namespace tachyon {

struct Alignment {
    std::shared_ptr<utils::FastaFileElem> target;
    unsigned long length;
    double percIdentical;
};

struct TachyonResultEntry {
    std::shared_ptr<utils::FastaFileElem> query_;
    std::vector<Alignment> alignments_;
};
}
