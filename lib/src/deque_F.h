/*
* Copyright(c) 1997     zorjen122
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

#define _DEQUE_IOP_DEFINE_
#ifdef _DEQUE_IOP_DEFINE_

#include "iterator_F.h"
#include "allocator_F.h"
#include "allocator_traits_F.h"
#include "algobase_F.h"
#include "algorithm.h"

#include <expected>


namespace IOP_STL {

    IOP_CONSTEXPR size_t __deque_buf_size(size_t __sz) NOEXCEPT {
        return
            (__sz > 512) ? size_t(512 / __sz) : size_t(1);
    }


    template<class _Ty, class _Ref, class _Ptr>
    class Deque_iterator {
    public:
        using iterator = Deque_iterator<_Ty, _Ty&, _Ty*>;
        using iterator_category = random_access_iterator_tag;
        using const_iterator = Deque_iterator<_Ty, const _Ty&, const _Ty*>;

        using value_type = _Ty;
        using Map_pointer_type = _Ty**;
        using pointer = _Ptr;
        using reference = _Ref;
        using difference_type = ::std::ptrdiff_t;
        using size_type = ::std::size_t;
        using Self = Deque_iterator;

        Map_pointer_type V_node;
        _Ty* V_first;
        _Ty* V_last;
        _Ty* V_cur;

        size_t S_buf_size() { return __deque_buf_size(sizeof(_Ty)); }

        void V_set_node(Map_pointer_type __res) {
            V_node = __res;
            V_first = *__res;
            V_last = V_first + difference_type(S_buf_size());
        }

        Deque_iterator()
            : V_node(nullptr), V_first(nullptr), V_last(nullptr), V_cur(nullptr) {}

        Deque_iterator(_Ty* __x, Map_pointer_type __y)
            : V_cur(__x), V_first(*__y),
            V_last(*__y + S_buf_size()), V_node(__y) {}

        Deque_iterator(const_iterator& __res)
            : V_node(__res.V_node),
            V_first(__res.V_first), V_last(__res.V_last),
            V_cur(__res.V_cur) {}

        Self& operator++() {
            ++V_cur;
            if (V_cur == V_last) {
                V_set_node(V_node + 1);
                V_cur = V_first;
            }
            return *this;
        }

        Self operator++(int) {
            Self t = *this;
            ++(*this);
            return t;
        }

        Self& operator--() {
            if (V_cur == V_first) {
                V_set_node(V_node - 1);
                V_cur = V_last;
            }
            --V_cur;
            return *this;
        }

        Self operator--(int) {
            Self t = *this;
            --(*this);
            return t;
        }

        Self& operator+=(difference_type __sz) {
            difference_type d = __sz + (V_cur - V_first);
            if (d >= 0 && d < difference_type(S_buf_size())) {
                V_cur += __sz;
            }
            else {
                difference_type l = d > 0 ?
                    d / difference_type(S_buf_size())
                    : -difference_type((-d - 1) / S_buf_size()) - 1;

                V_set_node(V_node + l);
                V_cur = V_first + (d - (l * difference_type(S_buf_size())));
            }
            return *this;
        }

        Self& operator-=(difference_type __sz) {
            return
                *this += (-__sz);
        }

        Self& operator+(difference_type __sz) {
            Self t = *this;
            return t += __sz;
        }

        Self& operator-(difference_type __sz) {
            Self t = *this;
            return t -= __sz;
        }

        difference_type operator-(const Self& __x) {
            return
                static_cast<difference_type>(S_buf_size()) * (V_node - __x.V_node - 1) +
                (V_cur - V_first) + (__x.V_last - __x.V_cur);
        }


        reference operator*() { return *(V_cur); }

        pointer operator->() { return V_cur; }

        bool operator==(const Self& __res) { return (V_cur == __res.V_cur); }

        bool operator!=(const Self& __res) { return !(V_cur == __res.V_cur); }

        bool operator<(const Self& __res) {
            return V_node == __res.V_node ?
                ((V_cur - __res.V_cur) < 0) : ((V_node - __res.V_node) < 0);
        }

        bool operator>(const Self& __res) { return (__res < *this); }
        bool operator<=(const Self& __res) { return !(__res < *this); }
        bool operator>=(const Self& __res) { return !(*this < __res); }

        reference operator[](const size_type& __i) { return *(*(this + __i)); }

    };


    template<class _Ty, class _Alloc = IOP_allocator<_Ty>>
    class deque {
    protected:
        using allocator_node_type = _Alloc;
        using allocator_map_type = IOP_allocator<_Ty*>;
        using allocator_traits_type = allocator_traits<_Ty, _Alloc>;

    public:
        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        using iterator = Deque_iterator<_Ty, _Ty&, _Ty*>;
        using const_iterator = Deque_iterator<const _Ty, const _Ty&, const _Ty*>;
        using reverse_iterator = Iterator_STL::reverse_iterator<iterator>;
        using const_reverse_iterator = Iterator_STL::reverse_iterator<const_iterator>;
        using Map_pointer_type = pointer*;

        enum { __INITIAL_MAP_SIZE = 8 };

        _Ty** V_map;
        iterator V_start;
        iterator V_finish;
        size_type V_map_size;
        allocator_node_type  V_allocator_node;
        allocator_map_type V_allocator_map;

        explicit deque(const allocator_node_type& __r = allocator_node_type())
            : V_map(nullptr), V_map_size(), V_start(), V_finish() {
            V_initialize_map(0, V_map, V_map_size, V_start, V_finish);
        };

        deque(size_type __s)
            :V_map(nullptr), V_map_size(), V_start(), V_finish() {
            V_initialize_map(__s, V_map, V_map_size, V_start, V_finish);
        }

        ~deque() {
            if (V_map) {
                V_destroy_node(V_start.V_node, V_finish.V_node + 1);
                V_deallocate_map(V_map, V_map_size);
            }
        }

        //*  No use const deque<_Ty,_Alloc>& type, see GCC #error 85004
        //      https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85004
        deque(deque<_Ty, _Alloc>& __res)
            : V_map(nullptr), V_map_size(), V_start(), V_finish() {
            V_initialize_map(__res.size(), V_map, V_map_size, V_start, V_finish);
            IOP_STL::uninitialized_copy(__res.begin(), __res.end(), V_start);
        }

    protected:
        size_t S_buf_size() { return __deque_buf_size(sizeof(_Ty)); }

        void V_deallocate_map(_Ty** __p) { V_allocator_map.deallocate(__p); }
        void V_deallocate_map(_Ty** __p, size_type __n) { V_allocator_map.deallocate(__p, __n); }
        void V_deallocate_node(_Ty* __p) { V_allocator_node.deallocate(__p); }
        void V_deallocate_node(_Ty* __p, size_type __n) { V_allocator_node.deallocate(__p, __n); }

        _Ty** V_allocate_map(size_type __s) {
            return
                V_allocator_map.allocate(__s);
        }

        _Ty* V_allocate_node() {
            return
                V_allocator_node.allocate();
        }

        void V_create_nodes(_Ty** __n, _Ty** __N) {
            _Ty** i;
            try {
                for (i = __n; i != __N;++i)
                    *i = V_allocate_node();
            }
            catch (...) {
                V_destroy_node(__n, i);
                throw;
            }
        }

        void V_destroy_node(_Ty** __q, _Ty** __p) {
            for (_Ty** i = __q;i != __p;++i)
                V_deallocate_node(*i);
        }


        void V_initialize_map(size_type __c,
            _Ty** __map, size_type& __map_size,
            iterator& __start, iterator& __finish,
            const size_type __S_map_size = __INITIAL_MAP_SIZE
        ) {
            size_type v_node = __c / __deque_buf_size(sizeof(_Ty)) + 1;

            __map_size = IOP_ALGO::max(__S_map_size, v_node + 2);
            __map = V_allocate_map(__map_size);

            _Ty** __nstart = __map + (__map_size - v_node) / 2;
            _Ty** __nfinish = __nstart + v_node;

            try {
                V_create_nodes(__nstart, __nfinish);
            }
            catch (...) {
                V_deallocate_map(__map, __map_size);
                __map = nullptr, __map_size = 0;
                throw;
            }
            __start.V_set_node(__nstart);
            __finish.V_set_node(__nfinish - 1);

            __start.V_cur = __start.V_first;
            __finish.V_cur = __finish.V_first +
                (__c % __deque_buf_size(sizeof(_Ty)));
        };


        //* reallocate will money allocate than new double slef-money.
        void V_reallocate_map(const size_type& __n, bool io_front) {

            size_type d_old = V_finish.V_node - V_start.V_node + 1;
            size_type d_new = d_old + __n;
            Map_pointer_type rstart;

        //* Set start or set start allocate after.
            if (V_map_size > d_new * 2) {
                // old adjustng + ( new - old ) / 2 + (( new - old ) % 2).
                rstart = V_map + ((V_map_size - d_new) / 2 + (io_front ? __n : 0));

            //* Start cur to finish cur is the copy.
                if (rstart < V_start.V_node) {
                    copy(V_start.V_node, V_finish.V_node + 1, rstart);
                }
                else {
                    copy_backward(V_start.V_node, V_finish.V_node + 1, rstart + d_old);
                }
            }
            else {
                V_map_size += IOP_ALGO::max(V_map_size, __n) + 2;
                Map_pointer_type rmap = V_allocate_map(V_map_size);

                rstart = rmap + ((V_map_size - d_new) / 2 + (io_front ? __n : 0));
                copy(V_start.V_node, V_finish.V_node + 1, rstart);

                // This is same to deallocate-n.
                V_deallocate_map(V_map);

                V_map = rmap;
            }

            V_start.V_set_node(rstart);
            V_finish.V_set_node(rstart + d_old - 1);
        }

        void V_reserve_map_at_back(size_type __nodes_to_add = 1) {
            if (__nodes_to_add > V_map_size - (V_finish.V_node - V_map) + 1)
                V_reallocate_map(__nodes_to_add, false);
        }

        void V_reserve_map_at_front(size_type __nodes_to_add = 1) {
            if (__nodes_to_add > V_map_size - (V_start.V_node - V_map))
                V_reallocate_map(__nodes_to_add, true);
        }

    public:

        iterator begin() { return V_start; }

        iterator end() { return V_finish; }

        size_type size() { return V_finish - V_start; }

        bool empty() { return V_start == V_finish; }
        size_type max_size() { return size_type(-1); }

        reference front() { return *begin(); }

        reference back() {
            iterator t = V_finish;
            --t;
            return *t;
        }

        // TODO
        void swap(deque<_Ty, _Alloc>& __res) {
            IOP_STL::swap(V_start, __res.V_start);
            IOP_STL::swap(V_finish, __res.V_finish);
            IOP_STL::swap(V_map, __res.V_map);
            IOP_STL::swap(V_map_size, __res.V_map_size);
        }

        void assign(size_type __c, const_reference __v) {
            size_type d = size() + static_cast<size_type>(static_cast<difference_type>(
                V_finish.V_last - V_finish.V_cur)) - __c;
            if (0 > d) {
                fill(begin(), end(), __v);
                for (size_type i = 0;i != -d;++i)
                    push_back(__v);
            }
            else {
                iterator a = begin();
                iterator b = begin() + d;
                fill(a, b, __v);
            }
        }

        template<class InputIt>
        void assign(InputIt first, InputIt last) {

            size_type c = static_cast<size_type>(IOP_ALGO::distance(first, last));
            size_type d = size()
                + static_cast<size_type>(
                    IOP_ALGO::distance(V_finish.V_last, V_finish.V_cur)) - c;

            iterator a = begin();
            if (0 < d) {
                while (a != end())
                    *(++a) = *(++first);
                while (first != last)
                    push_back(*first);
            }
            else {
                while (first != last)
                    *(++a) = *(++first);
            }
        }

        void assign(std::initializer_list<_Ty> __r) { assign(__r.begin(), __r.end()); }

        reference at(size_type __n) {
            if (__n >= size())
                throw IOP_STL::bad_range();
            else
                return *(begin() + __n);
        }


    protected:

    //* push back and front is no cur-Map_size than Map_size case.
        void push_back_impl(const_reference __v) {

            V_reserve_map_at_back();
            *(V_finish.V_node + 1) = V_allocate_node();

            try {
                construct(V_finish.V_cur, __v);
                V_finish.V_set_node(V_finish.V_node + 1);
                V_finish.V_cur = V_finish.V_first;
            }
            catch (...) {
                V_deallocate_node(*(V_finish.V_node + 1));
                throw;
            }

        }

        void push_front_impl(const_reference __v) {

            V_reserve_map_at_front();
            *(V_start.V_node - 1) = V_allocate_node();

            try {
                V_start.V_set_node(V_start.V_node - 1);

                V_start.V_cur = V_start.V_last - 1;
                construct(V_start.V_cur, __v);

            }
            catch (...) {
                // cur : last -> start.V_first 
                ++V_start;
                V_deallocate_node(*(V_start.V_node - 1));
                throw;
            }

        }

    //* Ref operator+/-
        void pop_back_impl() {
            V_deallocate_node(V_finish.V_first);

            V_finish.V_set_node(V_finish.V_node - 1);
            V_finish.V_cur = V_finish.V_last - 1;

            destroy(V_finish.V_cur);
        }

        void pop_front_impl() {
            destroy(V_finish.V_cur);
            V_deallocate_node(V_start.V_first);

            V_finish.V_set_node(V_finish.V_node + 1);
            V_finish.V_cur = V_finish.V_first;
        }


    public:

        void push_back(const_reference __v) {
            if (V_finish.V_cur != V_finish.V_last - 1) {
                construct(V_finish.V_cur, __v);
                ++V_finish.V_cur;
            }
            else {
                push_back_impl(__v);
            }
        }


        void push_front(const_reference __v) {
            if (V_start.V_cur != V_start.V_first) {
                construct(V_start.V_cur - 1, __v);
                --V_start.V_cur;
            }
            else {
                push_front_impl(__v);
            }
        }

        void pop_back() {
            if (V_finish.V_cur != V_finish.V_first) {
                --V_finish.V_cur;
                destroy(V_finish.V_cur);
            }
            else {
                pop_back_impl();
            }
        }

        void pop_front() {
            if (V_start.V_cur != V_start.V_last - 1) {
                destroy(V_start.V_cur);
                ++V_start.V_cur;
            }
            else {
                pop_back_impl();
            }
        }

        void clear() {
            for (Map_pointer_type i = V_start.V_node + 1; i != V_finish.V_node;++i) {
                destroy(*i, *i + S_buf_size());
                V_deallocate_node(*i);
            }

            if (V_start.V_node != V_finish.V_node) {
                destroy(V_start.V_cur, V_start.V_last);
                destroy(V_finish.V_first, V_finish.V_cur);
                V_deallocate_node(*V_finish.V_node);
            }
            else {
                destroy(V_start.V_cur, V_finish.V_cur);
            }

            V_finish = V_start;
        }
        iterator erase(iterator __pos) {

            difference_type d = __pos - V_start;
            iterator m = __pos;
            ++m;

            // diff( start,pos ) < diff( pos,back ).
            if (d < size() >> 1) {
                copy_backward(V_start, __pos, m);
                pop_front();
            }
            else {
                copy_backward(m, V_finish, __pos);
                pop_back();
            }

            return V_start + d;
        }

        iterator erase(const_iterator __first, const_iterator __last) {
            for (;__first == __last;++__first)
                erase(__first++);
        }

        iterator insert(iterator __pos, const_reference __v) {
            difference_type d = __pos - V_start;
            iterator c, r, r1, pos_old;

        //* Push-fun the pos to reset, and then move the area.
            if (d < size() / 2) {
                push_front(front());
                __pos = V_start + d;
                pos_old = __pos;
                ++pos_old;

                r = V_start;
                ++r;
                r1 = r;
                ++r1;

                copy(r1, pos_old, r);
            }
            else {
                push_back(back());

                __pos = V_start + d;

                r = V_finish;
                --r;
                r1 = r;
                --r1;

                copy_backward(__pos, r1, r);
            }

            *__pos = __v;

            return __pos;
        }


        void resize(size_type __n, const_reference __v)
            NOEXCEPT(NOEXCEPT(assert(__n >= max_size())))
        {
            int d = size() - __n;
            if (d < 0)
                for (;d != 0; ++d) push_back(__v);
            else
                for (;d != 0; --d) pop_back();
        }

        void resize(const size_type& __n) { resize(__n, value_type()); }

        void shrink_to_fit() { erase(++V_finish.V_cur, V_finish.V_last); }

    };

    template<class _T, class _Alloc, class _V>
    typename IOP_STL::deque<_T, _Alloc>::size_type
        erase(IOP_STL::deque<_T, _Alloc>& __c, const _V& __v) {

        auto iter = IOP_ALGO::remove(__c.begin(), __c.end(), __v);
        __c.erase(iter, __c.end());

        return IOP_ALGO::distance(__c.begin(), __c.end());
    }


    template<class _T, class _Alloc, class _V, class _F>
    typename IOP_STL::deque<_T, _Alloc>::size_type
        erase(IOP_STL::deque<_T, _Alloc>& __c, const _V& __v, _F __f) {

        auto iter = IOP_ALGO::remove_if(__c.begin(), __c.end(), __v, __f);
        __c.erase(iter, __c.end());

        return IOP_ALGO::distance(__c.begin(), __c.end());
    }

    template<class _T, class _Alloc>
    void swap(IOP_STL::deque<_T, _Alloc>& __c1, IOP_STL::deque<_T, _Alloc>& __c2)
        NOEXCEPT(NOEXCEPT(swap(__c2))) { __c1.swap(__c2); }

};      // namespace IOP_STL

#endif          // _DEQUE_IOP_DEFINE_