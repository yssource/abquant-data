#include <pybind11/cast.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <QtCore/QObject>
#include <cstring>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stocklist.hpp"
#include "abquant/actions/utils.hpp"

namespace abq
{
using StockListActionPtr = std::shared_ptr<StockListAction>;

class PyStockList : public std::enable_shared_from_this<PyStockList>
{
public:
    PyStockList() { m_sla_ptr = std::make_shared<StockListAction>(); };

    PyStockList(std::vector<std::string> codes, const string& end) : m_codes{codes}, m_end{end}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_sla_ptr = std::make_shared<StockListAction>(qcodes, m_end.c_str());
    };

    template <class T>
    std::vector<T> toSeries(const string& col) noexcept
    {
        auto series = m_sla_ptr->toSeries<T>(col.c_str());
        return series.toStdVector();
    }

    ~PyStockList() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_end{};
    StockListActionPtr m_sla_ptr{nullptr};
};

namespace py = pybind11;

PYBIND11_MODULE(abqstocklist, m)
{
    m.doc() = R"pbdoc(
        Pybind11 StockList plugin
        -----------------------

        .. abqstocklist:: abqstocklist_module_exmaple

        .. autosummary::
           :stocklist: toSeries

           toSeries
    )pbdoc";

    py::class_<MyDataFrame, MyDataFramePtr>(m, "MyDataFrame");
    py::class_<PyStockList, std::shared_ptr<PyStockList>> sm_class(m, "PyStockList");
    sm_class.def(py::init<>())
        .def(py::init<std::vector<std::string>, const string>())
        .def("toSeries", &PyStockList::toSeries<double> /* , py::return_value_policy::reference */,
             R"pbdoc(toSeries double function.)pbdoc")
        .def("toSeries_string", &PyStockList::toSeries<std::string>, R"pbdoc(toSeries string function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
} // namespace abq
