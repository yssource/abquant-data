#include <pybind11/pybind11.h>

#include <QtCore/QObject>
#include <iostream>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/utils.hpp"

using namespace std;
namespace abq
{
int add(int i, int j) { return i + j; }

size_t stock()
{
    Abquant::start();
    QStringList codes = {"000001", "000002", "000003"};
    // QStringList codes = {"000001"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";
    MIN_FREQ freq     = MIN_FREQ::FIVE;

    StockMinAction sma(codes, start, end, freq);

    auto fq = sma.toFq(FQ_TYPE::PRE);
    return fq.get_index().size();
}

namespace py = pybind11;

PYBIND11_MODULE(abqstockmin, m)
{
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("stock", &stock, R"pbdoc(
        abqstockmin fffffff Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def(
        "subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
