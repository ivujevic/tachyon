#pragma once

#include "../../Utils/Source/FastaFileUtils.hpp"

#include <memory>

namespace tachyon {

enum class AlignmentType {
    NW,
    HW,
    OV,
    SW
};

struct Alignment {

    Alignment(int score, const std::shared_ptr<utils::FastaFileElem>& target, unsigned int qBegin,
            unsigned int qEnd, unsigned int tBegin, unsigned int tEnd, unsigned int alignLen, unsigned char* alignment, double eValue)
                : score_(score), target(target), qBegin_(qBegin), qEnd_(qEnd),
                tBegin_(tBegin), tEnd_(tEnd), alignLen_(alignLen), alignment_(reinterpret_cast<const char*>(std::move(alignment)), alignLen),
                eValue_(eValue) {}

    int score_;
    std::shared_ptr<utils::FastaFileElem> target;

    unsigned int qBegin_;
    unsigned int qEnd_;

    unsigned int tBegin_;
    unsigned int tEnd_;

    unsigned int alignLen_;

    std::string alignment_;

    double eValue_;
};

}
