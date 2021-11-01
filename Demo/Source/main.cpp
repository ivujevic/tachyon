
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
        );
    }

    virtual void search() = 0;

protected:
    FirstStepResult baseSearch(TachyonQuery&& query) {

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
        return firstStepResult;
    }
};

struct SearchPCommand : public SearchCommand {

    explicit SearchPCommand(lyra::cli& cli) : SearchCommand("blastn", cli) {

    }

    void search() override {
        std::vector<ProteineQuery> queries;
        for (const auto& it : utils::FastaIterator(queryPath.c_str())) {
            queries.push_back(it);
        }
        if (queries.empty()) return;

        auto result(baseSearch({queries}));

        Writer writer(outputPath.c_str());

        for (const auto& it : std::get<FirstStepResult::ProteineResult>(result.entries_)) {
            writer.writeFirstStep(it);
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
                                                       .help("Number of  threads."))
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

