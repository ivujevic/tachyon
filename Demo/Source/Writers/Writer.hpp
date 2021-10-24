#pragma once

#include "../../../TachyonLib/Source/TachyonResultEntry.hpp"

#include <vector>
#include <memory>

namespace tachyon {

struct Writer {

    Writer(const char* path) : file_(fopen(path, "w"), &fclose) {

    }

    void write(const TachyonResultEntry& results);

private:
    std::unique_ptr<FILE, decltype(&fclose)> file_;
};


}
