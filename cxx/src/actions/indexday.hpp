/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
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
#include "abquant/models/indexday.h"

namespace abq
{
class IndexDayAction : public SecurityAction<IndexDayAction>, std::enable_shared_from_this<IndexDayAction>
{
public:
    //! Default constructor
    IndexDayAction() = default;

    IndexDayAction(QStringList codes, const char* start, const char* end, FQ_TYPE xdxr = FQ_TYPE::NONE);

    //! Copy constructor
    IndexDayAction(const IndexDayAction& other) = delete;

    //! Move constructor
    IndexDayAction(IndexDayAction&& other) noexcept = delete;

    //! Destructor
    ~IndexDayAction();

    //! Copy assignment operator
    IndexDayAction& operator=(const IndexDayAction& other) = default;

    //! Move assignment operator
    IndexDayAction& operator=(IndexDayAction&& other) noexcept;

    QList<IndexDay> get_securities() const;
    QVector<const char*> get_columns() const;
    MyDataFramePtr get_dataframe() const;
    QStringList get_codes() const;

    template <typename T>
    QVector<T> to_series(const char*) const noexcept;

private:
    class impl;
    std::experimental::propagate_const<std::shared_ptr<impl>> pImpl;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};

private:
    friend inline QDebug operator<<(QDebug d, const IndexDayAction& sa)
    {
        QVector<const char*> columns = sa.get_columns();
        d << columns << "\n";
        auto qs = sa.get_securities();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.open()) << QVariant(s.close()) << QVariant(s.high()) << QVariant(s.low())
                      << QVariant(s.vol()) << QVariant(s.amount()) << QVariant(s.date()) << QVariant(s.code())
                      << QVariant(s.dateStamp());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> IndexDayAction::to_series(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = get_columns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
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
            if (QString("date") == QString(col)) {
                series << s.date().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("code") == QString(col)) {
                series << s.code().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("date_stamp") == QString(col)) {
                series << s.dateStamp();
                continue;
            }
        }
    }
    return series;
}

} // namespace abq
