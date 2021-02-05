/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/stockday.hpp"

namespace abq
{
class StockDayAction::impl
{
public:
    inline MyDataFramePtr getDataFrame(const StockDayAction& sa) const;
    MyDataFramePtr toFq(const StockDayAction& sa, FQ_TYPE fq);
    inline QStringList getCodes(const StockDayAction&) const { return m_codes; };
    inline QList<StockDay> getStocks(const StockDayAction&) const { return m_stockdays; };
    inline QVector<const char*> getColumns(const StockDayAction&) const { return m_columns; };

    impl(StockDayAction& sa, QStringList codes, const char* start, const char* end, FQ_TYPE xdxr) : m_xdxr{xdxr}
    {
        m_stockdays = sa.run<StockDay>(codes, start, end);
        if (m_stockdays.isEmpty()) {
            qDebug() << "No stock day data.\n"
                     << codes << "\n"
                     << "start: " << start << "\n"
                     << "end: " << end << "\n";
        }
        setDataFrame();
        // m_df->template write<std::ostream, index_t, double, int>(std::cout);
    }

private:
    void setDataFrame();

private:
    QList<StockDay> m_stockdays;
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes;
    const char* m_start;
    const char* m_end;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
