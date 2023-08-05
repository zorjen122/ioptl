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

#define _VECTOR_IOP_
#ifdef _VECTOR_IOP_

#include "../cppconfig.h"
#include "../iterator_F.h"
#include "../allocator_traits_F.h"
#include "../allocator_F.h"
#include "../func_exception.h"
#include "../algorithm.h"

#include <initializer_list>

namespace iop {

    template<class _Ty, class _Alloc = allocator<_Ty>>
    class vector {
    protected:

        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<allocator_type>;

    public:
        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = pointer;
        using const_iterator = const iterator;
        using reverse_iterator = iop::iter::reverse_iterator<iterator>;
        using const_reverse_iterator = iop::iter::reverse_iterator<const_iterator>;


    public:

        explicit vector() NOEXCEPT : V_start(), V_finish(), V_cap() {}

        vector(const vector& vec) {
            V_uninitailzed_alloc(vec.size(), allocator_type());
            V_finish = iop::uninitialized_copy(vec.begin(), vec.end(), V_start);
            V_cap = V_finish;
        }

        ~vector() {
            iop::destroy(V_start, V_finish);
            allocator_type::deallocate(V_start, static_cast<size_type>(V_cap - V_start));
        }

        vector(const ::std::initializer_list<_Ty>& __rhs) {
            V_uninitailzed_alloc(__rhs.size(), allocator_type());
            V_finish = iop::uninitialized_copy(__rhs.begin(), __rhs.end(), V_start);
        }

        explicit vector(size_type __n, const _Alloc&)
            :V_start(), V_cap(), V_finish() {
            assert(__n > 0);
            V_uninitailzed_alloc(__n, allocator_type());
            V_finish += __n;
        }

        //* Similar to LLVM-LIBCPP vector of the conition define
        explicit vector(size_type __n)
            : V_start(), V_cap(), V_finish()
        {
            assert(__n > 0);
            V_uninitailzed_alloc(__n, allocator_type());
            V_finish += __n;
        }

        explicit vector(size_type __n, value_type __val)
            : V_start(), V_cap(), V_finish()
        {
            assert(__n > 0);
            V_uninitailzed_alloc(__n, allocator_type());
            uninitialized_fill_n(V_start, __n, __val);
            V_finish += __n;
        }

        explicit vector(vector&& __rhs) NOEXCEPT {
            V_start = __rhs.V_start;
            V_finish = __rhs.V_finish;
            V_cap = __rhs.V_cap;

            __rhs.V_start = nullptr;
            __rhs.V_finish = nullptr;
            __rhs.V_cap = nullptr;
        }


    protected:

        iterator V_start;
        iterator V_finish;
        iterator V_cap;


        IOP_CONSTEXPR_CXX17 pointer V_allocate(size_type __n) { return allocator_type::allocate(__n); }

        IOP_CONSTEXPR_CXX17 void V_deallocate(pointer __ptr, size_type __n) {
            return allocator_type::deallocate(__ptr, __n);
        }

        IOP_CONSTEXPR_CXX17 void V_deallocate(pointer __ptr) {
            return allocator_type::deallocate(__ptr);
        }


        IOP_CONSTEXPR_CXX17 void V_uninitailzed_alloc(size_type __n, const _Alloc&) {
            V_start = V_allocate(__n);
            V_finish = V_start;
            V_cap = V_start + __n;
        }


        void V_insert_one_aux(iterator __pos, const_reference __val) {
            if (V_finish != V_cap) {
                construct(--V_finish);
                ++V_finish;

                const value_type _copy_val = __val;
                copy_backward(__pos, V_finish - 2, V_finish - 1);

                *__pos = _copy_val;
            }
            else {
                const size_type old_n = size();
                const size_type n = old_n == 0 ? 1 : 2 * old_n;
                iterator new_start = V_allocate(n);
                iterator new_finish = new_start;

               //* since copy processing conduct try of the handle
                new_finish = iop::uninitialized_copy(V_start, __pos, new_start);
                construct(new_finish, __val);
                ++new_finish;

                new_finish = iop::uninitialized_copy(__pos, V_finish, new_finish);

                iop::destroy(begin(), end());
                V_deallocate(V_start, static_cast<size_type>(V_cap - V_start));
                V_start = new_start;
                V_finish = new_finish;
                V_cap = new_start + n;
            }

        }

    public:

        IOP_CONSTEXPR_CXX17 allocator_type get_allocator() NOEXCEPT {
            return allocator_type();
        }

        IOP_CONSTEXPR_CXX17 iterator begin() NOEXCEPT { return V_start; }
        IOP_CONSTEXPR_CXX17 const_iterator begin() const NOEXCEPT { return V_start; }

        IOP_CONSTEXPR_CXX17 iterator end() NOEXCEPT { return V_finish; }
        IOP_CONSTEXPR_CXX17 const_iterator end() const NOEXCEPT { return V_finish; }

        IOP_CONSTEXPR_CXX17 reference front() NOEXCEPT { return *V_start; }
        IOP_CONSTEXPR_CXX17 const_reference front() const NOEXCEPT { return *V_start; }

        IOP_CONSTEXPR_CXX17 reference back() NOEXCEPT { return *(V_finish - 1); }
        IOP_CONSTEXPR_CXX17 const_reference back() const NOEXCEPT { return *V_finish; }


        IOP_CONSTEXPR_CXX17 bool empty() NOEXCEPT { return begin() == end(); }
        IOP_CONSTEXPR_CXX17 bool empty() const NOEXCEPT { return begin() == end(); }


        IOP_CONSTEXPR_CXX17 void swap(vector& vec) {
            ::std::swap(V_start, vec.V_start);
            ::std::swap(V_finish, vec.V_finish);
            ::std::swap(V_cap, vec.V_cap);
        }


        IOP_CONSTEXPR_CXX17 size_type size() NOEXCEPT {
            return static_cast<size_type>(end() - begin());
        }

        IOP_CONSTEXPR_CXX17 size_type size() const NOEXCEPT {
            return static_cast<size_type>(end() - begin());
        }

        IOP_CONSTEXPR_CXX17 size_type capacity() NOEXCEPT {
            return static_cast<size_type>(V_cap - V_start);
        };

        IOP_CONSTEXPR_CXX17 const size_type capacity() const NOEXCEPT {
            return static_cast<size_type>(V_cap - V_start);
        };

        IOP_CONSTEXPR_CXX17 size_type max_size() NOEXCEPT { return size_type(-1); }
        IOP_CONSTEXPR_CXX17 const size_type max_size() const NOEXCEPT { return size_type(-1); }


        IOP_CONSTEXPR_CXX17 void reserve(size_type __ncap)
            NOEXCEPT(NOEXCEPT(__ncap > max_size())) {
            if (V_cap < __ncap) {
                iterator new_start = allocator_type::allocate(__ncap);
                iop::uninitialized_copy(V_start, V_finish, new_start);

                iop::destroy(V_start, V_finish);
                V_deallocate(V_start, static_cast<size_type>(V_cap - V_start));

                V_start = new_start;
                V_finish = new_start + __ncap;
                V_cap = __ncap;
            }
        }

        IOP_CONSTEXPR_CXX17 void shrink_to_fit() {
            if (V_finish != V_cap) {
                for (auto& i = V_finish;i != V_cap;++i)
                    iop::destroy(i);
            }
        }


        IOP_CONSTEXPR_CXX17 void push_back(const_reference __val) {
            if (V_finish != V_cap) {
                construct(V_finish, __val);
                ++V_finish;
            }
            else
                V_insert_one_aux(end(), __val);
        }

        IOP_CONSTEXPR_CXX17 void push_back() {
            if (V_finish != V_cap) { ++V_finish; }
            else
                V_insert_one_aux(end(), value_type());
        }


        IOP_CONSTEXPR_CXX17 void push_front(const_reference __val) { insert(begin(), __val); }

        IOP_CONSTEXPR_CXX17 void push_front() {
            insert(begin(), value_type());
        }


        template<class... _Args>
        IOP_CONSTEXPR_CXX17 void emplace(const_iterator __pos, _Args&&... __args) {
            // TODO
        }


        template<class... _Args>
        IOP_CONSTEXPR_CXX17 void emplace_back(_Args&&... __args) {
            if (V_finish != V_cap) {
                construct_at(V_finish, FViop::forward<_Args>(__args)...);
                ++V_finish;
            }
            else
                emplace(V_finish, FViop::forward<_Args>(__args)...);
        }

        IOP_CONSTEXPR_CXX17 void emplace_front(const_reference __val) { push_front(); }


        IOP_CONSTEXPR_CXX17 void pop_back() {
            assert(!empty());
            --V_finish;
            iop::destroy_at(V_finish);
        }

        iterator erase(const_iterator& __pos) {
            size_type d = static_cast<size_type>(__pos - V_start);
            iterator c = __pos;
            ++c;
            copy(c, V_finish, __pos);
            pop_back();

            return V_start + d;
        }

        IOP_CONSTEXPR_CXX17 void clear() {
            iop::destroy(V_start, V_finish);
            V_finish = V_start;
        }


        iterator insert(iterator __pos, value_type __val) {
            size_type l = static_cast<size_type>(__pos - begin());

            if (V_finish != V_cap && __pos == end()) {
                construct(V_finish, __val);
                ++V_finish;
            }
            else {
                V_insert_one_aux(__pos, __val);
            }

            return static_cast<iterator>(begin() + l);
        }

        iterator insert(iterator __pos) {
            size_type loca = static_cast<size_type>(__pos - begin());

            if (V_finish != V_cap && __pos == end()) {
                construct(V_finish, value_type());
                ++V_finish;
            }
            else {
                V_insert_one_aux(__pos, value_type());
            }

            return static_cast<iterator>(begin() + loca);
        }



        IOP_CONSTEXPR_CXX17 const_reference at(const size_type __n) {
            if (__n >= size())
                throw bad_range();
            else
                return *(begin() + __n);
        }

        IOP_CONSTEXPR_CXX17 reference operator[](const size_type& __n) NOEXCEPT {
            return *(begin() + __n);
        }

        constexpr vector operator=(const vector& vec) {
            if (size() < vec.size()) {
                iop::destroy(begin(), end());
                V_deallocate(V_start, static_cast<size_type>(V_cap - V_start));
                V_uninitailzed_alloc(vec.size(), allocator_type());
            }
            else {
                iop::destroy(begin(), end());
            }

            V_finish = iop::uninitialized_copy(vec.begin(), vec.end(), V_start);
            return *this;
        }


    };
};          // namespace iop

#endif      // _VECTOR_IOP
