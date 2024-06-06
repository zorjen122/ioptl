/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _IOP_STRING_DEF
#define _IOP_STRING_DEF

#include "../allocatorF.h"

#include "../iteratorF.h"

#include <bits/char_traits.h>
#include "../traits/allocator_traitsF.h"
#include "../util/iterator_baseF.h"

#include <initializer_list>

#include <ostream>

/*
    The realization of this string has not been optimized
        in any container (such as SSO, ESO, etc.),
        but is only a basic function realization,
        so use it with caution.
*/

namespace iop {

    // Eager copy string.
    template <class CharT, class Traits = ::std::char_traits<CharT>,
              class Allocator = allocator<CharT>>
    class basic_string
    {
        using allocator_type = Allocator;
        using allocator_traits_type = allocator_traits<allocator_type>;

      public:
        using traits_type = Traits;
        using value_type = CharT;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = iter::reverse_iterator<iterator>;
        using const_reverse_iterator = iter::reverse_iterator<const_iterator>;

      protected:
        size_type _Vcap;
        pointer _Vstring;
        [[no_unique_address]] traits_type _M_data_traits;
        [[no_unique_address]] allocator_type _V_allocator;

      public:
        explicit basic_string()
        {
            _Vstring = _V_allocator.allocate(1);
            *_Vstring = '\0';
            _Vcap = 0;
        }

        basic_string(const ::std::initializer_list<const_pointer> &__res)
        {
            reall_container_comp(__res);
        }

        basic_string(const ::std::initializer_list<value_type> &__res)
        {
            _Vcap = __res.size();
            _Vstring = _V_allocator.allocate(_Vcap + 1);

            int k = 0;
            for (auto it : __res)
                *(_Vstring + (k++)) = it;

            _Vstring + _Vcap + 1 = '\0';
        }

        basic_string(basic_string &__rhs)
        {
            _Vcap = __rhs._Vcap;
            _Vstring = _V_allocator.allocate(_Vcap + 1);

            iterator iter = __rhs.begin();
            uninitialized_copy(iter, __rhs.end(), _Vstring);
            *(_Vstring + _Vcap + 1) = '\0';
        }

        basic_string(basic_string &&__rhs)
        {
            _Vstring = __rhs._Vstring;
            _M_data_traits = __rhs._M_data_traits;
            _Vcap = __rhs._Vcap;
            __rhs._Vstring = nullptr;
            __rhs._Vcap = 0;
        }

        basic_string(const_pointer __val)
        {
            _Vcap = _M_data_traits.length(__val);

            _Vstring = _V_allocator.allocate(_Vcap + 1);

            try {
                for (size_type i = 0; i < _Vcap; ++i)
                    *(_Vstring + i) = __val[i];
            }
            catch (...) {
                throw;
            }

            *(_Vstring + _Vcap + 1) = '\0';
        }

        basic_string(const size_type &__sz)
        {
            _Vcap = __sz;
            _Vstring = _V_allocator.allocate(_Vcap + 1);
            *(_Vstring + _Vcap + 1) = '\0';
        }

        basic_string(const_pointer __s, size_type __len)
        {
            _Vcap = __len;

            _Vstring = _V_allocator.allocate(_Vcap + 1);
            const_pointer __iter = __s;

            uninitialized_copy(__iter, __s + _Vcap, _Vstring);
            *(_Vstring + _Vcap + 1) = '\0';
        }

      protected:
        void reall_container_comp(
            const ::std::initializer_list<const_pointer> &__cont,
            bool reall_io = true)
        {
            if (reall_io) {
                _Vcap = 0;
                for (auto it = __cont.begin(); it != __cont.end(); ++it)
                    _Vcap += _M_data_traits.length(*it);
                _Vstring = _V_allocator.allocate(_Vcap + 1);
            }

            size_type idx = 0, n = 0;
            for (auto it = __cont.begin(); it != __cont.end(); ++it) {
                n = _M_data_traits.length(*it);
                try {
                    for (size_type k = 0; k < n; ++k && ++idx)
                        *(_Vstring + idx) = (*it)[k];
                }
                catch (...) {
                    //...
                    throw;
                }
            }
            *(_Vstring + _Vcap + 1) = '\0';
        }

      public:
        basic_string &insert(iterator __pos, const_pointer __val)
        {

            size_type psz = _M_data_traits.length(__val);

            pointer res = _V_allocator.allocate(_Vcap + psz + 1);

            iterator iter = begin();
            const_pointer piter = __val;

            iterator start_pos = uninitialized_copy(iter, __pos, res);

            iterator add_pos =
                uninitialized_copy(piter, (piter + psz), start_pos);

            uninitialized_copy(__pos, end(), add_pos);

            iop::destroy_at(this->begin(), this->end());
            _V_allocator.deallocate(_Vstring);

            _Vstring = res;
            _Vcap += psz;

            return *this;
        }

        // basic_string &insert(const_iterator __pos, const_pointer __val)
        // { return insert(__pos, __val); }


        basic_string &erase(size_type index, int count = -1)
        {
            assert(index >= 0);
            if (index < _Vcap) {
                size_type sz = (count == -1) ? index : _Vcap - count;
                pointer new_s = _V_allocator.allocate(sz + 1);

                iterator iter = begin();

                uninitialized_copy(iter, iter + index + 1, new_s);
                if (count != -1)
                    uninitialized_copy(iter + index + 1 + count, end(),
                                       new_s + (index + 1));
                *(new_s + sz) = '\0';

                iop::destroy_at(begin(), end());
                _V_allocator.deallocate(_Vstring);
                _Vstring = new_s;
                _Vcap = sz;
            }
            else {
                iop::destroy_at(begin(), end());
                _V_allocator.deallocate(_Vstring);
                _Vcap = 0;
            }
            return *this;
        }

        constexpr iterator erase(iterator __first, iterator __last)
        {
            auto diff = static_cast<size_type>(__last - __first);
            auto start_n = iop::distance(begin(), __first);

            erase(start_n, diff);
            return begin() + start_n;
        }

        constexpr iterator erase(iterator __pos)
        {
            auto start_n = iop::distance(begin(), __pos);
            erase(start_n - 1, 1);
            return begin() + start_n;
        }
        constexpr const_iterator erase(const_iterator __first, const_iterator __last)
        { return erase(__first, __last); }
        constexpr const_iterator erase(const_iterator __pos)
        { return erase(__pos); }


        basic_string &append(int setp, const_pointer val)
        {
            if (setp == 1)
                return append(val);

            int len = _M_data_traits.length(val);
            int push_len = setp * len;
            pointer new_str = _V_allocator.allocate(_Vcap + push_len + 1);

            iterator iter = begin();
            uninitialized_copy(iter, end(), new_str);

            iterator new_iter = new_str + _Vcap;
            for (int i = 0; i != setp; ++i) {
                for (int i = 0; i != len; ++i)
                    *(new_iter++) = *(val + i);
            }

            *(++new_iter) = '\0';
            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);

            _Vstring = new_str;
            _Vcap += push_len;

            return *this;
        }

        basic_string &append(int setp, value_type val)
        {
            if (setp == 1)
                return append(val);

            pointer new_str = _V_allocator.allocate(_Vcap + setp + 1);

            iterator iter = begin();
            uninitialized_copy(iter, end(), new_str);

            iterator new_iter = new_str + _Vcap;
            for (int i = 0; i != setp; ++i)
                *(new_iter++) = val;

            *(++new_iter) = '\0';
            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);

            _Vstring = new_str;
            _Vcap += setp;
            return *this;
        }

        basic_string &append(const basic_string &str, int pos, int count = -1)
        {
            assert(count != -1 || count < 0);
            int len = (count == -1) ? pos : count;
            pointer new_str = _V_allocator.allocate(_Vcap + len + 1);
            iterator iter = begin();
            uninitialized_copy(iter, end(), new_str);

            if (count == -1)
                uninitialized_copy(str.data(), str.data() + pos + 1,
                                   new_str + _Vcap);
            else
                uninitialized_copy(str.data() + pos,
                                   str.data() + pos + count + 1,
                                   new_str + _Vcap);
            *(new_str + _Vcap + len) = '\0';

            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);
            _Vstring = new_str;
            _Vcap += len;

            return *this;
        }

        basic_string &append(const_pointer first, const_pointer last)
        {

            auto sz = iop::distance(first, last);
            pointer new_str = _V_allocator.allocate(_Vcap + sz + 1);
            iterator iter = begin();
            uninitialized_copy(iter, end(), new_str);
            uninitialized_copy(first, last, new_str + _Vcap);
            iop::destroy_at(begin(), end());

            *(new_str + _Vcap + sz) = '\0';

            _V_allocator.deallocate(_Vstring);
            _Vstring = new_str;
            _Vcap += sz;

            return *this;
        }

        basic_string &append(const_pointer __val)
        {

            insert(this->end() - 1, __val);

            return *this;
        }

        basic_string &append(const basic_string &str)
        {

            append(str, str.size());

            return *this;
        }

        basic_string &append(CharT __val)
        {

            append({__val});

            return *this;
        }

        basic_string &append(const ::std::initializer_list<value_type> &ilist)
        {
            auto sz = ilist.size();
            if (sz == 0)
                return *this;

            pointer new_str = _V_allocator.allocate(_Vcap + sz + 1);
            iterator iter = begin();
            iterator new_iter = new_str;
            uninitialized_copy(iter, end(), new_iter);
            new_iter += _Vcap;
            try {
                for (auto &&i : ilist)
                    *(new_iter++) = i;
            }
            catch (...) {
                throw;
            }

            *(new_str + _Vcap + sz) = '\0';
            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);
            _Vstring = new_str;
            _Vcap += sz;

            return *this;
        }

        basic_string &push_back(pointer __val)
        {

            insert(this->begin(), __val);

            return *this;
        }

      public:
        iterator find(iterator __s)
        {
            auto i = begin();
            while (i != __s)
                ++i;
            return i;
        }
        const_iterator find(const_iterator __s)
        { return find(__s); }


        size_type find(const value_type &__s)
        {
            int idx = 0;
            auto it = begin();
            for (; it != end() && *it != __s; ++it)
                ++idx;

            return idx;
        }

        constexpr basic_string &assign(const_pointer __s)
        {
            auto sz = _M_data_traits.length(__s);

            if constexpr (mpls::is_basic_char<value_type>::value) {
                if (sz == _Vcap) {
                    uninitialized_copy(__s, __s + sz, _Vstring);
                    *(_Vstring + sz) = '\0';
                    return *this;
                }
            }

            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);
            _V_allocator.allocate(sz + 1);
            uninitialized_copy(__s, __s + sz, _Vstring);
            uninitialized_copy(__s, __s + sz, _Vstring);
            _Vcap = sz;

            return *this;
        }

        constexpr basic_string &assign(const basic_string &str)
        {
            auto sz = str.size();

            if constexpr (mpls::is_basic_char<value_type>::value) {
                if (sz == _Vcap) {
                    uninitialized_copy(str.begin(), str.end(), _Vstring);
                    *(_Vstring + sz) = '\0';
                    return *this;
                }
            }

            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);
            _V_allocator.allocate(sz + 1);
            uninitialized_copy(str.begin(), str.end(), _Vstring);
            *(_Vstring + sz) = '\0';
            _Vcap = sz;

            return *this;
        }

        constexpr basic_string &
        assign(::std::initializer_list<const_pointer> &ilist)
        {
            if constexpr (mpls::is_basic_char<value_type>::value) {
                if (ilist.size() == _Vcap) {
                    reall_container_comp(ilist, false);
                    return *this;
                }
            }

            iop::destroy_at(begin(), end());
            _V_allocator.deallocate(_Vstring);
            reall_container_comp(ilist);
            return *this;
        }

        constexpr void reserve(size_type new_cap)
        {
            if (new_cap < size())
                shrink_to_fit();
            else
                copys(new_cap, *this);
        }

        const basic_string &copys(size_type new_cap, basic_string &res)
        {
            pointer new_str = _V_allocator.allocate(new_cap + 1);
            iterator iter = res.begin();
            uninitialized_copy(iter, res.end(), new_str);
            *(new_str + new_cap) = '\0';

            iop::destroy_at(res.begin(), res.end());
            _V_allocator.deallocate(res._Vstring);
            res._Vstring = new_str;
            res._Vcap = new_cap;

            return res;
        }

        constexpr void shrink_to_fit()
        {
            if (size() != capacity())
                copys(size(), *this);
        }

      public:
        // C++11 after
        constexpr pointer c_str() noexcept { return _Vstring; }
        constexpr const_pointer c_str() const noexcept { return _Vstring; }
        constexpr const_pointer data() noexcept { return _Vstring; }
        constexpr pointer data() const noexcept { return _Vstring; }

        iterator begin() { return static_cast<iterator>(_Vstring); }
        const_iterator begin() const
        {
            return static_cast<const_iterator>(_Vstring);
        }
        iterator end() { return static_cast<iterator>(_Vstring + _Vcap + 1); }
        const_iterator end() const
        {
            return static_cast<const_iterator>(_Vstring + _Vcap + 1);
        }
        constexpr size_type max_size() noexcept { return size_type(-1); }

        size_type size() { return _Vcap; }
        const size_type size() const { return _Vcap; }

        constexpr size_type capacity() noexcept { return _Vcap + 1; }

      public:
        basic_string &operator=(const basic_string &__rhs)
        {

            iop::destroy_at(begin(), end());
            _Vcap = __rhs._Vcap;
            _Vstring = _V_allocator.allocate(_Vcap + 1);

            iterator iter = __rhs.begin();
            uninitialized_copy(iter, __rhs.end(), _Vstring);

            return *this;
        }

        reference operator[](size_type __sz) { return *(begin() + __sz); }

        const_reference operator[](size_type __sz) const
        {
            return *(begin() + __sz);
        }

        friend ::std::ostream &operator<<(::std::ostream &os,
                                          const basic_string &__res)
        {
            for (size_type i = 0; i != __res.size(); ++i)
                os << __res[i];
            return os;
        }

        constexpr bool operator==(const basic_string &__res)
        {
            return (_Vstring == __res._Vstring);
        }

        constexpr basic_string &operator+=(const basic_string &__res)
        {
            if (__res.size() == 0)
                return *this;
            return append(__res);
        }

        constexpr basic_string &operator+=(const_pointer __res)
        {
            if (__res.size() == 0)
                return *this;
            return append(__res);
        }
    };

    using string = basic_string<char>;

    // operator++
    template <class CharT, class Traits, class Allocator>
    auto operator+(const basic_string<CharT, Traits, Allocator> &__lhs,
                   const basic_string<CharT, Traits, Allocator> &__rhs)
    {

        using return_type = basic_string<CharT, Traits, Allocator>;
        return_type new_str({__lhs.data(), __rhs.data()});

        return new_str;
    }

    template <class CharT, class Traits, class Allocator>
    auto operator+(const basic_string<CharT, Traits, Allocator> &__lhs,
                   const CharT *__rhs)
    {

        using return_type = basic_string<CharT, Traits, Allocator>;
        return_type new_str(__lhs.data());
        new_str.append(__rhs);

        return new_str;
    }

    template <class CharT, class Traits, class Allocator>
    auto operator+(basic_string<CharT, Traits, Allocator> &__lhs, CharT __rhs)
    {

        using return_type = basic_string<CharT, Traits, Allocator>;
        return_type new_str = __lhs;

        new_str.append(__rhs);

        return new_str;
    }

    // right
    template <class CharT, class Traits, class Allocator>
    auto operator+(const CharT *__rhs,
                   const basic_string<CharT, Traits, Allocator> &__lhs)
    {

        using return_type = basic_string<CharT, Traits, Allocator>;
        return_type new_str(__rhs);
        new_str.append(__lhs);

        return new_str;
    }

    template <class CharT, class Traits, class Allocator>
    auto operator+(CharT __rhs, basic_string<CharT, Traits, Allocator> &__lhs)
    {

        using return_type = basic_string<CharT, Traits, Allocator>;
        return_type new_str;
        new_str.append(__rhs);

        new_str.append(__lhs);

        return new_str;
    }

}; // namespace iop

#endif // _IOP_STRING_DEF