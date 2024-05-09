 /*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _ALLOC_OPERATOR_NEW_IOP_
#define _ALLOC_OPERATOR_NEW_IOP_

#include "cppconfig.h"
#include "utilitys.h"
#include <type_traits>

namespace iop {

    template <class _Ty> class __Default_alloc
    {
      public:
        using value_type = _Ty;
        using pointer = _Ty *;
        using const_pointer = const _Ty *;
        using void_pointer = void *;
        using const_void_pointer = const void *;

        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;

        IOP_CONSTEXPR __Default_alloc() noexcept = default;

#if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23 __allocation_result<_Ty>
        allocate_at_least(size_type __n)
        {
            return {allocate(__n), __n};
        }
#endif

        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(pointer __ptr,
                                                   const size_type __n)
        {
            if (__ptr != nullptr)
                ::operator delete(__ptr);
        }
        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(pointer __ptr)
        {
            if (__ptr != nullptr)
                ::operator delete(__ptr);
        }

        IOP_STATIC_CONSTEXPR_CXX17 pointer allocate(size_type __n)
        {
            return 0 == __n ? nullptr
                            : static_cast<pointer>(
                                  ::operator new(__n * sizeof(value_type)));
        }

        IOP_STATIC_CONSTEXPR_CXX17 pointer allocate(void)
        {
            return static_cast<pointer>(::operator new(sizeof(value_type)));
        }
    };

    template <class _Ty> class __Default_alloc<const _Ty>
    {
      public:
        using value_type = const _Ty;
        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;

        IOP_CONSTEXPR __Default_alloc() noexcept = default;

        template <class _Tp>
        IOP_CONSTEXPR __Default_alloc(const __Default_alloc<_Tp> &) noexcept
        {}

#if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23 __allocation_result<const _Ty>
        allocate_at_least(const size_type __n)
        {
            return {allocate(__n), __n};
        }
#endif

        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(const _Ty *__ptr,
                                                   const size_type __n)
        {
            if (__ptr != nullptr)
                ::operator delete(const_cast<_Ty *>(__ptr));
        }
        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(const _Ty *__ptr)
        {
            if (__ptr != nullptr)
                ::operator delete(const_cast<_Ty *>(__ptr));
        }

        IOP_STATIC_CONSTEXPR_CXX17 const _Ty *allocate(size_type __n)
        {
            return 0 == __n ? nullptr
                            : const_cast<_Ty *>(
                                  ::operator new(__n * sizeof(value_type)));
        }

        IOP_STATIC_CONSTEXPR_CXX17 const _Ty *allocate(void)
        {
            return const_cast<_Ty *>(::operator new(sizeof(value_type)));
        }

    }; //* namespace iop

    template <class _Ty> struct allocator : public __Default_alloc<_Ty>
    { };

} // namespace iop

#endif //* _ALLOC_OPERATOR_NEW_IOP_
