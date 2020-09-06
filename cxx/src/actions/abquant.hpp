/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <QByteArray>
#include <QEventLoop>
#include <QObject>
#include <QTextCodec>
#include <QtCore>
#include <QtTest/QtTest>
#include <TAppSettings>
#include <TWebApplication>

#include "tkvsdatabasepool.h"
#ifdef QT_SQL_LIB
#include <TActionThread>

#include "tsqldatabasepool.h"
#endif

// #include <TfTest/TfTest>
#include <iostream>
#include <vector>

#ifdef _WIN32
#ifdef LIBRARY_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif
#else
#define LIBRARY_API
#endif // _WIN32

namespace abq
{
using namespace std;

#define ABQ_TEST_MAIN(TestObject) ABQ_TEST_SQL_MAIN(TestObject, true);

#define ABQ_TEST_SQL_MAIN(TestObject, EnableTransactions)                                                              \
    int main(int argc, char* argv[])                                                                                   \
    {                                                                                                                  \
        class Thread : public TActionThread                                                                            \
        {                                                                                                              \
        public:                                                                                                        \
            Thread() : TActionThread(0), returnCode(0) {}                                                              \
            volatile int returnCode;                                                                                   \
                                                                                                                       \
        protected:                                                                                                     \
            virtual void run()                                                                                         \
            {                                                                                                          \
                setTransactionEnabled(EnableTransactions);                                                             \
                TestObject obj;                                                                                        \
                QStringList args = {"main"};                                                                           \
                returnCode       = QTest::qExec(&obj, args);                                                           \
                commitTransactions();                                                                                  \
                for (QMap<int, TSqlTransaction>::iterator it = sqlDatabases.begin(); it != sqlDatabases.end(); ++it) { \
                    it.value().database().close(); /* close SQL database */                                            \
                }                                                                                                      \
                for (QMap<int, TKvsDatabase>::iterator it = kvsDatabases.begin(); it != kvsDatabases.end(); ++it) {    \
                    it.value().close(); /* close KVS database */                                                       \
                }                                                                                                      \
                QEventLoop eventLoop;                                                                                  \
                while (eventLoop.processEvents()) {                                                                    \
                }                                                                                                      \
            }                                                                                                          \
        };                                                                                                             \
        QString xdg_config_home =                                                                                      \
            QProcess::systemEnvironment().filter("XDG_CONFIG_HOME=", Qt::CaseSensitive).value(0).mid(16);              \
        QString abq = "";                                                                                              \
        if (!xdg_config_home.isEmpty()) {                                                                              \
            abq = QString("%1%2%3").arg(xdg_config_home).arg(QDir::separator()).arg("abquant");                        \
        } else {                                                                                                       \
            abq = QString("%1%2%3").arg(QDir::homePath()).arg(QDir::separator()).arg(".abquant");                      \
        }                                                                                                              \
        std::vector<std::string> args = {"main", "-e", "test", abq.toStdString()};                                     \
        std::vector<char*> vec;                                                                                        \
        for (const auto& arg : args) vec.emplace_back((char*)arg.data());                                              \
        vec.emplace_back(nullptr);                                                                                     \
        argc = vec.size() - 1;                                                                                         \
        argv = vec.data();                                                                                             \
        TWebApplication app(argc, argv);                                                                               \
        QByteArray codecName = Tf::appSettings()->value(Tf::InternalEncoding, "UTF-8").toByteArray();                  \
        QTextCodec* codec    = QTextCodec::codecForName(codecName);                                                    \
        QTextCodec::setCodecForLocale(codec);                                                                          \
        int idx     = QCoreApplication::arguments().indexOf("-e");                                                     \
        QString env = (idx > 0) ? QCoreApplication::arguments().value(idx + 1) : QString("product");                   \
        app.setDatabaseEnvironment(env);                                                                               \
        Thread thread;                                                                                                 \
        thread.start();                                                                                                \
        thread.wait();                                                                                                 \
        _exit(thread.returnCode);                                                                                      \
        return thread.returnCode;                                                                                      \
    }

#define ABQ_TEST_SQLLESS_MAIN(TestObject)                                                             \
    int main(int argc, char* argv[])                                                                  \
    {                                                                                                 \
        TWebApplication app(argc, argv);                                                              \
        QByteArray codecName = Tf::appSettings()->value(Tf::InternalEncoding, "UTF-8").toByteArray(); \
        QTextCodec* codec    = QTextCodec::codecForName(codecName);                                   \
        QTextCodec::setCodecForLocale(codec);                                                         \
        TestObject tc;                                                                                \
        return QTest::qExec(&tc, argc, argv);                                                         \
    }

#define ABQ_OPEN_MAIN()                                                                               \
    QString xdg_config_home =                                                                         \
        QProcess::systemEnvironment().filter("XDG_CONFIG_HOME=", Qt::CaseSensitive).value(0).mid(16); \
    QString abq = "";                                                                                 \
    if (!xdg_config_home.isEmpty()) {                                                                 \
        abq = QString("%1%2%3").arg(xdg_config_home).arg(QDir::separator()).arg("abquant");           \
    } else {                                                                                          \
        abq = QString("%1%2%3").arg(QDir::homePath()).arg(QDir::separator()).arg(".abquant");         \
    }                                                                                                 \
    std::vector<std::string> args = {"main", "-e", "product", abq.toStdString()};                     \
    std::vector<char*> vec;                                                                           \
    for (const auto& arg : args) vec.emplace_back((char*)arg.data());                                 \
    vec.emplace_back(nullptr);                                                                        \
    int argc             = vec.size() - 1;                                                            \
    char** argv          = vec.data();                                                                \
    TWebApplication* app = new TWebApplication(argc, argv);                                           \
    QByteArray codecName = Tf::appSettings()->value(Tf::InternalEncoding, "UTF-8").toByteArray();     \
    QTextCodec* codec    = QTextCodec::codecForName(codecName);                                       \
    QTextCodec::setCodecForLocale(codec);                                                             \
    int idx     = QCoreApplication::arguments().indexOf("-e");                                        \
    QString env = (idx > 0) ? QCoreApplication::arguments().value(idx + 1) : QString("product");      \
    app->setDatabaseEnvironment(env);                                                                 \
    TSqlDatabasePool::instance();                                                                     \
    TKvsDatabasePool::instance();

#define ABQ_CLOSE_MAIN()                                                                                   \
    for (QMap<int, TSqlTransaction>::iterator it = sqlDatabases.begin(); it != sqlDatabases.end(); ++it) { \
        it.value().database().close(); /* close SQL database */                                            \
    }                                                                                                      \
    for (QMap<int, TKvsDatabase>::iterator it = kvsDatabases.begin(); it != kvsDatabases.end(); ++it) {    \
        it.value().close(); /* close KVS database */                                                       \
    }

class Abquant
{
public:
    static Abquant& start()
    {
        // FIXME: don't know why qDebug makes a segmentation fault(core dumped), if there is no qDeubg here.
        qDebug() << "Abquant starts ...";
        static Abquant inst;
        return inst;
    }
    static void finish() { _exit(0); }
    void hello() { return; }

private:
    Abquant(){ABQ_OPEN_MAIN()};
    Abquant(const Abquant&) = delete;
    Abquant& operator=(const Abquant&) = delete;
};

} // namespace abq
