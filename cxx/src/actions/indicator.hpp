#pragma once

#include <QDebug>

#include "abquant/actions/stock.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xarray.hpp"

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

    Indicator(stockaction_type* sa);

    template <typename T>
    void hello(T h);

    // auto DIFF(xt::xarray<size_t>, xt::xarray<double>, size_t);
    // xt::xarray<double> HHV(xt::xarray<double>, size_t);
    // xt::xarray<double> LLV(xt::xarray<double>, size_t);
    // void SUM(size_t);
    // auto IF_(xt::xarray<double>, xt::xarray<double>, xt::xarray<double>);
    // void MAX();
    // void MIN();
    // void REF(int);
    // void CROSS();
    // xt::xarray<double> SMA(xt::xarray<double>, size_t, size_t);
    // void KDJ(size_t, size_t, size_t);

    ~Indicator() = default;

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

} // namespace abq
