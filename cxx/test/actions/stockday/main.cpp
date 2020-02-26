/****************************************************************************
 * Copyright (c) 2020-2016 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

// #define private public
#include <QtTest/QtTest>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stock.hpp"
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/stockxdxr.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>
#include "string"

// namespace abq {

using namespace abq;
using namespace std;

// static QList<StockDay> stockdays;
// static MyDataFrame df;

class TestStockDay : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    // void getSize_data();
    // void getSize();
    // void getFirstObj_data();
    // void getFirstObj();
    // // void getLastObj_data();
    // // void getLastObj();
    // void toDataFrame_data();
    // void toDataFrame();
    // // void parse();
};

void TestStockDay::initTestCase()
{
    // QStringList codes = {"000001", "000002"};
    QStringList codes = {"000001"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";

    // const char *start = "2019-06-25";
    // const char *end = "2019-06-27";

    StockDayAction sa(codes, start, end);
    // auto ind = sa.makeIndicator<StockDay>();
    auto ind = sa.makeIndicator();
    ind.hello<double>(1.1);

    StockMinAction sb(codes, start, end);
    // auto ind2 = sb.makeIndicator<StockMin>();
    auto ind2 = sb.makeIndicator();
    std::string s{"jimmy"s};
    ind2.hello<std::string>(std::move(s));

    // StockDayAction sa(codes, start, end);
    // auto m_codes = sa.getCodes();
    // auto df = sa.toFq(FQ_TYPE::PRE);
    // // auto df = sa.toFq(FQ_TYPE::POST);
    // qDebug() << "m_codes main: " << m_codes << "\n";
    // qDebug() << sa << "\n";

    // StockMinAction sb(codes, start, end);
    // // auto df2 = sb.toFq(FQ_TYPE::PRE);
    // auto df2 = sb.toFq(FQ_TYPE::POST);
    // qDebug() << sb << "\n";

    // StockXdxrAction sc(codes);
    // qDebug() << sc << "\n";

    // auto mm0 = sa.toSerie<QString>("code");
    // for (auto o : mm0) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n code000 \n";

    // auto mm = sa.toSerie<double>("close");
    // for (auto o : mm) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }

    // auto mm2 = sa.toSerie<double>("high");
    // for (auto o : mm2) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n close \n";

    // auto mm3 = sa.toSerie<QString>("date");
    // for (auto o : mm3) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n date \n";

    // auto mm4 = sc.toSerie<double>("peigu");
    // for (auto o : mm4) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n xdxr peigu \n";

    // qDebug() << sb << "\n";
    // stockdays = sa.run(codes, start, end);
    // qDebug() << "------ stockdays.size ----: " << stockdays.size() << "\n";
    // df = sa.toDataFrame();
    // df = sa.to_fq(FQ_TYPE::PRE);
}

void TestStockDay::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestStockDay)
ABQ_TEST_MAIN(TestStockDay)
#include "main.moc"

// } // namespace abq
