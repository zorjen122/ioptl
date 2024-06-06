 /*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _ALLOCATOR_IOP_
#define _ALLOCATOR_IOP_

#include "cppconfig.h"

#include <cstddef>  // ptrdiff_t

NAMESPACE_BEGIN(iop)

    template <class T> class allocator
    {
      public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;

        IOP_CONSTEXPR allocator() noexcept = default;

#if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23 __allocation_result<T>
        allocate_at_least(size_type __n)
        {
            return {allocate(__n), __n};
        }
#endif

        static constexpr void deallocate(T* __ptr,
                                                   const size_type __n)
        {
            if (__ptr != nullptr)
                ::operator delete(__ptr);
        }
        static constexpr void deallocate(T* __ptr)
        {
            if (__ptr != nullptr)
                ::operator delete(__ptr);
        }

        static constexpr T* allocate(size_type __n)
        {
            return 0 == __n ? nullptr
                            : static_cast<T*>(
                                  ::operator new(__n * sizeof(value_type)));
        }

        static constexpr T* allocate(void)
        {
            return static_cast<T*>(::operator new(sizeof(value_type)));
        }
    };

NAMESPACE_END(iop)

#endif //* _ALLOCATOR_IOP_
