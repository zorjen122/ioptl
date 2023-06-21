/*
* Copyright(c) 1997	-zorjen122
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

#define _LIST_STL_DEFINE_ 
#ifdef _LIST_STL_DEFINE_ 

#include "iterator_stl.h"
#include "allocator_stl.h"
#include "allocator_traits_stl.h"
#include <typeinfo>

using namespace std;

#ifdef ALLOC_STL_BUG
#include "alloc_stl.h"
#else
#include "alloc_operator_new_stl.h"
template<class _Ty>
using default_alloc = __Default_alloc<_Ty>;
#endif

namespace CCC_STL {

    struct list_node_base {
        list_node_base* _M_next;       list_node_base* _M_prev;
    };

    template<class _Ty>
    struct list_node : public list_node_base { _Ty _M_data; };


    class list_iterator_base {
    public:
        using iteartor_category = ::bidirectional_iterator_tag;
        using iterator_traits = ::iterator_traits<bidirectional_iterator_tag>;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        list_node_base* _M_node;

        explicit list_iterator_base(list_node_base* __rhs) : _M_node(__rhs) {}
        explicit list_iterator_base() {}

        void _M_incr() { _M_node = _M_node->_M_next; }
        void _M_decr() { _M_node = _M_node->_M_prev; }


        CCC_CONSTEXPR bool operator==(const list_iterator_base& __rhs)
            const CCC_NOEXCEPT {
            return _M_node == __rhs._M_node;
        }
        CCC_CONSTEXPR bool operator!=(const list_iterator_base& __rhs)
            const CCC_NOEXCEPT {
            return _M_node != __rhs._M_node;
        }

    };


    template<class _Ty, class _Ref, class _Ptr>
    class list_iterator : public list_iterator_base {
    public:
        using iterator = list_iterator<_Ty, _Ty&, _Ty*>;
        using const_iterator = list_iterator<_Ty, const _Ty&, const _Ty*>;

        using pointer = _Ty*;
        using const_pointer = const _Ty*;
        using reference = _Ty&;
        using const_reference = const _Ty&;
        using node_T = list_node<_Ty>;
        using const_node = const list_node<const _Ty>;
        using Self_ = list_iterator<_Ty, _Ref, _Ptr>;


        explicit list_iterator() : list_iterator_base() {}
        explicit list_iterator(node_T* __rhs) : list_iterator_base(__rhs) {}
        explicit list_iterator(const_iterator& __rhs) : list_iterator_base(__rhs._M_node) {}

        CCC_CONSTEXPR_CXX17 reference operator*() const CCC_NOEXCEPT { return static_cast<node_T*>(_M_node)->_M_data; }

        CCC_CONSTEXPR_CXX17 Self_& operator++()CCC_NOEXCEPT {
            _M_incr();
            return *this;
        }

        CCC_CONSTEXPR_CXX17 Self_& operator++(int) CCC_NOEXCEPT {
            Self_& k = *this;
            _M_incr();
            return k;
        }

        CCC_CONSTEXPR_CXX17 Self_& operator--()CCC_NOEXCEPT {
            _M_decr();
            return *this;
        }

        CCC_CONSTEXPR_CXX17 Self_& operator--(int) CCC_NOEXCEPT {
            Self_& k = *this;
            _M_decr();
            return k;
        }

    };


    template<class _Ty, class _Alloc = default_alloc<list_node<_Ty>>>
    class list {
    protected:
        using allocator_type = _Alloc;

    public:
        using allocator_traits_type = allocator_traits<_Ty, _Alloc>;
        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;
        using node_T = list_node<_Ty>;
        using const_node = list_node<const _Ty>;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        using iterator = list_iterator<_Ty, _Ty&, _Ty*>;
        using const_iterator = list_iterator<_Ty, const _Ty&, const _Ty*>;


        explicit list() CCC_NOEXCEPT {
            _M_data_allocator = allocator_type();
            _M_node = _M_data_allocator.allocate();
            _M_node->_M_next = _M_node;
            _M_node->_M_prev = _M_node;
        }

        explicit list(list<_Ty, _Alloc>&& res) CCC_NOEXCEPT {
            _M_data_allocator = res._M_data_allocator;
            _M_node = res._M_node;

            res._M_data_allocator = allocator_type();
            res._M_node->_M_prev = nullptr;
            res._M_node->_M_next = nullptr;
        }

        explicit list(const list<_Ty, _Alloc>& res) CCC_NOEXCEPT {
            _M_data_allocator = allocator_type();
            node_T* __rhs = static_cast<node_T*>(res._M_node->_M_next);
            while (__rhs != res._M_node) {
                push_back(__rhs->_M_data);
                res = static_cast<node_T*>(res->_M_next);
            }
        }

        ~list() {
            assert(!empty());
            clear();
            _M_data_allocator.deallocate(_M_node, 1);
        }


    protected:
        node_T* _M_node;
        allocator_type _M_data_allocator;

    public:
        CCC_CONSTEXPR node_T* _M_unit_create(const _Ty& __val) CCC_NOEXCEPT {
            node_T* k = _M_data_allocator.allocate(1);
            construct(&k->_M_data, __val);
            return k;
        }

        CCC_CONSTEXPR void clear() {
            node_T* res = static_cast<node_T*>(_M_node->_M_next);
            while (res != _M_node) {
                node_T* k = res;
                res = static_cast<node_T*>(res->_M_next);
                _destroy(&k->_M_data);

                _M_data_allocator.deallocate(k, 1);
            }
            _M_node->_M_next = _M_node;
            _M_node->_M_prev = _M_node;
        }

        CCC_CONSTEXPR iterator insert(iterator __postition, value_type __val) {
            node_T* k = _M_unit_create(__val);
            k->_M_next = __postition._M_node;
            k->_M_prev = __postition._M_node->_M_prev;

            __postition._M_node->_M_prev->_M_next = k;
            __postition._M_node->_M_prev = k;       // prev end.

            return static_cast<iterator>(k);
        }

        CCC_CONSTEXPR iterator insert(iterator __postition) {
            return static_cast<iterator>(insert(__postition, _Ty()));
        }


        // Out-of-Money by system throw ERROR
        template<class... _Args>
        CCC_CONSTEXPR iterator emplace_back(const _Args&&... __args) {
            for (size_type i = 0;i < sizeof...(__args);++i)
                insert(--end(), __args...);
        }

        // copy to delete
        CCC_CONSTEXPR iterator erase(iterator __position) {
            list_node_base* __next_node = __position._M_node->_M_next;
            list_node_base* __prev_node = __position._M_node->_M_prev;

            node_T* res = static_cast<node_T*>(__position._M_node);

            __next_node->_M_prev = __prev_node;
            __prev_node->_M_next = __next_node;

            destroy(&res->_M_data);
            _M_data_allocator.deallocate(res, 1);

            return static_cast<iterator>(static_cast<node_T*>(__next_node));
        }

        CCC_CONSTEXPR iterator erase(iterator __frist, iterator __last) {

            if (__frist._M_node == __last._M_node) {
                return erase(static_cast<iterator>(__frist._M_node));
            }

            node_T* res = __frist._M_node;
            while (res->_M_next != __last._M_node) {
                node_T* k = res;
                res = static_cast<node_T*>(res->_M_next);
                erase(static_cast<iterator>(k));
            }

            return static_cast<iterator>(res);

        }

        CCC_CONSTEXPR bool empty() CCC_NOEXCEPT { return (_M_node->_M_next == _M_node); }

        CCC_CONSTEXPR bool empty() const CCC_NOEXCEPT { return (_M_node->_M_next == _M_node); }

        CCC_CONSTEXPR size_type size() CCC_NOEXCEPT {
            size_type _n = 0;
            for (iterator __iter = begin();
                __iter != end();++__iter, ++_n);
            return _n;
        }

        CCC_CONSTEXPR void swap(iterator& __q, iterator& __p) {
            value_type __tmep = *__p;
            *__p = *__q;
            *__q = __tmep;
        }

        CCC_CONSTEXPR size_type max_size() const CCC_NOEXCEPT {
            return size_type(-1);
        }

        CCC_CONSTEXPR value_type front() const  CCC_NOEXCEPT(assert(!empty())) {
            return *begin();
        }

        CCC_CONSTEXPR value_type back() const CCC_NOEXCEPT(assert(!empty())) {
            return *(--end());
        }


        iterator begin() CCC_NOEXCEPT { return static_cast<iterator>(static_cast<node_T*>(_M_node->_M_next)); }
        const_iterator begin() const CCC_NOEXCEPT { return static_cast<const_iterator>(_M_node->_M_next); }

        iterator end() CCC_NOEXCEPT { return static_cast<iterator>(_M_node); }
        const_iterator end() const CCC_NOEXCEPT { return static_cast<const_iterator>(_M_node); }

        iterator push_back(const _Ty& __val) CCC_NOEXCEPT { return insert(end(), __val); }
        const_iterator push_back(const _Ty& __val) const CCC_NOEXCEPT {
            return static_cast<const_iterator>(insert(end(), __val));
        }

        iterator push_front(const _Ty& __val) CCC_NOEXCEPT { return insert(begin(), __val); }
        const_iterator push_front(const _Ty& __val) const CCC_NOEXCEPT {
            return static_cast<const_iterator>(insert(begin(), __val));
        }

        iterator pop_back() CCC_NOEXCEPT(CCC_NOEXCEPT(!empty())) {
            return erase(--end());
        }
        const_iterator pop_back()
            const CCC_NOEXCEPT(CCC_NOEXCEPT(!empty())) {
            return static_cast<const_iterator>(erase(end()));
        }

        iterator pop_front() CCC_NOEXCEPT(CCC_NOEXCEPT(!empty())) {
            return erase(begin());
        }

        const_iterator pop_front()
            const CCC_NOEXCEPT(CCC_NOEXCEPT(assert(!empty()))) {
                {
                    return static_cast<const_iterator>(erase(begin()));
                }
        }

        // operation

        CCC_CONSTEXPR void sort() CCC_NOEXCEPT {

            node_T* it = _M_node;
            while (it->_M_next != _M_node) {
                it = static_cast<node_T*>(it->_M_next);
                node_T* subit = static_cast<node_T*>(it->_M_next);
                while (subit != _M_node) {
                    if (it->_M_data > subit->_M_data) {
                        value_type __tmp = it->_M_data;
                        it->_M_data = subit->_M_data;
                        subit->_M_data = __tmp;
                    }
                    subit = static_cast<node_T*>(subit->_M_next);
                }
            }
        }

    // TODO
        // IF (other._M_data_allocator!=_M_data_allocator) undefine behavior.
        CCC_CONSTEXPR void merge(list<_Ty>&& other) CCC_NOEXCEPT {
            if (!other.empty()) {

                auto&& it = begin();
                auto&& __rit = other.begin();

                for (;__rit != other.end();++it, ++__rit) {
                    if (*it > *__rit) {}
                }
            }
        }

    };
};          // namespace CCC_STL


#endif      //  _LIST_STL_DEFINE_
