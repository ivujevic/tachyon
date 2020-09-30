#include "Aligner.hpp"
#include "Opal/opal.h"
#include <cmath>
#include <algorithm>

namespace tachyon {

namespace {

unsigned char* charSeqToIndexesSeq(const std::string_view& sequence, const int alphabetLen, const unsigned char* alphabet) {
    unsigned char letterIdx[128];
    for (int i = 0; i < alphabetLen; ++i) {
        if (alphabet[i] == '*') {
            for (int j = 0; j < 128; ++j) letterIdx[j] = i;
            break;
        }
    }

    for (int i = 0; i < alphabetLen; ++i) letterIdx[alphabet[i]] = i;
    unsigned char* ret = new unsigned char[sequence.size()];

    for (int i = 0; i < (int) sequence.size(); ++i) {
        char c = sequence[i];
        if (c != '*') {
            ret[i] = letterIdx[std::toupper(c)];
        } else {
            ret[i] = letterIdx[(int) c];
        }
    }

    return ret;
}

static int convertAlignTypeToOpal(AlignmentType type) {
    switch (type) {
    case AlignmentType::NW:
        return OPAL_MODE_NW;
    case AlignmentType::HW:
        return OPAL_MODE_HW;
    case AlignmentType::OV:
        return OPAL_MODE_OV;
    default:
        return OPAL_MODE_SW;
    }
}

bool compareAlignment(const Alignment& left, const Alignment& right) {

    if (left.eValue_ == right.eValue_) {
        if (left.score_ == right.score_) {
            return left.target->name_ < right.target->name_;
        }
        return left.score_ > right.score_;
    }

    return left.eValue_ < right.eValue_;
}
}
std::vector<Alignment> Aligner::align(const ScoreMatrix& matrix, const EValue& evalueParam, const std::shared_ptr<utils::FastaFileElem>& query,
        const std::vector<std::shared_ptr<utils::FastaFileElem>>& targets, const AlignmentType& type, float maxEvalue) {

    std::unique_ptr<unsigned char> transformedQuery(charSeqToIndexesSeq(query->sequence_, matrix.alphabetLength_, matrix.alphabet));

    unsigned char* transformedMatches[targets.size()];


    int matchesLens[targets.size()];

    int k = 0;
    for (const auto& it : targets) {
        transformedMatches[k] = charSeqToIndexesSeq(it->sequence_, matrix.alphabetLength_, matrix.alphabet);
        matchesLens[k++] = it->sequence_.size();
    }

    OpalSearchResult* results[targets.size()];

    for (int i = 0; i < (int) targets.size(); ++i) {
        results[i] = new OpalSearchResult;
        opalInitSearchResult(results[i]);
    }

    const int* matrixCodes = matrix.getMatrix();

    auto error = opalSearchDatabase(transformedQuery.get(), (int) query->sequence_.size(), transformedMatches,
                                    (int) targets.size(), matchesLens, matrix.gapOpen_, matrix.gapExtend_,
                                    matrixCodes, matrix.alphabetLength_, results, OPAL_SEARCH_ALIGNMENT, convertAlignTypeToOpal(type),
                                    OPAL_OVERFLOW_BUCKETS);

    if (error) {
        printf("Error in Opal\n");
        return {};
    }

    k = 0;

    std::vector<Alignment> alignments;


    alignments.reserve(targets.size());
    for (int i = 0; i < (int) targets.size(); ++i) {
        const auto& r(results[i]);
        double evalue = evalueParam.calculate(r->score, query->sequence_.size(), matchesLens[k]);
        if (evalue<= maxEvalue) {
            alignments.emplace_back(r->score, targets[i], r->startLocationQuery, r->endLocationQuery, r->startLocationTarget,
                                    r->endLocationTarget, r->alignmentLength, std::move(r->alignment), evalue);
        }
        ++k;
    }

    std::sort(alignments.begin(), alignments.end(), compareAlignment);
    for (auto& it : transformedMatches) delete[] it;
    for (auto& it : results)  {
        free(it->alignment);
        delete it;
    }
    return alignments;
}
}
