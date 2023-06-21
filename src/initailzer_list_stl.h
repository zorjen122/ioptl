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

#define _INITAILIZER_LIST_STL_DEFINE_
#ifdef _INITAILIZER_LIST_STL_DEFINE_

#include "iterator_stl.h"
#include "allocator_stl.h"
#include "allocator_traits_stl.h"

#ifdef ALLOC_STL_BUG
#include "alloc_stl.h"
#else
#include "alloc_operator_new_stl.h"
template<class _Ty>
using default_alloc = __Default_alloc<_Ty>;
#endif

namespace CCC_STL {

    template<class _Ty>
    class initializer_list {
    public:

        using allocator_type = default_alloc<_Ty>;
        using allocator_traits_type = allocator_traits<_Ty, allocator_type>;

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
        allocator_type _M_data_allocate;
        iterator _Vilist;
        size_type _len;

    public:

        explicit initializer_list() {
            _len = 0;
            _M_data_allocate = allocator_type();
            _Vilist = _M_data_allocate.allocate(1);
            _Vilist = nullptr;
        }

        initializer_list(const _Ty& __val, const size_type& __c) :_len(__c) {
            _M_data_allocate = allocator_type();
            _Vilist = _M_data_allocate.allocate(__c + 1);
            iterator __res = begin();

            for (;__res != end();++__res)
                *(__res) = __val;
        }

        explicit initializer_list(const size_type& __c) : initializer_list(value_type(), __c) {}

        initializer_list(const initializer_list& __rhs) {
            _len = __rhs._len;
            _M_data_allocate = allocator_type();
            _Vilist = _M_data_allocate.allocate(_len + 1);
            uninitalized_copy(__rhs._Vilist, (__rhs._Vilist + _len), _Vilist);
        }

        initializer_list(initializer_list&& __rhs) {
            _len = __rhs._len;
            _Vilist = __rhs._Vilist;
            _M_data_allocate = __rhs._M_data_allocate;

            __rhs._len = 0;
            __rhs._Vilist = nullptr;
        }

        template<class... __Args>
        initializer_list(__Args&&... __args) {
            _len = sizeof...(__args);

            _M_data_allocate = allocator_type();
            _Vilist = _M_data_allocate.allocate(_len);

            construct(_Vilist, __args...);
        }


        const initializer_list& append(const initializer_list& __rhs) {
        // TODO
            iterator __res = _M_data_allocate.allocate(_len + __rhs._len + 1);
            iterator __Vpush = _Vilist;
            iterator __push = __rhs._Vilist;

            uninitalized_copy(__Vpush, __Vpush + _len, __res);
            uninitalized_copy(__push, __push + __rhs._len, __res);

            destroy(_Vilist, _Vilist + _len);
            _M_data_allocate.deallocate(_Vilist);

            _Vilist = __res;
            _len = _len + __rhs._len;

            return *this;
        }


        size_type size() const { return _len; }


        iterator begin() { return _Vilist; }
        iterator end() { return _Vilist + _len; }


        // free-function-template
        const_iterator cbegin() const { return static_cast<const_iterator>(begin()); }
        const_iterator cend() const { return static_cast<const_iterator>(end()); }


        reverse_iterator rbegin() { return static_cast<reverse_iterator>(begin()); }
        reverse_iterator rend() { return static_cast<reverse_iterator>(end()); }


        const_reverse_iterator crbegin() { return static_cast<const_reverse_iterator>(cbegin()); }
        const_reverse_iterator crend() { return static_cast<const_reverse_iterator>(cend()); }


        bool empty() { return (begin() == end()); }
        iterator data() { return _Vilist; }

    public:
        template<class... _Args>
        initializer_list& operator=(_Args... __args) {
            _len = sizeof...(__args);
            destroy(begin(), end());

            _Vilist = _M_data_allocate.allocate(_len + 1);
            construct(_Vilist, __args...);
        }

        initializer_list& operator=(const initializer_list& __rhs) {

            destroy(begin(), end());
            _len = __rhs._len;
            _M_data_allocate = allocator_type();
            _Vilist = _M_data_allocate.allocate(_len + 1);

            iterator it = __rhs.begin();

            uninitalized_copy(it, __rhs.end(), _Vilist);
        }
    };

};                    // namespace CCC_STL

#endif                // _INITAILIZER_LIST_STL_DEFINE_