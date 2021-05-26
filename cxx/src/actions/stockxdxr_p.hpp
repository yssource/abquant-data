/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/stockxdxr.hpp"

namespace abq
{
class StockXdxrAction::impl
{
public:
    impl(StockXdxrAction& sa, QStringList codes, int category);
    MyDataFramePtr getDataFrame(const StockXdxrAction& sa) const;
    MyDataFramePtr toFq(const StockXdxrAction& sa, FQ_TYPE fq);
    QList<StockXdxr> get_securities(const StockXdxrAction&) const { return m_stockxdxrs; };
    QVector<const char*> getColumns(const StockXdxrAction&) const { return m_columns; };

private:
    void setDataFrame();

private:
    QList<StockXdxr> m_stockxdxrs;
    const QVector<const char*> m_columns = {"category",
                                            "name",
                                            "fenhong",
                                            "peigujia",
                                            "songzhuangu",
                                            "peigu",
                                            "suogu",
                                            "liquidity_before",
                                            "liquidity_after",
                                            "shares_before",
                                            "shares_after",
                                            "fenshu",
                                            "xingquanjia",
                                            "date",
                                            "category_meaning",
                                            "code"};
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
