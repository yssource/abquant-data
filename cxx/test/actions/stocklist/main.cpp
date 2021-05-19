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
#include <iostream>
#include <utility>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/stocklist.hpp"
#include "abquant/actions/utils.hpp"
#include "string"

using namespace abq;
using namespace std;

using StockListActionPtr = std::shared_ptr<abq::StockListAction>;
Q_DECLARE_METATYPE(StockListActionPtr)

class TestStockList : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void getStocks_data();
    void getStocks();
};

void TestStockList::initTestCase() {}

void TestStockList::initTestCase_data()
{
    QStringList codes = {"000001", "000002"};
    const char* end   = "2019-12-01";

    auto sa  = std::make_shared<StockListAction>();
    auto sa2 = std::make_shared<StockListAction>(codes, end);

    QTest::addColumn<StockListActionPtr>("sla");
    QTest::addColumn<int>("rst");
    QTest::newRow("0") << sa << 3785;
    QTest::newRow("1") << sa2 << 2;
}

void TestStockList::getStocks_data() {}

void TestStockList::getStocks()
{
    QFETCH_GLOBAL(StockListActionPtr, sla);
    QFETCH_GLOBAL(int, rst);
    QCOMPARE(sla->getStocks().count(), rst);
}

void TestStockList::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestStockList)
ABQ_TEST_MAIN(TestStockList)
#include "main.moc"
