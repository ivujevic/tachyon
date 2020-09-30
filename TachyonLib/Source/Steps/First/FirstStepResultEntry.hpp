#pragma once

#include <vector>
#include <memory>

#include "../../Query.hpp"

namespace tachyon {

struct FirstStepResultEntry {
    FirstStepResultEntry() {

    }
    FirstStepResultEntry(const std::shared_ptr<utils::FastaFileElem>& query, std::vector<std::shared_ptr<utils::FastaFileElem>> targets)
        : query_(query), targets_(std::move(targets)) {

    }

    std::shared_ptr<utils::FastaFileElem> query_;
    std::vector<std::shared_ptr<utils::FastaFileElem>> targets_;
};

}
