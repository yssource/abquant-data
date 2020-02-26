#pragma once

#include <QDebug>

#include "abquant/actions/stock.hpp"

namespace abq
{
template <typename SA>
class Indicator
{
public:
    using stockaction_type = SA;

    Indicator(stockaction_type* sa);

    template <typename T>
    void hello(T h);

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
