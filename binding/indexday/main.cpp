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
#include "abquant/actions/indexday.hpp"
#include "abquant/actions/utils.hpp"

namespace abq
{
// PYBIND11_DECLARE_HOLDER_TYPE(MyDataFrame, MyDataFramePtr);

using roc_return_type   = std::unordered_map<const char*, std::vector<double>>;
using IndexDayActionPtr = std::shared_ptr<IndexDayAction>;

class PyIndexDay : public std::enable_shared_from_this<PyIndexDay>
{
public:
    PyIndexDay(std::vector<std::string> codes, const string& start, const string& end)
        : m_codes{codes}, m_start{start}, m_end{end}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_ida_ptr = std::make_shared<IndexDayAction>(qcodes, m_start.c_str(), m_end.c_str());
    };

    template <class T>
    std::vector<T> to_series(const string& col) noexcept
    {
        auto series = m_ida_ptr->to_series<T>(col.c_str());
        return series.toStdVector();
    }

    roc_return_type ROC(const string& col = "close", size_t N = 12, size_t M = 6) noexcept
    {
        auto ind            = m_ida_ptr->make_indicator();
        roc_return_type rst = ind->ROC(col.c_str(), N, M);
        return rst;
    }

    ~PyIndexDay() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    IndexDayActionPtr m_ida_ptr{nullptr};
};

namespace py = pybind11;

PYBIND11_MODULE(pyabqindexday, m)
{
    m.doc() = R"pbdoc(
        Pybind11 IndexDay plugin
        -----------------------

        .. pyabqindexday:: pyabqindexday_module_exmaple

        .. autosummary::
           :indexday: to_series

           to_series
    )pbdoc";

    py::class_<MyDataFrame, MyDataFramePtr>(m, "MyDataFrame");
    py::class_<PyIndexDay, std::shared_ptr<PyIndexDay>> sm_class(m, "PyIndexDay");
    sm_class.def(py::init<std::vector<std::string>, const string, const string>())
        .def("to_series", &PyIndexDay::to_series<double> /* , py::return_value_policy::reference */,
             R"pbdoc(to_series double function.)pbdoc")
        .def("to_series_string", &PyIndexDay::to_series<std::string>, R"pbdoc(to_series string function.)pbdoc")
        .def("ROC", &PyIndexDay::ROC, R"pbdoc(ROC function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
} // namespace abq
