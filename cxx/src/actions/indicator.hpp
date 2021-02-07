/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <QDebug>
#include <stdexcept>

#include "DataFrame/DataFrame.h"
#include "DataFrame/DataFrameStatsVisitors.h"
#include "abquant/actions/utils.hpp"
#include "iostream"
#include "xtensor/xadapt.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"

namespace abq
{
using namespace std;
using namespace hmdf;
using index_t        = std::string;
using MyDataFrame    = StdDataFrame<index_t>;
using MyDataFramePtr = std::shared_ptr<MyDataFrame>;

using xseries_cst_t    = const xt::xarray<double>&;
using xseries_t        = xt::xarray<double>&;
using series_cst_t     = const std::vector<double>&;
using series_t         = std::vector<double>&;
using xseries_no_cvp_t = std::decay<xseries_t>::type;
using series_no_cvp_t  = std::decay<series_t>::type;

using roc_return_t = std::unordered_map<const char*, xt::xarray<double>>;

template <class A>
class StockAction;
template <class A>
class IndexAction;

// template<typename T>
struct PySeries {
    size_t index;
    // T data;
    double data;
};

// abq::indicator outer interface for python binding
namespace indicator
{
xt::xarray<double> SMA(xseries_cst_t series, size_t N, size_t M);
xt::xarray<double> REF(xseries_cst_t series, int N);
xt::xarray<double> DIFF(xt::xarray<index_t> index, xseries_cst_t series, const char* col, long N);
xt::xarray<double> MA(xt::xarray<index_t> index, xseries_cst_t series, const char* col, size_t N);
roc_return_t ROC(xt::xarray<index_t> index, xseries_cst_t series, const char* col = "close", size_t N = 12,
                 size_t M = 6);
} // namespace indicator

template <typename A>
class Indicator : public std::enable_shared_from_this<Indicator<A>>
{
public:
    Indicator() = default;

    Indicator(const A* a);

    //! Copy constructor
    Indicator(const Indicator& other) = default;

    //! Move constructor
    Indicator(Indicator&& other) noexcept = default;

    //! Copy assignment operator
    Indicator& operator=(const Indicator& other) = default;

    //! Move assignment operator
    Indicator& operator=(Indicator&& other) noexcept = default;

    ~Indicator() noexcept = default;

    template <typename T>
    void hello(T h);

    // 威廉 SMA 算法
    xt::xarray<double> SMA(xseries_cst_t series, size_t N, size_t M = 1) const;
    xt::xarray<double> REF(xseries_cst_t series, int N) const;
    xt::xarray<double> DIFF(const char* col, long N = 1) const;
    xt::xarray<double> MA(const char* col, size_t N = 1) const;
    roc_return_t ROC(const char* col = "close", size_t N = 12, size_t M = 6) const;

    // xt::xarray<double> HHV(xt::xarray<double>, size_t);
    // xt::xarray<double> LLV(xt::xarray<double>, size_t);
    // void SUM(size_t);
    // auto IF_(xt::xarray<double>, xt::xarray<double>, xt::xarray<double>);
    // void MAX();
    // void MIN();
    // void CROSS();
    // void KDJ(size_t, size_t, size_t);

private:
    const char* m_hello{"hello world!"};
    const A* m_a;
};

template <typename A>
Indicator<A>::Indicator(const A* a) : m_a(std::move(a))
{
    auto df = a->getDataFrame();
    // df->template write<std::ostream, std::string, double, int>(std::cout);
    using action_t = typename std::decay<typename std::remove_pointer<decltype(a)>::type>::type;
    if constexpr (abq::is_base_of_template<IndexAction, action_t>::value) {
        auto qs = a->getIndexes();
    }
    if constexpr (abq::is_base_of_template<StockAction, action_t>::value) {
        auto qs = a->getStocks();
    }
}

template <typename A>
template <typename T>
void Indicator<A>::hello(T h)
{
    qDebug() << m_hello << " "
             << "\n";
}

template <typename A>
xt::xarray<double> Indicator<A>::SMA(xseries_cst_t series, size_t N, size_t M) const
{
    return abq::indicator::SMA(series, N, M);
}

template <typename A>
xt::xarray<double> Indicator<A>::REF(xseries_cst_t series, int N) const
{
    auto rst = abq::indicator::REF(series, N);
    return rst;
}

template <typename A>
xt::xarray<double> Indicator<A>::MA(const char* col, size_t N) const
{
    CHECK_COLUMN_EXIST(col)

    auto df = m_a->getDataFrame();

    SimpleRollAdopter<MinVisitor<double, index_t>, double> min_roller(MinVisitor<double, index_t>(), N);
    const auto& result = df->template single_act_visit<double>(col, min_roller).get_result();

    return xt::eval(xt::adapt(result));
}

template <typename A>
xt::xarray<double> Indicator<A>::DIFF(const char* col, long N) const
{
    CHECK_COLUMN_EXIST(col)

    auto df = m_a->getDataFrame();

    DiffVisitor<double> diff_visit(N, false);
    const auto& result = df->template single_act_visit<double>(col, diff_visit).get_result();

    return xt::eval(xt::adapt(result));
}

template <typename A>
roc_return_t Indicator<A>::ROC(const char* col, size_t N, size_t M) const
{
    CHECK_COLUMN_EXIST(col)

    series_no_cvp_t series;
    try {
        auto df = m_a->getDataFrame();
        // df->template write<std::ostream, index_t, double, int>(std::cout);
        series = df->template get_column<double>(col);
    } catch (...) {
        std::cout << " error ... "
                  << "\n";
    }
    std::cout << xt::adapt(series) << "\n";
    auto ref   = Indicator<A>::REF(xt::adapt(series), N);
    auto roc   = 100 * (xt::adapt(series) - ref) / ref;
    auto rocma = Indicator<A>::MA(col, M);
    roc_return_t result;
    result["ROC"]   = roc;
    result["ROCMA"] = rocma;
    return result;
}
} // namespace abq
