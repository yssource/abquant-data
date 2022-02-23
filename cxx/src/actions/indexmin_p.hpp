/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
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
    MyDataFramePtr get_dataframe(const IndexMinAction& sa) const;
    QStringList get_codes(const IndexMinAction&) const { return m_codes; };
    QList<IndexMin> get_securities(const IndexMinAction&) const { return m_indexmins; };
    QVector<const char*> get_columns(const IndexMinAction&) const { return m_columns; };

private:
    void set_dataframe();

private:
    QList<IndexMin> m_indexmins;
    const QVector<const char*> m_columns = {"open",     "close", "high", "low",        "vol",        "amount",
                                            "datetime", "code",  "date", "date_stamp", "time_stamp", "type"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
