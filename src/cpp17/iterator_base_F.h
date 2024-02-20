/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#define _ITERATOR_BASE_IOP_
#ifdef _ITERATOR_BASE_IOP_

#include "algobase_F.h"
#include "cppconfig.h"
#include "deftype.h"
#include "metacomponent.h"
#include "type_traits.h"
#include "utilitys.h"

#include <assert.h>
#include <new>
#include <utility>

#define ITERAOTR_TAG_DEFINE(NAME, IS_A_OBJ)                                    \
    template <class _Ty, class _Distance>                                      \
    struct NAME : public IS_A_OBJ<_Ty, _Distance>                              \
    {                                                                          \
        using iterator_category = NAME##_tag;                                  \
        using value_type = _Ty;                                                \
        using pointer = _Ty *;                                                 \
        using reference = _Ty &;                                               \
        using difference_type = _Distance;                                     \
    };

#define IS_ITERATOR_TAG_TOTAL(ITERATOR_TAG)                                    \
    template <class _Ty>                                                       \
    using is_##ITERATOR_TAG = mpls::is_same<_Ty, ITERATOR_TAG>;                \
    template <class _Ty>                                                       \
    using is_##ITERATOR_TAG##_t = typename is_##ITERATOR_TAG<_Ty>::type;       \
    template <class _Ty>                                                       \
    constexpr bool is_##ITERATOR_TAG##_v = is_##ITERATOR_TAG<_Ty>::value;

namespace iop {

    // Iterator type
    struct input_iterator_tag
    {};
    struct output_iterator_tag
    {};
    struct forward_iterator_tag : public input_iterator_tag
    {};
    struct bidirectional_iterator_tag : public forward_iterator_tag
    {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag
    {};

    // Iterator
    template <class _Category, class _Ty, class _Distance = ::std::ptrdiff_t,
              class _Pointer = _Ty *, class _Reference = _Ty &>
    class iterator
    {
      public:
        using iterator_category = _Category;

        using value_type = _Ty;
        using pointer = _Ty *;
        using reference = _Ty &;
        using difference_type = _Distance;
    };

    template <class _Ty, class _Distance> struct input_iterator
    {
        using iterator_category = input_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty *;
        using reference = _Ty &;
        using difference_type = _Distance;
    };

    struct output_iterator
    {
        using iterator_category = output_iterator_tag;

        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
    };

    ITERAOTR_TAG_DEFINE(forward_iterator, input_iterator);
    ITERAOTR_TAG_DEFINE(bidirectional_iterator, forward_iterator);
    ITERAOTR_TAG_DEFINE(random_access_iterator, bidirectional_iterator);

    // iterator traits
    template <class _Iterator> struct iterator_traits
    {
        using iterator_category = typename _Iterator::iterator_category;

        using value_type = typename _Iterator::value_type;
        using pointer = typename _Iterator::pointer;
        using reference = typename _Iterator::reference;
        using difference_type = typename _Iterator::difference_type;
    };

    template <class _Tp> struct iterator_traits<_Tp *>
    {
        using iterator_category = random_access_iterator_tag;

        using value_type = _Tp;
        using difference_type = ::std::ptrdiff_t;
        using pointer = _Tp *;
        using reference = _Tp &;
    };

    template <class _Tp> struct iterator_traits<const _Tp *>
    {
        using iterator_category = random_access_iterator_tag;

        using value_type = _Tp;
        using difference_type = ::std::ptrdiff_t;
        using pointer = const _Tp *;
        using reference = const _Tp &;
    };

    template <class I>
    constexpr typename iterator_traits<I>::iterator_category
    iterator_category(const I &)
    {
        using category = typename iterator_traits<I>::iterator_category;
        return category();
    }

    template <class _Iterty>
    using difference_type_t =
        typename iterator_traits<_Iterty>::difference_type;

    namespace util {
        // is iterator xxx type
        IS_ITERATOR_TAG_TOTAL(input_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(output_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(forward_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(bidirectional_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(random_access_iterator_tag);
    }; // namespace util

    // Iterator Function

    // distance
    template <class _InputIterator, class _Distance>
    inline void __distance(_InputIterator __first, _InputIterator __last,
                           _Distance &__n, input_iterator_tag)
    {
        while (__first != __last) {
            ++__first;
            ++__n;
        }
    }

    template <class _RandomAccessIterator, class _Distance>
    inline void __distance(_RandomAccessIterator __first,
                           _RandomAccessIterator __last, _Distance &__n,
                           random_access_iterator_tag)
    {
        __n += __last - __first;
    }

    template <class _Iterator, class _Distance>
    inline void distance(_Iterator __first, _Iterator __last, _Distance &__n)
    {
        __distance(__first, __last, __n, iterator_category(__first));
    }

    template <class _Iterator>
    auto distance(_Iterator __first, _Iterator __last)
    {
        difference_type_t<_Iterator> n = 0;
        __distance(__first, __last, n, iterator_category(__first));
        return n;
    }

    // advance
    template <class _Ty, class _IterDistance>
    void __advance(_Ty &__i, _IterDistance __n, input_iterator_tag) NOEXCEPT
    {}

    template <class _IterStart, class _IterDistance>
    void __advance(_IterStart &__i, _IterDistance __n,
                   forward_iterator_tag) NOEXCEPT
    {
        while (--__n)
            ++__i;
    }

    template <class _Ty, class _IterDistance>
    void __advance(_Ty &__i, _IterDistance __n,
                   bidirectional_iterator_tag) NOEXCEPT
    {
        if (__n >= 0)
            while (__n--)
                ++__i;
        else
            while (__n++)
                --__i;
    }

    template <class _Ty, class _IterDistance>
    void __advance(_Ty &__i, _IterDistance __n,
                   random_access_iterator_tag) NOEXCEPT
    {
        __i += __n;
    }

    template <class _Ty, class _D> void advance(_Ty &__i, _D __n) NOEXCEPT
    {
        __advance(__i, __n, iterator_category(__i));
    }

    template <class _Ty, class _IterDistance>
    _Ty next(_Ty __i, _IterDistance __n) NOEXCEPT
    {
        advance(__i, __n);
        return __i;
    }

    // copy_backward
    template <class _Iter>
    _Iter __copy_backward(_Iter __first, _Iter __last, _Iter __res,
                          bidirectional_iterator_tag) NOEXCEPT
    {
        while (__last != __first)
            *(--__res) = *(--__last);
        return __res;
    }

    template <class _Iter, class _Distance>
    _Iter __copy_backward(_Iter __first, _Iter __last, _Iter __res,
                          random_access_iterator_tag) NOEXCEPT
    {
        for (_Distance __n = __last - __first; __n > 0; --__n)
            *(--__res) = *(--__last);
        return __res;
    }

    template <class _Iter>
    _Iter copy_backward(_Iter __first, _Iter __last, _Iter __res) NOEXCEPT
    {
        return __copy_backward(__first, __last, __res,
                               iterator_category(__first));
    }

    // construct_at     ——C++17 after.
    template <class _T, class _Tv>
    constexpr _T *construct_at(_T *__ptr, const _Tv &__v)
        NOEXCEPT(::std::is_nothrow_constructible_v<_T>)
    {
        return ::new (static_cast<void *>(__ptr)) _T(__v);
    }

    template <class _T> constexpr _T *construct_at(_T *__ptr)
    {
        return construct_at(__ptr, _T());
    }

    template <class _T, class... _Args>
    constexpr _T *construct_at(_T *__ptr, _Args &&...__args)
        NOEXCEPT(::std::is_nothrow_constructible_v<_T>)
    {
        return ::new ((__ptr)) _T{Fiop::forward<_Args>(__args)...};
    }

    // destory
    template <class _T> constexpr void destroy_at(_T *__ptr)
    {
        if constexpr (mpls::is_array_v<_T>) {
            for (auto &elem : *__ptr)
                iop::destroy_at(&elem);
        }
        else
            __ptr->~_T();
    }

    template <class _T> constexpr void destroy_at(_T *__first, _T *__last)
    {
        while (__first != __last)
            iop::destroy_at(::std::addressof(*(__first++)));
    }

    // uninitialized(fill / copy / copy_n)
    template <class _Iter, class _Size, class _Tv>
    void uninitialized_fill_n(_Iter __first, _Size __n, const _Tv &__val)
    {
        fill_n(__first, __n, __val);
    }

    template <class _iter, class _Tv>
    void uninitialized_fill(_iter __first, _Tv __val)
    {
        fill(__first, __val);
    }

    template <class _InputIter, class _NoThrowForwardIt>
    _NoThrowForwardIt uninitialized_copy(_InputIter __first, _InputIter __last,
                                         _NoThrowForwardIt __d_first)
    {
        using T = typename iterator_traits<_NoThrowForwardIt>::value_type;

        _NoThrowForwardIt current = __d_first;
        try {
            for (; __first != __last; ++__first, (void)++current)
                ::new (static_cast<void *>(::std::addressof(*current)))
                    T(*__first);
            return current;
        }
        catch (...) {
            for (; __d_first != current; ++__d_first)
                __d_first->~T();
            throw;
        }
    }

    template <class _InputIter, class _Size, class _ForwardIter>
    auto uninitialized_copy_n(_InputIter __first, _Size __n,
                              _ForwardIter __d_first)
    {

        assert(__n > 0);
        using T = typename iterator_traits<_ForwardIter>::value_type;

        _InputIter current = __d_first;
        try {
            for (; __n > 0; --__n, ++__first, (void)++current)
                ::new (static_cast<void *>(::std::addressof(*current)))
                    T(*__first);
            return current;
        }
        catch (...) {
            for (; __d_first != current; ++__d_first)
                __d_first->~T();
            throw;
        }
    }

}; // namespace iop

#endif //*  _ITERATOR_BASE_IOP_
