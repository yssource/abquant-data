#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <QtCore/QObject>
#include <iostream>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/utils.hpp"

// using namespace std;
namespace abq
{
class PyAbquant
{
public:
    static void start() { Abquant::start(); }
    static void finish() { Abquant::finish(); }

    PyAbquant()  = default;
    ~PyAbquant() = default;

private:
    PyAbquant(const PyAbquant&) = delete;
    PyAbquant& operator=(const PyAbquant&) = delete;
};

namespace py = pybind11;

PYBIND11_MODULE(pyabquant, m)
{
    m.doc() = R"pbdoc(
        Pybind11 PyAbquant plugin
        -----------------------

        .. pyabquant:: currentmodule_exmaple

        .. autosummary::
           :pyabquant: PyAbquant

           pyabquant
    )pbdoc";

    py::class_<PyAbquant> sm_class(m, "PyAbquant");
    sm_class
        .def_static("start", &PyAbquant::start, R"pbdoc(
        start

        start function.
    )pbdoc")
        .def_static("finish", &PyAbquant::finish, R"pbdoc(
        finish void

        finish static void function.
    )pbdoc");

    py::enum_<FQ_TYPE>(m, "FQ_TYPE", py::arithmetic(), "FQ_TYPE::PRE, FQ_TYPE::NONE, FQ_TYPE::POST")
        .value("PRE", FQ_TYPE::PRE, "xdxr fq_type: PRE")
        .value("NONE", FQ_TYPE::NONE, "xdxr fq_type: NONE")
        .value("POST", FQ_TYPE::POST, "xdxr fq_type: POST")
        .export_values();

    py::enum_<INSTRUMENT_TYPE>(m, "INSTRUMENT_TYPE", py::arithmetic(), "INSTRUMENT_TYPE")
        .value("CS", INSTRUMENT_TYPE::CS, "CS: stock")
        .value("FUTURE", INSTRUMENT_TYPE::FUTURE, "Future: Future")
        .value("ETF", INSTRUMENT_TYPE::ETF, "ETF: ETF")
        .value("INDX", INSTRUMENT_TYPE::INDX, "INDX: INDX")
        .value("FENJI_MU", INSTRUMENT_TYPE::FENJI_MU, "FenjiMu: FENJI_MU")
        .value("FENJI_A", INSTRUMENT_TYPE::FENJI_A, "FenjiA: FENJI_A")
        .value("FENJI_B", INSTRUMENT_TYPE::FENJI_B, "FenjiB: FENJI_B")
        .value("PUBLIC_FUND", INSTRUMENT_TYPE::PUBLIC_FUND, "PublicFund: PUBLIC_FUND")
        .value("BOND", INSTRUMENT_TYPE::BOND, "Bond: BOND")
        .value("CONVERTIBLE", INSTRUMENT_TYPE::CONVERTIBLE, "Convertible: CONVERTIBLE")
        .value("SPOT", INSTRUMENT_TYPE::SPOT, "Spot: SPOT")
        .value("REPO", INSTRUMENT_TYPE::REPO, "Repo: REPO")
        .export_values();

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
