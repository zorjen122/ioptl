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

#define _LIST_IOP_DEFINE_ 
#ifdef _LIST_IOP_DEFINE_ 

#include "iterator_F.h"
#include "allocator_F.h"
#include "allocator_traits_F.h"
#include "alloc_operator_new_F.h"

#include <typeinfo>

namespace IOP_STL {

    struct list_node_base {
        list_node_base* V_next;       list_node_base* V_prev;
    };

    template<class _Ty>
    struct list_node : public list_node_base { _Ty V_data; };


    class list_iterator_base {
    public:
        using iteartor_category = IOP_STL::bidirectional_iterator_tag;
        using iterator_traits = IOP_STL::iterator_traits<iteartor_category>;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        list_node_base* V_node;

        list_iterator_base(const list_iterator_base& __rhs) : V_node(__rhs.V_node) {}

        explicit list_iterator_base(list_node_base* __rhs) : V_node(__rhs) {}
        explicit list_iterator_base() {}

        void V_incr() { V_node = V_node->V_next; }
        void V_decr() { V_node = V_node->V_prev; }


        IOP_CONSTEXPR bool operator==(const list_iterator_base& __rhs)
            const NOEXCEPT {
            return V_node == __rhs.V_node;
        }
        IOP_CONSTEXPR bool operator!=(const list_iterator_base& __rhs)
            const NOEXCEPT {
            return V_node != __rhs.V_node;
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
        explicit list_iterator(const_iterator& __rhs) : list_iterator_base(__rhs.V_node) {}

        IOP_CONSTEXPR_CXX17 reference operator*() const NOEXCEPT {
            return static_cast<node_T*>(V_node)->V_data;
        }

        IOP_CONSTEXPR_CXX17 Self_& operator++()NOEXCEPT {
            V_incr();
            return *this;
        }

        IOP_CONSTEXPR_CXX17 Self_& operator++(int) NOEXCEPT {
            Self_& k = *this;
            V_incr();
            return k;
        }

        IOP_CONSTEXPR_CXX17 Self_& operator--()NOEXCEPT {
            V_decr();
            return *this;
        }

        IOP_CONSTEXPR_CXX17 Self_& operator--(int) NOEXCEPT {
            Self_& k = *this;
            V_decr();
            return k;
        }

    };


    template<class _Ty, class _Alloc = IOP_allocator<list_node<_Ty>>>
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


        explicit list() NOEXCEPT { V_initializer_list(); }

        explicit list(const list<_Ty, _Alloc>& __res) NOEXCEPT {
            V_data_allocator = allocator_type();
            V_uninitializer_copy(__res);
        }

        explicit list(const value_type __v, const size_type __sz) NOEXCEPT {
            V_initializer_list();

            for (size_type i = 0;i < __sz - 1;++i)
                push_back(__v);
        }


        ~list() {
            clear();
            V_data_allocator.deallocate(V_node, 1);
        }


    protected:

        node_T* V_node;
        allocator_type V_data_allocator;

    protected:

        void V_initializer_list() NOEXCEPT {
            V_data_allocator = allocator_type();
            V_node = V_data_allocator.allocate(1);
            V_node->V_next = V_node;
            V_node->V_prev = V_node;
        }

        IOP_CONSTEXPR node_T* V_unit_create(const _Ty& __val) NOEXCEPT {
            node_T* k = V_data_allocator.allocate(1);
            construct(&k->V_data, __val);
            return k;
        }


        void V_uninitializer_copy(const list<_Ty, _Alloc>& __res) {
            node_T* __rhs = static_cast<node_T*>(__res.V_node->V_next);

            try {
                while (__rhs != __res.V_node) {
                    push_back(__rhs->V_data);
                    __rhs = static_cast<node_T*>(__rhs->V_next);
                }
            }
            catch (...) {
                while (__rhs != __res.V_node) {
                    node_T* tmp = __rhs;
                    tmp = static_cast<node_T*>(tmp->V_prev);
                    destroy(__rhs);
                    __rhs = tmp;
                }
                throw;
            }
        }

        IOP_CONSTEXPR void transfer(iterator __pos, iterator __first, iterator __last) NOEXCEPT {
            if (__pos != __last) {
              // Remove [first, last) from its old position.
                __last.V_node->V_prev->V_next = __pos.V_node;
                __first.V_node->V_prev->V_next = __last.V_node;
                __pos.V_node->V_prev->V_next = __first.V_node;

                // Splice [first, last) into its new position.
                list_node_base* __tmp = __pos.V_node->V_prev;
                __pos.V_node->V_prev = __last.V_node->V_prev;
                __last.V_node->V_prev = __first.V_node->V_prev;
                __first.V_node->V_prev = __tmp;
            }
        }

    public:

        IOP_CONSTEXPR void clear() {
            node_T* res = static_cast<node_T*>(V_node->V_next);
            while (res != V_node) {
                node_T* k = res;
                res = static_cast<node_T*>(res->V_next);
                _destroy(&k->V_data);

                V_data_allocator.deallocate(k, 1);
            }
            V_node->V_next = V_node;
            V_node->V_prev = V_node;
        }

        IOP_CONSTEXPR iterator insert(iterator __postition, value_type __val) {
            node_T* k = V_unit_create(__val);
            k->V_next = __postition.V_node;
            k->V_prev = __postition.V_node->V_prev;

            __postition.V_node->V_prev->V_next = k;
            __postition.V_node->V_prev = k;       // prev end.

            return static_cast<iterator>(k);
        }

        IOP_CONSTEXPR iterator insert(iterator __postition) {
            return static_cast<iterator>(insert(__postition, _Ty()));
        }

        // Out-of-Money by system throw ERROR
        template<class... _Args>
        IOP_CONSTEXPR iterator emplace_back(const _Args&&... __args) {
            for (size_type i = 0;i < sizeof...(__args);++i)
                insert(--end(), __args...);
        }

        // copy to delete
        IOP_CONSTEXPR iterator erase(iterator __position) {
            list_node_base* __next_node = __position.V_node->V_next;
            list_node_base* __prev_node = __position.V_node->V_prev;

            node_T* res = static_cast<node_T*>(__position.V_node);

            __next_node->V_prev = __prev_node;
            __prev_node->V_next = __next_node;

            destroy(&res->V_data);
            V_data_allocator.deallocate(res, 1);

            return static_cast<iterator>(static_cast<node_T*>(__next_node));
        }

        IOP_CONSTEXPR iterator erase(iterator __frist, iterator __last) {

            if (__frist.V_node == __last.V_node) {
                return erase(static_cast<iterator>(__frist.V_node));
            }

            node_T* res = __frist.V_node;
            while (res->V_next != __last.V_node) {
                node_T* k = res;
                res = static_cast<node_T*>(res->V_next);
                erase(static_cast<iterator>(k));
            }

            return static_cast<iterator>(res);

        }

        void resize(const size_type& __n, const_reference __v) { IOP_STL::resize(*this, __n, __v); }

        void resize(const size_type& __n) { IOP_STL::resize(*this, __n); }


        IOP_CONSTEXPR bool empty() NOEXCEPT { return (V_node->V_next == V_node); }

        IOP_CONSTEXPR bool empty() const NOEXCEPT { return (V_node->V_next == V_node); }

        IOP_CONSTEXPR size_type size() NOEXCEPT {
            size_type _n = 0;
            for (iterator __iter = begin();
                __iter != end();++__iter, ++_n);
            return _n;
        }

        IOP_CONSTEXPR void __swap(iterator& __q, iterator& __p)
            NOEXCEPT(NOEXCEPT(IOP_STL::swap(std::declval<reference>(), std::declval<reference>()))) {
            IOP_STL::swap(__q, __p);
        }

        IOP_CONSTEXPR size_type max_size() const NOEXCEPT {
            return size_type(-1);
        }

        IOP_CONSTEXPR value_type front() const  NOEXCEPT(NOEXCEPT(assert(!empty()))) {
            return *begin();
        }

        IOP_CONSTEXPR value_type back() const NOEXCEPT(NOEXCEPT(assert(!empty()))) {
            return *(--end());
        }


        iterator begin() NOEXCEPT { return static_cast<iterator>(static_cast<node_T*>(V_node->V_next)); }
        const_iterator begin() const NOEXCEPT { return static_cast<const_iterator>(V_node->V_next); }

        iterator end() NOEXCEPT { return static_cast<iterator>(V_node); }
        const_iterator end() const NOEXCEPT { return static_cast<const_iterator>(V_node); }


        iterator push_back(const _Ty& __val) NOEXCEPT { return insert(end(), __val); }

        const_iterator push_back(const _Ty& __val) const NOEXCEPT {
            return static_cast<const_iterator>(insert(end(), __val));
        }

        iterator push_front(const _Ty& __val) NOEXCEPT { return insert(begin(), __val); }

        const_iterator push_front(const _Ty& __val) const NOEXCEPT {
            return static_cast<const_iterator>(insert(begin(), __val));
        }


        iterator pop_back() NOEXCEPT(NOEXCEPT(!empty())) {
            return erase(--end());
        }
        const_iterator pop_back()
            const NOEXCEPT(NOEXCEPT(!empty())) {
            return static_cast<const_iterator>(erase(end()));
        }

        iterator pop_front() NOEXCEPT(NOEXCEPT(!empty())) {
            return erase(begin());
        }

        const_iterator pop_front()
            const NOEXCEPT(NOEXCEPT(assert(!empty()))) {
                {
                    return static_cast<const_iterator>(erase(begin()));
                }
        }

        IOP_CONSTEXPR void remove(const value_type& __v) NOEXCEPT {
            if (!empty()) {
                iterator it = begin();
                while (it != end()) {
                    iterator r = it;
                    ++r;
                    if (*it == __v)
                        erase(it);
                    it = r;
                }
            }
        }

        IOP_CONSTEXPR void unique() NOEXCEPT {
            if (!empty()) {
                iterator it = begin();
                iterator nit = it;
                while (++nit != end()) {
                    if (*it == *nit)
                        erase(nit);
                    else {
                        it = nit;
                    }
                    nit = it;
                }
            }
        }


        IOP_CONSTEXPR void splice(iterator __pos, list<_Ty>& __rhs) NOEXCEPT {
            if (!__rhs.empty()) {
                transfer(__pos, __rhs.begin(), __rhs.end());
            }
        }


        IOP_CONSTEXPR void sort() NOEXCEPT {

            node_T* it = V_node;
            while (it->V_next != V_node) {
                it = static_cast<node_T*>(it->V_next);
                node_T* __subit = static_cast<node_T*>(it->V_next);
                while (__subit != V_node) {
                    if (it->V_data > __subit->V_data) {
                        value_type __tmp = it->V_data;
                        it->V_data = __subit->V_data;
                        __subit->V_data = __tmp;
                    }
                    __subit = static_cast<node_T*>(__subit->V_next);
                }
            }
        }

        IOP_CONSTEXPR void merge(list<_Ty>& __res) NOEXCEPT {
            if (empty()) {
                for (iterator it = __res.begin();it != __res.end();++it)
                    push_back(*it);
                return;
            }

            if (!__res.empty()) {
                iterator it = begin();
                iterator eit = __res.begin();

                iterator rpos = __res.begin();
                for (;eit != __res.end();) {
                    if (*eit >= *it && it != end())
                        ++it;
                    else {
                        insert(it, *eit);
                        ++eit;
                    }
                }

            }

        }

        IOP_CONSTEXPR void reverse() NOEXCEPT {
            if (!empty()) {
                size_type i = size() - (size() / 2);
                iterator rit = end();
                for (iterator it = begin();i != 0;--i, ++it) {
                    --rit;
                    __swap(it, rit);
                }
            }
        }


        IOP_CONSTEXPR list<_Ty, _Alloc>& operator=(const list<_Ty, _Alloc>& __res) {
            if (&__res == this) { return *this; }

            clear();
            V_uninitializer_copy(__res);

            return *this;
        }

        IOP_CONSTEXPR list<_Ty, _Alloc>&& operator=(const list<_Ty, _Alloc>&& __res) {
            if (&__res == this) { return std::move(*this); }

            clear();
            V_node = __res.V_node;
            __res.V_node->V_prev = nullptr;
            __res.V_node->V_next = nullptr;

            return std::move(*this);
        }


        IOP_CONSTEXPR bool operator!=(const list<_Ty, _Alloc>& __res) NOEXCEPT {
            return
                !(__res.V_node == V_node);
        }

        IOP_CONSTEXPR bool operator==(const list<_Ty, _Alloc>& __res) NOEXCEPT {
            return
                (__res.V_node == V_node);
        }

    };
};          // namespace IOP_STL


#endif      //  _LIST_IOP_DEFINE_