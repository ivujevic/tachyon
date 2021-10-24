#pragma once

#include <vector>

#include "../../TachyonResultEntry.hpp"
#include "../First/FirstStepResultEntry.hpp"
#include "../First/FirstStepResult.hpp"
#include "../../TachyonResultJNI.hpp"

namespace tachyon {

struct SSearchOptions {
    int gapOpen = 10;
    int gapExtend = 1;
    double maxEvalue = 0.0001;
    unsigned long databaseSize;
};

struct SecondStepOptions {
    SSearchOptions ssearch_;
    int numberOfThreads = 8;
};

struct SecondStep {

    static TachyonResult::Result process(FirstStepResult&& matches, const SecondStepOptions& options);
};
}


