#include "DNATranslator.hpp"

#include <unordered_map>

namespace utils {

std::unordered_map<std::string_view, char> codonTable = {
        {"TTT", 'F'},
        {"TTC", 'F'},
        {"TTA", 'L'},
        {"TTG", 'L'},
        {"CTT", 'L'},
        {"CTC", 'L'},
        {"CTA", 'L'},
        {"CTG", 'L'},
        {"ATT", 'I'},
        {"ATC", 'I'},
        {"ATA", 'I'},
        {"ATG", 'M'},
        {"GTT", 'V'},
        {"GTC", 'V'},
        {"GTA", 'V'},
        {"GTG", 'V'},
        {"TCT", 'S'},
        {"TCC", 'S'},
        {"TCA", 'S'},
        {"TCG", 'S'},
        {"CCT", 'P'},
        {"CCC", 'P'},
        {"CCA", 'P'},
        {"CCG", 'P'},
        {"ACT", 'T'},
        {"ACC", 'T'},
        {"ACA", 'T'},
        {"ACG", 'T'},
        {"GCG", 'A'},
        {"GCA", 'A'},
        {"GCC", 'A'},
        {"GCT", 'A'},
        {"TAT", 'Y'},
        {"TAC", 'Y'},
        {"CAT", 'H'},
        {"CAC", 'H'},
        {"CAA", 'Q'},
        {"CAG", 'Q'},
        {"AAT", 'N'},
        {"AAC", 'N'},
        {"AAA", 'K'},
        {"AAG", 'K'},
        {"GAT", 'D'},
        {"GAC", 'D'},
        {"GAA", 'E'},
        {"GAG", 'E'},
        {"TGT", 'C'},
        {"TGC", 'C'},
        {"TGG", 'W'},
        {"CGT", 'R'},
        {"CGC", 'R'},
        {"CGA", 'R'},
        {"CGG", 'R'},
        {"AGA", 'R'},
        {"AGG", 'R'},
        {"AGT", 'S'},
        {"AGC", 'S'},
        {"GGT", 'G'},
        {"GGC", 'G'},
        {"GGA", 'G'},
        {"GGG", 'G'},
        {"TAA", '*'},
        {"TAG", '*'},
        {"TGA", '*'}
};

char getComplementNucleotide(char nucleotide) {
    switch (nucleotide) {
    case 'C':
        return 'G';
    case 'G':
        return 'C';
    case 'T':
        return 'A';
    case 'A':
        return 'T';
    default:
        return 'N';
    }
}

void getComplementChain(const std::string& chain, int len, std::string& complementChain) {
    for (int i = len - 1; i >= 0; i--) {
        char c = chain[i];
        complementChain += getComplementNucleotide(c);
    }
}

std::array<std::shared_ptr<utils::FastaFileElem>, 6> DNATranslator::translate(const std::shared_ptr<utils::FastaFileElem>& elem) {

    std::array<std::shared_ptr<utils::FastaFileElem>, 6> ret;

    int len = elem->sequence_.length();
    std::string complementChain = "";
    getComplementChain(elem->sequence_, len, complementChain);

    std::string_view dnaSeq(elem->sequence_);

    for (int i = 0; i < 3; i++) {
        std::string aaSeq = "";
        std::string aaSeqCompl = "";
        for (int j = i; j < len - 2; j += 3) {
            auto codon = dnaSeq.substr(j, 3);
            auto codonCompl = complementChain.substr(j, 3);
            if (codonTable.find(codon) != codonTable.end()) {
                aaSeq += codonTable[codon];
            } else {
                aaSeq += 'X';
            }

            if (codonTable.find(codonCompl) != codonTable.end()) {
                aaSeqCompl += codonTable[codonCompl];
            } else {
                aaSeqCompl += 'X';
            }
        }
        ret[i] = std::make_shared<utils::FastaFileElem>(elem->id_, elem->name_, aaSeq);
        ret[3 + i] = std::make_shared<utils::FastaFileElem>(elem->id_, elem->name_, aaSeqCompl);
    }

    return ret;
}
}
