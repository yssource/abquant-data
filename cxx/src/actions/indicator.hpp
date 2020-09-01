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
#include "abquant/actions/index.hpp"
#include "abquant/actions/stock.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"

namespace abq
{
using namespace hmdf;

// template<typename T>
struct PySeries {
    size_t index;
    // T data;
    double data;
};

using series_t = const xt::xarray<double>&;

namespace indicator
{
xt::xarray<double> SMA(series_t series, size_t N, size_t M);
xt::xarray<double> DIFF(xt::xarray<std::string> index, series_t series, const char* col, long N);
} // namespace indicator

template <typename SA>
class Indicator
{
public:
    using stockaction_type = SA;

    Indicator() = default;

    Indicator(stockaction_type* sa);

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
    xt::xarray<double> SMA(series_t series, size_t N, size_t M = 1) const;
    xt::xarray<double> DIFF(const char* col, long N = 1) const;
    // xt::xarray<double> HHV(xt::xarray<double>, size_t);
    // xt::xarray<double> LLV(xt::xarray<double>, size_t);
    // void SUM(size_t);
    // auto IF_(xt::xarray<double>, xt::xarray<double>, xt::xarray<double>);
    // void MAX();
    // void MIN();
    // void REF(int);
    // void CROSS();
    // void KDJ(size_t, size_t, size_t);

private:
    const char* m_hello{"hello world!"};
    stockaction_type* m_sa;
};

template <typename SA>
Indicator<SA>::Indicator(stockaction_type* sa) : m_sa(sa)
{
    auto qs = sa->getStocks();
    qDebug() << "indicator qs count: " << qs.count() << " : "
             << "qs"
             << "\n";
}

template <typename SA>
template <typename T>
void Indicator<SA>::hello(T h)
{
    qDebug() << m_hello << " "
             << "\n";
}

template <typename SA>
xt::xarray<double> Indicator<SA>::SMA(series_t series, size_t N, size_t M) const
{
    return abq::indicator::SMA(series, N, M);
}

template <typename SA>
xt::xarray<double> Indicator<SA>::DIFF(const char* col, long N) const
{
    std::vector<const char*> cols = {"open", "close", "high", "low", "vol", "amount"};
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        QString msg = QString(col) + QString(" series are not available for DIFF.");
        throw std::runtime_error(msg.toStdString());
    }

    auto df = m_sa->toDataFrame();

    DiffVisitor<double> diff_visit(N, false);
    const auto& result = df.template single_act_visit<double>(col, diff_visit).get_result();

    return xt::eval(xt::adapt(result));
}

} // namespace abq
