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
#include "abquant/actions/emconceptbase.hpp"
#include "abquant/actions/utils.hpp"

// using namespace std;
namespace abq
{
class PyEmConceptBase
{
public:
    PyEmConceptBase(std::vector<std::string> codes) : m_codes{codes}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        m_cda = EmConceptBaseAction(qcodes);
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
    ~PyEmConceptBase() = default;

private:
    std::vector<std::string> m_codes{};
    EmConceptBaseAction m_cda{};
};

namespace py = pybind11;

PYBIND11_MODULE(pyabqemconceptbase, m)
{
    m.doc() = R"pbdoc(
        Pybind11 EmConceptBase plugin
        -----------------------

        .. pyabqemconceptbase:: currentmodule_exmaple

        .. autosummary::
           :emcon: to_series

           to_series
    )pbdoc";

    py::class_<PyEmConceptBase> sm_class(m, "PyEmConceptBase");
    sm_class.def(py::init<std::vector<std::string>>())
        .def("to_series", &PyEmConceptBase::to_series<double>, R"pbdoc(to_series double to_series double function.)pbdoc")
        .def("to_series_int", &PyEmConceptBase::to_series<int>, R"pbdoc(to_series int to_series double function.)pbdoc")
        .def("to_series_string", &PyEmConceptBase::to_series<std::string>,
             R"pbdoc(to_series string to_series string function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
