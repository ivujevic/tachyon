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



void printTabFormat(FILE* output_file_, const ProteineQuery& query, const std::vector<Alignment>& alignments) {

    std::string_view queryName = std::string_view(query->name_).substr(0, query->name_.find_first_of(' '));

    for (const auto& alignment : alignments) {
        int mismatches = 0;
        int gap_openings = 0;
        int matches = 0;
        bool gap = false;

        const auto& alignment_str = alignment.alignment_;
        const auto alignment_len = alignment.alignLen_;

        for (uint32_t j = 0; j < alignment_len; ++j) {
            int alignment_res = alignment_str[j];
            // printf("Alignment res %d\n", alignment_res);
            // return;

            switch (alignment_res) {
            case kMismatch:
                mismatches++;
                gap = false;
                break;
            case kInsertion:
                if (!gap) {
                    gap = true;
                    gap_openings++;
                }
                break;
            case kDeletion:
                if (!gap) {
                    gap = true;
                    gap_openings++;
                }
                break;
            default:
                matches++;
                gap = false;
            }
        }

        double perc_id = (100.f * matches) / alignment_len;

        std::string_view targetName = std::string_view(alignment.target->name_).substr(0, alignment.target->name_.find_first_of(' '));

        fprintf(output_file_, "%.*s\t%.*s\t", (int) queryName.size(), queryName.data(), (int) targetName.size(), targetName.data());
        fprintf(output_file_, "%.2f\t%d\t%d\t", perc_id, alignment_len, mismatches);
        fprintf(output_file_, "%d\t%d\t", gap_openings, alignment.qBegin_ + 1);
        fprintf(output_file_, "%d\t%d\t", alignment.qEnd_ + 1, alignment.qBegin_+ 1);
        fprintf(output_file_, "%d\t", alignment.qEnd_ + 1);

        auto eval = alignment.eValue_;
        auto score = alignment.score_;

        if (eval >= 1e-2 && eval < 100) {
            fprintf(output_file_, "%.2lf\t", eval);
        } else {
            fprintf(output_file_, "%.2e\t", eval);
        }

        fprintf(output_file_, "%d", score);
        fprintf(output_file_, "\n");
    }
}
void Writer::writeBM8(const TachyonResultEntry& result) {

//    if (results.size() != 0) return;

        printTabFormat(file_.get(), result.query_, result.alignments_);
}

void Writer::writeBM9(const TachyonResultEntry& results) {

    fprintf(file_.get(), "# Fields:\n");
    fprintf(file_.get(),
            "Query id,Subject id,%% identity,alignment length,mismatches,"
            "gap openings,q. start,q. end,s. start,s. end,e-value,score\n");

    writeBM8(results);
}

}
