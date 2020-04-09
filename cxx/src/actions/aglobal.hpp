/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the Apache License 2.0 License.           *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

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
#define ABQ_DECLARE_PRIVATE(Class)                                                                     \
    inline Class##Private<SA>* d_func()                                                                \
    {                                                                                                  \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private<SA>*>(qGetPtrHelper(d_ptr));)       \
    }                                                                                                  \
    inline const Class##Private<SA>* d_func() const                                                    \
    {                                                                                                  \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private<SA>*>(qGetPtrHelper(d_ptr));) \
    }                                                                                                  \
    friend class Class##Private<SA>;

#define ABQ_DECLARE_PRIVATE_D(Dptr, Class)                                                            \
    inline Class##Private<SA>* d_func()                                                               \
    {                                                                                                 \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private<SA>*>(qGetPtrHelper(Dptr));)       \
    }                                                                                                 \
    inline const Class##Private<SA>* d_func() const                                                   \
    {                                                                                                 \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private<SA>*>(qGetPtrHelper(Dptr));) \
    }                                                                                                 \
    friend class Class##Private<SA>;

#define ABQ_DECLARE_PUBLIC(Class)                                                           \
    inline Class<SA>* q_func() { return static_cast<Class<SA>*>(q_ptr); }                   \
    inline const Class<SA>* q_func() const { return static_cast<const Class<SA>*>(q_ptr); } \
    friend class Class<SA>;

#define ABQ_D(Class) Class##Private<SA>* const d = d_func()
#define ABQ_Q(Class) Class<SA>* const q = q_func()

} // namespace abq
