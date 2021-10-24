#include "Writer.hpp"

namespace tachyon {

constexpr int kDeletion = 1;  // deletion from query (insertion to target)
constexpr int kInsertion = 2;  // insertion to query (deletion from target)
constexpr int kMismatch = 3;  // mismatch

std::string_view getQueryName(const ProteineQuery query) {
    return std::string_view(query->name_).substr(0, query->name_.find_first_of(' '));
}

std::string_view getQueryName(const NucleotideQuery query) {
    return std::string_view(query.query_->name_).substr(0, query.query_->name_.find_first_of(' '));
}

void Writer::write(const TachyonResultEntry& results) {
    fprintf(file_.get(), "# Fields:\n");
    fprintf(file_.get(),
            "Query id,Subject id,%% identity,subjectSeqLen,querySeqLen\n");

    if (results.alignments_.size() == 0) return;

    const auto& query = results.query_;
    std::string_view queryName = getQueryName(query);

    for (const auto& alignment: results.alignments_) {
        const auto targetName = getQueryName(alignment.target);
        fprintf(file_.get(), "%.*s\t%.*s\t", (int) queryName.size(), queryName.data(), (int) targetName.size(), targetName.data());
        fprintf(file_.get(), "%.2f\t%lu\t%lu\n", alignment.percIdentical, query->sequence_.length(), alignment.length);
    }
}
}
