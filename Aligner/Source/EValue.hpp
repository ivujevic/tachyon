#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <memory>

#include "ScoreMatrix.hpp"

namespace tachyon {

class EValue {
public:
    double calculate(int32_t score, uint32_t query_length, uint32_t target_length) const;

    EValue(uint64_t database_cells, ScoreMatrix& scorer);

private:
    double lambda_;
    double K_;
    double logK_;
    double H_;
    double a_;
    double C_;
    double alpha_;
    double sigma_;
    double b_;
    double beta_;
    double tau_;
    double G_;
    double aUn_;
    double alphaUn_;
    uint64_t length_;
};
}
