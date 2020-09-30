#pragma once

#include "../../Database/Source/Database.hpp"
#include "TachyonResultJNI.hpp"
#include "Steps/First/FirstStepOptions.hpp"
#include "Steps/Second/SecondStep.hpp"

namespace tachyon {

struct TachyonSearchOptions {
    FirstStepOptions firstStepOptions;
    SecondStepOptions secondStepOptions;
};

struct Tachyon {
    const Database database_;
    TachyonResult search(TachyonQuery&& query, int numberOfThreads);
};

}

