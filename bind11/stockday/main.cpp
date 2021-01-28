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
// PYBIND11_DECLARE_HOLDER_TYPE(MyDataFrame, MyDataFramePtr);

using roc_return_t      = std::unordered_map<const char*, xt::xarray<double>>;
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
        m_df      = m_sda_ptr->getDataFrame();
    };

    size_t toQfq() noexcept
    {
        auto fq = m_sda_ptr->toFq(FQ_TYPE::PRE);
        // fq->write<std::ostream, index_t, double, int>(std::cout);
        return fq->get_index().size();
    }

    template <class T>
    std::vector<T> toSeries(const string& col) noexcept
    {
        auto m_df = m_sda_ptr->getDataFrame();
        if constexpr (std::is_same_v<T, double>) {
            auto series = m_sda_ptr->get_pyseries(col.c_str());
            return series;
        }

        auto series = m_sda_ptr->toSeries<T>(col.c_str());
        return series.toStdVector();
    }

    int ROC(const string& col = "close", size_t N = 12, size_t M = 6) noexcept
    // roc_return_t ROC(const string& col = "close", size_t N = 12, size_t M = 6) noexcept
    {
        auto m_df        = m_sda_ptr->getDataFrame();
        auto ind         = m_sda_ptr->makeIndicator();
        roc_return_t rst = ind.ROC(col.c_str(), N, M);
        // std::cout << rst["ROC"] << "\n";
        return 0;
    }

    ~PyStockDay() = default;

private:
    std::vector<std::string> m_codes{};
    const string m_start{};
    const string m_end{};
    StockDayActionPtr m_sda_ptr{nullptr};
    StockDayAction m_sda;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};
    MyDataFramePtr m_df{nullptr};
    std::shared_ptr<std::string> m_name{nullptr};
};

namespace py = pybind11;

PYBIND11_MODULE(abqstockday, m)
{
    m.doc() = R"pbdoc(
        Pybind11 StockDay plugin
        -----------------------

        .. abqstockday:: abqstockday_module_exmaple

        .. autosummary::
           :stockday: toQfq

           toQfq
    )pbdoc";

    py::class_<MyDataFrame, MyDataFramePtr>(m, "MyDataFrame");
    py::class_<PyStockDay, std::shared_ptr<PyStockDay>> sm_class(m, "PyStockDay");
    sm_class.def(py::init<std::vector<std::string>, const string, const string, FQ_TYPE>())
        .def("toQfq", &PyStockDay::toQfq, R"pbdoc(toQfq qfq function.)pbdoc")
        .def("toSeries", &PyStockDay::toSeries<double> /* , py::return_value_policy::reference */,
             R"pbdoc(toSeries double function.)pbdoc")
        .def("toSeries_string", &PyStockDay::toSeries<std::string>, R"pbdoc(toSeries string function.)pbdoc")
        .def("ROC", &PyStockDay::ROC, R"pbdoc(ROC function.)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

} // namespace abq
