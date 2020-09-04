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
#include "abquant/actions/index.hpp"
#include "abquant/actions/indexday.hpp"
// #include "abquant/actions/indexmin.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>

#include "string"

using namespace abq;
using namespace std;

// static QList<IndexDay> indexdays;
// static MyDataFrame df;

class TestIndexDay : public QObject
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
    // void sma_data();
    // void sma();
};

void TestIndexDay::initTestCase()
{
    // QStringList codes = {"000001", "000002"};
    QStringList codes = {"000001"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";

    // const char *start = "2019-06-25";
    // const char *end = "2019-06-27";

    IndexDayAction ia(codes, start, end);

    // auto ind = ia.makeIndicator<IndexDay>();

    auto ind = ia.makeIndicator();
    ind.hello<double>(1.1);

    // // xt::xarray<double> xs = xt::adapt(sa.toSeries<double>("open").toStdVector());
    // xt::xarray<double> xs = ia.toSeries("open");
    // // xt::xarray<double> rs = ind.SMA(xs, 12);

    // // IndexMinAction sb(codes, start, end);
    // // // auto ind2 = sb.makeIndicator<IndexMin>();
    // // auto ind2 = sb.makeIndicator();
    // // std::string s{"jimmy"s};
    // // ind2.hello<std::string>(std::move(s));

    // // // IndexDayAction ia(codes, start, end);
    // // auto m_codes = ia.getCodes();
    // // auto df      = ia.toFq(FQ_TYPE::PRE);
    // // // auto df = ia.toFq(FQ_TYPE::POST);
    // // qDebug() << "m_codes main: " << m_codes << "\n";
    // // df.write<std::ostream, std::string, double, int>(std::cout);
    // // qDebug() << ia << "\n";

    // ia.setDataFrame();
    // auto ddff   = ia.getDataFrame();
    // auto ooopen = ia.getOpen();
    // std::cout << ooopen.size() << "\n";

    // IndexDayAction ia2(codes, start, end, FQ_TYPE::PRE);
    // ia2.setDataFrame();
    // auto xls2 = ia2.toSeries("open");
    // std::cout << xls2.size() << "\n";
    // // IndexMinAction sb(codes, start, end);
    // // // auto df2 = sb.toFq(FQ_TYPE::PRE);
    // // auto df2 = sb.toFq(FQ_TYPE::POST);
    // // qDebug() << sb << "\n";

    // IndexXdxrAction sc(codes);
    // qDebug() << sc << "\n";

    // auto mm0 = ia.toSeries<std::string>("code");
    // qDebug() << "------- mm0 " << mm0.size() << "\n";
    // for (auto o : mm0) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n code000 \n";

    // auto mm = ia.toSeries<double>("close");
    // for (auto o : mm) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }

    // auto mm2 = ia.toSeries<double>("high");
    // for (auto o : mm2) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n close \n";

    // auto mm3 = ia.toSeries<QString>("date");
    // for (auto o : mm3) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n date \n";

    // auto mm4 = sc.toSeries<double>("peigu");
    // for (auto o : mm4) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n xdxr peigu \n";

    // qDebug() << sb << "\n";
    // indexdays = ia.run(codes, start, end);
    // qDebug() << "------ indexdays.size ----: " << indexdays.size() << "\n";
    // df = ia.toDataFrame();
    // df = ia.to_fq(FQ_TYPE::PRE);
}

// void TestIndexDay::sma_data() {}

// void TestIndexDay::sma() {}

void TestIndexDay::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestIndexDay)
ABQ_TEST_MAIN(TestIndexDay)
#include "main.moc"
