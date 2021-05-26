/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#pragma once

#include <QDebug>
#include <algorithm>
#include <experimental/propagate_const>
#include <iostream>

#include "abquant/actions/security.hpp"
#include "abquant/actions/utils.hpp"
// #include "abquant/actions/xdxr.hpp"
#include "abquant/models/stockmin.h"

namespace abq
{
class StockMinAction : public SecurityAction<StockMinAction>, std::enable_shared_from_this<StockMinAction>
{
public:
    //! Default constructor
    StockMinAction() = default;

    StockMinAction(QStringList codes, const char* start, const char* end, MIN_FREQ freq = MIN_FREQ::ONE,
                   FQ_TYPE xdxr = FQ_TYPE::NONE);

    //! Copy constructor
    StockMinAction(const StockMinAction& other) = delete;

    //! Move constructor
    StockMinAction(StockMinAction&& other) noexcept = delete;

    //! Destructor
    ~StockMinAction();

    //! Copy assignment operator
    StockMinAction& operator=(const StockMinAction& other) = default;

    //! Move assignment operator
    StockMinAction& operator=(StockMinAction&& other) noexcept;

    QList<StockMin> get_securities() const;
    QVector<const char*> getColumns() const;
    MyDataFramePtr toFq(FQ_TYPE fq = FQ_TYPE::NONE);
    MyDataFramePtr getDataFrame() const;
    QStringList getCodes() const;

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

private:
    class impl;
    std::experimental::propagate_const<std::shared_ptr<impl>> pImpl;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};

private:
    friend inline QDebug operator<<(QDebug d, const StockMinAction& sa)
    {
        QVector<const char*> columns = sa.getColumns();
        d << columns << "\n";
        auto qs = sa.get_securities();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.open()) << QVariant(s.close()) << QVariant(s.high()) << QVariant(s.low())
                      << QVariant(s.vol()) << QVariant(s.amount()) << QVariant(s.datetime()) << QVariant(s.date())
                      << QVariant(s.code()) << QVariant(s.dateStamp()) << QVariant(s.timeStamp()) << QVariant(s.type());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> StockMinAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    if constexpr (std::is_same_v<T, double>) {
        if (m_xdxr == FQ_TYPE::PRE || m_xdxr == FQ_TYPE::POST) {
            std::shared_ptr<MyDataFrame> df;
            std::vector<T> stdv_series;

            try {
                df = getDataFrame();
                // df->write<std::ostream, std::string, double, int>(std::cout);
                const char* colname;
                if (QString(col) == QString("code")) {
                    colname = "lhs.code";
                } else if (QString(col) == QString("date")) {
                    colname = "lhs.date";
                } else if (QString(col) == QString("datetime")) {
                    colname = "lhs.datetime";
                } else {
                    colname = col;
                }
                stdv_series = df->get_column<T>(colname);
            } catch (...) {
                std::cout << "Errors with StockMinAction::toSeries ..."
                          << "\n";
            }
            return QVector<T>::fromStdVector(stdv_series);
        }
    }

    for (auto s : get_securities()) {
        if constexpr (std::is_same_v<T, double>) {
            if (QString("open") == QString(col)) {
                series << s.open();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("close") == QString(col)) {
                series << s.close();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("high") == QString(col)) {
                series << s.high();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("low") == QString(col)) {
                series << s.low();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("vol") == QString(col)) {
                series << s.vol();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("amount") == QString(col)) {
                series << s.amount();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("datetime") == QString(col)) {
                series << s.datetime().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("code") == QString(col)) {
                series << s.code().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("date") == QString(col)) {
                series << s.date().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("date_stamp") == QString(col)) {
                series << s.dateStamp();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("time_stamp") == QString(col)) {
                series << s.timeStamp();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("type") == QString(col)) {
                series << s.type().toStdString();
                continue;
            }
        }
    }
    return series;
}

} // namespace abq
