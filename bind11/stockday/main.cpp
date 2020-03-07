#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <QtCore/QObject>
#include <iostream>
#include <string>
#include <vector>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/utils.hpp"

// using namespace std;
namespace abq
{
class PyStockDay
{
public:
    PyStockDay(std::vector<std::string> codes, const string& start, const string& end)
        : m_codes{codes}, m_start{start}, m_end{end}
    {
        Abquant::start();
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_sda = StockDayAction(qcodes, m_start.c_str(), m_end.c_str());
    };

    size_t toQfq()
    {
        auto fq = m_sda.toFq(FQ_TYPE::PRE);
        return fq.get_index().size();
    }

    std::vector<double> toSeries(const string& col) const noexcept
    {
        auto seriese = m_sda.toSeries<double>(col.c_str());
        return seriese.toStdVector();
    }
    ~PyStockDay() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    StockDayAction m_sda{};
};

namespace py = pybind11;

PYBIND11_MODULE(abqstockday, m)
{
    m.doc() = R"pbdoc(
        Pybind11 StockDay plugin
        -----------------------

        .. abqstockday:: currentmodule_exmaple

        .. autosummary::
           :stockday: toQfq

           toQfq
    )pbdoc";

    py::class_<PyStockDay> sm_class(m, "PyStockDay");
    sm_class.def(py::init<std::vector<std::string>, const string, const string>())
        .def("toQfq", &PyStockDay::toQfq, R"pbdoc(
        toQfq

        qfq function.
    )pbdoc")
        .def("toSeries", &PyStockDay::toSeries, R"pbdoc(
        toSeries

        toSeries function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
