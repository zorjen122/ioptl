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

#define _VECTOR_STL_
#ifdef _VECTOR_STL_

#include "iterator_stl.h"
#include "allocator_stl.h"
#include "allocator_traits_stl.h"
#include "initailzer_list_stl.h"

#ifdef ALLOC_STL_BUG
#include "alloc_stl.h"
#else
#include "alloc_operator_new_stl.h"
template<class _Ty>
using default_alloc = __Default_alloc<_Ty>;
#endif


namespace CCC_STL {

    template<class _Ty, class _Alloc = default_alloc<_Ty>>
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

        CCC_CONSTEXPR_CXX17 explicit vector(size_type __n, const _Alloc&)
            :_M_start(), _M_end_of_storage(), _M_finish() {
            M_uninitailzed_alloc(__n, allocator_type());
        }

        //* Similar to LLVM-LIBCPP vector of the conition define
        CCC_CONSTEXPR_CXX17 explicit vector(size_type __n)
            : _M_start(), _M_end_of_storage(), _M_finish()
        {
            if (__n != 0) {
                M_uninitailzed_alloc(__n, allocator_type());
                uninitialized_fill_n(_M_start, __n, _Ty());
            }
        }

        CCC_CONSTEXPR_CXX17 explicit vector(const vector& __rhs) {
            size_type _rsize = __rhs.size();
            if (_rsize <= size()) {
                _M_finish = uninitalized_copy(__rhs._M_start, __rhs._M_finish, _M_start);
            }
            else {
                difference_type _rend_size = static_cast<difference_type>(__rhs.end() - __rhs.begin());
                _M_start = M_allocate(_rend_size * sizeof(value_type));
                _M_finish = uninitalized_copy(__rhs._M_start, __rhs._M_finish, _M_start);
            }
        }

        CCC_CONSTEXPR_CXX17 explicit vector(vector&& __rhs) CCC_NOEXCEPT {
            _M_start = __rhs._M_start;
            _M_finish = __rhs._M_finish;
            _M_end_of_storage = __rhs._M_end_of_storage;

            __rhs._M_start = nullptr;
            __rhs._M_finish = nullptr;
            __rhs._M_end_of_storage = nullptr;
        }


        CCC_CONSTEXPR_CXX17 explicit vector(initializer_list<_Ty>& __rhs) {
            _M_data_allocator = allocator_type();

            _M_start = M_allocate(__rhs.size());
            _M_finish = _M_start;
            _M_end_of_storage = _M_start + __rhs.size();

            uninitalized_copy(_M_finish, _M_end_of_storage, _M_start);
        }


        template<class... __Args>
        CCC_CONSTEXPR_CXX17 explicit vector(__Args&&... __args) {
            _M_data_allocator = allocator_type();

            _M_start = M_allocate(sizeof...(__args));
            _M_finish = _M_start + sizeof...(__args);
            _M_end_of_storage = _M_finish;

            construct(_M_start, __args...);
        }

    protected:

        iterator _M_start;
        iterator _M_finish;
        iterator _M_end_of_storage;
        allocator_type _M_data_allocator;


        CCC_CONSTEXPR_CXX17 pointer M_allocate(size_type __n) { return _M_data_allocator.allocate(__n); }
        CCC_CONSTEXPR_CXX17 void M_deallocate(pointer __ptr, size_type __n) {
            return _M_data_allocator.deallocate(__ptr, __n);
        }
        CCC_CONSTEXPR_CXX17 void M_deallocate(pointer __ptr) {
            return _M_data_allocator.deallocate(__ptr);
        }


        CCC_CONSTEXPR_CXX17 void M_uninitailzed_alloc(size_type __n, const _Alloc&) {
            _M_start = M_allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage = _M_start + __n;
        }

    public:

        CCC_CONSTEXPR_CXX17 vector() CCC_NOEXCEPT : _M_start(), _M_end_of_storage(), _M_finish() {}
        CCC_CONSTEXPR_CXX17 ~vector() { destroy(_M_start, _M_finish); }


        CCC_CONSTEXPR_CXX17 iterator begin() CCC_NOEXCEPT { return _M_start; }
        CCC_CONSTEXPR_CXX17 const_iterator begin() const CCC_NOEXCEPT { return _M_start; }

        CCC_CONSTEXPR_CXX17 iterator end() CCC_NOEXCEPT { return _M_finish; }
        CCC_CONSTEXPR_CXX17 const_iterator end() const CCC_NOEXCEPT { return _M_finish; }

        CCC_CONSTEXPR_CXX17 reference front() CCC_NOEXCEPT { return *_M_start; }
        CCC_CONSTEXPR_CXX17 const_reference front() const CCC_NOEXCEPT { return *_M_start; }

        CCC_CONSTEXPR_CXX17 reference back() CCC_NOEXCEPT { return *_M_finish; }
        CCC_CONSTEXPR_CXX17 const_reference back() const CCC_NOEXCEPT { return *_M_finish; }


        CCC_CONSTEXPR_CXX17 bool empty() CCC_NOEXCEPT { return begin() == end(); }
        CCC_CONSTEXPR_CXX17 bool empty() const CCC_NOEXCEPT { return begin() == end(); }


        CCC_CONSTEXPR_CXX17 void swap(iterator __ity, iterator __itys) { swap_ptr(__ity, __itys); }

        // size non-using distance ? -> call function overhead ?
        CCC_CONSTEXPR_CXX17 size_type size() CCC_NOEXCEPT {
            return static_cast<size_type>(end() - begin());
        }
        CCC_CONSTEXPR_CXX17 size_type size() const CCC_NOEXCEPT {
            return static_cast<size_type>(end() - begin());
        }

        CCC_CONSTEXPR_CXX17 void clear() { destroy(_M_start, _M_finish); }


        template<class _Ptr, class... _Args>
        CCC_CONSTEXPR_CXX17 void emplace_loca_slow_path(_Ptr* const __ptr, _Args&&... __args) {

            const size_type u = sizeof...(__args);
            const size_type n = size() > u ? 2 * size() + (u >> 4)
                : 2 * size() + u;

            iterator new_start = M_allocate(n);
            iterator new_finish = new_start;

            new_finish = uninitalized_copy(_M_start, __ptr, new_start);
            construct(new_finish, FVstd::forward<_Args>(__args)...);
            new_finish += u;

            destroy(begin(), end());
            M_deallocate(_M_start, static_cast<size_type>(_M_end_of_storage - _M_start));

            _M_start = new_start;
            _M_finish = new_finish;
            _M_end_of_storage = new_start + n;
        }


        template<class... _Args>
        CCC_CONSTEXPR_CXX17 void emplace_back_slow_path(_Args&&... __args) {
            emplace_loca_slow_path(_M_finish, FVstd::forward<_Args>(__args)...);
        }

    //* constexpr inline is equivalent for type is void
        template<class... _Args>
        CCC_CONSTEXPR_CXX17
            #if CCC_VERSION > 14 
            reference
            #else
            void
            #endif
            emplace_back(_Args&&... __args) {
            size_type n = size() + sizeof...(__args);
            size_type esn = sizeof(_M_end_of_storage) / sizeof(value_type);
            if (n < esn) {
                construct(_M_finish, __args...);
                ++_M_finish;
            }
            else
                emplace_back_slow_path(__args...);

            #if CCC_VERSION > 14

            return back();

            #endif
        }


        //* For only one parameter, such that code inline also for T... multiplex code  
        CCC_CONSTEXPR_CXX17 void _M_insert_one_aux(iterator __postition, const_reference __val) {
            if (_M_finish != _M_end_of_storage) {
                construct(_M_finish, *(--_M_finish));
                ++_M_finish;

                const value_type _copy_val = __val;
                copy_backward(__postition, _M_finish - 2, _M_finish - 1);

                *__postition = _copy_val;
            }
            else {
                const size_type old_n = size();
                const size_type n = old_n == 0 ? 1 : 2 * old_n;
                iterator new_start = M_allocate(n);
                iterator new_finish = new_start;

               //* since copy processing conduct try of the handle
                new_finish = uninitalized_copy(_M_start, __postition, new_start);
                construct(new_finish, __val);
                ++new_finish;

                new_finish = uninitalized_copy(__postition, _M_finish, new_finish);

                destroy(begin(), end());
                M_deallocate(_M_start, static_cast<size_type>(_M_end_of_storage - _M_start));
                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + n;
            }
        }

        // TODO: insert_one_aux wrong.
        template<class _It, class... _Args>
        void insert(_It* __pos, _Args&&... __args) {
            (..., _M_insert_one_aux(__pos, std::forward<_Args>(__args)));
        }


        CCC_CONSTEXPR_CXX17 void push_back(const value_type __val) {
            if (_M_finish != _M_end_of_storage)
                emplace_back(__val);
            else
                _M_insert_one_aux(_M_finish, __val);
        }

        template<class... __Args>
        CCC_CONSTEXPR_CXX17 void push_back(__Args&&... __args) {
            (push_back(__args), ...);
        }

        CCC_CONSTEXPR_CXX17 void push_back() {
            if (_M_finish != _M_end_of_storage) { ++_M_finish; }
            else _M_insert_one_aux(end(), value_type());
        }

        CCC_CONSTEXPR_CXX17 iterator push_front(const value_type __val) { insert(begin(), __val); }

        CCC_CONSTEXPR_CXX17 iterator push_front() {
            insert(begin(), value_type());
            return begin();
        }

        CCC_CONSTEXPR_CXX17 void pop_back() CCC_NOEXCEPT(CCC_NOEXCEPT(assert(!empty()))) {
            assert(!empty());
            --_M_finish;
            destroy(_M_finish);
        }

        CCC_CONSTEXPR_CXX17 iterator insert(iterator __postition, value_type __val) {
            size_type loca = static_cast<size_type>(__postition - begin());

            if (_M_finish != _M_end_of_storage && __postition == end()) {
                construct(_M_finish, __val);
                ++_M_finish;
            }
            else {
                _M_insert_one_aux(__postition, __val);
            }

            return static_cast<iterator>(begin() + loca);
        }

        CCC_CONSTEXPR_CXX17 iterator insert(iterator __postition) {
            size_type loca = static_cast<size_type>(__postition - begin());

            if (_M_finish != _M_end_of_storage && __postition == end()) {
                construct(_M_finish, value_type());
                ++_M_finish;
            }
            else {
                _M_insert_one_aux(__postition, value_type());
            }

            return static_cast<iterator>(begin() + loca);
        }


        CCC_CONSTEXPR_CXX17 value_type index(const size_type __n) CCC_NOEXCEPT {
            return *(begin() + __n);
        }

        CCC_CONSTEXPR_CXX17 value_type operator[](const size_type __n)  CCC_NOEXCEPT {
            return *(begin() + __n);
        }

    };
};          // namespace CCC_STL

#endif      // _VECTOR_STL
