#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <QtCore/QObject>
#include <iostream>
#include <string>
#include <vector>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/indexmin.hpp"
#include "abquant/actions/utils.hpp"

// using namespace std;
namespace abq
{
class PyIndexMin
{
public:
    PyIndexMin(std::vector<std::string> codes, const string& start, const string& end, const string& sfreq = "5min")
        : m_codes{codes}, m_start{start}, m_end{end}, m_sfreq{sfreq}
    {
        MIN_FREQ freq;
        if (QString::fromStdString(m_sfreq) == QString("5min") || QString::fromStdString(m_sfreq) == QString("5m")) {
            freq = MIN_FREQ::FIVE;
        }

        QStringList qcodes;
        for (auto c : m_codes) {
            qcodes << QString::fromStdString(c);
        }
        m_ima = IndexMinAction(qcodes, m_start.c_str(), m_end.c_str(), freq);
    };

    template <class T>
    std::vector<T> to_series(const string& col) const noexcept
    {
        auto series = m_ima.to_series<T>(col.c_str());
        return series.toStdVector();
    }
    ~PyIndexMin() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    const string m_sfreq{};
    IndexMinAction m_ima{};
};

namespace py = pybind11;

PYBIND11_MODULE(pyabqindexmin, m)
{
    m.doc() = R"pbdoc(
        Pybind11 IndexMin plugin
        -----------------------

        .. pyabqindexmin:: currentmodule_exmaple

        .. autosummary::
           :indexmin: to_series

           to_series
    )pbdoc";

    py::class_<PyIndexMin> sm_class(m, "PyIndexMin");
    sm_class.def(py::init<std::vector<std::string>, const string, const string, const string>())
        .def("to_series", &PyIndexMin::to_series<double>, R"pbdoc(
        to_series double

        to_series double function.
    )pbdoc")
        .def("to_series_string", &PyIndexMin::to_series<std::string>, R"pbdoc(
        to_series string

        to_series string function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
