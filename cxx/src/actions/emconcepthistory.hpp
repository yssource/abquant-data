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

#include "abquant/actions/emconcept.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/emconcepthistory.h"

namespace abq
{
using namespace std;

class EmConceptHistoryAction : public EmConceptAction<EmConceptHistoryAction>
{
public:
    //! Default constructor
    EmConceptHistoryAction() = default;

    EmConceptHistoryAction(QStringList codes, const char* start, const char* end);

    //! Copy constructor
    EmConceptHistoryAction(const EmConceptHistoryAction& other) = delete;

    //! Move constructor
    EmConceptHistoryAction(EmConceptHistoryAction&& other) noexcept = delete;

    //! Destructor
    ~EmConceptHistoryAction() noexcept = default;

    //! Copy assignment operator
    EmConceptHistoryAction& operator=(const EmConceptHistoryAction& other) = default;

    //! Move assignment operator
    EmConceptHistoryAction& operator=(EmConceptHistoryAction&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }
        std::swap(m_codes, other.m_codes);
        std::swap(m_start, other.m_start);
        std::swap(m_end, other.m_end);
        std::swap(m_emconcepthistorys, other.m_emconcepthistorys);
        return *this;
    };

    inline QList<EmConceptHistory> getEmConceptes() const { return m_emconcepthistorys; };
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

private:
    QList<EmConceptHistory> m_emconcepthistorys{};
    const QVector<const char*> m_columns{"date",     "open",     "close",     "high",          "low",
                                         "volume",   "amount",   "amplitude", "rise_fall_pct", "rise_fall_amt",
                                         "turnover", "mkt_code", "date_stamp"};
    QStringList m_codes{};
    const char* m_start{};
    const char* m_end{};
    std::shared_ptr<MyDataFrame> m_df{nullptr};

private:
    friend inline QDebug operator<<(QDebug d, const EmConceptHistoryAction& ia)
    {
        QVector<const char*> columns = ia.getColumns();
        d << columns << "\n";
        auto qs = ia.getEmConceptes();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.date()) << QVariant(s.open()) << QVariant(s.close()) << QVariant(s.high())
                      << QVariant(s.low()) << QVariant(s.volume()) << QVariant(s.amount()) << QVariant(s.amplitude())
                      << QVariant(s.riseFallPct()) << QVariant(s.riseFallAmt()) << QVariant(s.turnover())
                      << QVariant(s.mktCode()) << QVariant(s.dateStamp());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> EmConceptHistoryAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    for (auto s : getEmConceptes()) {
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("date") == QString(col)) {
                series << s.date().toStdString();
                continue;
            }
        }
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
            if (QString("volume") == QString(col)) {
                series << s.volume();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("amount") == QString(col)) {
                series << s.amount();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("amplitude") == QString(col)) {
                series << s.amplitude();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("rise_fall_pct") == QString(col)) {
                series << s.riseFallPct();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("rise_fall_amt") == QString(col)) {
                series << s.riseFallAmt();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("turnover") == QString(col)) {
                series << s.turnover();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("mkt_code") == QString(col)) {
                series << s.mktCode().toStdString();
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
