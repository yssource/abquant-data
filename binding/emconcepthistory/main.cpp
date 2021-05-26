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
#include "abquant/actions/emconcepthistory.hpp"
#include "abquant/actions/utils.hpp"

// using namespace std;
namespace abq
{
class PyEmConceptHistory
{
public:
    PyEmConceptHistory(std::vector<std::string> codes, const string& start, const string& end)
        : m_codes{codes}, m_start{start}, m_end{end}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_cda = EmConceptHistoryAction(qcodes, m_start.c_str(), m_end.c_str());
        m_cda.set_dataframe();
    };

    template <class T>
    std::vector<T> to_series(const string& col) const noexcept
    {
        if constexpr (std::is_same_v<T, double>) {
            auto series = m_cda.get_pyseries(col.c_str());
            return series;
        }

        auto series = m_cda.to_series<T>(col.c_str());
        return series.toStdVector();
    }
    ~PyEmConceptHistory() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    EmConceptHistoryAction m_cda{};
};

namespace py = pybind11;

PYBIND11_MODULE(pyabqemconcepthistory, m)
{
    m.doc() = R"pbdoc(
        Pybind11 EmConceptHistory plugin
        -----------------------

        .. pyabqemconcepthistory:: currentmodule_exmaple

        .. autosummary::
           :emconcepthistory: to_series

           to_series
    )pbdoc";

    py::class_<PyEmConceptHistory> sm_class(m, "PyEmConceptHistory");
    sm_class.def(py::init<std::vector<std::string>, const string, const string>())
        .def("to_series", &PyEmConceptHistory::to_series<double>,
             R"pbdoc(to_series double to_series double function.)pbdoc")
        .def("to_series_string", &PyEmConceptHistory::to_series<std::string>,
             R"pbdoc(to_series string to_series string function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
