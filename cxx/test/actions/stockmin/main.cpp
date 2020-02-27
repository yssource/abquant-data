/****************************************************************************
 * Copyright (c) 2020-2016 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <chrono>
// #include <iostream>

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
    // auto df2 = sb.toFq(FQ_TYPE::PRE);
    // auto df3 = sb.toFq(FQ_TYPE::POST);
    int N      = 10;
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        qDebug() << i;
        sb.toFq(FQ_TYPE::PRE);
    }
    auto finish_ = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish_ - begin;
    qDebug() << "Elapsed time: " << elapsed.count() << " s\n";
    // qDebug() << sb << "\n";

    return 0;
}
