#pragma once


#include "Indexes/Indexes.hpp"
#include "Indexes/IndexingOptions.hpp"
#include "../../Utils/Source/BinaryStream.hpp"
#include "../../Utils/Source/FastaFileUtils.hpp"

#include <unordered_set>
namespace tachyon {

struct Database {

    Database(Indexes indexes, unsigned long databaseSize, std::vector<unsigned long> offsets, utils::BinaryInStream bs)
    : indexes_(std::move(indexes)), databaseSize_(databaseSize), offsets_(std::move(offsets)), bs_(std::move(bs)) {

    }
    static std::optional<Indexes> makeIndexes(const char* path, const char* outPath, const IndexingOptions& options);

    static Database loadDatabase(const char* path);
    static void fixDatabase(const char* path, const char* outPath, const char* fasta);

    std::vector<std::shared_ptr<utils::FastaFileElem>> getSequences(const std::unordered_set<long>& ids) const;

    const Indexes indexes_;
    unsigned long databaseSize_;
private:
    const std::vector<unsigned long> offsets_;
    mutable utils::BinaryInStream bs_;
};

}

