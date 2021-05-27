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
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/utils.hpp"

namespace abq
{
using roc_return_type   = std::unordered_map<const char*, std::vector<double>>;
using StockDayActionPtr = std::shared_ptr<StockDayAction>;

class PyStockDay : public std::enable_shared_from_this<PyStockDay>
{
public:
    PyStockDay(std::vector<std::string> codes, const string& start, const string& end, FQ_TYPE xdxr = FQ_TYPE::NONE)
        : m_codes{codes}, m_start{start}, m_end{end}, m_xdxr{xdxr}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_sda_ptr = std::make_shared<StockDayAction>(qcodes, m_start.c_str(), m_end.c_str(), m_xdxr);
    };

    // size_t toQfq() noexcept
    // {
    //     auto fq = m_sda_ptr->to_fq_type(FQ_TYPE::PRE);
    //     // fq->write<std::ostream, index_type, double, int>(std::cout);
    //     return fq->get_index().size();
    // }

    template <class T>
    std::vector<T> to_series(const string& col) noexcept
    {
        auto series = m_sda_ptr->to_series<T>(col.c_str());
        return series.toStdVector();
    }

    roc_return_type ROC(const string& col = "close", size_t N = 12, size_t M = 6) noexcept
    {
        auto ind            = m_sda_ptr->make_indicator();
        roc_return_type rst = ind->ROC(col.c_str(), N, M);
        return rst;
    }

    ~PyStockDay() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    StockDayActionPtr m_sda_ptr{nullptr};
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};
};

namespace py = pybind11;

PYBIND11_MODULE(pyabqstockday, m)
{
    m.doc() = R"pbdoc(
        Pybind11 StockDay plugin
        -----------------------

        .. pyabqstockday:: pyabqstockday_module_exmaple

        .. autosummary::
           :stockday: to_series

           to_series
    )pbdoc";

    py::class_<PyStockDay, std::shared_ptr<PyStockDay>> sm_class(m, "PyStockDay");
    sm_class
        .def(py::init<std::vector<std::string>, const string, const string, FQ_TYPE>())
        // .def("toQfq", &PyStockDay::toQfq, R"pbdoc(toQfq qfq function.)pbdoc")
        .def("to_series", &PyStockDay::to_series<double> /* , py::return_value_policy::reference */,
             R"pbdoc(to_series double function.)pbdoc")
        .def("to_series_string", &PyStockDay::to_series<std::string>, R"pbdoc(to_series string function.)pbdoc")
        .def("ROC", &PyStockDay::ROC, R"pbdoc(ROC function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
} // namespace abq
