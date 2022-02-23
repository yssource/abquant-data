/****************************************************************************
 * Copyright (c) 2020-present Jimmy M. Gong                                 *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#pragma once

#include <QtCore/qglobal.h>

namespace abq
{
template <typename T>
inline T* qGetPtrHelper(T* ptr)
{
    return ptr;
}
template <typename Ptr>
inline auto qGetPtrHelper(Ptr& ptr) -> decltype(ptr.operator->())
{
    return ptr.operator->();
}

// The body must be a statement:
#define ABQ_DECLARE_PRIVATE(Class)                                                                    \
    inline Class##Private<A>* d_func()                                                                \
    {                                                                                                 \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private<A>*>(qGetPtrHelper(d_ptr));)       \
    }                                                                                                 \
    inline const Class##Private<A>* d_func() const                                                    \
    {                                                                                                 \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private<A>*>(qGetPtrHelper(d_ptr));) \
    }                                                                                                 \
    friend class Class##Private<A>;

#define ABQ_DECLARE_PRIVATE_D(Dptr, Class)                                                           \
    inline Class##Private<A>* d_func()                                                               \
    {                                                                                                \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private<A>*>(qGetPtrHelper(Dptr));)       \
    }                                                                                                \
    inline const Class##Private<A>* d_func() const                                                   \
    {                                                                                                \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private<A>*>(qGetPtrHelper(Dptr));) \
    }                                                                                                \
    friend class Class##Private<A>;

#define ABQ_DECLARE_PUBLIC(Class)                                                         \
    inline Class<A>* q_func() { return static_cast<Class<A>*>(q_ptr); }                   \
    inline const Class<A>* q_func() const { return static_cast<const Class<A>*>(q_ptr); } \
    friend class Class<A>;

#define ABQ_D(Class) Class##Private<A>* const d = d_func()
#define ABQ_Q(Class) Class<A>* const q = q_func()

} // namespace abq
