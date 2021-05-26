/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

// #define private public
#include <QtTest/QtTest>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/security.hpp"
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/stockxdxr.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>

#include "string"

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
    void sma_data();
    void sma();
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

    // // auto ind = sa.make_indicator<StockDay>();
    // auto ind = sa.make_indicator();
    // ind.hello<double>(1.1);
    // // xt::xarray<double> xs = xt::adapt(sa.to_series<double>("open").toStdVector());
    // xt::xarray<double> xs = sa.to_series("open");
    // xt::xarray<double> rs = ind.SMA(xs, 12);

    // StockMinAction sb(codes, start, end);
    // // auto ind2 = sb.make_indicator<StockMin>();
    // auto ind2 = sb.make_indicator();
    // std::string s{"jimmy"s};
    // ind2.hello<std::string>(std::move(s));

    // // StockDayAction sa(codes, start, end);
    // auto m_codes = sa.get_codes();
    // auto df = sa.to_fq_type(FQ_TYPE::PRE);
    // // auto df = sa.to_fq_type(FQ_TYPE::POST);
    // qDebug() << "m_codes main: " << m_codes << "\n";
    // df->write<std::ostream, std::string, double, int>(std::cout);
    qDebug() << sa << "\n";

    // // sa.set_dataframe();
    // auto ddff = sa.get_dataframe();
    // auto ooopen = sa.getOpen();
    // std::cout << ooopen.size() << "\n";

    // StockDayAction sa2(codes, start, end, FQ_TYPE::PRE);
    // // sa2.set_dataframe();
    // auto xls2 = sa2.to_series("open");
    // // auto nn = sa2.getName();

    // sa.getName();

    // std::cout << xls2.size() << "\n";
    // StockMinAction sb(codes, start, end);
    // // auto df2 = sb.to_fq_type(FQ_TYPE::PRE);
    // auto df2 = sb.to_fq_type(FQ_TYPE::POST);
    // qDebug() << sb << "\n";

    StockXdxrAction sc(codes);
    qDebug() << sc << "\n";

    // auto mm0 = sa.to_series<std::string>("code");
    // qDebug() << "------- mm0 " << mm0.size() << "\n";
    // for (auto o : mm0) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n code000 \n";

    // auto mm = sa.to_series<double>("close");
    // for (auto o : mm) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }

    // auto mm2 = sa.to_series<double>("high");
    // for (auto o : mm2) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n close \n";

    // auto mm3 = sa.to_series<QString>("date");
    // for (auto o : mm3) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n date \n";

    // auto mm4 = sc.to_series<double>("peigu");
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

void TestStockDay::sma_data() {}

void TestStockDay::sma() {}

void TestStockDay::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestStockDay)
ABQ_TEST_MAIN(TestStockDay)
#include "main.moc"
