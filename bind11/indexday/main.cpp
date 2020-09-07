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

// using namespace std;
namespace abq
{
class PyIndexDay
{
public:
    PyIndexDay(std::vector<std::string> codes, const string& start, const string& end)
        : m_codes{codes}, m_start{start}, m_end{end}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_ida = IndexDayAction(qcodes, m_start.c_str(), m_end.c_str());
        m_ida.setDataFrame();
    };

    template <class T>
    std::vector<T> toSeries(const string& col) const noexcept
    {
        if constexpr (std::is_same_v<T, double>) {
            auto series = m_ida.get_pyseries(col.c_str());
            return series;
        }

        auto series = m_ida.toSeries<T>(col.c_str());
        return series.toStdVector();
    }
    ~PyIndexDay() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    IndexDayAction m_ida{};
};

namespace py = pybind11;

PYBIND11_MODULE(abqindexday, m)
{
    m.doc() = R"pbdoc(
        Pybind11 IndexDay plugin
        -----------------------

        .. abqindexday:: currentmodule_exmaple

        .. autosummary::
           :indexday: toSeries

           toSeries
    )pbdoc";

    py::class_<PyIndexDay> sm_class(m, "PyIndexDay");
    sm_class.def(py::init<std::vector<std::string>, const string, const string>())
        .def("toSeries", &PyIndexDay::toSeries<double>, R"pbdoc(toSeries double toSeries double function.)pbdoc")
        .def("toSeries_string", &PyIndexDay::toSeries<std::string>,
             R"pbdoc(toSeries string toSeries string function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
