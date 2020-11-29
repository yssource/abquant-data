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
#include "abquant/models/emconceptbase.h" //  include the model class

namespace abq
{
using namespace std;

class EmConceptBaseAction : public EmConceptAction<EmConceptBaseAction>
{
public:
    //! Default constructor
    EmConceptBaseAction() = default;

    EmConceptBaseAction(QStringList codes);

    //! Copy constructor
    EmConceptBaseAction(const EmConceptBaseAction& other) = delete;

    //! Move constructor
    EmConceptBaseAction(EmConceptBaseAction&& other) noexcept = delete;

    //! Destructor
    ~EmConceptBaseAction() noexcept = default;

    //! Copy assignment operator
    EmConceptBaseAction& operator=(const EmConceptBaseAction& other) = default;

    //! Move assignment operator
    EmConceptBaseAction& operator=(EmConceptBaseAction&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }
        std::swap(m_codes, other.m_codes);
        std::swap(m_start, other.m_start);
        std::swap(m_end, other.m_end);
        std::swap(m_emconceptbases, other.m_emconceptbases);
        return *this;
    };

    inline QList<EmConceptBase> getEmConceptes() const { return m_emconceptbases; };
    inline QVector<const char*> getColumns() const { return m_columns; };
    // MyDataFrame toDataFrame() const;
    // std::shared_ptr<MyDataFrame> getDataFrame() const;
    // vector<double> getOpen() const;
    // void setDataFrame();

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    // // FIXME: a workaround for pybind11, maybe a bug that pybind11 does not work well with template, since unable get
    // // MyDataFrame for binding
    // std::vector<double> get_pyseries(const char*) const noexcept;

    // template <>
    xt::xarray<double> toSeries(const char*) const noexcept;

private:
    QList<EmConceptBase> m_emconceptbases{};
    const QVector<const char*> m_columns{"f104_bk_rise_cnt", "f105_bk_fall_cnt",    "f12_code",   "f13_market",
                                         "f14_name",         "f3_bk_rise_fall_pct", "f8_turnover"};
    QStringList m_codes{};
    const char* m_start{};
    const char* m_end{};
    // std::shared_ptr<MyDataFrame> m_df{nullptr};

private:
    friend inline QDebug operator<<(QDebug d, const EmConceptBaseAction& ia)
    {
        QVector<const char*> columns = ia.getColumns();
        d << columns << "\n";
        auto qs = ia.getEmConceptes();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.f104BkRiseCnt()) << QVariant(s.f105BkFallCnt()) << QVariant(s.f12Code())
                      << QVariant(s.f13Market()) << QVariant(s.f14Name()) << QVariant(s.f3BkRiseFallPct())
                      << QVariant(s.f8Turnover());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> EmConceptBaseAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }

    for (auto s : getEmConceptes()) {
        if constexpr (std::is_same_v<T, int>) {
            if (QString("f104_bk_rise_cnt") == QString(col)) {
                series << s.f104BkRiseCnt();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, int>) {
            if (QString("f105_bk_fall_cnt") == QString(col)) {
                series << s.f105BkFallCnt();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("f12_code") == QString(col)) {
                series << s.f12Code().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, int>) {
            if (QString("f13_market") == QString(col)) {
                series << s.f13Market();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("f14_name") == QString(col)) {
                series << s.f14Name().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("f3_bk_rise_fall_pct") == QString(col)) {
                series << s.f3BkRiseFallPct();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("f8_turnover") == QString(col)) {
                series << s.f8Turnover();
                continue;
            }
        }
    }
    return series;
}

// template <>
xt::xarray<double> EmConceptBaseAction::toSeries(const char* col) const noexcept
{
    std::vector<double> qv = toSeries<double>(col).toStdVector();
    return xt::adapt(qv);
}

} // namespace abq
