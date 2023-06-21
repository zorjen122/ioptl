/*
* Copyright(c) 1997
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
*that both that copyright notice and this permission notice appear
* in supporting documentation.Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.It is provided "as is" without express or implied warranty.
*/

#pragma once

#define _ARRAY_STL_DEFINE_ 
#ifdef _ARRAY_STL_DEFINE_ 


#include "lib/src/iterator_stl.h"
#include "lib/src/allocator_stl.h"
#include "lib/src/allocator_traits_stl.h"
#include "lib/src/algobase_stl.h"

#include<array>
#include<cmath>

#ifdef ALLOC_STL_BUG
#include "alloc_stl.h"
#else
#include "lib/src/alloc_operator_new_stl.h"
template<class _Ty>
using default_alloc = __Default_alloc<_Ty>;
#endif

#define _OPERATOR_THAN(op)                              \
    bool operator op(array& __rhs) {                   \
        value_type __iter = 0;                          \
        for (auto __it = __rhs.begin();                 \
            __it != __rhs.end();++__it, ++_Varray) {    \
            __iter += ((*__it) - (*__it));              \
        }                                               \
        return (__iter op 0);                           \
    }                                                   \


namespace CCC_STL {


    template<class _Ty, ::std::size_t _N>
    class array {
    public:
        using allocator_type = default_alloc<_Ty>;

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
        allocator_type _M_data_allocator;
        pointer _Varray;   size_type _len;

    public:
        array() : _len(_N) {
            _Varray = _M_data_allocator.allocate(_N + 1);
        }

        template<class _Tp, size_type _Np>
        array(const array<_Tp, _Np>& __rhs) {
            _Varray = _M_data_allocator.allocate(_N);
            _len = __rhs._len;

            pointer __res = _Varray;

            for (auto __iter = __rhs.begin();__iter != __rhs.end(); ++__iter) {
                construct(__res, *__iter);
            }
        }


        template<class _Tp, size_type _Np>
        array(array<_Tp, _Np>& __rhs) {
            _Varray = _M_data_allocator.allocate(_N);
            _len = __rhs._len;

            pointer __res = _Varray;

            for (auto __iter = __rhs.begin();__iter != __rhs.end(); ++__iter) {
                construct(__res, *__iter);
            }
        }

        ~array() {
            destroy(begin(), end());
        }

        size_type size() {
            return _len;
        }
        pointer data() CCC_NOEXCEPT(CCC_NOEXCEPT(assert(_Varray != nullptr))) {
            return _Varray;
        }

        iterator end() {
            return static_cast<iterator>(_Varray + (_len));
        }
        iterator begin() {
            return static_cast<iterator>(_Varray);
        }

        const_iterator cend()const {
            return static_cast<const_iterator>(_Varray + (_len));
        }
        const_iterator cbegin()const {
            return static_cast<const_iterator>(_Varray);
        }

        reference back() {
            return *(end() - 1);
        }
        reference front() {
            return *(begin());
        }

        void fiil(const value_type& __val) {
            pointer __res = _Varray;
            for (;__res != end();++__res) {
                *__res = __val;
            }
        }

        void swap(array& __rhs)noexcept(
            noexcept(swap_ptr(
                std::declval<reference>(), std::declval<reference>()))
            ) {
            pointer __res = _Varray;
            for (;__res != end();++__res, ++__rhs)
                swap_ptr(__res, __rhs);

        }

        bool empty() {
            return (_Varray == nullptr);
        }

    public:
        value_type operator[](size_type __idx) {
            return *(_Varray + __idx + 1);
        }

        bool operator==(array& __rhs) { return (__rhs == *this); }
        bool operator!=(array& __rhs) { return (__rhs != *this); }

        bool operator>(array& __rhs) {
            value_type __iter = 0;
            pointer __res = __rhs.data();
            for (auto __it = begin();
                __it != end(); ++__it, ++__res) {
                __iter += ((*__it) - (*__res));
            }
            return (__iter > 0);
        }

        _OPERATOR_THAN(< );
        _OPERATOR_THAN(<= );
        _OPERATOR_THAN(>= );
    };


};          //* namespace CCC_STL


#endif      //* _ARRAY_STL_DEFINE_