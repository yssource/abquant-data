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
#include <iostream>

#include "abquant/actions/stock.hpp"
#include "abquant/models/stockxdxr.h" //  include the model class

namespace abq
{
class StockXdxrAction : public StockAction<StockXdxrAction>
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
    virtual ~StockXdxrAction() noexcept = default;

    // QStringList getCodes() const;
    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    //! Copy assignment operator
    StockXdxrAction& operator=(const StockXdxrAction& other) = default;

    //! Move assignment operator
    StockXdxrAction& operator=(StockXdxrAction&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }
        std::swap(m_codes, other.m_codes);
        std::swap(m_category, other.m_category);
        std::swap(m_stockxdxrs, other.m_stockxdxrs);
        return *this;
    }

    inline QList<StockXdxr> getStocks() const { return m_stockxdxrs; };
    inline QVector<const char*> getColumns() const { return m_columns; };
    void setDataFrame();
    inline MyDataFramePtr getDataFrame() const { return m_df; }

private:
    QList<StockXdxr> m_stockxdxrs;
    const QVector<const char*> m_columns = {"category",
                                            "name",
                                            "fenhong",
                                            "peigujia",
                                            "songzhuangu",
                                            "peigu",
                                            "suogu",
                                            "liquidity_before",
                                            "liquidity_after",
                                            "shares_before",
                                            "shares_after",
                                            "fenshu",
                                            "xingquanjia",
                                            "date",
                                            "category_meaning",
                                            "code"};
    QStringList m_codes;
    int m_category;
    MyDataFramePtr m_df{nullptr};

private:
    friend inline QDebug operator<<(QDebug d, const StockXdxrAction& sa)
    {
        QVector<const char*> columns = sa.getColumns();
        d << columns << "\n";
        auto qs = sa.getStocks();
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
QVector<T> StockXdxrAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }
    for (auto s : getStocks()) {
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
