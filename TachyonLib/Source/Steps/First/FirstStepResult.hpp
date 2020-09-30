#pragma once

#include <vector>

#include "FirstStepResultEntry.hpp"

namespace tachyon {

struct FirstStepResult {

    using ProteineResult = std::vector<FirstStepResultEntry>;

    struct NucleotideEntry {
        NucleotideEntry(const std::shared_ptr<utils::FastaFileElem>& query, std::array<FirstStepResultEntry, 6> entries)
            : nucleotideQuery_(query), entries_(std::move(entries)){

        }
        std::shared_ptr<utils::FastaFileElem> nucleotideQuery_;
        std::array<FirstStepResultEntry, 6> entries_;
    };

    using NucleotideResult = std::vector<NucleotideEntry>;

    using Result = std::variant<ProteineResult, NucleotideResult>;

    Result entries_;
};
}
