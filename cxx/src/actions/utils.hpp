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

enum class MIN_FREQ : short int { ONE = 0x01, FIVE = 0x05, FIFTEEN = 0x0F, THIRTY = 0x1E, SIXTY = 0x3C };

inline QDebug operator<<(QDebug debug, MIN_FREQ freq)
{
    switch (freq) {
    case MIN_FREQ::ONE:
        debug << "MIN_FREQ::ONE";
        break;
    case MIN_FREQ::FIVE:
        debug << "MIN_FREQ::FIVE";
        break;
    case MIN_FREQ::FIFTEEN:
        debug << "MIN_FREQ::FIFTEEN";
        break;
    case MIN_FREQ::THIRTY:
        debug << "MIN_FREQ::THIRTY";
        break;
    case MIN_FREQ::SIXTY:
        debug << "MIN_FREQ::SIXTY";
        break;
    }
    return debug;
}

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
