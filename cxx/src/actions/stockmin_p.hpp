/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/stockmin.hpp"

namespace abq
{
class StockMinAction::impl
{
public:
    impl(StockMinAction& sa, QStringList codes, const char* start, const char* end, MIN_FREQ freq, FQ_TYPE xdxr);
    MyDataFramePtr getDataFrame(const StockMinAction& sa) const;
    MyDataFramePtr toFq(FQ_TYPE fq);
    QStringList getCodes(const StockMinAction&) const { return m_codes; };
    QList<StockMin> get_securities(const StockMinAction&) const { return m_stockmins; };
    QVector<const char*> getColumns(const StockMinAction&) const { return m_columns; };

private:
    void setDataFrame();

private:
    QList<StockMin> m_stockmins;
    const QVector<const char*> m_columns = {"open",     "close", "high", "low",        "vol",        "amount",
                                            "datetime", "code",  "date", "date_stamp", "time_stamp", "type"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
