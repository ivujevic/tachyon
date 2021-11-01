#pragma once

namespace tachyon {
struct FirstStepOptions {
    const int kmerLen = 12;
    int highFreqCutOff = 3;
    int lowFreqCutOff = 2;
    int numberOfThreads = 8;
};
}
