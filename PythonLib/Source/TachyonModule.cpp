#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <utility>

#include "../../Database/Source/Indexes/IndexingOptions.hpp"
#include "../../Database/Source/Database.hpp"
#include "../../TachyonLib/Source/Tachyon.hpp"
#include "../../TachyonLib/Source/Steps/First/FirstStep.hpp"

namespace py = pybind11;

static void createIndexes(const std::string& inputPath, const std::string& outputPath, int numberOfThreads) {

    IndexingOptions options;
    options.numberOfThreads = numberOfThreads;

    tachyon::Database::makeIndexes(inputPath.c_str(), outputPath.c_str(), options);
}

static std::shared_ptr<tachyon::Tachyon> initializeTachyon(const std::string& dbPath) {
    return std::shared_ptr<tachyon::Tachyon>(new tachyon::Tachyon { tachyon::Database::loadDatabase(dbPath.c_str()) });
}

struct TachyonResult {

    explicit TachyonResult(tachyon::FirstStepResult::ProteineResult result) : entries(std::move(result)) {

    }

    const tachyon::FirstStepResult::ProteineResult entries;
};

static std::unique_ptr<TachyonResult> search(const tachyon::Tachyon& tachyon, const std::string& queryFile, int numberOfThreads) {

    std::vector<tachyon::ProteineQuery> queries;

    for (const auto& it : utils::FastaIterator(queryFile.c_str())) {
        queries.push_back(it);
    }

    if (queries.empty()) return {};

    tachyon::FirstStepOptions options;
    options.numberOfThreads = numberOfThreads;

    auto results(tachyon::FirstStep::getPotentialMatches(tachyon.database_, { std::move(queries) }, options));

    return std::make_unique<TachyonResult>(std::move(std::get<tachyon::FirstStepResult::ProteineResult>(results.entries_) ));
}


PYBIND11_MODULE(TachyonPy, m) {

    m.doc() = "Tachyon";


    // Used for create indexes on disk
    m.def("createIndexes", &createIndexes, "Create indexes for tachyon");

    // Initialize tachyon object from created indexes
    m.def("initialize", &initializeTachyon, py::return_value_policy::take_ownership);


    py::class_<tachyon::Tachyon, std::shared_ptr<tachyon::Tachyon>>(m, "Tachyon")
            .def("search", &search, py::return_value_policy::take_ownership);

    py::class_<TachyonResult>(m, "TachyonResult")
            .def_readonly("entries", &TachyonResult::entries);

    py::class_<utils::FastaFileElem, std::shared_ptr<utils::FastaFileElem>>(m, "FastaFileElem")
            .def_readonly("id", &utils::FastaFileElem::id_)
            .def_readonly("name", &utils::FastaFileElem::name_)
            .def_readonly("sequence", &utils::FastaFileElem::sequence_);

    py::class_<tachyon::FirstStepResultEntry>(m, "TachyonResultEntry")
            .def_readonly("query", &tachyon::FirstStepResultEntry::query_)
            .def_readonly("targets", &tachyon::FirstStepResultEntry::targets_);

}
