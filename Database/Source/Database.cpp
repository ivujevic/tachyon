#include "Database.hpp"
#include "Indexes/IndexFactory.hpp"
#include "../../Utils/Source/FastaFileUtils.hpp"
#include "../../Utils/Source/BinaryStream.hpp"


#include <memory>
#include <set>
#include <bitset>
#include <math.h>
#include <algorithm>
float VERSION = 1.0;

namespace utils {

    namespace {

    using CodedSequence = std::bitset<40>;

    CodedSequence getCode(const std::string_view& window) {

        CodedSequence code;

        for (const auto& c : window) {
            code <<= 5;

            if (c == '*') {
                code |= 27;
            } else if (c >= 'a' && c <= 'z')  {
                code |= c - 'a';
            } else if (c >= 'A' && c <= 'Z') {
                code |= c - 'A';
            } else {
                std::terminate();
            }
        }

        return code;
    }

    }

    BinaryOutStream& operator<<(BinaryOutStream& bs, const CodedSequence& hash) {
        unsigned long tmp = hash.to_ulong();
        bs.write(&tmp, 5);
        return bs;
    }


    BinaryOutStream& operator<<(BinaryOutStream& bs, const FastaFileElem& elem) {

        bs << elem.id_ << elem.name_ << (int) elem.sequence_.length();

        std::string_view seq(elem.sequence_);

        for (int i = 0; i < (int) elem.sequence_.length(); i += 8) {
            bs << getCode(seq.substr(i, 8));
        }

        return bs;
    }

    void nextWindow(BinaryInStream& bs, int& position, std::string& seq) {
        std::bitset<40> code;
        bs >> code;

        unsigned long cl = code.to_ulong();
        int padding = std::max(position + 8 - (int) seq.length(), 0);
        int mask = 31; // 2^5 - 1

        int start = position;
        while (padding <= 7) {
            int v = cl & mask;
            cl >>= 5;
            seq[start + 7 - padding] = v == 27 ? '*' : v + 'A';
            padding++;
            position++;
        }

    }

    std::string loadSequence(BinaryInStream& bs) {
        int seqLen;
        bs >> seqLen;
        int numberOfCodes = std::ceil(seqLen / 8.0f);

        std::string seq;
        seq.resize(seqLen);
        int position = 0;

        while (numberOfCodes) {
            nextWindow(bs, position, seq);
            numberOfCodes--;
        }

        return seq;
    }

    BinaryInStream& operator>>(BinaryInStream& bs, FastaFileElem& elem) {
        bs >> elem.id_ >> elem.name_;
        elem.sequence_ = loadSequence(bs);

        return bs;
    }
}

namespace tachyon {

std::vector<std::shared_ptr<utils::FastaFileElem>> Database::getSequences(const std::unordered_set<long>& ids) const {

    std::vector<std::shared_ptr<utils::FastaFileElem>> dst;
    dst.reserve(ids.size());

    std::vector<long> idsv;
    idsv.reserve(ids.size());
    for (const auto& it : ids) idsv.push_back(it);
    std::sort(idsv.begin(), idsv.end());

    for (const auto& id : idsv) {

        const auto& offset(offsets_[id]);
        bs_.seekOn(offset, SEEK_SET);

        dst.emplace_back(std::make_shared<utils::FastaFileElem>());

        bs_ >> *dst.back();
    }

    return dst;
}

std::optional<Indexes> Database::makeIndexes(const char* path, const char* out, const IndexingOptions& options) {

    auto indexes(IndexFactory::buildIndexes(path, options));

    utils::BinaryOutStream bs(out);

    bs << VERSION;
    bs << indexes->map_;

    // Write elements
    unsigned long databaseSize = 0;
    std::vector<unsigned long> elemOffsets;
    for (const auto& it : utils::FastaIterator(path)) {
        elemOffsets.emplace_back(bs.position_);
        databaseSize += it->sequence_.size();
        bs << it;
    }

    unsigned long p1 = bs.position_;
    bs << databaseSize << elemOffsets << p1;

    bs.flush();

    return indexes;
}

//void Database::fixDatabase(const char* path, const char* outPath, const char* fasta) {
//
//    utils::BinaryInStream bs(path);
//
//    IndexStruct highMap, lowMap;
//
//    float version;
//    bs >> version;
//
//    bs >> highMap >> lowMap;
//
//    bs.~BinaryInStream();
//
//    utils::BinaryOutStream out(outPath);
//
//    out << version << highMap << lowMap;
//
//    unsigned long databaseSize = 0;
//    std::vector<unsigned long> elemOffsets;
//    for (const auto& it : utils::FastaIterator(fasta)) {
//        elemOffsets.emplace_back(out.position_);
//        databaseSize += it->sequence_.size();
//        out << it;
//    }
//
//    unsigned long p1 = out.position_;
//    out << databaseSize << elemOffsets << p1;
//
//    out.flush();
//}

Database Database::loadDatabase(const char* path) {

    utils::BinaryInStream bs(path);
    IndexStruct map;

    float version;
    bs >> version;

    bs >> map;

    Indexes indexes(std::move(map));

    bs = utils::BinaryInStream(path);

    unsigned long start;
    bs.seekOn(-1 * sizeof(start), SEEK_END);
    bs >> start;
    bs.seekOn(start, SEEK_SET);

    unsigned long databaseSize;
    std::vector<unsigned long> offsets;

    bs >> databaseSize >> offsets;

    return {std::move(indexes), databaseSize, std::move(offsets), std::move(bs)};
}
}
