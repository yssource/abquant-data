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
#include "abquant/models/stockxdxr.h"

namespace abq
{
class StockXdxrAction : public SecurityAction<StockXdxrAction>, std::enable_shared_from_this<StockXdxrAction>
{
public:
    //! Default constructor
    StockXdxrAction() = default;

    StockXdxrAction(const QStringList codes, int category = 1);

    //! Copy constructor
    StockXdxrAction(const StockXdxrAction& other) = delete;

    //! Move constructor
    StockXdxrAction(StockXdxrAction&& other) noexcept = delete;

    //! Destructor
    ~StockXdxrAction();

    //! Copy assignment operator
    StockXdxrAction& operator=(const StockXdxrAction& other) = default;

    //! Move assignment operator
    StockXdxrAction& operator=(StockXdxrAction&& other) noexcept;

    QList<StockXdxr> get_securities() const;
    QVector<const char*> get_columns() const;
    MyDataFramePtr get_dataframe() const;

    template <typename T>
    QVector<T> to_series(const char*) const noexcept;

private:
    class impl;
    std::experimental::propagate_const<std::shared_ptr<impl>> pImpl;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};

private:
    friend inline QDebug operator<<(QDebug d, const StockXdxrAction& sa)
    {
        QVector<const char*> columns = sa.get_columns();
        d << columns << "\n";
        auto qs = sa.get_securities();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.category()) << QVariant(s.name()) << QVariant(s.fenhong()) << QVariant(s.peigujia())
                      << QVariant(s.songzhuangu()) << QVariant(s.peigu()) << QVariant(s.suogu())
                      << QVariant(s.liquidityBefore()) << QVariant(s.liquidityAfter()) << QVariant(s.sharesBefore())
                      << QVariant(s.sharesAfter()) << QVariant(s.fenshu()) << QVariant(s.xingquanjia())
                      << QVariant(s.date()) << QVariant(s.categoryMeaning()) << QVariant(s.code());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> StockXdxrAction::to_series(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = get_columns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }
    for (auto s : get_securities()) {
        if constexpr (std::is_same_v<T, double>) {
            if (QString("category") == QString(col)) {
                series << static_cast<double>(s.category());
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("name") == QString(col)) {
                series << s.name();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("fenhong") == QString(col)) {
                series << s.fenhong();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("peigujia") == QString(col)) {
                series << s.peigujia();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("songzhuangu") == QString(col)) {
                series << s.songzhuangu();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("peigu") == QString(col)) {
                series << s.peigu();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("suogu") == QString(col)) {
                series << s.suogu();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("liquidity_before") == QString(col)) {
                series << s.liquidityBefore();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("liquidity_after") == QString(col)) {
                series << s.liquidityAfter();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("shares_before") == QString(col)) {
                series << s.sharesBefore();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("shares_after") == QString(col)) {
                series << s.sharesAfter();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("fenshu") == QString(col)) {
                series << s.fenshu();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("xingquanjia") == QString(col)) {
                series << s.xingquanjia();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("date") == QString(col)) {
                series << s.date();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("category_meaning") == QString(col)) {
                series << s.categoryMeaning();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("code") == QString(col)) {
                series << s.code();
                continue;
            }
        }
    }
    return series;
}
} // namespace abq
