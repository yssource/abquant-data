/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/stocklist.hpp"

namespace abq
{
class StockListAction::impl
{
public:
    impl(StockListAction& sa);
    // impl(StockListAction& sa, const char* end = "");
    impl(StockListAction& sa, const QStringList codes, const char* end = "");
    MyDataFramePtr getDataFrame(const StockListAction& sa) const;
    QStringList getCodes(const StockListAction&) const { return m_codes; };
    QList<StockList> getStocks(const StockListAction&) const { return m_stocklists; };
    QVector<const char*> getColumns(const StockListAction&) const { return m_columns; };

private:
    void setDataFrame();

private:
    QList<StockList> m_stocklists;
    const QVector<const char*> m_columns{"code", "volunit", "decimal_point", "name", "pre_close", "sse", "sec"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
