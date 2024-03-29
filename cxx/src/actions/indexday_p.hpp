/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/indexday.hpp"

namespace abq
{
class IndexDayAction::impl
{
public:
    impl(IndexDayAction& sa, QStringList codes, const char* start, const char* end, FQ_TYPE xdxr);
    MyDataFramePtr get_dataframe(const IndexDayAction& sa) const;
    QStringList get_codes(const IndexDayAction&) const { return m_codes; };
    QList<IndexDay> get_securities(const IndexDayAction&) const { return m_indexdays; };
    QVector<const char*> get_columns(const IndexDayAction&) const { return m_columns; };

private:
    void set_dataframe();

private:
    QList<IndexDay> m_indexdays;
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};
};
} // namespace abq
