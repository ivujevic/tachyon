#include "SecondStep.hpp"
#include "../../TachyonResultJNI.hpp"
#include "../../../../Utils/Source/ThreadPool.hpp"
#include <algorithm>

namespace tachyon {

namespace {

static std::vector<Alignment> align(const std::shared_ptr<utils::FastaFileElem>& query,
        const std::vector<std::shared_ptr<utils::FastaFileElem>>& targets) {

    std::vector<Alignment> results;
    for (const auto& it : targets) {
        if (query->sequence_ == it->sequence_) {
            results.push_back({it, it->sequence_.length(), 1 });
        }
    }

    std::sort(results.begin(), results.end(),
              [](const Alignment& a, const Alignment& b) { return a.target->name_ < b.target->name_; });
    return results;
}

static TachyonResult::Result align(FirstStepResult::ProteineResult&& entries, const SecondStepOptions& options) {

    return utils::ThreadPool::run<TachyonResultEntry>(options.numberOfThreads, entries, [](const auto& entry) {
        auto alignments = align(entry.query_, entry.targets_);
        return TachyonResultEntry{entry.query_, std::move(alignments)};
    });
}

static TachyonResult::Result align(FirstStepResult::NucleotideResult&& entries, const SecondStepOptions& options) {
    return {};
}

}


TachyonResult::Result SecondStep::process(FirstStepResult&& matches, const SecondStepOptions& options) {
    return std::visit([&] (auto&& it){ return align(std::move(it), options); }, matches.entries_);
}
}
