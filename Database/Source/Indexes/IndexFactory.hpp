#pragma once

#include "Indexes.hpp"
#include "IndexingOptions.hpp"

namespace tachyon {
struct IndexFactory {

    static std::optional<Indexes> buildIndexes(const char* inputPath, const IndexingOptions& options);
};
}


