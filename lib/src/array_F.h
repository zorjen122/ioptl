/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */


#pragma once

#define _ARRAY_IOP_DEFINE_ 
#ifdef _ARRAY_IOP_DEFINE_ 

#include "iterator_F.h"
#include "allocator_F.h"
#include "allocator_traits_F.h"
#include "algobase_F.h"
#include "alloc_operator_new_F.h"
#include "func_exception.h"

#include<cmath>


#define _OPERATOR_THAN(op)                                 \
    IOP_CONSTEXPR_CXX17 bool operator op(array& __rhs) {   \
        value_type __iter = 0;                             \
        for (auto __it = __rhs.begin();                    \
            __it != __rhs.end();++__it, ++_Varray) {       \
            __iter += ((*__it) - (*__it));                 \
        }                                                  \
        return (__iter op 0);                              \
    }                                                      \


namespace IOP_STL {

    template<class _Ty, ::std::size_t _N>
    class array {
    public:
        using allocator_type = IOP_allocator<_Ty>;
        using allocator_traits_type = allocator_traits<_Ty, IOP_allocator<_Ty>>;

        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        using iterator = pointer;
        using const_iterator = const iterator;
        using reverse_iterator = Iterator_STL::reverse_iterator<iterator>;
        using const_reverse_iterator = Iterator_STL::reverse_iterator<const_iterator>;


    protected:
        allocator_type V_data_allocator;
        pointer _Varray;   size_type _len;

    public:
        explicit array() : _len(_N) {
            _Varray = V_data_allocator.allocate(_N + 1);
        }

        ~array() { destroy(begin(), end()); }


        template<class _Tp, size_type _Np>
        explicit array(const array<_Tp, _Np>& __rhs) {
            _Varray = V_data_allocator.allocate(_N);
            _len = __rhs._len;

            pointer __res = _Varray;

            for (auto __iter = __rhs.begin();__iter != __rhs.end(); ++__iter) {
                construct(__res, *__iter);
            }
        }

        template<class _Tp, size_type _Np>
        explicit array(array<_Tp, _Np>& __rhs) {
            _Varray = V_data_allocator.allocate(_N);
            _len = __rhs._len;

            pointer __res = _Varray;

            for (auto __iter = __rhs.begin();__iter != __rhs.end(); ++__iter) {
                construct(__res, *__iter);
            }
        }


        IOP_CONSTEXPR_CXX17 iterator end() NOEXCEPT {
            return
                static_cast<iterator>(_Varray + (_len));
        }

        IOP_CONSTEXPR_CXX17 iterator begin() NOEXCEPT {
            return
                static_cast<iterator>(_Varray);
        }


        IOP_CONSTEXPR_CXX17 const_iterator cend() const NOEXCEPT {
            return
                static_cast<const_iterator>(_Varray + (_len));
        }
        IOP_CONSTEXPR_CXX17 const_iterator cbegin() const NOEXCEPT {
            return
                static_cast<const_iterator>(_Varray);
        }


        IOP_CONSTEXPR_CXX17 reference back() NOEXCEPT {
            return
                *(end() - 1);
        }
        IOP_CONSTEXPR_CXX17 reference front() NOEXCEPT {
            return
                *(begin());
        }

        reference at(const size_type& __v) {
            if (__v >= size())
                throw bad_range();
            else
                return _Varray + __v + 1;
        }

        IOP_CONSTEXPR_CXX17 void fiil(const value_type& __val) NOEXCEPT {
            pointer __res = _Varray;
            for (;__res != end();++__res) {
                *__res = __val;
            }
        }

        IOP_CONSTEXPR_CXX17 void swap(array<_Ty, _N>& __rhs) NOEXCEPT(
            NOEXCEPT(IOP_STL::swap(
                std::declval<reference>(), std::declval<reference>()))
        ) {
            pointer __res = _Varray;
            for (;__res != end();++__res, ++__rhs)
                IOP_STL::swap(__res, __rhs);

        }

        IOP_CONSTEXPR_CXX17 const_reference at(size_type __idx) {
            if (__idx >= size())
                throw;
            else
                return *(_Varray + __idx + 1);
        }

        template<class _T, ::std::size_t __N>
        IOP_CONSTEXPR_CXX17 _T&& get(array<_Ty, _N>&& __rhs) NOEXCEPT {
            return IOP_STL::get(__rhs);
        }

        IOP_CONSTEXPR_CXX17 bool empty() NOEXCEPT { return (_Varray == nullptr); }
        IOP_CONSTEXPR_CXX17 const_reference data() NOEXCEPT { return *_Varray; }

        IOP_CONSTEXPR_CXX17 size_type size() NOEXCEPT { return _len; }
        IOP_CONSTEXPR_CXX17 size_type max_size() NOEXCEPT { return size_type(-1); }


    public:

        IOP_CONSTEXPR_CXX17 bool operator>(array<_Ty, _N>& __rhs) {
            value_type __iter = 0;
            pointer __res = __rhs.data();
            for (auto __it = begin();
                __it != end(); ++__it, ++__res) {
                __iter += ((*__it) - (*__res));
            }
            return (__iter > 0);
        }


        IOP_CONSTEXPR_CXX17 const_reference operator[](size_type __idx) NOEXCEPT {
            return *(_Varray + __idx + 1);
        }

        IOP_CONSTEXPR_CXX17 bool operator==(array<_Ty, _N>& __rhs) NOEXCEPT { return (__rhs == *this); }
        IOP_CONSTEXPR_CXX17 bool operator!=(array<_Ty, _N>& __rhs) NOEXCEPT { return (__rhs != *this); }

        _OPERATOR_THAN(< );
        _OPERATOR_THAN(<= );
        _OPERATOR_THAN(>= );
    };


};          //* namespace IOP_STL


#endif      //* _ARRAY_IOP_DEFINE_