#pragma once

#include <unordered_set>
#include <vector>
#include <variant>
#include "../../../../Database/Source/Database.hpp"
#include "FirstStepOptions.hpp"
#include "../../Query.hpp"

namespace tachyon {

using HitsP = std::unordered_set<long>;
using HitsN = std::array<HitsP, 6>;

using DatabaseHits = std::variant<std::vector<HitsP>, std::vector<HitsN>>;

struct HitsFinder {
    static DatabaseHits findInDatabase(const Database& database, TachyonQuery&& query, const FirstStepOptions& options);
};
}

