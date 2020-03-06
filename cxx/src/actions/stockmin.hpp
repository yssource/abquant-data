/****************************************************************************
 * Copyright (c) 2020-2016 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <QDebug>
#include <iostream>

#include "abquant/actions/stock.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/actions/xdxr.hpp"
#include "abquant/models/stockmin.h" //  include the model class

namespace abq
{
using namespace std;

class StockMinAction : public StockAction<StockMinAction>
{
public:
    //! Default constructor
    StockMinAction() = delete;

    StockMinAction(QStringList codes, const char* start, const char* end, MIN_FREQ freq = MIN_FREQ::ONE);

    //! Copy constructor
    StockMinAction(const StockMinAction& other) = default;

    //! Move constructor
    StockMinAction(StockMinAction&& other) noexcept = default;

    //! Destructor
    virtual ~StockMinAction() noexcept = default;

    //! Copy assignment operator
    StockMinAction& operator=(const StockMinAction& other) = default;

    //! Move assignment operator
    StockMinAction& operator=(StockMinAction&& other) noexcept = default;

    inline QList<StockMin> getStocks() const { return m_stockmins; };
    inline QVector<const char*> getColumns() const { return m_columns; };

    MyDataFrame toFq(FQ_TYPE fq = FQ_TYPE::NONE);
    MyDataFrame toDataFrame() const;

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

private:
    QList<StockMin> m_stockmins;
    const QVector<const char*> m_columns = {"open",     "close", "high", "low",       "vol",       "amount",
                                            "datetime", "code",  "date", "datestamp", "timestamp", "type"};
    QStringList m_codes;
    const char* m_start;
    const char* m_end;
    MIN_FREQ m_freq;

private:
    friend inline QDebug operator<<(QDebug d, const StockMinAction& sa)
    {
        QVector<const char*> columns = sa.getColumns();
        d << columns << "\n";
        auto qs = sa.getStocks();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.open()) << QVariant(s.close()) << QVariant(s.high()) << QVariant(s.low())
                      << QVariant(s.vol()) << QVariant(s.amount()) << QVariant(s.datetime()) << QVariant(s.date())
                      << QVariant(s.code()) << QVariant(s.dateStamp()) << QVariant(s.timeStamp()) << QVariant(s.type());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> StockMinAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
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
        if constexpr (std::is_same_v<T, double>) {
            if (QString("datetime") == QString(col)) {
                series << s.datetime();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, QString>) {
            if (QString("code") == QString(col)) {
                series << s.code();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, QString>) {
            if (QString("date") == QString(col)) {
                series << s.date();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("datestamp") == QString(col)) {
                series << s.dateStamp();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("timestamp") == QString(col)) {
                series << s.timeStamp();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, QString>) {
            if (QString("type") == QString(col)) {
                series << s.type();
                continue;
            }
        }
    }
    return series;
}
} // namespace abq
