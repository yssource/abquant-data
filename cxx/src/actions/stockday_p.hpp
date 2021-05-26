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
    impl(StockDayAction& sa, QStringList codes, const char* start, const char* end, FQ_TYPE xdxr);
    MyDataFramePtr getDataFrame(const StockDayAction& sa) const;
    MyDataFramePtr toFq(FQ_TYPE fq);
    QStringList getCodes(const StockDayAction&) const { return m_codes; };
    QList<StockDay> get_securities(const StockDayAction&) const { return m_stockdays; };
    QVector<const char*> getColumns(const StockDayAction&) const { return m_columns; };

private:
    void setDataFrame();

private:
    QList<StockDay> m_stockdays;
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
