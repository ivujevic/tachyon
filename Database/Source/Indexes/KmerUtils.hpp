#pragma once

#include <string_view>
#include <vector>
#include <unordered_set>

namespace tachyon {

using KmerHashType = unsigned long long;

struct KmerProperty {
    KmerHashType hash;
    int position_;
};

struct KmerUtils {

    static std::vector<KmerProperty> findCodes(const std::string_view& sequence, int kmerLen) {

        std::vector<KmerProperty> results;
//        std::unordered_set<unsigned long> added;

//        results.reserve(sequence.size() - (kmerLen - 1));

        for (int i = 0; i < (int) sequence.size() - (kmerLen - 1); ++i) {
            unsigned long long code = 0;
            if (!KmerUtils::hashFunction(sequence.substr(i, kmerLen), code)) continue;
//            if (added.count(code)) continue;
//            added.insert(code);
            results.push_back({code, i});
        }

        return results;
    }

    static bool hashFunction(const std::string_view& kmer, unsigned long long& code) {

        auto convertToNumb = [] (const char& c) {
            switch (c) {
            case 'A':
            case 'a':
                return 0;
            case 'C':
            case 'c':
                return 1;
            case 'T':
            case 't':
                return 2;
            case 'G':
            case 'g':
                return 3;
            case 'U':
            case 'u':
                return 4;
            default:
                return 5;
            }
        };

        for (const char& c : kmer) {
            code <<= 3;

            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) return false;

            if (c == '*' || c == 'X' || c == 'B' || c == 'J' || c == 'O') {
                return false;
            }

            if (c == '*' || c == 'x' || c == 'b' || c == 'j' || c == 'o') {
                return false;
            }
            code |= convertToNumb(c);
        }

        return true;
    }
};

}
