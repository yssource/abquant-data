/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

// #define private public
#include <QtTest/QtTest>
#include <iostream>
#include <utility>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stockday.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/indexlist.h"
#include "abquant/models/stocklist.h"

using namespace abq;
using namespace std;

using StockDayActionPtr = std::shared_ptr<abq::StockDayAction>;
using MyDataFramePtr    = std::shared_ptr<MyDataFrame>;

Q_DECLARE_METATYPE(StockDayActionPtr)
Q_DECLARE_METATYPE(MyDataFramePtr)

class TestStockDay : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void get_securities_data();
    void get_securities();
    void get_dataframe_data();
    void get_dataframe();
};

void TestStockDay::initTestCase() {}

void TestStockDay::initTestCase_data()
{
    QStringList s_codes = {"000001", "300001", "600000"};
    const char* start   = "2019-12-01";
    const char* end     = "2019-12-31";

    auto sa = std::make_shared<StockDayAction>(s_codes, start, end);

    QTest::addColumn<StockDayActionPtr>("sla");

    QTest::newRow("0") << sa;
}

void TestStockDay::get_securities_data()
{
    QTest::addColumn<int>("s_rst");
    QTest::newRow("0") << 66;
}

void TestStockDay::get_securities()
{
    QFETCH_GLOBAL(StockDayActionPtr, sla);
    QFETCH(int, s_rst);
    QCOMPARE(sla->get_securities().count(), s_rst);
}

void TestStockDay::get_dataframe_data()
{
    QTest::addColumn<int>("df_rst");
    QTest::newRow("0") << 66;
}

void TestStockDay::get_dataframe()
{
    QFETCH_GLOBAL(StockDayActionPtr, sla);
    QFETCH(int, df_rst);
    QCOMPARE(sla->get_dataframe()->get_index().size(), df_rst);
}

void TestStockDay::cleanupTestCase() {}

ABQ_TEST_MAIN(TestStockDay)
#include "main.moc"
