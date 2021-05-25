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
// #include "abquant/models/securitylist.h"

namespace abq
{
template <typename M>
class SecuritylistAction : public StockAction<SecuritylistAction<S>>, std::enable_shared_from_this<SecuritylistAction<S>>
{
public:
    //! Default constructor
    // SecuritylistAction() = default;

    SecuritylistAction();

    SecuritylistAction(const QStringList codes, const char* end = "");

    //! Copy constructor
    SecuritylistAction(const SecuritylistAction& other) = delete;

    //! Move constructor
    SecuritylistAction(SecuritylistAction&& other) noexcept = delete;

    //! Destructor
    ~SecuritylistAction() = default;

    //! Copy assignment operator
    SecuritylistAction& operator=(const SecuritylistAction& other) = default;

    //! Move assignment operator
    SecuritylistAction& operator=(SecuritylistAction&& other) noexcept;

    QStringList getCodes() const { return m_codes; };
    QList<S> getStocks() const { return m_security_list; };
    QVector<const char*> getColumns() const { return m_columns; };
    MyDataFramePtr getDataFrame() const;

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

private:
    QList<S> m_security_list;
    const QVector<const char*> m_columns{"code", "volunit", "decimal_point", "name", "pre_close", "sse", "sec"};
    QStringList m_codes;
    MyDataFramePtr m_df{nullptr};

private:
    void setDataFrame();

    friend inline QDebug operator<<(QDebug d, const SecuritylistAction& sa)
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

/***********************
 * SecuritylistAction impl *
 **********************/

template <typename M>
SecuritylistAction<S>::SecuritylistAction()
{
    m_security_list = this->template run<S>("");
    if (m_security_list.isEmpty()) {
        qDebug() << "No stock list data.\n";
    }
    setDataFrame();
}

template <typename M>
SecuritylistAction<S>::SecuritylistAction(const QStringList codes, const char* end)
{
    m_security_list = run<S>(codes, end);
    if (m_security_list.isEmpty()) {
        qDebug() << "No stock list data.\n"
                 << codes << "\n"
                 << "end: " << end << "\n";
    }
    setDataFrame();
}

template <typename M>
MyDataFramePtr SecuritylistAction<S>::getDataFrame() const
{
    // m_df->template write<std::ostream, index_type, double, int>(std::cout);
    return m_df;
}

template <typename M>
void SecuritylistAction<S>::setDataFrame()
{
    MyDataFrame df;
    try {
        std::vector<index_type> datetimeCodeIdx;
        std::vector<std::string> code;
        series_no_cvp_type volunit;
        series_no_cvp_type decimal_point;
        std::vector<std::string> name;
        series_no_cvp_type pre_close;
        std::vector<std::string> sse;
        std::vector<std::string> sec;

        foreach (auto s, m_security_list) {
            datetimeCodeIdx.push_back(s.code().toStdString());
            // code.push_back(s.code().toStdString());
            volunit.push_back(s.volunit());
            decimal_point.push_back(s.decimalPoint());
            name.push_back(s.name().toStdString());
            pre_close.push_back(s.preClose());
            sse.push_back(s.sse().toStdString());
            sec.push_back(s.sec().toStdString());
        }

        df.load_data(std::move(datetimeCodeIdx),
                     /* std::make_pair("code", code),  */ std::make_pair("volunit", volunit),
                     std::make_pair("decimal_point", decimal_point), std::make_pair("name", name),
                     std::make_pair("pre_close", pre_close), std::make_pair("sse", sse), std::make_pair("sec", sec));

        // df.template write<std::ostream, index_type, double, int>(std::cout);
    } catch (exception& e) {
        cout << e.what() << endl;
    }

    m_df = std::make_shared<MyDataFrame>(df);
};

template <typename M>
template <typename T>
QVector<T> SecuritylistAction<S>::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
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
