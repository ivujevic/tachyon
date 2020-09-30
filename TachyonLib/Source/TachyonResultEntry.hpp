#pragma once

#include <memory>
#include <variant>
#include "../../Utils/Source/FastaFileUtils.hpp"
#include "../../Aligner/Source/Alignment.hpp"
#include "Query.hpp"

namespace tachyon {

struct TachyonResultEntry {
    std::shared_ptr<utils::FastaFileElem> query_;
    std::vector<Alignment> alignments_;
};
}
