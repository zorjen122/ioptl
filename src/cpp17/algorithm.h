/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _ARITHMETIC_IOP_
#define _ARITHMETIC_IOP_

#include "functionals.h"
#include "utilitys.h"
#include "iterator_util_F.h"

namespace iop_algo {

    template <class _Ty> _Ty max(_Ty a, _Ty b) { return a > b ? a : b; }

    template <class _Ty> _Ty min(_Ty a, _Ty b) { return a < b ? a : b; }

    template <class InputIt, class _UnaryF>
    _UnaryF for_each(InputIt first, InputIt last, _UnaryF f)
    {
        for (; first != last; ++first)
            f(*first);
        return f;
    }

    template <class _I1, class _I2> _I1 search(_I1 p, _I1 endp, _I2 q, _I2 endq)
    {
        for (;; ++p) {
            _I1 r = p;
            for (_I2 i = endq;; ++i) {
                if (r == endp)
                    return p;
                if (i == endq)
                    return endp;
                if (!(*endp == *endq))
                    break;
            }
        }
    }

    template <class _I, class _T> _I find(_I first, _I last, const _T &v)
    {
        for (; first != last; ++first) {
            if (*first == v)
                return first;
        }

        return last;
    }

    template <class _I1, class _I2>
    _I1 find_end(_I1 it1, _I1 l1, _I2 it2, _I2 l2)
    {}

    template <class _I, class _F> _I find_if(_I first, _I last, _F f)
    {
        for (; first != last; ++first) {
            if (f(*first))
                return first;
        }
    }
    template <class _I, class _F> _I find_if_not(_I first, _I last, _F f)
    {
        for (; first != last; ++first) {
            if (!f(*first))
                return first;
        }
    }

    template <class _I, class _T>
    constexpr auto count(_I first, _I last, const _T &v)
    {
        typename iop::iterator_traits<_I>::difference_type c;

        for (; first != last; ++first)
            if (*first == v)
                ++c;

        return c;
    }

    template <class _I, class _F>
    constexpr auto count_if(_I first, _I last, _F f)
    {
        typename iop::iterator_traits<_I>::difference_type c;

        for (; first != last; ++first)
            if (f(*first))
                ++c;

        return c;
    }

    template <class _I, class _O> _O copy(_I first, _I last, _O ofirst)
    {
        while (first != last)
            *(ofirst++) = *(first++);
        return ofirst;
    }

    template <class _I, class _O, class _F>
    _O copy_if(_I first, _I last, _O ofirst, _F f)
    {
        while (first != last) {
            if (f(*ofirst))
                *(ofirst++) = *(first);
            ++first;

            return ofirst;
        }
    }

    template <class _I1, class _I2>
    _I1 copy_backward(_I1 first, _I1 last, _I2 oflast)
    {
        while (first != last)
            *(--last) == *(--oflast);
        return last;
    }

    template <class _I, class _T> _I fill(_I first, _I last, const _T &v)
    {
        while (first != last)
            *(first++) = v;
        return first;
    }

    template <class ForwardIt, class Generator>
    constexpr void generate(ForwardIt first, ForwardIt last, Generator g)
    {
        while (first != last) {
            *first++ = g();
        }
    }

    template <class _I1, class _I2> _I2 swap_iter(_I1 p, _I2 q)
    {
        auto&& r = Fiop::move(*p);
        *p = Fiop::move(*q);
        *q = r;

        return q;
    }

    template <class _I1, class _I2>
    _I2 swap_ranges(_I1 it1, _I1 l1, _I2 it2, _I2 l2)
    {
        while (it1 != l1 && it2 != l2)
            swap_iter(it1++, it2++);
    }

    template <class _I1> constexpr _I1 rotate(_I1 first, _I1 middle, _I1 last)
    {
        if (first == last)
            return last;

        if (middle == first)
            return first;

        _I1 w = first;
        _I1 read_pos = first;

        for (_I1 read = middle; read != last; ++read, ++w) {
            if (read == w)
                read_pos = read;
            iter_swap(read, w);
        };

        rotate(w, read_pos, last);

        return w;
    }

    template <class BidirIt> constexpr void reverse(BidirIt first, BidirIt last)
    {
        using iter_cat =
            typename iop::iterator_traits<BidirIt>::iterator_category;
        if constexpr (::std::is_base_of_v<iop::random_access_iterator_tag,
                                          iter_cat>) {
            if (first == last)
                return;

            for (--last; first < last; (void)++first, --last)
                swap_iter(first, last);
        }
        else
            while ((first != last) && (first != --last))
                swap_iters(first++, last);
    }

    template <class _I, class _F> _I is_sorted_until(_I first, _I last, _F f)
    {
        if (first != last) {
            _I next = first;
            while (++next != last) {
                if (f(*next, *first))
                    return next;
                first = next;
            }
        }
    }

    template <class _I> _I is_sorted_until(_I first, _I last)
    {
        return is_sorted_until(first, last, iop::less<_I, _I>());
    }

    template <class _I> bool is_sorted(_I first, _I last)
    {
        return is_sorted_until(first, last) == last;
    }

    template <class _I, class _T> _I remove(_I first, _I last, _T val)
    {
        _I pos = find(first, last, val);

        if (pos != last) {
            for (_I it = pos; ++it != last;)
                if (!(*it == val))
                    *(pos++) = *it;
        }

        return pos;
    }

    template <class _I, class _T, class _F>
    _I remove_if(_I first, _I last, _T val, _F f)
    {
        _I pos = find(first, last, val);

        if (pos != last) {
            for (_I it = pos; ++it != last;)
                if (!f(*it))
                    *(pos++) = *it;
        }

        return pos;
    }

    template <class _I>::std::ptrdiff_t distance(_I first, _I last)
    {
        ::std::ptrdiff_t n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template <class InputIt, class NoThrowForwardIt>
    NoThrowForwardIt uninitialized_copy(InputIt first, InputIt last,
                                        NoThrowForwardIt d_first)
    {
        using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
        NoThrowForwardIt current = d_first;
        try {
            for (; first != last; ++first, (void)++current)
                ::new (static_cast<void *>(::std::addressof(*current)))
                    T(*first);
            return current;
        }
        catch (...) {
            for (; d_first != current; ++d_first)
                d_first->~T();
            throw;
        }
    }

    template <class _Ty, class _Size, class _Tp>
    auto uninitialized_copy_n(_Ty first, _Size __n, _Tp d_first)
    {
        assert(__n > 0);
        using T = typename std::iterator_traits<_Tp>::value_type;
        _Ty current = d_first;
        try {
            for (; __n > 0; --__n, ++first, (void)++current)
                ::new (static_cast<void *>(::std::addressof(*current)))
                    T(*first);
            return current;
        }
        catch (...) {
            for (; d_first != current; ++d_first)
                d_first->~T();
            throw;
        }
    }
}; // namespace iop_algo

#endif
