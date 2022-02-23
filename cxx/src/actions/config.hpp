/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

// Define if the library is going to be using exceptions.
#if (!defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND))
#undef ABQUANT_DISABLE_EXCEPTIONS
#define ABQUANT_DISABLE_EXCEPTIONS
#endif

// Exception support.
#if defined(ABQUANT_DISABLE_EXCEPTIONS)
#include <iostream>
#define ABQUANT_THROW(_, msg)          \
    {                                  \
        std::cerr << msg << std::endl; \
        std::abort();                  \
    }
#else
#define ABQUANT_THROW(exception, msg) throw exception(msg)
#endif
