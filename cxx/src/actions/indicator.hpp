#pragma once

#include <QDebug>

#include "abquant/actions/stock.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"

namespace abq
{
// template<typename T>
struct PySeries {
    size_t index;
    // T data;
    double data;
};

template <typename SA>
class Indicator
{
public:
    using stockaction_type = SA;
    using series_t         = const xt::xarray<double>&;

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
    // auto DIFF(xt::xarray<size_t>, xt::xarray<double>, size_t);
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
    // auto qs = sa->getCodes();
    // qDebug() << "indicator qs count: " << qs.count() << " : " << qs <<"\n";
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
    double preY = *series.begin();
    std::vector<double> result;
    result.reserve(series.size());
    std::transform(std::begin(series), std::end(series), std::back_inserter(result), [N, M, &preY](double i) -> double {
        auto r = ((M * i) + (N - M) * preY) / N;
        preY = r;
        return r;
    });

    return xt::eval(xt::adapt(result));
}

} // namespace abq
