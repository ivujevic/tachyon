#pragma once

#include <variant>
#include <array>

#include "../../Utils/Source/FastaFileUtils.hpp"

namespace tachyon {

using ProteineQuery = std::shared_ptr<utils::FastaFileElem>;

struct NucleotideQuery {
    std::shared_ptr<utils::FastaFileElem> query_;
    std::array<std::shared_ptr<utils::FastaFileElem>, 6> translated;
};

struct TachyonQuery {
    using Query = std::variant<std::vector<ProteineQuery>, std::vector<NucleotideQuery>>;
    Query queries;
};
}


