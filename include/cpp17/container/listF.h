/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _IOP_LIST_DEF
#define _IOP_LIST_DEF

#include "../allocatorF.h"
#include "../iteratorF.h"

#include "../traits/allocator_traitsF.h"
#include "../util/iterator_baseF.h"
#include "../util/utilityF.h"

#include <initializer_list>
#include <typeinfo>

namespace iop {

    struct __list_node_base
    {
        __list_node_base *V_next;
        __list_node_base *V_prev;
    };

    template <class _Ty> struct __list_node : public __list_node_base
    {
        _Ty V_data;
    };

    class list_iterator_base
    {
      public:
        using iterator_category = bidirectional_iterator_tag;
        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;

        __list_node_base *V_node;

        list_iterator_base(const list_iterator_base &__rhs)
            : V_node(__rhs.V_node)
        {}

        explicit list_iterator_base(__list_node_base *__rhs) : V_node(__rhs) {}
        explicit list_iterator_base() {}

        void V_incr() { V_node = V_node->V_next; }
        void V_decr() { V_node = V_node->V_prev; }

        bool operator==(const list_iterator_base &__rhs) const noexcept
        {
            return V_node == __rhs.V_node;
        }
        bool operator!=(const list_iterator_base &__rhs) const noexcept
        {
            return V_node != __rhs.V_node;
        }
    };

    template <class _Ty, class _Ref, class _Ptr>
    class list_iterator : public list_iterator_base
    {
      public:
        using iterator = list_iterator<_Ty, _Ty &, _Ty *>;
        using const_iterator = list_iterator<_Ty, const _Ty &, const _Ty *>;

        using value_type = _Ty;
        using pointer = _Ty *;
        using const_pointer = const _Ty *;
        using reference = _Ty &;
        using const_reference = const _Ty &;
        using node_T = __list_node<_Ty>;
        using const_node = const __list_node<const _Ty>;
        using Self_ = list_iterator<_Ty, _Ref, _Ptr>;

        explicit list_iterator() : list_iterator_base() {}
        explicit list_iterator(node_T *__rhs) : list_iterator_base(__rhs) {}
        explicit list_iterator(const_iterator &__rhs)
            : list_iterator_base(__rhs.V_node)
        {}

        reference operator*() const noexcept
        {
            return static_cast<node_T *>(V_node)->V_data;
        }

        Self_ &operator++() noexcept
        {
            V_incr();
            return *this;
        }

        Self_ operator++(int) noexcept
        {
            Self_ k = *this;
            V_incr();
            return k;
        }

        Self_ &operator--() noexcept
        {
            V_decr();
            return *this;
        }

        Self_ operator--(int) noexcept
        {
            Self_ k = *this;
            V_decr();
            return k;
        }
    };

    template <class _Ty, class _Alloc = allocator<_Ty>> class list
    {
      protected:
        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<_Alloc>;
        using allocator_node_type = typename allocator_traits_type::template 
                                rebind_alloc<__list_node<_Ty>>::other;
        using node_T = __list_node<_Ty>;
        using const_node = __list_node<const _Ty>;

      public:
        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = list_iterator<_Ty, _Ty &, _Ty *>;
        using const_iterator = list_iterator<_Ty, const _Ty &, const _Ty *>;

      public:
        explicit list() noexcept { V_initializer_list(); }

        explicit list(const_reference __v, const size_type __sz) noexcept
        {
            V_initializer_list();

            for (size_type i = 0; i < __sz - 1; ++i)
                push_back(__v);
                
        }

        explicit list(const list &__res) noexcept
        {
            V_uninitializer_copy(__res);
        }

        list(const ::std::initializer_list<_Ty> &__res)
        {
            V_initializer_list();
            for (auto i = __res.begin(); i != __res.end(); ++i)
                push_back(*i);
        }

        ~list()
        {
            clear();
            __List_allocator.deallocate(V_node, 1);
        }

      protected:
        node_T *V_node;
        [[no_unique_address]] allocator_node_type __List_allocator;

      protected:
        void V_initializer_list() noexcept
        {
            V_node = __List_allocator.allocate(1);
            V_node->V_next = V_node;
            V_node->V_prev = V_node;
        }

        node_T *V_unit_create(const _Ty &__val) noexcept
        {
            node_T *k = __List_allocator.allocate(1);
            construct_at(&k->V_data, __val);
            return k;
        }

        void V_uninitializer_copy(const list &__res)
        {
            node_T *__rhs = static_cast<node_T *>(__res.V_node->V_next);

            try {
                while (__rhs != __res.V_node) {
                    push_back(__rhs->V_data);
                    __rhs = static_cast<node_T *>(__rhs->V_next);
                }
            }
            catch (...) {
                while (__rhs != __res.V_node) {
                    node_T *tmp = __rhs;
                    tmp = static_cast<node_T *>(tmp->V_prev);
                    iop::destroy_at(__rhs);
                    __rhs = tmp;
                }
                throw;
            }
        }

      public:
        allocator_node_type get_allocator() noexcept { return allocator_node_type(); }

        void clear()
        {
            node_T *res = static_cast<node_T *>(V_node->V_next);
            while (res != V_node) {
                node_T *k = res;
                res = static_cast<node_T *>(res->V_next);
                iop::destroy_at(&k->V_data);

                __List_allocator.deallocate(k, 1);
            }
            V_node->V_next = V_node;
            V_node->V_prev = V_node;
        }

        // insert
        iterator insert(iterator __postition, const_reference __val)
        {
            node_T *k = V_unit_create(__val);
            k->V_next = __postition.V_node;
            k->V_prev = __postition.V_node->V_prev;

            __postition.V_node->V_prev->V_next = k;
            __postition.V_node->V_prev = k;

            return static_cast<iterator>(k);
        }

        template <class InputIt>
        iterator insert(iterator pos, InputIt first, InputIt last)
        {

            if constexpr (mpls::is_integral_v<InputIt>) {
                while (first) {
                    insert(pos, last);
                    --first;
                }
                return pos;
            }
            else {
                while (first != last)
                    insert(pos, *(first++));
                return pos;
            }
        }

        iterator insert(iterator pos, ::std::initializer_list<value_type> ilist)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        // Out-of-Money by system throw ERROR
        template <class... _Args> iterator emplace_back(const _Args &&...__args)
        {
            for (size_type i = 0; i < sizeof...(__args); ++i)
                insert(--end(), __args...);
        }

        // copy to delete
        iterator erase(iterator __position)
        {
            __list_node_base *__next_node = __position.V_node->V_next;
            __list_node_base *__prev_node = __position.V_node->V_prev;

            node_T *res = static_cast<node_T *>(__position.V_node);

            __next_node->V_prev = __prev_node;
            __prev_node->V_next = __next_node;

            iop::destroy_at(&res->V_data);
            __List_allocator.deallocate(res, 1);

            return static_cast<iterator>(static_cast<node_T *>(__next_node));
        }

        iterator erase(iterator __first, iterator __last)
        {

            if (__first.V_node == __last.V_node) {
                return erase(static_cast<iterator>(
                    static_cast<node_T *>(__first.V_node)));
            }

            node_T *res = static_cast<node_T *>(__first.V_node);
            while (res != __last.V_node) {
                node_T *k = res;
                res = static_cast<node_T *>(res->V_next);
                erase(static_cast<iterator>(k));
            }

            return static_cast<iterator>(res);
        }

        bool empty() noexcept { return (V_node->V_next == V_node); }

        bool empty() const noexcept { return (V_node->V_next == V_node); }

        size_type size() noexcept
        {
            size_type _n = 0;
            for (iterator __iter = begin(); __iter != end(); ++__iter, ++_n)
                ;
            return _n;
        }

        void swap(list &__res)
        {
            // TODO
            ::std::swap(V_node, __res.V_node);
        }

        size_type max_size() const noexcept { return size_type(-1); }

        reference front() noexcept { return *begin(); }
        const_reference front() const noexcept { return *begin(); }

        reference back() noexcept { return *(--end()); }
        const_reference back() const noexcept { return *(--end()); }

        iterator begin() noexcept
        {
            return static_cast<iterator>(static_cast<node_T *>(V_node->V_next));
        }
        const_iterator begin() const noexcept
        {
            return static_cast<const_iterator>(
                static_cast<node_T *>(V_node->V_next));
        }

        iterator end() noexcept { return static_cast<iterator>(V_node); }
        const_iterator end() const noexcept
        {
            return static_cast<const_iterator>(V_node);
        }

        iterator push_back(const _Ty &__val) noexcept
        {
            return insert(end(), __val);
        }

        const_iterator push_back(const _Ty &__val) const noexcept
        {
            return static_cast<const_iterator>(insert(end(), __val));
        }

        iterator push_front(const _Ty &__val) noexcept
        {
            return insert(begin(), __val);
        }

        const_iterator push_front(const _Ty &__val) const noexcept
        {
            return static_cast<const_iterator>(insert(begin(), __val));
        }

        iterator pop_back() noexcept(noexcept(!empty()))
        {
            return erase(--end());
        }
        const_iterator pop_back() const noexcept(noexcept(!empty()))
        {
            return static_cast<const_iterator>(erase(end()));
        }

        iterator pop_front() noexcept(noexcept(!empty()))
        {
            return erase(begin());
        }

        const_iterator pop_front() const noexcept(!empty())
        {
            {
                return static_cast<const_iterator>(erase(begin()));
            }
        }

        void remove(const value_type &__v) noexcept
        {
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

        // remove_if TODO

        void unique() noexcept
        {
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

        void transfer(iterator __pos, iterator __first,
                      iterator __last) noexcept
        {
            if (__pos != __last) {
                // Remove [first, last) from its old position.
                __last.V_node->V_prev->V_next = __pos.V_node;
                __first.V_node->V_prev->V_next = __last.V_node;
                __pos.V_node->V_prev->V_next = __first.V_node;

                // Splice [first, last) into its new position.
                __list_node_base *__tmp = __pos.V_node->V_prev;
                __pos.V_node->V_prev = __last.V_node->V_prev;
                __last.V_node->V_prev = __first.V_node->V_prev;
                __first.V_node->V_prev = __tmp;
            }
        }

        void splice(iterator __pos, list<_Ty> &__rhs) noexcept
        {
            if (!__rhs.empty()) {
                transfer(__pos, __rhs.begin(), __rhs.end());
            }
        }

        void merge(list<_Ty> &__res) noexcept
        {
            if (empty()) {
                for (iterator it = __res.begin(); it != __res.end(); ++it)
                    push_back(*it);
                return;
            }

            if (!__res.empty()) {
                iterator it = begin();
                iterator eit = __res.begin();

                iterator rpos = __res.begin();
                for (; eit != __res.end();) {
                    if (*eit >= *it && it != end())
                        ++it;
                    else {
                        insert(it, *eit);
                        ++eit;
                    }
                }
            }
        }

        void reverse() noexcept
        {
            if (!empty()) {
                size_type i = size() - (size() / 2);
                iterator rit = end();
                for (iterator it = begin(); i != 0; --i, ++it) {
                    --rit;
                    swap_ptr(it, rit);
                }
            }
        }

        void resize(const size_type &__n, const_reference __v)
        {
            if (size() >= __n)
                for (auto i = size() - __n; i != 0; --i)
                    pop_back();
            else
                for (auto i = size() - __n; i != 0; ++i)
                    push_back(__v);
        }

        void resize(size_type __n) { resize(__n, _Ty()); }

        void assign(size_type count, const_reference value)
            noexcept(noexcept(!empty()))
        {
            iterator it = begin();

            if (size() >= count) {
                for (; count != 0; --count)
                    *(it++) = value;

                for (auto it = size() - count; it != 0; --it)
                    pop_back();
            }
            else {
                for (; it != end(); --count)
                    *(it++) = value;

                for (; count != 0; --count)
                    push_back(value);
            }
        }

        template <class InputIt> void assign(InputIt first, InputIt last)
        {
            iterator it = begin();
            size_type d = distance(first, last);

            if (d >= size()) {
                while (it != end())
                    *(it++) = *(first++);
                while (first != last)
                    push_back(*first++);
            }
            else {
                while (first != last)
                    *(it++) = *(first++);
                erase(it, --end());
            }
        }

        void assign(::std::initializer_list<_Ty> ilist)
        {
            assign(ilist.begin(), ilist.end());
        }

        void sort() noexcept
        {
            node_T *it = V_node;
            while (it->V_next != V_node) {
                it = static_cast<node_T *>(it->V_next);
                node_T *__subit = static_cast<node_T *>(it->V_next);
                while (__subit != V_node) {
                    if (it->V_data > __subit->V_data) {
                        value_type __tmp = it->V_data;
                        it->V_data = __subit->V_data;
                        __subit->V_data = __tmp;
                    }
                    __subit = static_cast<node_T *>(__subit->V_next);
                }
            }
        }

        list &operator=(const list &__res)
        {
            if (&__res == this) {
                return *this;
            }

            clear();
            V_uninitializer_copy(__res);

            return *this;
        }

        list &&operator=(const list &&__res)
        {
            if (&__res == this) {
                return Fiop::move(*this);
            }

            clear();
            V_node = __res.V_node;
            __res.V_node->V_prev = nullptr;
            __res.V_node->V_next = nullptr;

            return Fiop::move(*this);
        }

        bool operator!=(const list &__res) noexcept
        {
            return !(__res.V_node == V_node);
        }

        bool operator==(const list &__res) noexcept
        {
            return (__res.V_node == V_node);
        }
    };
}; // namespace iop

#endif //  _IOP_LIST_DEF
