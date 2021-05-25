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
#include "abquant/actions/securitylist.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/indexlist.h"
#include "abquant/models/stocklist.h"

using namespace abq;
using namespace std;

using StockListActionPtr = std::shared_ptr<abq::SecurityListAction<StockList>>;
using IndexListActionPtr = std::shared_ptr<abq::SecurityListAction<IndexList>>;
Q_DECLARE_METATYPE(StockListActionPtr)
Q_DECLARE_METATYPE(IndexListActionPtr)

class TestSecurityList : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void getSecurities_data();
    void getSecurities();
};

void TestSecurityList::initTestCase() {}

void TestSecurityList::initTestCase_data()
{
    QStringList s_codes = {"000001", "300001", "600000"};
    const char* end     = "2019-12-01";

    auto sa  = std::make_shared<SecurityListAction<StockList>>();
    auto sa2 = std::make_shared<SecurityListAction<StockList>>(s_codes, end);

    QTest::addColumn<StockListActionPtr>("sla");
    QTest::addColumn<int>("s_rst");

    QStringList i_codes = {"000001", "399001", "399006"};
    auto ia             = std::make_shared<SecurityListAction<IndexList>>();
    auto ia2            = std::make_shared<SecurityListAction<IndexList>>(i_codes, end);

    QTest::addColumn<IndexListActionPtr>("ila");
    QTest::addColumn<int>("i_rst");

    QTest::newRow("0") << sa << 3785 << ia << 1155;
    QTest::newRow("1") << sa2 << 3 << ia2 << 3;
}

void TestSecurityList::getSecurities_data() {}

void TestSecurityList::getSecurities()
{
    QFETCH_GLOBAL(StockListActionPtr, sla);
    QFETCH_GLOBAL(int, s_rst);
    QCOMPARE(sla->getSecurities().count(), s_rst);

    QFETCH_GLOBAL(IndexListActionPtr, ila);
    QFETCH_GLOBAL(int, i_rst);
    QCOMPARE(ila->getSecurities().count(), i_rst);
}

void TestSecurityList::cleanupTestCase() {}

// ABQ_TEST_SQLLESS_MAIN(TestSecurityList)
ABQ_TEST_MAIN(TestSecurityList)
#include "main.moc"
