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
xt::xarray<double> SMA(xseries_cst_type series, size_t N, size_t M)
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

xt::xarray<double> MA(xt::xarray<index_type> index, xseries_cst_type series, const char* col, size_t N)
{
    CHECK_COLUMN_EXIST(col)

    MyDataFrame::set_thread_level(10);
    MyDataFrame df;

    auto idx  = std::vector<index_type>(index.begin(), index.end());
    auto data = std::vector<double>(series.begin(), series.end());
    df.load_data(std::move(idx), std::make_pair(col, data));
    SimpleRollAdopter<MinVisitor<double, index_type>, double> min_roller(MinVisitor<double, index_type>(), N);
    const auto& result = df.single_act_visit<double>(col, min_roller).get_result();

    return xt::eval(xt::adapt(result));
}

xt::xarray<double> REF(xseries_cst_type series, int N)
{
    xt::xarray<double> result;
    try {
        std::cout << series << "\n";
        result = xt::roll(series, N);
    } catch (...) {
        std::cout << "REF error"
                  << "\n";
    }
    auto rst = xt::eval(result);
    return rst;
}

xt::xarray<double> DIFF(xt::xarray<index_type> index, xseries_cst_type series, const char* col, long N)
{
    CHECK_COLUMN_EXIST(col)

    MyDataFrame::set_thread_level(10);
    MyDataFrame df;

    auto idx  = std::vector<index_type>(index.begin(), index.end());
    auto data = std::vector<double>(series.begin(), series.end());
    df.load_data(std::move(idx), std::make_pair(col, data));
    DiffVisitor<double> diff_visit(N, false);
    const auto& result = df.single_act_visit<double>(col, diff_visit).get_result();

    return xt::eval(xt::adapt(result));
}

roc_return_type ROC(xt::xarray<index_type> index, xseries_cst_type series, const char* col, size_t N, size_t M)
{
    CHECK_COLUMN_EXIST(col)

    auto ref   = REF(series, N);
    auto roc   = 100 * (series - ref) / ref;
    auto rocma = MA(index, roc, col, M);
    roc_return_type result;
    result["ROC"]   = series_no_cvp_type(roc.begin(), roc.end());
    result["ROCMA"] = series_no_cvp_type(rocma.begin(), rocma.end());
    return result;
}

} // namespace indicator

} // namespace abq
