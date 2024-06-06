#ifndef _ITERATOR_BASE_IOP_
#define _ITERATOR_BASE_IOP_

#include "iterator_utilF.h"

#include "../algo/algobaseF.h"
#include "../traits/type_traitsF.h"
#include "../util/utilityF.h"
#include "../memoryF.h"

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

    // uninitialized(fill / copy / copy_n)
    template <class _Iter, class _Size, class _Tv>
    void uninitialized_fill_n(_Iter __first, _Size __n, const _Tv &__val)
    {
        for(; __n > 0; --__n)
            *__first = __val;
    }

    template <class _Iter, class _Tv>
    void uninitialized_fill(_Iter first,  _Iter last, const _Tv& val)
    {
        while(first != last)
            *(first++) = val;
    }

    template <class _InputIter, class _NoThrowForwardIt>
    _NoThrowForwardIt uninitialized_copy(_InputIter __first, _InputIter __last,
                                         _NoThrowForwardIt __d_first)
    {
        using T = typename iterator_traits<_NoThrowForwardIt>::value_type;

        _NoThrowForwardIt current = __d_first;
        try {
            for (; __first != __last; ++__first, (void)++current)
                ::new (static_cast<void *>(Fiop::addressof(*current)))
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
                ::new (static_cast<void *>(Fiop::addressof(*current)))
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