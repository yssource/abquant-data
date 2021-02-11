#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <QtCore/QObject>
#include <iostream>
#include <string>
#include <vector>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/utils.hpp"

// using namespace std;
namespace abq
{
class PyStockMin
{
public:
    PyStockMin(std::vector<std::string> codes, const string& start, const string& end, const string& sfreq = "5min",
               FQ_TYPE xdxr = FQ_TYPE::NONE)
        : m_codes{codes}, m_start{start}, m_end{end}, m_sfreq{sfreq}, m_xdxr{xdxr}
    {
        MIN_FREQ freq;
        if (QString::fromStdString(m_sfreq) == QString("5min") || QString::fromStdString(m_sfreq) == QString("5m")) {
            freq = MIN_FREQ::FIVE;
        }

        QStringList qcodes;
        for (auto c : m_codes) {
            qcodes << QString::fromStdString(c);
        }
        m_sma = StockMinAction(qcodes, m_start.c_str(), m_end.c_str(), freq, m_xdxr);
    };

    size_t toQfq()
    {
        // const auto fq = m_sma.toFq(FQ_TYPE::PRE);
        auto fq = m_sma.getDataFrame();
        // fq.write<std::ostream, std::string, double, int>(std::cout);
        fq->write<std::ostream, std::string, double, int>(std::cout);
        return fq->get_index().size();
        // return fq.get_index().size();
    }

    template <class T>
    std::vector<T> toSeries(const string& col) const noexcept
    {
        auto series = m_sma.toSeries<T>(col.c_str());
        return series.toStdVector();
    }
    ~PyStockMin() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    const string m_sfreq{};
    StockMinAction m_sma{};
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};
};

namespace py = pybind11;

PYBIND11_MODULE(abqstockmin, m)
{
    m.doc() = R"pbdoc(
        Pybind11 StockMin plugin
        -----------------------

        .. abqstockmin:: currentmodule_exmaple

        .. autosummary::
           :stockmin: toQfq

           toQfq
    )pbdoc";

    py::class_<PyStockMin> sm_class(m, "PyStockMin");
    sm_class.def(py::init<std::vector<std::string>, const string, const string, const string, FQ_TYPE>())
        .def("toQfq", &PyStockMin::toQfq, R"pbdoc(
        toQfq

        qfq function.
    )pbdoc")
        .def("toSeries", &PyStockMin::toSeries<double>, R"pbdoc(
        toSeries double

        toSeries double function.
    )pbdoc")
        .def("toSeries_string", &PyStockMin::toSeries<std::string>, R"pbdoc(
        toSeries string

        toSeries string function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
