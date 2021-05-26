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
#include "abquant/actions/emconceptbase.hpp"
// #include "abquant/actions/indexmin.hpp"
#include "abquant/actions/utils.hpp"
// #include "gtest/gtest.h"
#include <iostream>
#include <utility>

#include "string"

using namespace abq;
using namespace std;

static QList<EmConceptBase> emconceptbases;
static EmConceptBaseAction ca;
// static MyDataFrame df;

class TestEmConceptBase : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void f8_turnover_data();
    void f8_turnover();
    void f14_name_data();
    void f14_name();
};

void TestEmConceptBase::initTestCase()
{
    QStringList codes = {"bk0980", "bk0990"};
    // QStringList codes = {"bk0990"};
    ca = EmConceptBaseAction(codes);
    qDebug() << ca << "\n";
}

void TestEmConceptBase::f8_turnover_data()
{
    QTest::addColumn<double>("f8_turnover");
    xt::xarray<double> xs = xt::adapt(ca.to_series<double>("f8_turnover").toStdVector());
    QTest::newRow("1") << xs(0);
}

void TestEmConceptBase::f8_turnover()
{
    QFETCH(double, f8_turnover);
    double expected = 1.06;
    qFuzzyCompare(f8_turnover, expected);
}

void TestEmConceptBase::f14_name_data()
{
    QTest::addColumn<QString>("f14_name");
    xt::xarray<std::string> xs = xt::adapt(ca.to_series<std::string>("f14_name").toStdVector());
    QTest::newRow("1") << QString::fromStdString(xs(0));
}

void TestEmConceptBase::f14_name()
{
    QFETCH(QString, f14_name);
    QString expected = "债转股";
    QCOMPARE(f14_name, expected);
}

void TestEmConceptBase::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestEmConceptBase)
ABQ_TEST_MAIN(TestEmConceptBase)
#include "main.moc"
