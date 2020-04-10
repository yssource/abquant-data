#pragma once

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

} // namespace indicator

} // namespace abq
