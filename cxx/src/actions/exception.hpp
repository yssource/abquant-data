/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once
#include <stdexcept>

#include "abquant/actions/config.hpp"

namespace abq
{
struct AbqQuantError : public std::runtime_error {
    explicit AbqQuantError(const char* desc) : std::runtime_error(desc) {}
};
struct NotImplemented : public AbqQuantError {
    explicit NotImplemented(const char* desc) : AbqQuantError(desc) {}
};

#if (defined(ABQUANT_ENABLE_ASSERT) && !defined(ABQUANT_DISABLE_EXCEPTIONS))
#define ABQUANT_TRY(expr) ABQUANT_TRY_IMPL(expr, __FILE__, __LINE__)
#define ABQUANT_TRY_IMPL(expr, file, line)                                                                            \
    try {                                                                                                             \
        expr;                                                                                                         \
    } catch (std::exception & e) {                                                                                    \
        ABQUANT_THROW(std::runtime_error,                                                                             \
                      std::string(file) + ':' + std::to_string(line) + ": check failed\n\t" + std::string(e.what())); \
    }
#else
#define ABQUANT_TRY(expr)
#endif

} // namespace abq
