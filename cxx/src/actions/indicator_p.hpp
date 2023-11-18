/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include "DataFrame/DataFrame.h"

namespace abq
{
using namespace std;
using namespace hmdf;
using index_type     = std::string;
using MyDataFrame    = StdDataFrame<index_type>;
using MyDataFramePtr = std::shared_ptr<MyDataFrame>;

using xseries_cst_type    = const xt::xarray<double>&;
using xseries_type        = xt::xarray<double>&;
using series_cst_type     = const std::vector<double>&;
using series_type         = std::vector<double>&;
using xseries_no_cvp_type = std::decay<xseries_type>::type;
using series_no_cvp_type  = std::decay<series_type>::type;

using roc_return_type = std::unordered_map<const char*, std::vector<double>>;

namespace indicator
{
xseries_no_cvp_type SMA(xseries_cst_type series, size_t N, size_t M);
xseries_no_cvp_type REF(xseries_cst_type series, int N);
xseries_no_cvp_type DIFF(xt::xarray<index_type> index, xseries_cst_type series, const char* col, long N);
xseries_no_cvp_type MA(xt::xarray<index_type> index, xseries_cst_type series, const char* col, size_t N);
roc_return_type ROC(xt::xarray<index_type> index, xseries_cst_type series, const char* col = "close", size_t N = 12,
                    size_t M = 6);
} // namespace indicator
} // namespace abq
