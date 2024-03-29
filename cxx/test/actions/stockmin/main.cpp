/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
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
    Abquant::start();

    QStringList codes = {"000001", "000002", "000003"};
    // QStringList codes = {"000001"};
    const char* start = "2022-01-01 00:00:00";
    const char* end   = "2022-12-01 23:55:00";
    MIN_FREQ freq     = MIN_FREQ::FIVE;

    auto begin = std::chrono::high_resolution_clock::now();
    StockMinAction sma(codes, start, end, freq, FQ_TYPE::PRE);
    auto finish_                          = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish_ - begin;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    Abquant::finish();
    return 0;
}
