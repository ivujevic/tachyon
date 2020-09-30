#include "FastaFileUtils.hpp"

#include <memory>
#include <cstdio>
#include <fstream>

#define BUFFER_SIZE 1024 * 1024


namespace utils {

struct FastaFileData {

    char buffer_[BUFFER_SIZE];
    int position_ = 0;
    int readed_ = 0;
    unsigned long numberOfChains_ = 0;
    bool isEnd_ = false;
};

void FastaIteratorIt::getNext() {

    if (!file_) {
        elem_ = {};
        data_ = {};
        return;
    }

    data_->isEnd_ = (bool) std::feof(file_);
    bool isName = true;

    constexpr int maxLength = 65000;
    std::string name;
    name.reserve(maxLength);

    std::string sequence;
    sequence.reserve(maxLength);

    do {

        for (; data_->position_ < data_->readed_; ++data_->position_) {

            auto& c = data_->buffer_[data_->position_];
            if (!isName && (c == '>' || (data_->isEnd_ && data_->position_ == data_->readed_ - 1))) {
                elem_ = std::make_shared<FastaFileElem>(data_->numberOfChains_++, std::move(name), std::move(sequence));
                return;
            }

            if (isName) {
                if (c == '\n') {
                    isName = false;
                } else if (name.length() == maxLength) {
                    continue;
                } else if (!(name.length() == 0 && (c == '>' || isspace(c)))) {
                    if (c != '\r') {
                        name.push_back(c);
                    }
                }
            } else {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) sequence.push_back(c);
            }
        }

        data_->readed_ = fread(data_->buffer_, sizeof(char), BUFFER_SIZE, file_);
        data_->position_ = 0;
        data_->isEnd_ = (bool) std::feof(file_);

    } while (!data_->isEnd_ || data_->position_ < data_->readed_);

    elem_ = {};
    data_ = {};
}

bool FastaIteratorIt::operator==(const FastaIteratorIt& o) {
    return elem_ == o.elem_ && data_ == o.data_;
}

std::shared_ptr<FastaFileElem> FastaIteratorIt::operator*() {
    return elem_;
}

FastaIteratorIt& FastaIteratorIt::operator++() {
    getNext();
    return *this;
}

FastaIteratorIt::~FastaIteratorIt() = default;

FastaIteratorIt::FastaIteratorIt(FILE* f) : file_(f), data_(std::make_unique<FastaFileData>()) {
    getNext();
}


std::vector<std::shared_ptr<FastaFileElem>> createEntriesFromFastaString(std::string fasta) {

    std::vector<std::shared_ptr<FastaFileElem>> elems;
    bool isName = true;
    constexpr int maxLength = 65000;

    std::string name;
    name.reserve(maxLength);

    std::string sequence;
    sequence.reserve(maxLength);

    int i = 0;
    for (const auto& c : fasta) {
        if (!isName && c == '>') {
            elems.emplace_back(std::make_shared<FastaFileElem>(i++, std::move(name), std::move(sequence)));
            name.clear();
            sequence.clear();
            isName = true;
            continue;
        }

        if (isName) {
            if (c == '\n') {
                isName = false;
                continue;
            } else if (name.length() == maxLength) {
                continue;
            } else if (!(name.length() == 0 && (c == '>' || isspace(c)))) {
                if (c != '\r') {
                    name.push_back(c);
                }
            }
        } else {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) sequence.push_back(c);
        }
    }

    elems.emplace_back(std::make_shared<FastaFileElem>(i++, std::move(name), std::move(sequence)));
    return elems;
}
}
