/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
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
    MyDataFramePtr get_dataframe(const StockDayAction& sa) const;
    MyDataFramePtr to_fq_type(FQ_TYPE fq);
    QStringList get_codes(const StockDayAction&) const { return m_codes; };
    QList<StockDay> get_securities(const StockDayAction&) const { return m_stockdays; };
    QVector<const char*> get_columns(const StockDayAction&) const { return m_columns; };

private:
    void set_dataframe();

private:
    QList<StockDay> m_stockdays;
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
