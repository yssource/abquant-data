/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <experimental/propagate_const>
#include <memory>
#include <type_traits>

#include "DataFrame/DataFrame.h"
#include "abquant/actions/utils.hpp"

namespace abq
{
using namespace std;
using namespace hmdf;
using index_type     = std::string;
using MyDataFrame    = StdDataFrame<index_type>;
using MyDataFramePtr = std::shared_ptr<MyDataFrame>;

class Xdxr
{
public:
    Xdxr(const QStringList& codes);

    //! Copy constructor
    Xdxr(const Xdxr& other) = default;

    //! Move constructor
    Xdxr(Xdxr&& other) noexcept = default;

    //! Copy assignment operator
    Xdxr& operator=(const Xdxr& other) = default;

    //! Move assignment operator
    Xdxr& operator=(Xdxr&& other) noexcept;

    ~Xdxr() noexcept;

    MyDataFramePtr getXdxr(const MyDataFramePtr df, FQ_TYPE fq);

private:
    class impl;
    std::experimental::propagate_const<std::shared_ptr<impl>> pImpl;
};
} // namespace abq
