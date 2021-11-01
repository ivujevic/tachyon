#pragma once

#include "../../../Aligner/Source/Alignment.hpp"
#include "../../../TachyonLib/Source/TachyonResultEntry.hpp"
#include "../../../TachyonLib/Source/Steps/First/FirstStep.hpp"
#include <vector>
#include <memory>

namespace tachyon {

struct Writer {

    Writer(const char* path) : file_(fopen(path, "w"), &fclose) {

    }

    void writeFirstStep(const FirstStepResultEntry& entry);

    void writeBM0(const TachyonResultEntry& results, int maxAligns);
    void writeBM8(const TachyonResultEntry& results, int maxAligns);
    void writeBM9(const TachyonResultEntry& results, int maxAligns);

private:
    std::unique_ptr<FILE, decltype(&fclose)> file_;
};


}
