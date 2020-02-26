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
#include "abquant/models/stockday.h" //  include the model class

namespace abq
{
using namespace std;

class StockDayAction : public StockAction<StockDayAction>
{
public:
    //! Default constructor
    StockDayAction() = delete;

    StockDayAction(QStringList codes, const char* start, const char* end);

    //! Copy constructor
    StockDayAction(const StockDayAction& other) = default;

    //! Move constructor
    StockDayAction(StockDayAction&& other) noexcept = default;

    //! Destructor
    virtual ~StockDayAction() noexcept = default;

    //! Copy assignment operator
    StockDayAction& operator=(const StockDayAction& other) = default;

    //! Move assignment operator
    StockDayAction& operator=(StockDayAction&& other) noexcept = default;

    inline QList<StockDay> getStocks() const { return m_stockdays; };
    inline QVector<const char*> getColumns() const { return m_columns; };
    MyDataFrame toFq(FQ_TYPE fq = FQ_TYPE::NONE);
    MyDataFrame toDataFrame() const;

    template <typename T>
    QMap<const char*, QVector<T>> toSerie(const char*) const noexcept;

private:
    QList<StockDay> m_stockdays;
    const QVector<const char*> m_columns = {"open",   "close", "high", "low",       "vol",
                                            "amount", "date",  "code", "date_stamp"};
    QStringList m_codes;
    const char* m_start;
    const char* m_end;

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
QMap<const char*, QVector<T>> StockDayAction::toSerie(const char* col) const noexcept
{
    QVector<T> data;
    QMap<const char*, QVector<T>> serie;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return serie;
    }
    for (auto s : getStocks()) {
        if constexpr (std::is_same_v<T, QString>) {
            if (QString("code") == QString(col)) {
                data << s.code();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("close") == QString(col)) {
                data << s.close();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("high") == QString(col)) {
                data << s.high();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("low") == QString(col)) {
                data << s.low();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, QString>) {
            if (QString("date") == QString(col)) {
                data << s.date();
                continue;
            }
        }
    }
    serie.insert(col, data);
    return serie;
}
} // namespace abq
