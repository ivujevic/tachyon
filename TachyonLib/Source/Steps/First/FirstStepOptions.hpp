#pragma once

namespace tachyon {
struct FirstStepOptions {
    const int kmerLen = 5;
    const int highFreqCutOff = 3;
    const int lowFreqCutOff = 2;
    int numberOfThreads = 8;
};
}
