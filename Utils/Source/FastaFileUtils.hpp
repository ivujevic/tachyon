#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <memory>

namespace utils {

struct FastaFileData;

struct FastaFileElem {

    FastaFileElem() {

    }
    FastaFileElem(int id, std::string name, std::string sequence) : id_(id), name_(std::move(name)), sequence_(std::move(sequence)) {

    }

    FastaFileElem(const FastaFileElem&) = delete;
    FastaFileElem& operator=(const FastaFileElem& ) = delete;

    unsigned int id_;
    std::string name_;
    std::string sequence_;
};


struct FastaIteratorIt : public std::iterator<std::input_iterator_tag, std::pair<std::string, std::string>> {

    explicit FastaIteratorIt(FILE* f);
    ~FastaIteratorIt();

    std::shared_ptr<FastaFileElem> operator*();
    FastaIteratorIt& operator++();

    bool operator!=(const FastaIteratorIt& o) {
        return !operator==(o);
    }

    bool operator==(const FastaIteratorIt& o);

private:
    FILE* file_;

    void getNext();

    std::shared_ptr<FastaFileElem> elem_;
    std::unique_ptr<FastaFileData> data_;
};

struct FastaIterator {

    explicit FastaIterator(const char* p) : file_(fopen(p, "r"), &fclose) {

    }

    FastaIteratorIt begin() {
        return FastaIteratorIt(file_.get());
    }

    FastaIteratorIt end() {
        return FastaIteratorIt(NULL);
    }

private:
    std::unique_ptr<FILE, decltype(&fclose)> file_;
};


std::vector<std::shared_ptr<FastaFileElem>> createEntriesFromFastaString(std::string fasta);
}


