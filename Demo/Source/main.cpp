
#include "lyra.hpp"
#include "../../Database/Source/Database.hpp"

#include "Writers/Writer.hpp"
#include "../../TachyonLib/Source/Query.hpp"

#include "../../TachyonLib//Source/Steps/First/FirstStep.hpp"
#include "../../TachyonLib/Source/Steps/Second/SecondStep.hpp"
#include "../../Utils/Source/Timer.hpp"
#include "../../Utils/Source/DNATranslator.hpp"


using namespace tachyon;

struct SearchCommand {

    std::string queryPath;
    std::string outputPath;
    std::string databasePath;

    FirstStepOptions firstStepOptions;
    SecondStepOptions secondStepOptions;

    std::string outFormat = "bm9";
    int maxAligns = 10;

    SearchCommand(std::string command, lyra::cli& cli) {
        cli.add_argument(lyra::command(command,
                                       [this](const lyra::group&) { this->search(); })
                                 .add_argument(
                                         lyra::opt(databasePath, "databasePath")
                                                 .name("-d")
                                                 .name("--db")
                                                 .help("Path to constructed tachyon's database")
                                 )
                                 .add_argument(
                                         lyra::opt(queryPath, "queryPath")
                                                 .name("-q")
                                                 .name("--query")
                                                 .help("Path to query fasta file")
                                 )
                                 .add_argument(
                                         lyra::opt(outputPath, "outputPath")
                                                 .name("-o")
                                                 .name("--out")
                                                 .help("Path to output file")
                                 )
                                 .add_argument(
                                         lyra::opt(firstStepOptions.numberOfThreads, "threads")
                                                 .name("-p")
                                                 .name("--threads")
                                                 .help("Number of threads")
                                 )

                                 .add_argument(
                                         lyra::opt(firstStepOptions.highFreqCutOff, "highCutoff")
                                                 .name("-hc")
                                                 .name("--high-cutoff")
                                                 .help("Cutoff of high frequence kmers, default = 3")
                                 )
                                 .add_argument(
                                         lyra::opt(firstStepOptions.lowFreqCutOff, "lowCutoff")
                                                 .name("-lc")
                                                 .name("--low-cutoff")
                                                 .help("Cutoff of low frequence kmers, default = 2")
                                 )
                                 .add_argument(
                                         lyra::opt(secondStepOptions.ssearch_.maxEvalue, "maxEvalue")
                                                 .name("-v")
                                                 .name("--evalue")
                                                 .help("Maximum evalue, default = 0.0001")
                                 )
                                 .add_argument(
                                         lyra::opt(secondStepOptions.ssearch_.gapOpen, "gapOpen")
                                                 .name("-g")
                                                 .name("--gap-open")
                                                 .help("gap open penalty, default = 10")
                                 )
                                 .add_argument(
                                         lyra::opt(secondStepOptions.ssearch_.gapExtend, "gapExtend")
                                                 .name("-e")
                                                 .name("--gap-extend")
                                                 .help("gap extend penalty, default = 1")
                                 )
                                 .add_argument(
                                         lyra::opt(maxAligns, "maxAligns")
                                                 .name("-a")
                                                 .name("--max-aligns")
                                                 .help("maximum number of outputs per query, default = 10")
                                 )
        );
    }

    virtual void search() = 0;

protected:
    TachyonResult::Result baseSearch(TachyonQuery&& query) {

        utils::Timer timer;

        printf("Loading database...\n");
        auto database = tachyon::Database::loadDatabase(databasePath.c_str());
        printf("Base loaded [%fms]\n", timer.end());

        secondStepOptions.ssearch_.databaseSize = database.databaseSize_;
        secondStepOptions.numberOfThreads = firstStepOptions.numberOfThreads;

        timer.start();
        printf("First step search...\n");
        auto firstStepResult(FirstStep::getPotentialMatches(database, std::move(query), firstStepOptions));
        printf("First step finished [%fms]\n", timer.end());
        timer.start();
        printf("Second step search...\n");
        auto secondStepResult(SecondStep::process(std::move(firstStepResult), secondStepOptions));
        printf("Second step finished [%fms]\n", timer.end());
        return secondStepResult;
    }
};

struct SearchPCommand : public SearchCommand {

    explicit SearchPCommand(lyra::cli& cli) : SearchCommand("blastp", cli) {

    }

    void search() override {
        std::vector<ProteineQuery> queries;
        for (const auto& it : utils::FastaIterator(queryPath.c_str())) {
            queries.push_back(it);
        }
        if (queries.empty()) return;

        auto result(baseSearch({queries}));

        Writer writer(outputPath.c_str());
        for (const auto& it : std::get<TachyonResult::ProteineResult>(result)) {
            writer.writeBM9(it, maxAligns);
        }
    }
};


struct SearchNCommand : public SearchCommand {

    explicit SearchNCommand(lyra::cli& cli) : SearchCommand("blastn", cli) {

    }

    void search() override {
        std::vector<NucleotideQuery> queries;
        for (const auto& it : utils::FastaIterator(queryPath.c_str())) {
            queries.push_back(NucleotideQuery{it, utils::DNATranslator::translate(it)});
            if (queries.size() > 10000) {
                auto result(baseSearch({queries}));
                Writer writer(outputPath.c_str());
                for (const auto& it : std::get<std::vector<TachyonResult::NucleotideResult>>(result)) {
                    for (const auto& it1 : it.entries_) {
                        writer.writeBM9(it1, maxAligns);
                    }
                }

                queries.clear();
            }
        }

        if (queries.empty()) return;

        auto result(baseSearch({queries}));
        Writer writer(outputPath.c_str());
        for (const auto& it : std::get<std::vector<TachyonResult::NucleotideResult>>(result)) {
            for (const auto& it1 : it.entries_) {
                writer.writeBM9(it1, maxAligns);
            }
        }
    }
};

struct MakeDBCommand {

    IndexingOptions indexingOptions;
    std::string inputFasta;
    std::string outputIndexes;

    explicit MakeDBCommand(lyra::cli& cli) {
        cli.add_argument(lyra::command("makedb",
                                       [this](const lyra::group&) { this->runCommand(); })

                                 .add_argument(lyra::opt(inputFasta, "inputFasta")
                                                       .name("--in")
                                                       .help("Input fasta file").required())
                                 .add_argument(lyra::opt(outputIndexes, "outputIndexes")
                                                       .name("-d")
                                                       .name("--db")
                                                       .required()
                                                       .help("Tachyon created indexes"))
                                 .add_argument(lyra::opt(indexingOptions.numberOfThreads, "numberOfThreads")
                                                       .name("-p")
                                                       .name("--threads")
                                                       .help("Number of   threads."))
                                 .add_argument(lyra::opt(indexingOptions.highFreq.regionSize, "highRegionSize")
                                                       .name("--hr")
                                                       .name("--region-size-high"))
                                 .add_argument(lyra::opt(indexingOptions.highFreq.number, "numberOfHighFreq")
                                                       .name("--hk")
                                                       .name("--num-high-kmers"))
                                 .add_argument(lyra::opt(indexingOptions.lowFreq.regionSize, "lowRegionSize")
                                                       .name("--lr")
                                                       .name("--region-size-low"))
                                 .add_argument(lyra::opt(indexingOptions.lowFreq.number, "numberOfLowFreq")
                                                       .name("--lk")
                                                       .name("--num-low-kmers"))
                                 .add_argument(lyra::opt(indexingOptions.segParam.window, "segWindow")
                                                       .name("--sw")
                                                       .name("--seg-window-size"))
                                 .add_argument(lyra::opt(indexingOptions.segParam.segHighCutOff, "segHighCutOff")
                                                       .name("--shc")
                                                       .name("--seg-high_cutoff"))
                                 .add_argument(lyra::opt(indexingOptions.segParam.segLowCutOff, "segLowCutOff")
                                                       .name("--slc")
                                                       .name("--seg-low_cutoff"))
        );
    }

    void runCommand() {
        tachyon::Database::makeIndexes(inputFasta.c_str(), outputIndexes.c_str(), indexingOptions);
    }
};

int main(int argc, char** argv) {


    bool getHelp = false;

    auto cli = lyra::cli()
               | lyra::help(getHelp);

    MakeDBCommand makeDB{cli};
    SearchPCommand searchP{cli};
    SearchNCommand searchN{cli};

    auto result = cli.parse({argc, argv});

    if (getHelp) {
        std::cout << cli;
        return 0;
    }

    if (!result) {
        printf("Error\n");
        std::cerr << result.errorMessage() << "\n";
    }

}

