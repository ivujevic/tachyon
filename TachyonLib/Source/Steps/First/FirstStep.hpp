#pragma once

#include "FirstStepResult.hpp"
#include "../../../../Database/Source/Database.hpp"
#include "FirstStepOptions.hpp"

namespace tachyon {

struct FirstStep {
    static FirstStepResult getPotentialMatches(const Database& database, TachyonQuery&& query, const FirstStepOptions& options = {});
};
}

