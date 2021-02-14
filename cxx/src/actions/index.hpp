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
#include "xtensor/xadapt.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"

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
 * IndexAction declaration *
 ***************************/

template <class IA>
class Indicator;

template <class IA>
class IndexAction : public TActionContext
{
public:
    using self_type    = IndexAction<IA>;
    using derived_type = IA;
    using IndicatorPtr = std::shared_ptr<Indicator<derived_type>>;

public:
    // derived_type &derived_cast() & noexcept;
    derived_type* derived_cast() & noexcept;
    const derived_type& derived_cast() const& noexcept;
    derived_type derived_cast() && noexcept;

    // TODO: friend class Indicator<S, derived_type, IndexAction>
    QStringList getCodes() const;

    IndicatorPtr makeIndicator();

protected:
    //! Default constructor
    IndexAction() = default;

    IndexAction(QStringList);

    //! Copy constructor
    IndexAction(const IndexAction& other) = delete;

    //! Move constructor
    IndexAction(IndexAction&& other) noexcept = delete;

    // ! Destructor
    ~IndexAction() noexcept = default;

    //! Copy assignment operator
    IndexAction& operator=(const IndexAction& other) = default;

    //! Move assignment operator
    IndexAction& operator=(IndexAction&& other) noexcept = default;

protected:
    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    MyDataFrame toDataFrame();
    template <typename I>
    QList<I> run(const QStringList codes, const char* start, const char* end);
    template <typename I>
    QList<I> run(const QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    template <typename I>
    QList<I> run(const QStringList codes, int category = 1);
    template <typename I>
    QList<I> get_price(const QStringList codes, const char* start, const char* end);
    template <typename I>
    QList<I> get_price(const QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    template <typename I>
    QList<I> get_price(const QStringList codes, int category);

    auto getIndexes();

private:
    friend IA;
    friend class Indicator<self_type>;
    QStringList m_codes;
    const char* m_start;
    const char* m_end;
};

/******************************
 * IndexAction implementation *
 ******************************/

template <class IA>
IndexAction<IA>::IndexAction(QStringList codes) : m_codes{codes}
{
}

/**
 * @name Downcast functions
 */
//@{
/**
 * Returns a pointer to the actual derived type of the IndexAction.
 */
template <class IA>
inline auto IndexAction<IA>::derived_cast() & noexcept -> derived_type*
{
    return static_cast<derived_type*>(this);
}

/**
 * Returns a constant reference to the actual derived type of the IndexAction.
 */
template <class IA>
inline auto IndexAction<IA>::derived_cast() const& noexcept -> const derived_type&
{
    return *static_cast<const derived_type*>(this);
}

/**
 * Returns a constant reference to the actual derived type of the IndexAction.
 */
template <class IA>
inline auto IndexAction<IA>::derived_cast() && noexcept -> derived_type
{
    return *static_cast<derived_type*>(this);
}

template <class IA>
template <typename I>
QList<I> IndexAction<IA>::run(const QStringList codes, const char* start, const char* end)
{
    QFuture<QList<I>> future = QtConcurrent::run(this, &self_type::get_price<I>, codes, start, end);

    QList<I> indexes = future.result();
    return indexes;
}

template <class IA>
template <typename I>
QList<I> IndexAction<IA>::run(const QStringList codes, const char* start, const char* end, MIN_FREQ freq)
{
    QFuture<QList<I>> future = QtConcurrent::run(this, &self_type::get_price<I>, codes, start, end, freq);

    QList<I> indexes = future.result();
    return indexes;
}

template <class IA>
template <typename I>
QList<I> IndexAction<IA>::run(const QStringList codes, int category)
{
    QFuture<QList<I>> future = QtConcurrent::run(this, &self_type::get_price<I>, codes, category);

    QList<I> indexes = future.result();
    return indexes;
}

template <class IA>
template <typename T>
QVector<T> IndexAction<IA>::toSeries(const char* col) const noexcept
{
    auto ia = derived_cast();
    return ia->toSeries(col);
}

template <class IA>
template <typename I>
QList<I> IndexAction<IA>::get_price(const QStringList codes, const char* start, const char* end)
{
    double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch();
    double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    auto ia        = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(ia);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<I> indexes = I::get_price(codes, start_d, end_d);
    commitTransactions();
    return indexes;
}

template <class IA>
template <typename I>
QList<I> IndexAction<IA>::get_price(const QStringList codes, const char* start, const char* end, MIN_FREQ freq)
{
    double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch();
    double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
    auto ia        = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(ia);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<I> indexes = I::get_price(codes, start_d, end_d, freq);
    commitTransactions();
    return indexes;
}

template <class IA>
template <typename I>
QList<I> IndexAction<IA>::get_price(const QStringList codes, int category)
{
    auto ia = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(ia);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<I> indexes = I::get_price(codes, category);
    commitTransactions();
    return indexes;
}

template <class IA>
QStringList IndexAction<IA>::getCodes() const
{
    return m_codes;
}

template <class IA>
inline auto IndexAction<IA>::getIndexes()
{
    auto ia = derived_cast();
    return ia->getIndexes();
}

template <class IA>
inline auto IndexAction<IA>::makeIndicator() -> IndicatorPtr
{
    auto ia = derived_cast();
    return std::make_shared<Indicator<derived_type>>(ia);
}

} // namespace abq
