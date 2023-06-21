/*
 * Copyright (c) 1997
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

#define _ITERATOR_STL_
#ifdef _ITERATOR_STL_

#include "iterator_base_stl.h"
#include "config_def.h"

/*
 * Output_iterator_tag:
 *      back_inserter_iterator, front_insert_iterator
 *      insert_iterator   iterator_category
 */

#define CCC_FUNCTIONAL_ITERATOR_OF_CONTAIER(name,iterator_tag,function)             \
template<class _Contaier>                                                           \
class name {                                                                        \
protected:                                                                          \
    _Contaier* contaier;                                                            \
public:                                                                             \
    using contaier_type = _Contaier;                                                \
    using iteartor_category = iterator_tag;                                         \
    using difference_type = void;                                                   \
    using value_type = void;                                                        \
    using pointer = void;                                                           \
    using reference = void;                                                         \
                                                                                    \
    explicit name<_Contaier>(const _Contaier& __res)                                \
     noexcept : contaier(&__res) {}                                                 \
                                                                                    \
    name<_Contaier>&                                                                \
    operator=(const typename _Contaier::value_type& __res) {                        \
        contaier->function(__res);                                                  \
        return *this;                                                               \
    }                                                                               \
                                                                                    \
    constexpr _Contaier* Base() const noexcept { return contaier; }                 \
                                                                                    \
    name<_Contaier>& operator*() noexcept { return *this; }                         \
    name<_Contaier>& operator++() noexcept { return *this; }                        \
    name<_Contaier>& operator++(int) noexcept { return *this; }                     \
};                                                                                  \


#define CCC_PARAMETER_FUN_OF_ITERATOR_CONTAIER(iterator_type_tag,                   \
                                           functional_iterator)                     \
template<class _General_Ty>                                                         \
constexpr iterator_type_tag                                                         \
    iterator_category(const functional_iterator<_General_Ty>&) noexcept {           \
        return iterator_type_tag();                                                 \
}                                                                                   \


#define CCC_PARAMETER_FUN_OF_ITERATOR(iterator_type_tag,                            \
                                  iterator_type_tag_cond,functional_iterator)       \
template <iterator_type_tag_cond _Iterator, class _Ty, class _Reference,            \
    class _Distance>                                                                \
constexpr _Iterator                                                                 \
iterator_category(const functional_iterator<_Iterator, _Ty, _Reference, _Distance>&)\
noexcept {                                                                          \
     return _Iterator();                                                            \
}                                                                                   \

namespace CCC_STL {
    namespace Iterator_STL {


        //* use explicit for effect of the copy construct with type parameter change
        CCC_FUNCTIONAL_ITERATOR_OF_CONTAIER(back_insert_iterator, output_iterator_tag, push_back);
        CCC_FUNCTIONAL_ITERATOR_OF_CONTAIER(front_insert_iterator, output_iterator_tag, push_front);


        template<class _Contaier>
        class insert_iterator {
        protected:
            _Contaier* contaier;
            typename _Contaier::iterator iter;

        public:
            using contaier_type = _Contaier;
            using iteartor_category = output_iterator_tag;
            using difference_type = void;
            using value_type = void;
            using pointer = void;
            using reference = void;

            CCC_CONSTEXPR explicit insert_iterator
            (const contaier_type& __res, typename _Contaier::iterator __iter)
                CCC_NOEXCEPT: contaier(&__res), iter(__iter) {}


            CCC_CONSTEXPR insert_iterator<contaier_type>& operator=
                (const typename contaier_type::value_type& __res) {
                iter = contaier->insert(iter, __res);
                ++iter;                                            //:  ?
                return *this;
            }

            CCC_CONSTEXPR insert_iterator<contaier_type>& operator*() CCC_NOEXCEPT { return *this; }
            CCC_CONSTEXPR insert_iterator<contaier_type>& operator++() CCC_NOEXCEPT { return *this; }
            CCC_CONSTEXPR insert_iterator<contaier_type>& operator++(int) CCC_NOEXCEPT { return *this; }
        };


        template<class _Iterator,
            class _Ty, class _Reference = _Ty&, class _Distance = ::std::ptrdiff_t>
        class reverse_history_bidirectional_iterator {
        protected:
            _Iterator current;

        public:
            using iteartor_category = bidirectional_iterator_tag;
            using value_type = _Ty;
            using pointer = _Ty*;
            using reference = _Reference;
            using difference_type = _Distance;
            using Self = reverse_history_bidirectional_iterator<_Iterator,
                _Ty, _Reference, _Distance>;

            CCC_CONSTEXPR reverse_history_bidirectional_iterator() CCC_NOEXCEPT {}
            CCC_CONSTEXPR explicit reverse_history_bidirectional_iterator(_Iterator __res)
                CCC_NOEXCEPT: current(__res) {}

            CCC_CONSTEXPR _Iterator Base()const CCC_NOEXCEPT { return current; }

            CCC_CONSTEXPR reference operator*()const CCC_NOEXCEPT {
                _Iterator inner = current;
                return *--inner;
            }

            CCC_CONSTEXPR pointer operator->()const CCC_NOEXCEPT { return &(operator*()); }

            CCC_CONSTEXPR Self& operator++() CCC_NOEXCEPT {
                --current;
                return *this;
            }

            CCC_CONSTEXPR Self operator++(int) CCC_NOEXCEPT {
                _Iterator _inner = current;
                --current;
                return _inner;
            }

            CCC_CONSTEXPR Self operator--() CCC_NOEXCEPT {
                ++current;
                return *this;
            }
            CCC_CONSTEXPR Self& operator--(int) CCC_NOEXCEPT {
                _Iterator _inner = current;
                ++current;
                return _inner;
            }
        };


        //* use general-name -> since the class for this iterator is a complete(++-- +- +-=)
        template<class _Iterator,
            class _Ty, class _Reference = _Ty&, class _Distance = ::std::ptrdiff_t>
        class reverse_history_iterator {
        protected:
            _Iterator current;

        public:
            using iteartor_category = random_access_iterator_tag;
            using value_type = _Ty;
            using pointer = _Ty*;
            using reference = _Reference;
            using difference_type = _Distance;
            using Self = reverse_history_iterator<_Iterator,
                _Ty, _Reference, _Distance>;

            CCC_CONSTEXPR reverse_history_iterator() CCC_NOEXCEPT {}
            CCC_CONSTEXPR explicit reverse_history_iterator(_Iterator __res)
                CCC_NOEXCEPT: current(__res) {}

            CCC_CONSTEXPR _Iterator Base()const CCC_NOEXCEPT { return current; }

            reference operator*()const CCC_NOEXCEPT { return *(--current); }
            pointer operator->()const CCC_NOEXCEPT { return &(operator*()); }

            CCC_CONSTEXPR Self& operator++() CCC_NOEXCEPT {
                --current;
                return *this;
            }

            CCC_CONSTEXPR Self operator++(int) CCC_NOEXCEPT {
                _Iterator _inner = current;
                --current;
                return _inner;
            }

            CCC_CONSTEXPR Self& operator--() CCC_NOEXCEPT {
                ++current;
                return *this;
            }

            CCC_CONSTEXPR Self operator--(int) CCC_NOEXCEPT {
                _Iterator _inner = current;
                ++current;
                return _inner;
            }

            CCC_CONSTEXPR Self& operator+=(_Distance __n) CCC_NOEXCEPT {
                current -= __n;
                return *this;
            }

            CCC_CONSTEXPR Self& operator-=(_Distance __n) CCC_NOEXCEPT {
                current += __n;
                return *this;
            }


            CCC_CONSTEXPR Self operator+(_Distance __n) const CCC_NOEXCEPT { return _Self(current - __n); }
            CCC_CONSTEXPR Self operator-(_Distance __n) const CCC_NOEXCEPT { return _Self(current + __n); }
            CCC_CONSTEXPR _Reference operator[](_Distance __n) const CCC_NOEXCEPT { return *(*this + __n); }

        };


        /*
         *- This is the new version of reverse_iterator, as defined in the
         *  draft C++ standard.  It relies on the iterator_traits template,
         *  which in turn relies on partial specialization.  The class
         *  reverse_bidirectional_iterator is no longer part of the draft
         *  standard, but it is retained for backward compatibility.
         */

        template <class _Iterator>
        class reverse_iterator
        {
        protected:
            _Iterator current;

        public:
            using iterator_category = typename iterator_traits<_Iterator>::iterator_category;
            using value_type = typename iterator_traits<_Iterator>::value_type;
            using difference_type = typename iterator_traits<_Iterator>::difference_type;
            using pointer = typename iterator_traits<_Iterator>::pointer;
            using reference = typename iterator_traits<_Iterator>::reference;

            using iterator_type = _Iterator;
            using Self = reverse_iterator<_Iterator>;

        public:
            CCC_CONSTEXPR reverse_iterator() CCC_NOEXCEPT {}
            CCC_CONSTEXPR explicit reverse_iterator(iterator_type __x) : current(__x) {}

            CCC_CONSTEXPR reverse_iterator(const Self& __x) : current(__x.current) {}

            CCC_CONSTEXPR iterator_type base() const CCC_NOEXCEPT { return current; }

            CCC_CONSTEXPR reference operator*()const CCC_NOEXCEPT {
                _Iterator inner = current;
                return *--inner;
            }

            CCC_CONSTEXPR pointer operator->()const CCC_NOEXCEPT { return &(operator*()); }

            CCC_CONSTEXPR Self& operator++() CCC_NOEXCEPT {
                --current;
                return *this;
            }

            CCC_CONSTEXPR Self operator++(int) CCC_NOEXCEPT {
                _Iterator _inner = current;
                --current;
                return _inner;
            }

            CCC_CONSTEXPR Self& operator--() CCC_NOEXCEPT {
                ++current;
                return *this;
            }

            CCC_CONSTEXPR Self operator--(int) CCC_NOEXCEPT {
                _Iterator _inner = current;
                ++current;
                return _inner;
            }

            CCC_CONSTEXPR Self& operator+=(difference_type __n) CCC_NOEXCEPT {
                current -= __n;
                return *this;
            }

            CCC_CONSTEXPR Self& operator-=(difference_type __n) CCC_NOEXCEPT {
                current += __n;
                return *this;
            }

            CCC_CONSTEXPR Self operator+(difference_type __n) const CCC_NOEXCEPT { return _Self(current - __n); }
            CCC_CONSTEXPR Self operator-(difference_type __n) const CCC_NOEXCEPT { return _Self(current + __n); }
            CCC_CONSTEXPR reference operator[](difference_type __n) const CCC_NOEXCEPT { return *(*this + __n); }
        };


        CCC_PARAMETER_FUN_OF_ITERATOR_CONTAIER(output_iterator_tag, insert_iterator);
        CCC_PARAMETER_FUN_OF_ITERATOR_CONTAIER(output_iterator_tag, back_insert_iterator);
        CCC_PARAMETER_FUN_OF_ITERATOR_CONTAIER(output_iterator_tag, front_insert_iterator);


        // #if __cplusplus >= 202003L

        template<random_access_iterator_tag_cond _Iterator>
        CCC_CONSTEXPR random_access_iterator_tag
            iterator_category(const reverse_iterator<_Iterator>&) CCC_NOEXCEPT {
            return random_access_iterator_tag();
        }

        CCC_PARAMETER_FUN_OF_ITERATOR(bidirectional_iterator_tag,
            poly_bidirectional_iterator_tag_cond,
            reverse_history_bidirectional_iterator);

        CCC_PARAMETER_FUN_OF_ITERATOR(random_access_iterator_tag,
            poly_random_access_iterator_tag_cond,
            reverse_history_iterator);

        // #endif
    };


    template< class _Ty, std::size_t N >
    CCC_CONSTEXPR_CXX17 auto rbegin(_Ty(&__rhs)[N]) -> typename decltype(__rhs)::reverse_iteraotr {
        return static_cast<typename decltype(__rhs)::reverse_iteraotr>(__rhs.begin());
    }

    template<class _Tp>
    CCC_CONSTEXPR_CXX17 auto rbegin(const _Tp& __rhs) -> decltype(__rhs.rbegin()) {
        return static_cast<typename decltype(__rhs)::reverse_iterator>(__rhs.begin());
    }


    template<class _Tp>
    CCC_CONSTEXPR_CXX17 auto rend(const _Tp& __rhs) -> decltype(__rhs.rend()) {
        return static_cast<typename decltype(__rhs)::reverse_iterator>(__rhs.rend());
    }

    template<class _Ty, std::size_t N>
    CCC_CONSTEXPR_CXX17 auto rend(_Ty(&__rhs)[N]) -> typename decltype(__rhs)::reverse_iteraotr {
        return static_cast<typename decltype(__rhs)::reverse_iterator>(__rhs.begin());
    }


    template<class _Ty, std::size_t N>
    CCC_CONSTEXPR_CXX17 auto crbegin(_Ty(&__rhs)[N]) -> typename decltype(__rhs)::const_reverse_iterator {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(__rhs.rbegin());
    }

    template<class _Ty>
    CCC_CONSTEXPR_CXX17 auto crbegin(const _Ty& __rhs) -> decltype(__rhs.crbegin()) {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(__rhs.rbegin());
    }


    template<class _Ty, std::size_t N>
    CCC_CONSTEXPR_CXX17 auto crend(_Ty(&__rhs)[N]) -> typename decltype(__rhs)::const_reverse_iterator {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(__rhs.rend());
    }

    template<class _Ty>
    CCC_CONSTEXPR_CXX17 auto crend(const _Ty& __rhs) -> decltype(__rhs.crend()) {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(__rhs.crend());
    }

};
#endif      //* _ITERATOR_STL_