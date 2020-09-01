/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "indicator.hpp"

namespace abq
{
namespace indicator
{
xt::xarray<double> SMA(series_t series, size_t N, size_t M)
{
    double preY = *series.begin();
    std::vector<double> result;
    result.reserve(series.size());
    std::transform(std::begin(series), std::end(series), std::back_inserter(result), [N, M, &preY](double i) -> double {
        auto r = ((M * i) + (N - M) * preY) / N;
        preY   = r;
        return r;
    });

    return xt::eval(xt::adapt(result));
}

xt::xarray<double> DIFF(xt::xarray<std::string> index, series_t series, const char* col, long N)
{
    std::vector<const char*> cols = {"open", "close", "high", "low", "vol", "amount"};
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        QString msg = QString(col) + QString(" series are not available for DIFF.");
        throw std::runtime_error(msg.toStdString());
    }

    typedef StdDataFrame<std::string> MyDataFrame;
    MyDataFrame::set_thread_level(10);
    MyDataFrame df;

    auto idx                  = std::vector<std::string>(index.begin(), index.end());
    auto data                 = std::vector<double>(series.begin(), series.end());
    MyDataFrame::size_type rc = df.load_data(std::move(idx), std::make_pair(col, data));
    DiffVisitor<double> diff_visit(N, false);
    const auto& result = df.single_act_visit<double>(col, diff_visit).get_result();

    return xt::eval(xt::adapt(result));
}

} // namespace indicator

} // namespace abq
