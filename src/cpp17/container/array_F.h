/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#define _ARRAY_IOP_DEFINE_
#ifdef _ARRAY_IOP_DEFINE_

#include "../algobase_F.h"
#include "../allocator_F.h"
#include "../allocator_traits_F.h"
#include "../utilitys.h"

#include "../func_exception.h"
#include "../iterator_F.h"

#include <initializer_list>

#define _OPERATOR_THAN(op)                                                     \
    IOP_CONSTEXPR_CXX17 bool operator op(array &__rhs)                         \
    {                                                                          \
        value_type __iter = 0;                                                 \
        for (auto __it = __rhs.begin(); __it != __rhs.end();                   \
             ++__it, ++_Varray) {                                              \
            __iter += ((*__it) - (*__it));                                     \
        }                                                                      \
        return (__iter op 0);                                                  \
    }

namespace iop {

    template <class _Ty, size_t _N> class array
    {
      public:
        using allocator_type = iop::allocator<_Ty>;
        using allocator_traits_type = iop::allocator_traits<allocator_type>;

        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = pointer;
        using const_iterator = const iterator;
        using reverse_iterator = iop::iter::reverse_iterator<iterator>;
        using const_reverse_iterator =
            iop::iter::reverse_iterator<const_iterator>;

      protected:
        pointer _Varray;
        size_type _len;
        [[no_unique_address]] allocator_type __allocator;

      public:
        explicit array() : _len(_N) { _Varray = __allocator.allocate(_N + 1); }

        ~array() { iop::destroy_at(begin(), end()); }

        explicit array(const array &__rhs) : _len(_N)
        {
            _Varray = __allocator.allocate(_N);

            uninitialized_copy(__rhs.begin(), __rhs.end(), _Varray);
        }

        array(const ::std::initializer_list<_Ty> &__rhs) : _len(_N)
        {
            assert(__rhs.size() == _N);

            _Varray = __allocator.allocate(_len);

            uninitialized_copy(__rhs.begin(), __rhs.end(), _Varray);
        }

        IOP_CONSTEXPR_CXX17 iterator end() NOEXCEPT
        {
            return static_cast<iterator>(_Varray + (_len));
        }

        IOP_CONSTEXPR_CXX17 iterator begin() NOEXCEPT
        {
            return static_cast<iterator>(_Varray);
        }

        IOP_CONSTEXPR_CXX17 const_iterator end() const NOEXCEPT
        {
            return static_cast<const_iterator>(_Varray + (_len));
        }

        IOP_CONSTEXPR_CXX17 const_iterator begin() const NOEXCEPT
        {
            return static_cast<const_iterator>(_Varray);
        }

        IOP_CONSTEXPR_CXX17 const_iterator cend() const NOEXCEPT
        {
            return static_cast<const_iterator>(_Varray + (_len));
        }
        IOP_CONSTEXPR_CXX17 const_iterator cbegin() const NOEXCEPT
        {
            return static_cast<const_iterator>(_Varray);
        }

        IOP_CONSTEXPR_CXX17 reference back() NOEXCEPT { return *(end() - 1); }
        IOP_CONSTEXPR_CXX17 reference front() NOEXCEPT { return *(begin()); }

        reference at(const size_type &__v)
        {
            if (__v >= size())
                throw bad_range();
            else
                return _Varray + __v + 1;
        }

        IOP_CONSTEXPR_CXX17 void fill(const value_type &__val) NOEXCEPT
        {
            pointer __res = _Varray;
            for (; __res != end(); ++__res) {
                *__res = __val;
            }
        }

        IOP_CONSTEXPR_CXX17 void swap(array &__rhs)
            NOEXCEPT(NOEXCEPT(::std::swap(::std::declval<reference>(),
                                          ::std::declval<reference>())))
        {
            iterator it = this->begin();
            iterator rit = __rhs.begin();

            while (it != this->end()) {
                swap_ptr(&*(it++), &*(rit++));
            }
        }

        IOP_CONSTEXPR_CXX17 const_reference at(size_type __idx)
        {
            if (__idx >= size())
                throw;
            else
                return *(_Varray + __idx + 1);
        }

        IOP_CONSTEXPR_CXX17 bool empty() NOEXCEPT
        {
            return (_Varray == nullptr);
        }
        IOP_CONSTEXPR_CXX17 const_reference data() NOEXCEPT { return *_Varray; }

        IOP_CONSTEXPR_CXX17 size_type size() NOEXCEPT { return _len; }
        IOP_CONSTEXPR_CXX17 const size_type size() const NOEXCEPT
        {
            return _len;
        }
        IOP_CONSTEXPR_CXX17 size_type max_size() NOEXCEPT
        {
            return size_type(-1);
        }
        IOP_CONSTEXPR_CXX17 const size_type max_size() const NOEXCEPT
        {
            return size_type(-1);
        }

      public:
        IOP_CONSTEXPR_CXX17 bool operator>(array &__rhs)
        {
            value_type __iter = 0;
            pointer __res = __rhs.data();
            for (auto __it = begin(); __it != end(); ++__it, ++__res) {
                __iter += ((*__it) - (*__res));
            }
            return (__iter > 0);
        }

        _OPERATOR_THAN(<);
        _OPERATOR_THAN(<=);
        _OPERATOR_THAN(>=);

        IOP_CONSTEXPR_CXX17 reference operator[](size_type __idx) NOEXCEPT
        {
            return *(_Varray + __idx);
        }

        IOP_CONSTEXPR_CXX17 const_reference
        operator[](size_type __idx) const NOEXCEPT
        {
            return *(_Varray + __idx);
        }

        IOP_CONSTEXPR_CXX17 bool operator==(array &__rhs) NOEXCEPT
        {
            return (__rhs == *this);
        }
        IOP_CONSTEXPR_CXX17 bool operator!=(array &__rhs) NOEXCEPT
        {
            return (__rhs != *this);
        }
    };

    template <size_t _I, class _T, size_t _N>
    IOP_CONSTEXPR_CXX17 _T &get(const iop::array<_T, _N> &__rhs) NOEXCEPT
    {
        static_assert(_I < _N, "Error: more index than cap size");
        return const_cast<_T &>(__rhs[_I]);
    }

    template <size_t _I, class _T, size_t _N>
    IOP_CONSTEXPR_CXX17 _T &&get(iop::array<_T, _N> &&__rhs) NOEXCEPT
    {
        static_assert(_I < _N, "Error: more index than cap size");
        return Fiop::move(__rhs[_I]);
    }

}; // namespace iop

#endif //* _ARRAY_IOP_DEFINE_