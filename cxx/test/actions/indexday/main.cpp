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

    // auto ind = ia.make_indicator<IndexDay>();

    auto ind = ia.make_indicator();

    // // xt::xarray<double> xs = xt::adapt(sa.to_series<double>("open").toStdVector());
    // xt::xarray<double> xs = ia.to_series("open");
    // // xt::xarray<double> rs = ind.SMA(xs, 12);

    // // IndexMinAction sb(codes, start, end);
    // // // auto ind2 = sb.make_indicator<IndexMin>();
    // // auto ind2 = sb.make_indicator();
    // // std::string s{"jimmy"s};
    // // ind2.hello<std::string>(std::move(s));

    // // // IndexDayAction ia(codes, start, end);
    // // auto m_codes = ia.get_codes();
    // // auto df      = ia.to_fq_type(FQ_TYPE::PRE);
    // // // auto df = ia.to_fq_type(FQ_TYPE::POST);
    // // qDebug() << "m_codes main: " << m_codes << "\n";
    // // df.write<std::ostream, std::string, double, int>(std::cout);
    // // qDebug() << ia << "\n";

    // ia.set_dataframe();
    // auto ddff   = ia.get_dataframe();
    // auto ooopen = ia.getOpen();
    // std::cout << ooopen.size() << "\n";

    // IndexDayAction ia2(codes, start, end, FQ_TYPE::PRE);
    // ia2.set_dataframe();
    // auto xls2 = ia2.to_series("open");
    // std::cout << xls2.size() << "\n";
    // // IndexMinAction sb(codes, start, end);
    // // // auto df2 = sb.to_fq_type(FQ_TYPE::PRE);
    // // auto df2 = sb.to_fq_type(FQ_TYPE::POST);
    // // qDebug() << sb << "\n";

    // IndexXdxrAction sc(codes);
    // qDebug() << sc << "\n";

    // auto mm0 = ia.to_series<std::string>("code");
    // qDebug() << "------- mm0 " << mm0.size() << "\n";
    // for (auto o : mm0) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n code000 \n";

    // auto mm = ia.to_series<double>("close");
    // for (auto o : mm) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }

    // auto mm2 = ia.to_series<double>("high");
    // for (auto o : mm2) {
    //     for (auto i : o) {
    //         qDebug() << i;
    //     }
    // }
    // qDebug() << "\n close \n";

    // auto mm3 = ia.to_series<QString>("date");
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
