#pragma once


#include "ScoreMatrix.hpp"
#include "EValue.hpp"
#include "../../TachyonLib/Source/TachyonResultEntry.hpp"
#include "Alignment.hpp"

namespace tachyon {

    struct Aligner {

        static std::vector<Alignment> align(const ScoreMatrix& matrix, const EValue& evalueParam, const std::shared_ptr<utils::FastaFileElem>& query,
                const std::vector<std::shared_ptr<utils::FastaFileElem>>& targets, const AlignmentType& type, float maxEvalue);
    };
}

