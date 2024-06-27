/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef IOP_ARRAY_DEF
#define IOP_ARRAY_DEF

#include "../algo/algobaseF.h"
#include "../allocatorF.h"
#include "../traits/allocator_traitsF.h"
#include "../util/utilityF.h"

#include "../iteratorF.h"
#include "../util/exceptionF.h"

#include <initializer_list> // std::initializer_list

#define _OPERATOR_THAN(op)                                                     \
    constexpr bool operator op(array &__rhs)                                   \
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
        using const_iterator = const_pointer;
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

        constexpr iterator end() noexcept
        {
            return static_cast<iterator>(_Varray + (_len));
        }

        constexpr iterator begin() noexcept
        {
            return static_cast<iterator>(_Varray);
        }

        constexpr const_iterator end() const noexcept
        {
            return static_cast<const_iterator>(_Varray + (_len));
        }

        constexpr const_iterator begin() const noexcept
        {
            return static_cast<const_iterator>(_Varray);
        }

        constexpr const_iterator cend() const noexcept
        {
            return static_cast<const_iterator>(_Varray + (_len));
        }
        constexpr const_iterator cbegin() const noexcept
        {
            return static_cast<const_iterator>(_Varray);
        }

        constexpr reference back() noexcept { return *(end() - 1); }
        constexpr reference front() noexcept { return *(begin()); }

        reference at(const size_type &__v)
        {
            if (__v >= size())
                throw bad_range();
            else
                return _Varray + __v + 1;
        }

        constexpr void fill(const value_type &__val) noexcept
        {
            pointer __res = _Varray;
            for (; __res != end(); ++__res) {
                *__res = __val;
            }
        }

        constexpr void swap(array &__rhs) noexcept(noexcept(::std::swap(
            ::std::declval<reference>(), ::std::declval<reference>())))
        {
            iterator it = this->begin();
            iterator rit = __rhs.begin();

            while (it != this->end()) {
                swap_ptr(&*(it++), &*(rit++));
            }
        }

        constexpr const_reference at(size_type __idx)
        {
            if (__idx >= size())
                throw;
            else
                return *(_Varray + __idx + 1);
        }

        constexpr bool empty() noexcept { return (_Varray == nullptr); }
        constexpr const_pointer data() noexcept { return _Varray; }

        constexpr size_type size() noexcept { return _len; }
        constexpr const size_type size() const noexcept { return _len; }
        constexpr size_type max_size() noexcept { return size_type(-1); }
        constexpr const size_type max_size() const noexcept
        {
            return size_type(-1);
        }

      public:
        constexpr bool operator>(array &__rhs)
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

        constexpr reference operator[](size_type __idx) noexcept
        {
            return *(_Varray + __idx);
        }

        constexpr const_reference operator[](size_type __idx) const noexcept
        {
            return *(_Varray + __idx);
        }

        constexpr bool operator==(array &__rhs) noexcept
        {
            return (__rhs == *this);
        }
        constexpr bool operator!=(array &__rhs) noexcept
        {
            return (__rhs != *this);
        }
    };

    template <size_t _I, class _T, size_t _N>
    constexpr _T &get(const iop::array<_T, _N> &__rhs) noexcept
    {
        static_assert(_I < _N, "Error: more index than cap size");
        return const_cast<_T &>(__rhs[_I]);
    }

    template <size_t _I, class _T, size_t _N>
    constexpr _T &&get(iop::array<_T, _N> &&__rhs) noexcept
    {
        static_assert(_I < _N, "Error: more index than cap size");
        return Fiop::move(__rhs[_I]);
    }

}; // namespace iop

#endif //* IOP_ARRAY_DEF