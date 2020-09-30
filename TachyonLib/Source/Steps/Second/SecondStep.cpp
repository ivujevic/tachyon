#include "SecondStep.hpp"
#include "../../../../Aligner/Source/Aligner.hpp"
#include "../../TachyonResultJNI.hpp"
#include "../../../../Utils/Source/ThreadPool.hpp"

namespace tachyon {

namespace {

static TachyonResult::Result align(FirstStepResult::ProteineResult&& entries, const SecondStepOptions& options) {

    ScoreMatrix scoreMatrix(options.ssearch_.type_, options.ssearch_.gapOpen, options.ssearch_.gapExtend);
    EValue evalueParams(options.ssearch_.databaseSize, scoreMatrix);

    return utils::ThreadPool::run<TachyonResultEntry>(options.numberOfThreads, entries, [&](const auto& entry) {
        auto alignments = Aligner::align(scoreMatrix, evalueParams, entry.query_, entry.targets_, options.ssearch_.alignmentType_, options.ssearch_.maxEvalue);
        return TachyonResultEntry{entry.query_, std::move(alignments)};
    });
}

static TachyonResult::Result align(FirstStepResult::NucleotideResult&& entries, const SecondStepOptions& options) {

    ScoreMatrix scoreMatrix(options.ssearch_.type_, options.ssearch_.gapOpen, options.ssearch_.gapExtend);
    EValue evalueParams(options.ssearch_.databaseSize, scoreMatrix);

    return utils::ThreadPool::run<TachyonResult::NucleotideResult>(options.numberOfThreads, entries, [&](const auto& entry) {
        std::array<TachyonResultEntry, 6> dst;
        for (int i = 0; i < 6; ++i) {
            const auto& e(entry.entries_[i]);

            auto alignments(Aligner::align(scoreMatrix, evalueParams, e.query_, e.targets_, options.ssearch_.alignmentType_,
                                             options.ssearch_.maxEvalue));

            dst[i] = {e.query_, std::move(alignments)};
        }

        return TachyonResult::NucleotideResult{entry.nucleotideQuery_, std::move(dst)};
    });
}

}


TachyonResult::Result SecondStep::process(FirstStepResult&& matches, const SecondStepOptions& options) {
    return std::visit([&] (auto&& it){ return align(std::move(it), options); }, matches.entries_);
}
}
