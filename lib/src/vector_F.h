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

#include "config_def.h"
#include "iterator_F.h"
#include "allocator_F.h"
#include "allocator_traits_F.h"
#include "initailzer_list_F.h"
#include "alloc_operator_new_F.h"
#include "func_exception.h"

namespace IOP_STL {


    template<class _Ty, class _Alloc = IOP_allocator<_Ty>>
    class vector {
    protected:

        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<_Ty, _Alloc>;

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
        using reverse_iterator = Iterator_STL::reverse_iterator<iterator>;
        using const_reverse_iterator = Iterator_STL::reverse_iterator<const_iterator>;


    public:

        explicit vector() NOEXCEPT : V_start(), V_end_of_storage(), V_finish() {}
        ~vector() { destroy(V_start, V_finish); }

        explicit vector(size_type __n, const _Alloc&)
            :V_start(), V_end_of_storage(), V_finish() {
            V_uninitailzed_alloc(__n, allocator_type());
        }

        //* Similar to LLVM-LIBCPP vector of the conition define
        explicit vector(size_type __n)
            : V_start(), V_end_of_storage(), V_finish()
        {
            if (__n != 0) {
                V_uninitailzed_alloc(__n, allocator_type());
                uninitialized_fill_n(V_start, __n, _Ty());
            }
        }

        explicit vector(const vector& __rhs) {
            size_type _rsize = __rhs.size();
            if (_rsize <= size()) {
                V_finish = uninitialized_copy(__rhs.V_start, __rhs.V_finish, V_start);
            }
            else {
                difference_type _rend_size = static_cast<difference_type>(__rhs.end() - __rhs.begin());
                V_start = V_allocate(_rend_size * sizeof(value_type));
                V_finish = uninitialized_copy(__rhs.V_start, __rhs.V_finish, V_start);
            }
        }

        explicit vector(vector&& __rhs) NOEXCEPT {
            V_start = __rhs.V_start;
            V_finish = __rhs.V_finish;
            V_end_of_storage = __rhs.V_end_of_storage;

            __rhs.V_start = nullptr;
            __rhs.V_finish = nullptr;
            __rhs.V_end_of_storage = nullptr;
        }


        explicit vector(initializer_list<_Ty>& __rhs) {

            V_start = V_allocate(__rhs.size());
            V_finish = V_start;
            V_end_of_storage = V_start + __rhs.size();

            uninitialized_copy(V_finish, V_end_of_storage, V_start);
        }


    protected:

        iterator V_start;
        iterator V_finish;
        iterator V_end_of_storage;
        allocator_type V_data_allocator;


        IOP_CONSTEXPR_CXX17 pointer V_allocate(size_type __n) { return V_data_allocator.allocate(__n); }

        IOP_CONSTEXPR_CXX17 void V_deallocate(pointer __ptr, size_type __n) {
            return V_data_allocator.deallocate(__ptr, __n);
        }

        IOP_CONSTEXPR_CXX17 void V_deallocate(pointer __ptr) {
            return V_data_allocator.deallocate(__ptr);
        }


        IOP_CONSTEXPR_CXX17 void V_uninitailzed_alloc(size_type __n, const _Alloc&) {
            V_start = V_allocate(__n);
            V_finish = V_start;
            V_end_of_storage = V_start + __n;
        }


        void V_insert_one_aux(iterator __postition, const_reference __val) {
            if (V_finish != V_end_of_storage) {
                construct(V_finish, *(--V_finish));
                ++V_finish;

                const value_type _copy_val = __val;
                copy_backward(__postition, V_finish - 2, V_finish - 1);

                *__postition = _copy_val;
            }
            else {
                const size_type old_n = size();
                const size_type n = old_n == 0 ? 1 : 2 * old_n;
                iterator new_start = V_allocate(n);
                iterator new_finish = new_start;

               //* since copy processing conduct try of the handle
                new_finish = uninitialized_copy(V_start, __postition, new_start);
                construct(new_finish, __val);
                ++new_finish;

                new_finish = uninitialized_copy(__postition, V_finish, new_finish);

                destroy(begin(), end());
                V_deallocate(V_start, static_cast<size_type>(V_end_of_storage - V_start));
                V_start = new_start;
                V_finish = new_finish;
                V_end_of_storage = new_start + n;
            }

        }

    public:

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


        IOP_CONSTEXPR_CXX17 void swap(iterator& __q, iterator& __p)
            NOEXCEPT(IOP_STL::swap(
                std::declval<reference>(), std::declval<reference>())
            ) {
            swap_ptr(__q, __p);
        }


        IOP_CONSTEXPR_CXX17 size_type size() NOEXCEPT {
            return static_cast<size_type>(end() - begin());
        }

        IOP_CONSTEXPR_CXX17 size_type size() const NOEXCEPT {
            return static_cast<size_type>(end() - begin());
        }

        IOP_CONSTEXPR_CXX17 size_type max_size() const NOEXCEPT { return size_type(-1); }


        void push_back(const_reference __val) {
            if (V_finish != V_end_of_storage) {
                construct(V_finish, __val);
                ++V_finish;
            }
            V_insert_one_aux(end(), __val);
        }

        void push_back() {
            if (V_finish != V_end_of_storage) { ++V_finish; }
            else V_insert_one_aux(end(), value_type());
        }


        void push_front(const_reference __val) { insert(begin(), __val); }

        void push_front() {
            insert(begin(), value_type());
        }


        void emplace_back(const_reference __val) { push_back(); }

        void emplace_front(const_reference __val) { push_front(); }


        void pop_back() NOEXCEPT(NOEXCEPT(assert(!empty()))) {
            assert(!empty());
            --V_finish;
            destroy(V_finish);
        }

        void pop_front() NOEXCEPT(NOEXCEPT(assert(!empty()))) {
            assert(!empty());
            destroy(V_start++);
        }


        iterator insert(iterator __postition, value_type __val) {
            size_type l = static_cast<size_type>(__postition - begin());

            if (V_finish != V_end_of_storage && __postition == end()) {
                construct(V_finish, __val);
                ++V_finish;
            }
            else {
                V_insert_one_aux(__postition, __val);
            }

            return static_cast<iterator>(begin() + l);
        }

        iterator insert(iterator __postition) {
            size_type loca = static_cast<size_type>(__postition - begin());

            if (V_finish != V_end_of_storage && __postition == end()) {
                construct(V_finish, value_type());
                ++V_finish;
            }
            else {
                V_insert_one_aux(__postition, value_type());
            }

            return static_cast<iterator>(begin() + loca);
        }


        IOP_CONSTEXPR_CXX17 const_reference at(const size_type __n) {
            if (__n >= size())
                throw bad_range();
            else
                return *(begin() + __n);
        }

        IOP_CONSTEXPR_CXX17 const_reference operator[](const size_type __n)  NOEXCEPT {
            return *(begin() + __n);
        }

    };
};          // namespace IOP_STL

#endif      // _VECTOR_IOP
