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

#include "abquant/actions/index.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/indexday.h" //  include the model class

namespace abq
{
using namespace std;

class IndexDayAction : public IndexAction<IndexDayAction>
{
public:
    //! Default constructor
    IndexDayAction() = default;

    IndexDayAction(QStringList codes, const char* start, const char* end);

    //! Copy constructor
    IndexDayAction(const IndexDayAction& other) = delete;

    //! Move constructor
    IndexDayAction(IndexDayAction&& other) noexcept = delete;

    //! Destructor
    ~IndexDayAction() noexcept = default;

    //! Copy assignment operator
    IndexDayAction& operator=(const IndexDayAction& other) = default;

    //! Move assignment operator
    IndexDayAction& operator=(IndexDayAction&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }
        std::swap(m_codes, other.m_codes);
        std::swap(m_start, other.m_start);
        std::swap(m_end, other.m_end);
        std::swap(m_indexdays, other.m_indexdays);
        return *this;
    };

    inline QList<IndexDay> getIndexes() const { return m_indexdays; };
    inline QVector<const char*> getColumns() const { return m_columns; };
    MyDataFrame toDataFrame() const;
    std::shared_ptr<MyDataFrame> getDataFrame() const;
    vector<double> getOpen() const;
    void setDataFrame();

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    // FIXME: a workaround for pybind11, maybe a bug that pybind11 does not work well with template, since unable get
    // MyDataFrame for binding
    std::vector<double> get_pyseries(const char*) const noexcept;

    // template <>
    xt::xarray<double> toSeries(const char*) const noexcept;

private:
    QList<IndexDay> m_indexdays{};
    const QVector<const char*> m_columns{"open", "close", "high", "low", "vol", "amount", "date", "code", "date_stamp"};
    QStringList m_codes{};
    const char* m_start{};
    const char* m_end{};
    std::shared_ptr<MyDataFrame> m_df{nullptr};

private:
    friend inline QDebug operator<<(QDebug d, const IndexDayAction& ia)
    {
        QVector<const char*> columns = ia.getColumns();
        d << columns << "\n";
        auto qs = ia.getIndexes();
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
QVector<T> IndexDayAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    for (auto s : getIndexes()) {
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

// template <>
xt::xarray<double> IndexDayAction::toSeries(const char* col) const noexcept
{
    std::vector<double> qv = toSeries<double>(col).toStdVector();
    return xt::adapt(qv);
}

} // namespace abq
