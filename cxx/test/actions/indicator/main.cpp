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
#include "abquant/actions/stock.hpp"
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/stockxdxr.hpp"
#include "abquant/actions/indicator.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>
#include "string"

using namespace abq;
using namespace std;

static QList<StockDay> stockdays;
static StockDayAction sa;
static Indicator<abq::StockAction<abq::StockDayAction>::self_type> indday;
// static MyDataFrame df;

class TestIndicator : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void sma_data();
    void sma();
};

void TestIndicator::initTestCase()
{
    // QStringList codes = {"000001", "000002"};
    QStringList codes = {"000001"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";

    // const char *start = "2019-06-25";
    // const char *end = "2019-06-27";

    sa = StockDayAction(codes, start, end);
    indday = sa.makeIndicator();

}

void TestIndicator::sma_data() {
    QTest::addColumn<double>("open");
    // xt::xarray<double> xs = xt::adapt(sa.toSeries<double>("open").toStdVector());
    xt::xarray<double> xs = sa.toSeries("open");
    xt::xarray<double> rs = indday.SMA(xs, 12);
    QTest::newRow("1") << rs(0);
}

void TestIndicator::sma() {
    QFETCH(double, open);
    double expected          = 9.11;
    QCOMPARE(open, expected);
}

void TestIndicator::cleanupTestCase() {}

ABQ_TEST_MAIN(TestIndicator)
#include "main.moc"
