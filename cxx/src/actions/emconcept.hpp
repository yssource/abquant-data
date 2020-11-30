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
using namespace hmdf;
using namespace std;
using MyDataFrame = StdDataFrame<std::string>;

/***************************
 * EmConceptAction declaration *
 ***************************/

// template <class CA>
// class Indicator;

template <class CA>
class EmConceptAction : public TActionContext
{
public:
    using self_type    = EmConceptAction<CA>;
    using derived_type = CA;

public:
    // derived_type &derived_cast() & noexcept;
    derived_type* derived_cast() & noexcept;
    const derived_type& derived_cast() const& noexcept;
    derived_type derived_cast() && noexcept;

    // TODO: friend class Indicator<S, derived_type, EmConceptAction>
    QStringList getCodes() const;
    // Indicator<self_type> makeIndicator();

protected:
    //! Default constructor
    EmConceptAction() = default;

    EmConceptAction(QStringList);

    //! Copy constructor
    EmConceptAction(const EmConceptAction& other) = delete;

    //! Move constructor
    EmConceptAction(EmConceptAction&& other) noexcept = delete;

    // ! Destructor
    ~EmConceptAction() noexcept = default;

    //! Copy assignment operator
    EmConceptAction& operator=(const EmConceptAction& other) = default;

    //! Move assignment operator
    EmConceptAction& operator=(EmConceptAction&& other) noexcept = default;

protected:
    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

    MyDataFrame toDataFrame();
    template <typename C>
    QList<C> run(const QStringList codes, const char* start, const char* end);
    // template <typename C>
    // QList<C> run(const QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    // template <typename C>
    // QList<C> run(const QStringList codes, int category = 1);
    template <typename C>
    QList<C> run(const QStringList codes);
    template <typename C>
    QList<C> get_price(const QStringList codes, const char* start, const char* end);
    // template <typename C>
    // QList<C> get_price(const QStringList codes, const char* start, const char* end, MIN_FREQ freq);
    // template <typename C>
    // QList<C> get_price(const QStringList codes, int category);
    template <typename C>
    QList<C> get_blocks(const QStringList codes);

    auto getEmConceptes();

private:
    friend CA;
    // friend class Indicator<self_type>;
    QStringList m_codes;
    const char* m_start;
    const char* m_end;
};

/**********************************
 * EmConceptAction implementation *
 **********************************/

template <class CA>
EmConceptAction<CA>::EmConceptAction(QStringList codes) : m_codes{codes}
{
}

// since: /usr/include/treefrog/tactioncontext.h:59:20: note: 'TActionContext'
// has been explicitly marked deleted here
// T_DICABLE_COPY(TActionContext)
// workaround here, CA*
// template <class CA>
//     inline auto EmConceptAction<CA>::derived_cast() & noexcept -> derived_type & {
//     return *static_cast<derived_type *>(this);
// }

/**
 * @name Downcast functions
 */
//@{
/**
 * Returns a pointer to the actual derived type of the EmConceptAction.
 */
template <class CA>
inline auto EmConceptAction<CA>::derived_cast() & noexcept -> derived_type*
{
    return static_cast<derived_type*>(this);
}

/**
 * Returns a constant reference to the actual derived type of the EmConceptAction.
 */
template <class CA>
inline auto EmConceptAction<CA>::derived_cast() const& noexcept -> const derived_type&
{
    return *static_cast<const derived_type*>(this);
}

/**
 * Returns a constant reference to the actual derived type of the EmConceptAction.
 */
template <class CA>
inline auto EmConceptAction<CA>::derived_cast() && noexcept -> derived_type
{
    return *static_cast<derived_type*>(this);
}

template <class CA>
template <typename C>
QList<C> EmConceptAction<CA>::run(QStringList codes, const char* start, const char* end)
{
    QFuture<QList<C>> future = QtConcurrent::run(this, &self_type::get_price<C>, codes, start, end);

    QList<C> em_concepts = future.result();
    return em_concepts;
}

// template <class CA>
// template <typename C>
// QList<C> EmConceptAction<CA>::run(const QStringList codes, const char* start, const char* end, MIN_FREQ freq)
// {
//     QFuture<QList<C>> future = QtConcurrent::run(this, &self_type::get_price<C>, codes, start, end, freq);

//     QList<C> em_concepts = future.result();
//     return em_concepts;
// }

// template <class CA>
// template <typename C>
// QList<C> EmConceptAction<CA>::run(const QStringList codes, int category)
// {
//     QFuture<QList<C>> future = QtConcurrent::run(this, &self_type::get_price<C>, codes, category);

//     QList<C> em_concepts = future.result();
//     return em_concepts;
// }

template <class CA>
template <typename C>
QList<C> EmConceptAction<CA>::run(const QStringList codes)
{
    QFuture<QList<C>> future = QtConcurrent::run(this, &self_type::get_blocks<C>, codes);

    QList<C> em_concepts = future.result();
    return em_concepts;
}

template <class CA>
template <typename T>
QVector<T> EmConceptAction<CA>::toSeries(const char* col) const noexcept
{
    auto ca = derived_cast();
    return ca->toSeries(col);
}

template <class CA>
template <typename C>
QList<C> EmConceptAction<CA>::get_price(const QStringList codes, const char* start, const char* end)
{
    double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch() * 1000;
    double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch() * 1000;
    auto ca        = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(ca);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<C> em_concepts = C::get_price(codes, start_d, end_d);
    commitTransactions();
    return em_concepts;
}

// template <class CA>
// template <typename C>
// QList<C> EmConceptAction<CA>::get_price(const QStringList codes, const char* start, const char* end, MIN_FREQ freq)
// {
//     double start_d = QDateTime::fromString(QString::fromUtf8(start), Qt::ISODate).toSecsSinceEpoch();
//     double end_d   = QDateTime::fromString(QString::fromUtf8(end), Qt::ISODate).toSecsSinceEpoch();
//     auto ca        = derived_cast();
//     TDatabaseContext::setCurrentDatabaseContext(ca);
//     bool EnableTransactions = true;
//     setTransactionEnabled(EnableTransactions);
//     QList<C> em_concepts = C::get_price(codes, start_d, end_d, freq);
//     commitTransactions();
// }

// template <class CA>
// template <typename C>
// QList<C> EmConceptAction<CA>::get_price(const QStringList codes, int category)
// {
//     auto ca = derived_cast();
//     TDatabaseContext::setCurrentDatabaseContext(ca);
//     bool EnableTransactions = true;
//     setTransactionEnabled(EnableTransactions);
//     QList<C> em_concepts = C::get_price(codes, category);
//     commitTransactions();
//     return em_concepts;
// }

template <class CA>
template <typename C>
QList<C> EmConceptAction<CA>::get_blocks(QStringList codes)
{
    auto ca = derived_cast();
    TDatabaseContext::setCurrentDatabaseContext(ca);
    bool EnableTransactions = true;
    setTransactionEnabled(EnableTransactions);
    QList<C> em_concepts = C::get_blocks(codes);
    commitTransactions();
    return em_concepts;
}

template <class CA>
QStringList EmConceptAction<CA>::getCodes() const
{
    return m_codes;
}

template <class CA>
inline auto EmConceptAction<CA>::getEmConceptes()
{
    auto ca = derived_cast();
    return ca->getEmConceptes();
}

// template <class CA>
// inline auto EmConceptAction<CA>::makeIndicator()->Indicator<self_type>
// {
//     auto ca = derived_cast();
//     return Indicator<self_type>(ca);
// }

} // namespace abq
