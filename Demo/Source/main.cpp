
#include "../../Database/Source/Database.hpp"

#include "Writers/Writer.hpp"
#include "../../TachyonLib/Source/Query.hpp"

#include "../../TachyonLib//Source/Steps/First/FirstStep.hpp"
#include "../../TachyonLib/Source/Steps/Second/SecondStep.hpp"
#include "../../Utils/Source/Timer.hpp"
#include "../../Utils/Source/DNATranslator.hpp"


using namespace tachyon;

TachyonResult::Result search(const Database& base, TachyonQuery&& query) {
    utils::Timer timer;

    timer.start();

    FirstStepOptions options;
    options.numberOfThreads = 24;
    auto firstStepResult(FirstStep::getPotentialMatches(base, std::move(query), options));
    printf("First step %f\n", timer.end());
    timer.start();

    SecondStepOptions so;
    so.ssearch_.databaseSize = base.databaseSize_;
    so.numberOfThreads = 24;

    auto secondStepResult(SecondStep::process(std::move(firstStepResult), so));
    printf("Second step %f\n", timer.end());

    return secondStepResult;
}

void nucleotideSearch(const Database& base, const char* inputPath, const char* output) {
    utils::Timer timer;

    std::vector<NucleotideQuery> queries;
    for(const auto& it : utils::FastaIterator(inputPath)) {
        queries.push_back(NucleotideQuery{it, utils::DNATranslator::translate(it)});
        if (queries.size() > 10000) {
            auto result(search(base, {queries}));
            timer.start();
            Writer writer(output);

            for (const auto& it : std::get<std::vector<TachyonResult::NucleotideResult>>(result)) {
                for (const auto& it1 : it.entries_) {
                    writer.writeBM9(it1);
                }
            }

            printf("Write %f\n", timer.end());

            queries.clear();
        }
    }

    if (queries.empty()) return;

    auto result(search(base, {queries}));

    timer.start();
    Writer writer(output);

    for (const auto& it : std::get<std::vector<TachyonResult::NucleotideResult>>(result)) {
        for (const auto& it1 : it.entries_) {
            writer.writeBM9(it1);
        }
    }

    printf("Write %f\n", timer.end());
}

void proteineSearch(const Database& base, const char* inputPath, const char* output) {
    utils::Timer timer;

    std::vector<ProteineQuery> queries;
    for(const auto& it : utils::FastaIterator(inputPath)) {
        queries.push_back(it);
    }

    auto result(search(base, {queries}));

    timer.start();
    Writer writer(output);
    for (const auto& it : std::get<TachyonResult::ProteineResult>(result)) writer.writeBM9(it);
    printf("Write %f\n", timer.end());
}

int main(int, char** ) {

    tachyon::Database::makeIndexes("astral1.fa", "astral.bin", {});

//    auto type(argv[1]);
//    auto indexPath(argv[2]);
//    auto inputPath(argv[3]);
//    auto output(argv[4]);
//
//    utils::Timer timer;
//    auto base(tachyon::Database::loadDatabase(indexPath));
//    printf("Base load %f\n", timer.end());
//    timer.start();
//
//    if (std::string(type) == "blastp") proteineSearch(base, inputPath, output);
//    if (std::string(type) == "blastn") nucleotideSearch(base, inputPath, output);
}

