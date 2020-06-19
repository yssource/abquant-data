/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <QDebug>
#include <algorithm>
#include <iostream>

#include "abquant/actions/stock.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/actions/xdxr.hpp"
#include "abquant/models/stockday.h" //  include the model class

namespace abq
{
using namespace std;

class StockDayAction : public StockAction<StockDayAction>
{
public:
    //! Default constructor
    StockDayAction() = default;

    StockDayAction(QStringList codes, const char* start, const char* end, FQ_TYPE xdxr = FQ_TYPE::NONE);

    //! Copy constructor
    StockDayAction(const StockDayAction& other) = delete;

    //! Move constructor
    StockDayAction(StockDayAction&& other) noexcept = delete;

    //! Destructor
    ~StockDayAction() noexcept = default;

    //! Copy assignment operator
    StockDayAction& operator=(const StockDayAction& other) = default;

    //! Move assignment operator
    StockDayAction& operator=(StockDayAction&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }
        std::swap(m_codes, other.m_codes);
        std::swap(m_start, other.m_start);
        std::swap(m_end, other.m_end);
        std::swap(m_stockdays, other.m_stockdays);
        std::swap(m_xdxr, other.m_xdxr);
        return *this;
    };

    inline QList<StockDay> getStocks() const { return m_stockdays; };
    inline QVector<const char*> getColumns() const { return m_columns; };
    const MyDataFrame toFq(FQ_TYPE fq = FQ_TYPE::NONE) const;
    MyDataFrame toDataFrame() const;
    std::shared_ptr<MyDataFrame> getDataFrame() const;
    vector<double> getOpen() const;
    void setDataFrame();

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    // FIXME: a workaround for pybind11, maybe a bug that pybind11 does not work well with template, since unable get
    // MyDataFrame for binding
    std::vector<double> get_pyseries(const char*) const noexcept;

    // template <>
    xt::xarray<double> toSeries(const char*) const noexcept;

private:
    QList<StockDay> m_stockdays{};
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes{};
    const char* m_start{};
    const char* m_end{};
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};
    std::shared_ptr<MyDataFrame> m_df{nullptr};

private:
    friend inline QDebug operator<<(QDebug d, const StockDayAction& sa)
    {
        QVector<const char*> columns = sa.getColumns();
        d << columns << "\n";
        auto qs = sa.getStocks();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.open()) << QVariant(s.close()) << QVariant(s.high()) << QVariant(s.low())
                      << QVariant(s.vol()) << QVariant(s.amount()) << QVariant(s.date()) << QVariant(s.code())
                      << QVariant(s.dateStamp());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> StockDayAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    if constexpr (std::is_same_v<T, double>) {
        if (m_xdxr == FQ_TYPE::PRE || m_xdxr == FQ_TYPE::POST) {
            std::shared_ptr<MyDataFrame> df;

            std::vector<T> seriese;

            try {
                // df = toFq(m_xdxr);
                df = getDataFrame();

                // df->write<std::ostream, std::string, double, int>(std::cout);
                const char* colname;
                if (QString(col) == QString("code")) {
                    colname = "lhs.code";
                } else if (QString(col) == QString("date")) {
                    colname = "lhs.date";
                } else {
                    colname = col;
                }

                seriese = df->get_column<T>(colname);
            } catch (...) {
                std::cout << " ... "
                          << "\n";
            }
            return QVector<T>::fromStdVector(seriese);
        }
    }

    for (auto s : getStocks()) {
        if constexpr (std::is_same_v<T, double>) {
            if (QString("open") == QString(col)) {
                series << s.open();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("close") == QString(col)) {
                series << s.close();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("high") == QString(col)) {
                series << s.high();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("low") == QString(col)) {
                series << s.low();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("vol") == QString(col)) {
                series << s.vol();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("amount") == QString(col)) {
                series << s.amount();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("date") == QString(col)) {
                series << s.date().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("code") == QString(col)) {
                series << s.code().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("date_stamp") == QString(col)) {
                series << s.dateStamp();
                continue;
            }
        }
    }
    return series;
}

// template <>
xt::xarray<double> StockDayAction::toSeries(const char* col) const noexcept
{
    std::vector<double> qv = toSeries<double>(col).toStdVector();
    return xt::adapt(qv);
}

} // namespace abq
