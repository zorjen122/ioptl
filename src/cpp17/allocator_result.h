/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _ALLOCATOR_RESULT_
#define _ALLOCATOR_RESULT_

#include "cppconfig.h"

namespace iop {

#if IOP_VERSION > 20
    template <class _Ptr> class __allocation_result
    {
        _Ptr ptr;
        size_t count;
    };

    template <class _Ty, class _Alloc>
    [[nodiscard]] IOP_CONSTEXPR_CXX23
        allocation_result<typename allocator_traits<_Ty, _Alloc>::pointer>
        allocate_at_least(_Alloc &__alloc, size_t __n)
    {
        if constexpr (requires { __alloc.allocate_at_least(__n); }) {
            return __alloc.allocate_at_least(__n);
        }
        else {
            return {__alloc.allocate(__n), __n};
        }
    }

    template <class _Alloc>
    [[nodiscard]] IOP_CONSTEXPR_CXX23 auto __allocate_at_least(_Alloc &__alloc,
                                                               size_t __n)
    {
        return ::std::allocate_at_least(__alloc, __n);
    }

#endif
} // namespace iop

#endif //* _ALLOCATOR_RESULT_