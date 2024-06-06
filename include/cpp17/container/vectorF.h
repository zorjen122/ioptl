/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _IOP_VECTOR_DEF
#define _IOP_VECTOR_DEF

#include "../allocatorF.h"
#include "../iteratorF.h"
#include "../cppconfig.h"

#include "../algo/algorithmF.h"
#include "../traits/allocator_traitsF.h"
#include "../util/exceptionF.h"

#include "../vec_iter.h"

#include <initializer_list>

namespace iop {

    template <class _Ty, class _Alloc = allocator<_Ty>> class vector
    {
      protected:
        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<allocator_type>;

      public:
        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = vec_iter::trivial_iterator<value_type>;
        using const_iterator = const iterator;
        using reverse_iterator = iter::reverse_iterator<iterator>;
        using const_reverse_iterator =
            iter::reverse_iterator<const_iterator>;

      public:
        vector() noexcept : V_start(), V_finish(), V_storage() {}

        vector(const vector &vec)
        {
            V_uninitailzed_alloc(vec.size());
            V_finish = uninitialized_copy(vec.begin(), vec.end(), V_start);
        }

        ~vector()
        {
            destroy(V_start, V_finish);
            V_deallocate(V_start, static_cast<size_type>(V_storage - V_start));
        }

        vector(const ::std::initializer_list<_Ty> &__rhs)
        {
            V_uninitailzed_alloc(__rhs.size());
            V_finish =
                uninitialized_copy(__rhs.begin(), __rhs.end(), V_start);
        }

        explicit vector(size_type __n, const _Alloc &)
            : V_start(), V_finish(), V_storage()
        {
            assert(__n > 0);
            V_uninitailzed_alloc(__n);
            V_finish += __n;
        }

        //* Similar to LLVM-LIBCPP vector of the conition define
        explicit vector(size_type __n) : V_start(), V_finish(), V_storage()
        {
            assert(__n > 0);
            V_uninitailzed_alloc(__n);
            V_finish += __n;
        }

        explicit vector(size_type __n, const_reference __val)
            : V_start(), V_finish(), V_storage()
        {
            assert(__n > 0);
            V_uninitailzed_alloc(__n);
            uninitialized_fill_n(V_start, __n, __val);
            V_finish += __n;
        }

        explicit vector(vector &&__rhs) noexcept
        {
            V_start = __rhs.V_start;
            V_finish = __rhs.V_finish;
            V_storage = __rhs.V_storage;

            __rhs.V_start = nullptr;
            __rhs.V_finish = nullptr;
            __rhs.V_storage = nullptr;
        }

      protected:
        iterator V_start;
        iterator V_finish;
        iterator V_storage;
        [[no_unique_address]] allocator_type __Vec_allocator;

        constexpr iterator V_allocate(size_type __n)
        {
            return static_cast<iterator>(__Vec_allocator.allocate(__n));
        }

        constexpr void V_deallocate(iterator __ptr, size_type __n)
        {
            __Vec_allocator.deallocate(&*__ptr, __n);
        }

        constexpr void V_deallocate(iterator __ptr)
        {
            __Vec_allocator.deallocate(&*__ptr);
        }

        //* This function is compatible
        // with common construction and copy construction.
        constexpr void V_uninitailzed_alloc(size_type __n)
        {
            V_start = V_allocate(__n);
            V_finish = V_start;
            V_storage = V_start + __n;
        }

        void V_insert_one_aux(iterator __pos, const_reference __val)
        {
            if (V_finish != V_storage) {
                construct_at(&*--V_finish);
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
                new_finish = uninitialized_copy(V_start, __pos, new_start);
                construct_at(&*new_finish, __val);
                ++new_finish;

                new_finish =
                    uninitialized_copy(__pos, V_finish, new_finish);

                destroy(begin(), end());
                V_deallocate(V_start, static_cast<size_type>(V_storage - V_start));
                V_start = new_start;
                V_finish = new_finish;
                V_storage = new_start + n;
            }
        }

      public:
        constexpr allocator_type get_allocator() noexcept
        {
            return allocator_type();
        }

        constexpr iterator begin() noexcept { return V_start; }
        constexpr const_iterator begin() const noexcept
        {
            return V_start;
        }

        constexpr iterator end() noexcept { return V_finish; }
        constexpr const_iterator end() const noexcept
        {
            return V_finish;
        }

        constexpr reference front() noexcept { return *V_start; }
        constexpr const_reference front() const noexcept
        {
            return *V_start;
        }

        constexpr reference back() noexcept
        {
            return *(V_finish - 1);
        }
        constexpr const_reference back() const noexcept
        {
            return *(V_finish - 1);
        }

        constexpr bool empty() noexcept { return begin() == end(); }
        constexpr bool empty() const noexcept
        {
            return begin() == end();
        }

        constexpr void swap(vector &vec)
        {
            ::std::swap(V_start, vec.V_start);
            ::std::swap(V_finish, vec.V_finish);
            ::std::swap(V_storage, vec.V_storage);
        }

        constexpr size_type size() noexcept
        {
            return static_cast<size_type>(end() - begin());
        }

        constexpr size_type size() const noexcept
        {
            return static_cast<size_type>(end() - begin());
        }

        constexpr size_type capacity() noexcept
        {
            return static_cast<size_type>(V_storage - V_start);
        };

        constexpr const size_type capacity() const noexcept
        {
            return static_cast<size_type>(V_storage - V_start);
        };

        constexpr size_type max_size() noexcept
        {
            return size_type(-1);
        }
        constexpr const size_type max_size() const noexcept
        {
            return size_type(-1);
        }

        constexpr void reserve(size_type __ncap)
            noexcept(noexcept(__ncap > max_size()))
        {
            size_type end_storage = (size_type)(V_storage - V_start);
            size_type size = (size_type)(V_finish - V_start);
            if (end_storage < __ncap) {
                iterator new_start = __Vec_allocator.allocate(__ncap);
                uninitialized_copy(V_start, V_finish, new_start);

                destroy(V_start, V_finish);
                V_deallocate(V_start, static_cast<size_type>(V_storage - V_start));


                V_start = new_start;
                V_finish = new_start + size;
                V_storage = V_start + __ncap;
            }
        }

        constexpr void shrink_to_fit()
        {
            if (V_finish != V_storage) {
                for (auto &i = V_finish; i != V_storage; ++i)
                    destroy(i);
            }
        }

        constexpr void push_back(const_reference __val)
        {
            if (V_finish != V_storage) {
                construct_at(&*V_finish, __val);
                ++V_finish;
            }
            else
                V_insert_one_aux(end(), __val);
        }

        constexpr void push_back()
        {
            if (V_finish != V_storage) {
                ++V_finish;
            }
            else
                V_insert_one_aux(end(), value_type());
        }

        constexpr void push_front(const_reference __val)
        {
            insert(begin(), __val);
        }

        constexpr void push_front() { insert(begin(), value_type()); }

        template <class... _Args>
        constexpr void emplace(iterator __pos, _Args &&...__args)
        {
            insert(__pos, value_type(Fiop::forward<_Args>(__args)...));
        }
        template <class... _Args>
        constexpr void emplace(const_iterator __pos,
                                         _Args &&...__args) const;
        template <class... _Args>
        constexpr void emplace_back(_Args &&...__args)
        {
            emplace(end(), __args...);
        }

        template <class... _Args>
        constexpr void emplace_front(_Args &&...__args)
        {
            push_front(value_type(Fiop::forward<_Args>(__args)...));
        }

        constexpr void pop_back()
        {
            assert(!empty());
            --V_finish;
            destroy(V_finish);
        }

        iterator erase(iterator __pos)
        {
            size_type d = static_cast<size_type>(__pos - V_start);
            iterator c = __pos;
            ++c;
            iop_algo::copy(c, V_finish, __pos);
            pop_back();

            return V_start + d;
        }
        iterator erase(const_iterator __pos) const;
        constexpr void clear()
        {
            destroy(V_start, V_finish);
            V_finish = V_start;
        }

        iterator insert(iterator __pos, value_type __val)
        {
            size_type l = static_cast<size_type>(__pos - begin());

            if (V_finish != V_storage && __pos == end()) {
                construct_at(&*V_finish, __val);
                ++V_finish;
            }
            else {
                V_insert_one_aux(__pos, __val);
            }

            return static_cast<iterator>(begin() + l);
        }

        iterator insert(iterator __pos)
        {
            size_type loca = static_cast<size_type>(__pos - begin());

            if (V_finish != V_storage && __pos == end()) {
                construct_at(&*V_finish, value_type());
                ++V_finish;
            }
            else {
                V_insert_one_aux(__pos, value_type());
            }

            return static_cast<iterator>(begin() + loca);
        }
        
        const_iterator insert(const_iterator __pos, value_type __val) const
        { return insert(__pos, __val); }
        
        const_iterator insert(const_iterator __pos) const;


        iterator find(const_reference __val)
        {   
            iterator it = begin();
            for(; it != end(); ++it)
            {
                if(*it == __val) break;
            }

            return it;
        }

        constexpr const_reference at(const size_type __n)
        {
            if (__n >= size())
                throw bad_range();
            else
                return *(begin() + __n);
        }

        constexpr reference operator[](const size_type &__n) noexcept
        {
            return *(begin() + __n);
        }

        constexpr vector operator=(const vector &vec)
        {
            if (size() < vec.size()) {
                destroy(begin(), end());
                V_deallocate(V_start, static_cast<size_type>(V_storage - V_start));
                V_uninitailzed_alloc(vec.size());
            }
            else {
                destroy(begin(), end());
            }

            V_finish = uninitialized_copy(vec.begin(), vec.end(), V_start);
            return *this;
        }
    };
}; // namespace iop

#endif // _VECTOR_IOP
