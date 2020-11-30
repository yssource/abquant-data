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
        m_cda.setDataFrame();
    };

    template <class T>
    std::vector<T> toSeries(const string& col) const noexcept
    {
        if constexpr (std::is_same_v<T, double>) {
            auto series = m_cda.get_pyseries(col.c_str());
            return series;
        }

        auto series = m_cda.toSeries<T>(col.c_str());
        return series.toStdVector();
    }
    ~PyEmConceptBase() = default;

private:
    std::vector<std::string> m_codes{};
    EmConceptBaseAction m_cda{};
};

namespace py = pybind11;

PYBIND11_MODULE(abqemconceptbase, m)
{
    m.doc() = R"pbdoc(
        Pybind11 EmConceptBase plugin
        -----------------------

        .. abqemconceptbase:: currentmodule_exmaple

        .. autosummary::
           :emcon: toSeries

           toSeries
    )pbdoc";

    py::class_<PyEmConceptBase> sm_class(m, "PyEmConceptBase");
    sm_class.def(py::init<std::vector<std::string>>())
        .def("toSeries", &PyEmConceptBase::toSeries<double>, R"pbdoc(toSeries double toSeries double function.)pbdoc")
        .def("toSeries_int", &PyEmConceptBase::toSeries<int>, R"pbdoc(toSeries int toSeries double function.)pbdoc")
        .def("toSeries_string", &PyEmConceptBase::toSeries<std::string>,
             R"pbdoc(toSeries string toSeries string function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
