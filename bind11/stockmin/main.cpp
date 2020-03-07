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
    PyStockMin(std::vector<std::string> codes, const string& start, const string& end, const string& sfreq = "5min")
        : m_codes{codes}, m_start{start}, m_end{end}, m_sfreq{sfreq}
    {
        Abquant::start();
        MIN_FREQ freq;
        if (QString::fromStdString(m_sfreq) == QString("5min")) {
            freq = MIN_FREQ::FIVE;
        }

        QStringList qcodes;
        for (auto c : m_codes) {
            qcodes << QString::fromStdString(c);
        }
        m_sma = StockMinAction(qcodes, m_start.c_str(), m_end.c_str(), freq);
    };

    size_t toQfq()
    {
        auto fq = m_sma.toFq(FQ_TYPE::PRE);
        return fq.get_index().size();
    }
    ~PyStockMin() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    const string m_sfreq{};
    StockMinAction m_sma{};
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
    sm_class.def(py::init<std::vector<std::string>, const string, const string, const string>())
        .def("toQfq", &PyStockMin::toQfq, R"pbdoc(
        toQfq

        qfq function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
