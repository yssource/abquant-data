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
#include "abquant/actions/securitylist.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/indexlist.h"
#include "abquant/models/stocklist.h"

namespace abq
{
using StockListActionPtr = std::shared_ptr<SecurityListAction<StockList>>;
using IndexListActionPtr = std::shared_ptr<SecurityListAction<IndexList>>;

class PySecurityList : public std::enable_shared_from_this<PySecurityList>
{
public:
    PySecurityList(INSTRUMENT_TYPE ins_type) : m_ins_type{ins_type}
    {
        if (ins_type == INSTRUMENT_TYPE::CS) {
            m_sla_ptr = std::make_shared<SecurityListAction<StockList>>();
        }
        if (ins_type == INSTRUMENT_TYPE::INDX) {
            m_ila_ptr = std::make_shared<SecurityListAction<IndexList>>();
        }
    };

    PySecurityList(std::vector<std::string> codes, const string& end = "",
                   INSTRUMENT_TYPE ins_type = INSTRUMENT_TYPE::CS)
        : m_codes{codes}, m_end{end}, m_ins_type{ins_type}
    {
        QStringList qcodes;
        for (auto c : codes) {
            qcodes << QString::fromStdString(c);
        }
        if (ins_type == INSTRUMENT_TYPE::CS) {
            m_sla_ptr = std::make_shared<SecurityListAction<StockList>>(qcodes, end.c_str());
        }
        if (ins_type == INSTRUMENT_TYPE::INDX) {
            m_ila_ptr = std::make_shared<SecurityListAction<IndexList>>(qcodes, end.c_str());
        }
    };

    template <class T>
    std::vector<T> to_series(const string& col) noexcept
    {
        QVector<T> series;
        if (m_ins_type == INSTRUMENT_TYPE::CS) {
            series = m_sla_ptr->to_series<T>(col.c_str());
        }
        if (m_ins_type == INSTRUMENT_TYPE::INDX) {
            series = m_ila_ptr->to_series<T>(col.c_str());
        }
        return series.toStdVector();
    }

    ~PySecurityList() = default;

private:
    std::vector<std::string> m_codes;
    const string m_end;
    INSTRUMENT_TYPE m_ins_type;
    StockListActionPtr m_sla_ptr{nullptr};
    IndexListActionPtr m_ila_ptr{nullptr};
};

namespace py = pybind11;

PYBIND11_MODULE(pyabqsecuritylist, m)
{
    m.doc() = R"pbdoc(
        Pybind11 SecurityList plugin
        -----------------------

        .. pyabqsecuritylist:: pyabqsecuritylist_module_exmaple

        .. autosummary::
           :securitylist: to_series

           to_series
    )pbdoc";

    py::class_<MyDataFrame, MyDataFramePtr>(m, "MyDataFrame");
    py::class_<PySecurityList, std::shared_ptr<PySecurityList>> sm_class(m, "PySecurityList");
    sm_class.def(py::init<INSTRUMENT_TYPE>())
        .def(py::init<std::vector<std::string>, const string, INSTRUMENT_TYPE>(), py::arg("codes"), py::arg("end") = "",
             py::arg("ins_type") = INSTRUMENT_TYPE::CS)
        .def("to_series", &PySecurityList::to_series<double> /* , py::return_value_policy::reference */,
             R"pbdoc(to_series double function.)pbdoc")
        .def("to_series_string", &PySecurityList::to_series<std::string>, R"pbdoc(to_series string function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
} // namespace abq
