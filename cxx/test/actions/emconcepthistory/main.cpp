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
#include "abquant/actions/emconcept.hpp"
#include "abquant/actions/emconcepthistory.hpp"
// #include "abquant/actions/indexmin.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>

#include "string"

using namespace abq;
using namespace std;

static QList<EmConceptHistory> emconcepthistorys;
static EmConceptHistoryAction ca;
// static MyDataFrame df;

class TestEmConceptHistory : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void f8_turnover_data();
    void f8_turnover();
    void mkt_code_data();
    void mkt_code();
};

void TestEmConceptHistory::initTestCase()
{
    QStringList codes = {"90.bk0980", "90.bk0990"};
    // QStringList codes = {"90.bk0990"};
    const char* start = "2017-01-01";
    const char* end   = "2019-12-01";
    ca                = EmConceptHistoryAction(codes, start, end);
    qDebug() << ca << "\n";
}

void TestEmConceptHistory::f8_turnover_data()
{
    QTest::addColumn<double>("f8_turnover");
    // xt::xarray<double> xs = xt::adapt(ca.toSeries<double>("f8_turnover").toStdVector());
    xt::xarray<double> xs = ca.toSeries("f8_turnover");
    QTest::newRow("1") << xs(0);
}

void TestEmConceptHistory::f8_turnover()
{
    QFETCH(double, f8_turnover);
    double expected = 1.69;
    qFuzzyCompare(f8_turnover, expected);
}

void TestEmConceptHistory::mkt_code_data()
{
    QTest::addColumn<QString>("mkt_code");
    xt::xarray<std::string> xs = xt::adapt(ca.toSeries<std::string>("mkt_code").toStdVector());
    QTest::newRow("1") << QString::fromStdString(xs(0));
}

void TestEmConceptHistory::mkt_code()
{
    QFETCH(QString, mkt_code);
    QString expected = "90.BK0980";
    QCOMPARE(mkt_code, expected);
}

void TestEmConceptHistory::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestEmConceptHistory)
ABQ_TEST_MAIN(TestEmConceptHistory)
#include "main.moc"
