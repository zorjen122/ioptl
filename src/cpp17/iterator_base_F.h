#ifndef _ITERATOR_BASE_IOP_
#define _ITERATOR_BASE_IOP_

#include "./iterator_util_F.h"

#include "algobase_F.h"
#include "deftype.h"
#include "type_traits.h"
#include "utilitys.h"
#include <assert.h>
#include <new>
#include <utility>


namespace iop {

    // Iterator Function
    template <class InputIt>
    inline auto distance(InputIt __first, InputIt __last)
    {
        using category_type = iterator_category_t<InputIt>;
        using type = difference_type_t<InputIt>;
        type n = 0;

        if constexpr (util::is_random_access_iterator_tag_v<category_type>) {
            n = static_cast<type>(__last - __first);
        }
        else if (util::is_input_iterator_tag_v<category_type>) {
            while (__first != __last) {
                ++__first;
                ++n;
            }
        }

        // cppref: only random_access_iterator exist -count case.
        return n;
    }

    template <class InputIt, class Distance>
    constexpr void advance(InputIt &it, Distance n)
    {
        using category_type = iterator_category_t<InputIt>;
        if constexpr (util::is_random_access_iterator_tag_v<category_type>) {
            it += n;
        }
        else if (util::is_bidirectional_iterator_tag_v<category_type>) {
            while (n > 0)
                ++it, --n;
            while (n < 0)
                --it, ++n;
        }
        else if (util::is_forward_iterator_tag_v<category_type>) {
            while (n > 0)
                ++it, --n;
        }
    }

    template <class InputIt>
    constexpr InputIt next(InputIt it, difference_type_t<InputIt> n = 1)
    {
        advance(it, n);
        return it;
    }

    template <class BidirIt>
    constexpr auto prev(BidirIt it, difference_type_t<BidirIt> n = 1)
    {
        advance(it, -n);
        return it;
    }

    template <class BidirIt1, class BidirIt2>
    BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
    {
        using category_type = iterator_category_t<BidirIt1>;
        using category2_type = iterator_category_t<BidirIt2>;

        if constexpr (util::is_bidirectional_iterator_tag_args_v<
                          category_type, category2_type>) {
            // cppref: BidirIt1/BidirIt2 -> [first, last)
            while (last != first)
                *(--d_last) = *(--last);
        }

        return d_last;
    }

    // construct_at     ——C++17 after.
    template <class _T, class _Tv>
    constexpr _T *construct_at(_T *__ptr, const _Tv &__v = _Tv()) noexcept(
        ::std::is_nothrow_constructible_v<_T>)
    {
        return ::new (Fiop::_Voidify_ptr(__ptr)) _T(__v);
    }

    template <class _T> constexpr _T *construct_at(_T *__ptr)
    {
        return construct_at(__ptr, _T());
    }

    template <class _T, class... _Args>
    constexpr _T *construct_at(_T *__ptr, _Args &&...__args) noexcept(
        ::std::is_nothrow_constructible_v<_T>)
    {
        return ::new (Fiop::_Voidify_ptr(__ptr))
            _T(Fiop::forward<_Args>(__args)...);
    }

    // destroy
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

    template <class _Iter> constexpr void destroy(_Iter __first, _Iter __last)
    {
        while (__first != __last)
            iop::destroy_at(::std::addressof(*(__first++)));
    }

    template <class _ForwardIt, class _Size>
    constexpr _ForwardIt destroy_n(_ForwardIt __first, _Size __n)
    {
        for (; __n > 0; --__n, ++__first)
            iop::destroy_at(::std::addressof(*__first));

        return __first;
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

#endif