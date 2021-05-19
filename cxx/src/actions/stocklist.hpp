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

#include "abquant/actions/stock.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/stocklist.h"

namespace abq
{
class StockListAction : public StockAction<StockListAction>, std::enable_shared_from_this<StockListAction>
{
public:
    //! Default constructor
    // StockListAction() = default;

    StockListAction();

    // StockListAction(const char* end = "");

    StockListAction(const QStringList codes, const char* end = "");

    //! Copy constructor
    StockListAction(const StockListAction& other) = delete;

    //! Move constructor
    StockListAction(StockListAction&& other) noexcept = delete;

    //! Destructor
    ~StockListAction();

    //! Copy assignment operator
    StockListAction& operator=(const StockListAction& other) = default;

    //! Move assignment operator
    StockListAction& operator=(StockListAction&& other) noexcept;

    QList<StockList> getStocks() const;
    QVector<const char*> getColumns() const;
    MyDataFramePtr getDataFrame() const;
    QStringList getCodes() const;

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

private:
    class impl;
    std::experimental::propagate_const<std::shared_ptr<impl>> pImpl;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};

private:
    friend inline QDebug operator<<(QDebug d, const StockListAction& sa)
    {
        QVector<const char*> columns = sa.getColumns();
        d << columns << "\n";
        auto qs = sa.getStocks();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.code()) << QVariant(s.volunit()) << QVariant(s.decimalPoint()) << QVariant(s.name())
                      << QVariant(s.preClose()) << QVariant(s.sse()) << QVariant(s.sec());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> StockListAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    if constexpr (std::is_same_v<T, double>) {
        if (m_xdxr == FQ_TYPE::PRE || m_xdxr == FQ_TYPE::POST) {
            MyDataFramePtr df;
            std::vector<T> stdv_series;

            try {
                df = getDataFrame();
                // df->write<std::ostream, std::string, double, int>(std::cout);
                stdv_series = df->get_column<T>(col);
            } catch (...) {
                std::cout << "Errors with StockListAction::toSeries ..."
                          << "\n";
            }
            return QVector<T>::fromStdVector(stdv_series);
        }
    }

    for (auto s : getStocks()) {
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("code") == QString(col)) {
                series << s.code().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("volunit") == QString(col)) {
                series << static_cast<double>(s.volunit());
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("decimal_point") == QString(col)) {
                series << static_cast<double>(s.decimalPoint());
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("name") == QString(col)) {
                series << s.name().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("pre_close") == QString(col)) {
                series << s.preClose();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("sse") == QString(col)) {
                series << s.sse().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("sec") == QString(col)) {
                series << s.sec().toStdString();
                continue;
            }
        }
    }
    return series;
}
} // namespace abq
