/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <QDataStream>
#include <QFuture>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QtConcurrent/QtConcurrentRun>
#include <TActionContext>
#include <TfTest/TfTest>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>

#include "DataFrame/DataFrame.h"
#include "abquant/actions/indicator.hpp"
#include "abquant/actions/utils.hpp"

namespace abq
{
using namespace std;
using namespace hmdf;
using index_type     = std::string;
using MyDataFrame    = StdDataFrame<index_type>;
using MyDataFramePtr = std::shared_ptr<MyDataFrame>;

using xseries_cst_type    = const xt::xarray<double>&;
using xseries_type        = xt::xarray<double>&;
using series_cst_type     = const std::vector<double>&;
using series_type         = std::vector<double>&;
using xseries_no_cvp_type = std::decay<xseries_type>::type;
using series_no_cvp_type  = std::decay<series_type>::type;

/***************************
 * SecurityAction declaration *
 ***************************/
template <class SA>
class SecurityAction : public TActionContext
{
public:
    using self_type    = SecurityAction<SA>;
    using derived_type = SA;
    using IndicatorPtr = std::shared_ptr<Indicator<derived_type>>;

public:
    // derived_type &derived_cast() & noexcept;

    derived_type* derived_cast() & noexcept;
    const derived_type& derived_cast() const& noexcept;
    derived_type derived_cast() && noexcept;

    IndicatorPtr makeIndicator();

protected:
    //! Default constructor
    SecurityAction() = default;

    SecurityAction(QStringList);

    //! Copy constructor
    SecurityAction(const SecurityAction& other) = delete;

    //! Move constructor
    SecurityAction(SecurityAction&& other) noexcept = delete;

    // ! Destructor
    ~SecurityAction() noexcept = default;

    //! Copy assignment operator
    SecurityAction& operator=(const SecurityAction& other) = default;

    //! Move assignment operator
    SecurityAction& operator=(SecurityAction&& other) noexcept = default;

protected:
    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    template <typename S>
    QList<S> run(const QStringList codes, const char* start, const char* end);
    template <typename S>
    QList<S> run(const QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    template <typename S>
    QList<S> run(const QStringList codes, int category = 1);
    template <typename S>
    QList<S> run(const char* end = "");
    template <typename S>
    QList<S> run(const QStringList codes, const char* end = "");

    template <typename S>
    QList<S> get_price(const QStringList codes, const char* start, const char* end);
    template <typename S>
    QList<S> get_price(const QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    template <typename S>
    QList<S> get_price(const QStringList codes, int category);
    template <typename S>
    QList<S> get_all_securities(const char* end = "");
    template <typename S>
    QList<S> get_all_securities(const QStringList codes, const char* end = "");

    auto getSecurities();

private:
    friend SA;
    friend class Indicator<self_type>;
};

/******************************
 * SecurityAction implementation *
 ******************************/
/**
 * @name Downcast functions
 */
//@{
/**
 * Returns a pointer to the actual derived type of the SecurityAction.
 */
template <class SA>
inline auto SecurityAction<SA>::derived_cast() & noexcept -> derived_type*
{
    return static_cast<derived_type*>(this);
}

/**
 * Returns a constant reference to the actual derived type of the SecurityAction.
 */
template <class SA>
inline auto SecurityAction<SA>::derived_cast() const& noexcept -> const derived_type&
{
    return *static_cast<const derived_type*>(this);
}

/**
 * Returns a constant reference to the actual derived type of the SecurityAction.
 */
template <class SA>
inline auto SecurityAction<SA>::derived_cast() && noexcept -> derived_type
{
    return *static_cast<derived_type*>(this);
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::run(const QStringList codes, const char* start, const char* end)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &self_type::get_price<S>, codes, start, end);

    QList<S> secriies = future.result();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::run(const QStringList codes, const char* start, const char* end, MIN_FREQ freq)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &self_type::get_price<S>, codes, start, end, freq);

    QList<S> secriies = future.result();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::run(const QStringList codes, int category)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &self_type::get_price<S>, codes, category);

    QList<S> secriies = future.result();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::run(const char* end)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &self_type::get_all_securities<S>, end);
    QList<S> secriies        = future.result();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::run(const QStringList codes, const char* end)
{
    QFuture<QList<S>> future = QtConcurrent::run(this, &self_type::get_all_securities<S>, codes, end);
    QList<S> secriies        = future.result();
    return secriies;
}

template <class SA>
template <typename T>
QVector<T> SecurityAction<SA>::toSeries(const char* col) const noexcept
{
    auto sa = derived_cast();
    return sa->toSeries(col);
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::get_price(const QStringList codes, const char* start, const char* end)
{
    double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch();
    double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    auto sa        = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    TDatabaseContext::setTransactionEnabled(EnableTransactions);
    QList<S> secriies = S::get_price(codes, start_d, end_d);
    commitTransactions();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::get_price(const QStringList codes, const char* start, const char* end, MIN_FREQ freq)
{
    double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch();
    double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    auto sa        = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<S> secriies = S::get_price(codes, start_d, end_d, freq);
    commitTransactions();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::get_price(const QStringList codes, int category)
{
    auto sa = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<S> secriies = S::get_price(codes, category);
    commitTransactions();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::get_all_securities(const char* end)
{
    double end_d = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    auto sa      = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<S> secriies = S::get_all_securities(end_d);
    commitTransactions();
    return secriies;
}

template <class SA>
template <typename S>
QList<S> SecurityAction<SA>::get_all_securities(const QStringList codes, const char* end)
{
    double end_d = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    auto sa      = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(sa);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<S> secriies = S::get_all_securities(codes, end_d);
    commitTransactions();
    return secriies;
}

template <class SA>
inline auto SecurityAction<SA>::getSecurities()
{
    auto sa = derived_cast();
    return sa->getSecurities();
}

template <class SA>
inline auto SecurityAction<SA>::makeIndicator() -> IndicatorPtr
{
    auto sa = derived_cast();
    return std::make_shared<Indicator<derived_type>>(sa);
}

} // namespace abq
