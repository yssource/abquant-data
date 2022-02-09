/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#include <QtTest/QtTest>

#include "abquant/actions/abquant.hpp"
#include "abquant/actions/security.hpp"
#include "abquant/actions/indexday.hpp"
#include "abquant/actions/utils.hpp"
#include <iostream>
#include <utility>

#include "string"

using namespace abq;
using namespace std;


class TestIndexDay : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
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


    auto ind = ia.make_indicator();
}


void TestIndexDay::cleanupTestCase() {}

ABQ_TEST_MAIN(TestIndexDay)
#include "main.moc"
