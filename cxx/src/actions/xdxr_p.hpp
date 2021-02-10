/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "abquant/actions/xdxr.hpp"

namespace abq
{
class Xdxr::impl
{
public:
    impl(const StockDayAction&);
    impl(const StockMinAction&);
    MyDataFramePtr getXdxr(const MyDataFramePtr df, FQ_TYPE fq);

private:
    void fillConcatDataframe(MyDataFramePtr df) const;
    MyDataFramePtr concat(const MyDataFramePtr ldf, const MyDataFrame& rdf) const;
    MyDataFramePtr calc(const MyDataFramePtr df, FQ_TYPE fq) const;

private:
    QStringList m_codes;
    MyDataFramePtr m_xdxr_df;
};
} // namespace abq
