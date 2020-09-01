/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <QDebug>
#include <chrono>
#include <iostream>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/indexmin.hpp"
#include "abquant/actions/utils.hpp"

using namespace abq;

int main(int argc, char* argv[])
{
    Abquant::start();

    QStringList codes = {"000001", "000002", "000003"};
    // QStringList codes = {"000001"};
    const char* start = "2017-01-01 00:00:00";
    const char* end   = "2019-12-01 23:55:00";
    MIN_FREQ freq     = MIN_FREQ::FIVE;

    IndexMinAction ima(codes, start, end, freq);
    ima.setDataFrame();

    int N      = 10;
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        // qDebug() << i;
        std::cout << i << "\n";
        auto df = ima.getDataFrame();
        // df->write<std::ostream, std::string, double, int>(std::cout);
    }
    auto finish_                          = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish_ - begin;
    // qDebug() << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    Abquant::finish();
    return 0;
}
