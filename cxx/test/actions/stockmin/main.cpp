/****************************************************************************
 * Copyright (c) 2020-2016 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/utils.hpp"

using namespace abq;

int main(int argc, char* argv[])
{
    Abquant& a = Abquant::start();
    a.hello();

    QStringList codes = {"000001", "000002", "000003"};
    // QStringList codes = {"000001"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";

    StockMinAction sb(codes, start, end);
    // // auto df2 = sb.toFq(FQ_TYPE::PRE);
    auto df2 = sb.toFq(FQ_TYPE::POST);
    // qDebug() << sb << "\n";

    return 0;
}
