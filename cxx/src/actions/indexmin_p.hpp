/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/indexmin.hpp"

namespace abq
{
class IndexMinAction::impl
{
public:
    impl(IndexMinAction& sa, QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    MyDataFramePtr getDataFrame(const IndexMinAction& sa) const;
    QStringList getCodes(const IndexMinAction&) const { return m_codes; };
    QList<IndexMin> getIndexes(const IndexMinAction&) const { return m_indexmins; };
    QVector<const char*> getColumns(const IndexMinAction&) const { return m_columns; };

private:
    void setDataFrame();

private:
    QList<IndexMin> m_indexmins;
    const QVector<const char*> m_columns = {"open",     "close", "high", "low",        "vol",        "amount",
                                            "datetime", "code",  "date", "date_stamp", "time_stamp", "type"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
