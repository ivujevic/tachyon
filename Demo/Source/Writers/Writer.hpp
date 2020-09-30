#pragma once

#include "../../../Aligner/Source/Alignment.hpp"
#include "../../../TachyonLib/Source/TachyonResultEntry.hpp"

#include <vector>
#include <memory>

namespace tachyon {

struct Writer {

    Writer(const char* path) : file_(fopen(path, "w"), &fclose) {

    }

    void writeBM0(const TachyonResultEntry& results);
    void writeBM8(const TachyonResultEntry& results);
    void writeBM9(const TachyonResultEntry& results);

private:
    std::unique_ptr<FILE, decltype(&fclose)> file_;
};


}
