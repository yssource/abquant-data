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
#include "abquant/actions/indicator.hpp"
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/stockmin.hpp"
#include "abquant/actions/stockxdxr.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>

#include "any"
#include "string"

using namespace abq;
using namespace std;

static QList<StockDay> stockdays;
static QList<IndexDay> indexdays;
static std::shared_ptr<StockDayAction> sa;
static std::shared_ptr<IndexDayAction> ia;

static std::shared_ptr<Indicator<abq::SecurityAction<abq::StockDayAction>::derived_type>> indstockday;
static std::shared_ptr<Indicator<abq::SecurityAction<abq::IndexDayAction>::derived_type>> indindexday;

class TestIndicator : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void sma_data();
    void sma();
    void roc_data();
    void roc();
};

void TestIndicator::initTestCase()
{
    // QStringList codes = {"000001", "000002"};
    QStringList codes = {"000001"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";

    sa = std::make_shared<StockDayAction>(codes, start, end);
    indstockday = sa->make_indicator();

    ia = std::make_shared<IndexDayAction>(codes, start, end);
    indindexday = ia->make_indicator();
}

void TestIndicator::sma_data()
{
    QTest::addColumn<double>("open");
    xt::xarray<double> xs = xt::adapt(sa->to_series<double>("open").toStdVector());
    xt::xarray<double> rs = indstockday->SMA(xs, 12);
    QTest::newRow("1") << rs(0);
}

void TestIndicator::sma()
{
    QFETCH(double, open);
    double expected = 9.11;
    QCOMPARE(open, expected);
}

void TestIndicator::roc_data()
{
    QTest::addColumn<double>("roc");
    QTest::addColumn<double>("roc_result");
    QTest::addColumn<double>("rocma");
    QTest::addColumn<double>("rocma_result");

    const char* col       = "close";
    roc_return_type roc_map         = indstockday->ROC(col, 12, 6);
    series_no_cvp_type roc_actual   = roc_map["ROC"];
    series_no_cvp_type rocma_actual = roc_map["ROCMA"];

    QTest::newRow("1") << roc_actual[0] << -43.87 << rocma_actual[0] << std::numeric_limits<double>::quiet_NaN();
    QTest::newRow("5") << roc_actual[5] << -42.30 << rocma_actual[5] << 9.13;
}

void TestIndicator::roc()
{
    QFETCH(double, roc);
    QFETCH(double, roc_result);
    QFETCH(double, rocma);
    QFETCH(double, rocma_result);

    QCOMPARE(std::abs(roc - roc_result) < 0.1, true);
    if (std::isnan(rocma)) {
        QCOMPARE(std::isnan(rocma_result), true);
    } else {
        QCOMPARE(std::abs(rocma - rocma_result) < 0.1, true);
    }
}

void TestIndicator::cleanupTestCase() {}

ABQ_TEST_MAIN(TestIndicator)
#include "main.moc"
