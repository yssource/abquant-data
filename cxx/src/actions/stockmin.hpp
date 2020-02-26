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

    StockMinAction(QStringList codes, const char* start, const char* end);

    //! Copy constructor
    StockMinAction(const StockMinAction &other) = default;

    //! Move constructor
    StockMinAction(StockMinAction &&other) noexcept = default;

    //! Destructor
    virtual ~StockMinAction() noexcept = default;

    //! Copy assignment operator
    StockMinAction &operator=(const StockMinAction &other) = default;

    //! Move assignment operator
    StockMinAction &operator=(StockMinAction &&other) noexcept = default;

    inline QList<StockMin> getStocks() const { return m_stockmins; };
    inline QVector<const char*> getColumns() const { return m_columns; };

    MyDataFrame toFq(FQ_TYPE fq = FQ_TYPE::NONE);
    MyDataFrame toDataFrame() const;
private:
    QList<StockMin> m_stockmins;
    const QVector<const char*> m_columns = {"open",     "close", "high", "low",       "vol",       "amount",
                                            "datetime", "code",  "date", "datestamp", "timestamp", "type"};
    QStringList m_codes;
    const char* m_start;
    const char* m_end;
    const char* m_type;

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
} // namespace abq
