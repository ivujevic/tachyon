#pragma once

#include <vector>

#include "TachyonResultEntry.hpp"

namespace tachyon {

struct TachyonResult {

    using ProteineResult = std::vector<TachyonResultEntry>;

    struct NucleotideResult {
        std::shared_ptr<utils::FastaFileElem> nucleotideQuery_;
        std::array<TachyonResultEntry, 6> entries_;
    };

    using Result = std::variant<ProteineResult, std::vector<NucleotideResult>>;

    const Result entries_;
};
}
