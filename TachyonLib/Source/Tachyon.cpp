#include "Tachyon.hpp"
#include "Steps/First/FirstStepOptions.hpp"
#include "Steps/First/FirstStep.hpp"
#include "Steps/Second/SecondStep.hpp"

namespace tachyon {

TachyonResult Tachyon::search(TachyonQuery&& query, int numberOfThreads) {

    FirstStepOptions options;
    options.numberOfThreads = numberOfThreads;
    auto firstStepResult(FirstStep::getPotentialMatches(this->database_, std::move(query), options));
    SecondStepOptions so;
    so.numberOfThreads = numberOfThreads;
    so.ssearch_.databaseSize = this->database_.databaseSize_;

    return { SecondStep::process(std::move(firstStepResult), so) };
}

}
