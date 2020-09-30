#pragma once
#include <array>
#include "FastaFileUtils.hpp"

namespace utils {
class DNATranslator {
public:
    static std::array<std::shared_ptr<utils::FastaFileElem>, 6> translate(const std::shared_ptr<utils::FastaFileElem>& elem);
};
}


