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
#include "abquant/actions/securitylist.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/indexlist.h"
#include "abquant/models/etflist.h"
#include "abquant/models/stocklist.h"

using namespace abq;
using namespace std;

using StockListActionPtr = std::shared_ptr<abq::SecurityListAction<StockList>>;
using IndexListActionPtr = std::shared_ptr<abq::SecurityListAction<IndexList>>;
using EtfListActionPtr = std::shared_ptr<abq::SecurityListAction<EtfList>>;
using MyDataFramePtr     = std::shared_ptr<MyDataFrame>;

Q_DECLARE_METATYPE(StockListActionPtr)
Q_DECLARE_METATYPE(IndexListActionPtr)
Q_DECLARE_METATYPE(EtfListActionPtr)
Q_DECLARE_METATYPE(MyDataFramePtr)

class TestSecurityList : public QObject
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

void TestSecurityList::initTestCase() {}

void TestSecurityList::initTestCase_data()
{
    const char* end     = "2022-12-01";

    QStringList s_codes = {"000001", "300001", "600000"};
    auto sa  = std::make_shared<SecurityListAction<StockList>>();
    auto sa2 = std::make_shared<SecurityListAction<StockList>>(s_codes, end);
    QTest::addColumn<StockListActionPtr>("sla");

    QStringList i_codes = {"000001", "399001", "399006"};
    auto ia             = std::make_shared<SecurityListAction<IndexList>>();
    auto ia2            = std::make_shared<SecurityListAction<IndexList>>(i_codes, end);
    QTest::addColumn<IndexListActionPtr>("ila");

    QStringList e_codes = {"159001", "510010", "519997"};
    auto ea             = std::make_shared<SecurityListAction<EtfList>>();
    auto ea2            = std::make_shared<SecurityListAction<EtfList>>(e_codes, end);
    QTest::addColumn<EtfListActionPtr>("ela");

    QTest::newRow("0") << sa << ia << ea;
    QTest::newRow("1") << sa2 << ia2 << ea2;
}

void TestSecurityList::get_securities_data()
{
    QTest::addColumn<int>("s_rst");
    QTest::addColumn<int>("i_rst");
    QTest::addColumn<int>("e_rst");
    QTest::newRow("0") << 4990 << 1142 << 885;
    QTest::newRow("1") << 3 << 3 << 3;
}

void TestSecurityList::get_securities()
{
    QFETCH_GLOBAL(StockListActionPtr, sla);
    QFETCH(int, s_rst);
    QCOMPARE(sla->get_securities().count(), s_rst);

    QFETCH_GLOBAL(IndexListActionPtr, ila);
    QFETCH(int, i_rst);
    QCOMPARE(ila->get_securities().count(), i_rst);

    QFETCH_GLOBAL(EtfListActionPtr, ela);
    QFETCH(int, e_rst);
    QCOMPARE(ela->get_securities().count(), e_rst);
}

void TestSecurityList::get_dataframe_data()
{
    QTest::addColumn<int>("df_rst");
    QTest::addColumn<int>("df2_rst");
    QTest::addColumn<int>("df3_rst");
    QTest::newRow("0") << 4990 << 1142 << 885;
    QTest::newRow("1") << 3 << 3 << 3;
}

void TestSecurityList::get_dataframe()
{
    QFETCH_GLOBAL(StockListActionPtr, sla);
    QFETCH(int, df_rst);
    QCOMPARE(sla->get_dataframe()->get_index().size(), df_rst);

    QFETCH_GLOBAL(IndexListActionPtr, ila);
    QFETCH(int, df2_rst);
    QCOMPARE(ila->get_dataframe()->get_index().size(), df2_rst);

    QFETCH_GLOBAL(EtfListActionPtr, ela);
    QFETCH(int, df3_rst);
    QCOMPARE(ela->get_dataframe()->get_index().size(), df3_rst);
}

void TestSecurityList::cleanupTestCase() {}

ABQ_TEST_MAIN(TestSecurityList)
#include "main.moc"
