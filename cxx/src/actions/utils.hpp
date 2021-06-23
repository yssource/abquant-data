/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once
#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore/QVector>
#include <algorithm>
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "DataFrame/DataFrame.h"
#include "abquant/helpers/applicationhelper.h"
#include "xtensor/xarray.hpp"

namespace abq
{
using namespace hmdf;
using namespace std;

template <typename T, std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, int> = 0>
void xfillna(xt::xarray<T>& v, T n)
{
    std::transform(std::begin(v), std::end(v), std::begin(v), [n](T m) -> T {
        if (std::isnan(m)) {
            return n;
        } else {
            return m;
        }
    });
}

enum class FQ_TYPE : short int {
    NONE = 0x00,
    PRE  = 0x01,
    POST = 0xFF,
};

inline QDebug operator<<(QDebug debug, FQ_TYPE fq)
{
    switch (fq) {
    case FQ_TYPE::NONE:
        debug << "FQ_TYPE::NONE";
        break;
    case FQ_TYPE::PRE:
        debug << "FQ_TYPE::PRE";
        break;
    case FQ_TYPE::POST:
        debug << "FQ_TYPE::POST";
        break;
    }
    return debug;
}

enum class INSTRUMENT_TYPE : short int {
    CS          = 0x00,
    FUTURE      = 0x01,
    ETF         = 0x02,
    INDX        = 0x03,
    FENJI_MU    = 0x04,
    FENJI_A     = 0x05,
    FENJI_B     = 0x06,
    PUBLIC_FUND = 0x07,
    BOND        = 0x08,
    CONVERTIBLE = 0x09,
    SPOT        = 0x0a,
    REPO        = 0x0b,
};

inline QDebug operator<<(QDebug debug, INSTRUMENT_TYPE ins)
{
    switch (ins) {
    case INSTRUMENT_TYPE::CS:
        debug << "INSTRUMENT_TYPE::CS";
        break;
    case INSTRUMENT_TYPE::FUTURE:
        debug << "INSTRUMENT_TYPE::FUTURE";
        break;
    case INSTRUMENT_TYPE::ETF:
        debug << "INSTRUMENT_TYPE::ETF";
        break;
    case INSTRUMENT_TYPE::INDX:
        debug << "INSTRUMENT_TYPE::INDX";
        break;
    case INSTRUMENT_TYPE::FENJI_MU:
        debug << "INSTRUMENT_TYPE::FENJI_MU";
        break;
    case INSTRUMENT_TYPE::FENJI_A:
        debug << "INSTRUMENT_TYPE::FENJI_A";
        break;
    case INSTRUMENT_TYPE::FENJI_B:
        debug << "INSTRUMENT_TYPE::FENJI_B";
        break;
    case INSTRUMENT_TYPE::PUBLIC_FUND:
        debug << "INSTRUMENT_TYPE::PUBLIC_FUND";
        break;
    case INSTRUMENT_TYPE::BOND:
        debug << "INSTRUMENT_TYPE::BOND";
        break;
    case INSTRUMENT_TYPE::CONVERTIBLE:
        debug << "INSTRUMENT_TYPE::CONVERTIBLE";
        break;
    case INSTRUMENT_TYPE::SPOT:
        debug << "INSTRUMENT_TYPE::SPOT";
        break;
    case INSTRUMENT_TYPE::REPO:
        debug << "INSTRUMENT_TYPE::REPO";
        break;
    }
    return debug;
}

// enum class MIN_FREQ : short int { ONE = 0x01, FIVE = 0x05, FIFTEEN = 0x0F, THIRTY = 0x1E, SIXTY = 0x3C };

// inline QDebug operator<<(QDebug debug, MIN_FREQ freq)
// {
//     switch (freq) {
//     case MIN_FREQ::ONE:
//         debug << "MIN_FREQ::ONE";
//         break;
//     case MIN_FREQ::FIVE:
//         debug << "MIN_FREQ::FIVE";
//         break;
//     case MIN_FREQ::FIFTEEN:
//         debug << "MIN_FREQ::FIFTEEN";
//         break;
//     case MIN_FREQ::THIRTY:
//         debug << "MIN_FREQ::THIRTY";
//         break;
//     case MIN_FREQ::SIXTY:
//         debug << "MIN_FREQ::SIXTY";
//         break;
//     }
//     return debug;
// }

template <template <typename...> class base, typename derived>
struct is_base_of_template_impl {
    template <typename... Ts>
    static constexpr std::true_type test(const base<Ts...>*);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<derived*>()));
};

template <template <typename...> class base, typename derived>
using is_base_of_template = typename is_base_of_template_impl<base, derived>::type;

#define CHECK_COLUMN_EXIST(col)                                                                                  \
    std::vector<const char*> cols = {"open", "close", "high", "low", "vol", "amount"};                           \
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) { \
        QString msg = QString(col) + QString(" series are not available here.");                                 \
        throw std::runtime_error(msg.toStdString());                                                             \
    }

} // namespace abq
