#pragma once

#include "BlosumTables.hpp"

namespace tachyon {
struct ScoreMatrix {

    ScoreMatrix(BlosumType type, int gapOpen, int gapExtend) : type_(type), gapOpen_(gapOpen), gapExtend_(gapExtend) {

    }

    const int* getMatrix() const {
        switch (type_) {
            case BlosumType::BLOSUM_50:
                return BlosumTables::blosum50;
        case BlosumType::BLOSUM_62:
                return BlosumTables::blosum62;
        }

        return BlosumTables::blosum62;
    }

    const BlosumType type_;
    const int gapOpen_;
    const int gapExtend_;

    static constexpr int alphabetLength_ = 24;
    const unsigned char alphabet[alphabetLength_] = {'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y',
                                            'V', 'B', 'Z', 'X', '*'};
};
}
