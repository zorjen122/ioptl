/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
*/

#ifndef _ITERATOR_IOP_
#define _ITERATOR_IOP_

#include "util/iterator_baseF.h"
#include "util/utilityF.h"

#include "cppconfig.h"

/*
    该文件用于支持迭代器中的适配器部分.
        (This file is used to support the adapter part in iterators)
*/

#define IOP_FUNCTIONAL_ITERATOR_OF_CONTAIER(name, iterator_tag, function)      \
    template <class _Contaier> class name                                      \
    {                                                                          \
      protected:                                                               \
        _Contaier *contaier;                                                   \
                                                                               \
      public:                                                                  \
        using contaier_type = _Contaier;                                       \
        using iterator_category = iterator_tag;                                \
        using difference_type = void;                                          \
        using value_type = void;                                               \
        using pointer = void;                                                  \
        using reference = void;                                                \
                                                                               \
        explicit name<_Contaier>(const _Contaier &__res) noexcept              \
            : contaier(&__res)                                                 \
        {}                                                                     \
                                                                               \
        name<_Contaier> &                                                      \
        operator=(const typename _Contaier::value_type &__res)                 \
        {                                                                      \
            contaier->function(__res);                                         \
            return *this;                                                      \
        }                                                                      \
                                                                               \
        constexpr _Contaier *Base() const noexcept { return contaier; }        \
                                                                               \
        name<_Contaier> &operator*() noexcept { return *this; }                \
        name<_Contaier> &operator++() noexcept { return *this; }               \
        name<_Contaier> &operator++(int) noexcept { return *this; }            \
    };

#define IOP_PARAMETER_FUN_OF_ITERATOR_CONTAIER(iterator_type_tag,              \
                                               functional_iterator)            \
    template <class _General_Ty>                                               \
    constexpr iterator_type_tag iterator_category(                             \
        const functional_iterator<_General_Ty> &) noexcept                     \
    {                                                                          \
        return iterator_type_tag();                                            \
    }

#define IOP_PARAMETER_FUN_OF_ITERATOR(                                         \
    iterator_type_tag, iterator_type_tag_cond, functional_iterator)            \
    template <iterator_type_tag_cond _Iterator, class _Ty, class _Reference,   \
              class _Distance>                                                 \
    constexpr _Iterator iterator_category(                                     \
        const functional_iterator<_Iterator, _Ty, _Reference, _Distance>       \
            &) noexcept                                                        \
    {                                                                          \
        return _Iterator();                                                    \
    }

namespace iop {
    namespace iter {

        //* use explicit for effect of the copy construct_at with type parameter
        // change
        IOP_FUNCTIONAL_ITERATOR_OF_CONTAIER(back_insert_iterator,
                                            output_iterator_tag, push_back);
        IOP_FUNCTIONAL_ITERATOR_OF_CONTAIER(front_insert_iterator,
                                            output_iterator_tag, push_front);

        template <class _Contaier> class insert_iterator
        {
          protected:
            _Contaier *contaier;
            typename _Contaier::iterator iter;

          public:
            using contaier_type = _Contaier;
            using iterator_category = output_iterator_tag;
            using difference_type = void;
            using value_type = void;
            using pointer = void;
            using reference = void;

            constexpr explicit insert_iterator(
                const contaier_type &__res,
                typename _Contaier::iterator __iter) noexcept
                : contaier(&__res),
                  iter(__iter)
            {}

            constexpr insert_iterator<contaier_type> &
            operator=(const typename contaier_type::value_type &__res)
            {
                iter = contaier->insert(iter, __res);
                ++iter;
                return *this;
            }

            constexpr insert_iterator<contaier_type> &operator*() noexcept
            {
                return *this;
            }
            constexpr insert_iterator<contaier_type> &operator++() noexcept
            {
                return *this;
            }
            constexpr insert_iterator<contaier_type> &operator++(int) noexcept
            {
                return *this;
            }
        };

        template <class _Iterator, class _Ty, class _Reference = _Ty &,
                  class _Distance = ::std::ptrdiff_t>
        class reverse_history_bidirectional_iterator
        {
          protected:
            _Iterator current;

          public:
            using iterator_category = bidirectional_iterator_tag;
            using value_type = _Ty;
            using pointer = _Ty *;
            using reference = _Reference;
            using difference_type = _Distance;
            using Self =
                reverse_history_bidirectional_iterator<_Iterator, _Ty,
                                                       _Reference, _Distance>;

            constexpr reverse_history_bidirectional_iterator() noexcept {}
            constexpr explicit reverse_history_bidirectional_iterator(
                _Iterator __res) noexcept : current(__res)
            {}

            constexpr _Iterator Base() const noexcept { return current; }

            constexpr reference operator*() const noexcept
            {
                _Iterator inner = current;
                return *--inner;
            }

            constexpr pointer operator->() const noexcept
            {
                return &(operator*());
            }

            constexpr Self &operator++() noexcept
            {
                --current;
                return *this;
            }

            constexpr Self operator++(int) noexcept
            {
                _Iterator _inner = current;
                --current;
                return _inner;
            }

            constexpr Self operator--() noexcept
            {
                ++current;
                return *this;
            }
            constexpr Self &operator--(int) noexcept
            {
                _Iterator _inner = current;
                ++current;
                return _inner;
            }
        };

        //* use general-name -> since the class for this iterator is a
        // complete(++-- +- +-=)
        template <class _Iterator, class _Ty, class _Reference = _Ty &,
                  class _Distance = ::std::ptrdiff_t>
        class reverse_history_iterator
        {
          protected:
            _Iterator current;

          public:
            using iterator_category = random_access_iterator_tag;
            using value_type = _Ty;
            using pointer = _Ty *;
            using reference = _Reference;
            using difference_type = _Distance;
            using Self =
                reverse_history_iterator<_Iterator, _Ty, _Reference, _Distance>;

            constexpr reverse_history_iterator() noexcept {}
            constexpr explicit reverse_history_iterator(
                _Iterator __res) noexcept : current(__res)
            {}

            constexpr _Iterator Base() const noexcept { return current; }

            reference operator*() const noexcept { return *(--current); }
            pointer operator->() const noexcept { return &(operator*()); }

            constexpr Self &operator++() noexcept
            {
                --current;
                return *this;
            }

            constexpr Self operator++(int) noexcept
            {
                _Iterator _inner = current;
                --current;
                return _inner;
            }

            constexpr Self &operator--() noexcept
            {
                ++current;
                return *this;
            }

            constexpr Self operator--(int) noexcept
            {
                _Iterator _inner = current;
                ++current;
                return _inner;
            }

            constexpr Self &operator+=(_Distance __n) noexcept
            {
                current -= __n;
                return *this;
            }

            constexpr Self &operator-=(_Distance __n) noexcept
            {
                current += __n;
                return *this;
            }

            constexpr Self operator+(_Distance __n) const noexcept
            {
                return _Self(current - __n);
            }
            constexpr Self operator-(_Distance __n) const noexcept
            {
                return _Self(current + __n);
            }
            constexpr _Reference operator[](_Distance __n) const noexcept
            {
                return *(*this + __n);
            }
        };

        /*
         *- This is the new version of reverse_iterator, as defined in the
         *  draft C++ standard.  It relies on the iterator_traits template,
         *  which in turn relies on partial specialization.  The class
         *  reverse_bidirectional_iterator is no longer part of the draft
         *  standard, but it is retained for backward compatibility.
         */

        template <class _Iterator> class reverse_iterator
        {
        private:
            using Traits_type = iterator_traits<_Iterator>;
          protected:
            _Iterator current;
          public:
            using iterator_category =
                typename Traits_type::iterator_category;
            using value_type = typename Traits_type::value_type;
            using difference_type =
                typename Traits_type::difference_type;
            using pointer = typename Traits_type::pointer;
            using reference = typename Traits_type::reference;

            using iterator_type = _Iterator;
            using Self = reverse_iterator<_Iterator>;

          public:
            constexpr reverse_iterator() noexcept {}
            constexpr explicit reverse_iterator(iterator_type __x)
                : current(__x)
            {}

            constexpr reverse_iterator(const Self &__x) : current(__x.current)
            {}

            constexpr iterator_type base() const noexcept { return current; }

            constexpr reference operator*() const noexcept
            {
                _Iterator inner = current;
                return *--inner;
            }

            constexpr pointer operator->() const noexcept
            {
                return &(operator*());
            }

            constexpr Self &operator++() noexcept
            {
                --current;
                return *this;
            }

            constexpr Self operator++(int) noexcept
            {
                _Iterator _inner = current;
                --current;
                return _inner;
            }

            constexpr Self &operator--() noexcept
            {
                ++current;
                return *this;
            }

            constexpr Self operator--(int) noexcept
            {
                _Iterator _inner = current;
                ++current;
                return _inner;
            }

            constexpr Self &operator+=(difference_type __n) noexcept
            {
                current -= __n;
                return *this;
            }

            constexpr Self &operator-=(difference_type __n) noexcept
            {
                current += __n;
                return *this;
            }

            constexpr Self operator+(difference_type __n) const noexcept
            {
                return _Self(current - __n);
            }
            constexpr Self operator-(difference_type __n) const noexcept
            {
                return _Self(current + __n);
            }
            constexpr reference operator[](difference_type __n) const noexcept
            {
                return *(*this + __n);
            }
        };

        IOP_PARAMETER_FUN_OF_ITERATOR_CONTAIER(output_iterator_tag,
                                               insert_iterator);
        IOP_PARAMETER_FUN_OF_ITERATOR_CONTAIER(output_iterator_tag,
                                               back_insert_iterator);
        IOP_PARAMETER_FUN_OF_ITERATOR_CONTAIER(output_iterator_tag,
                                               front_insert_iterator);

#if __cplusplus >= 202003L

        template <random_access_iterator_tag_cond _Iterator>
        constexpr random_access_iterator_tag
        iterator_category(const reverse_iterator<_Iterator> &) noexcept
        {
            return random_access_iterator_tag();
        }

        IOP_PARAMETER_FUN_OF_ITERATOR(bidirectional_iterator_tag,
                                      bidirectional_iterator_tag_cond,
                                      reverse_history_bidirectional_iterator);

        IOP_PARAMETER_FUN_OF_ITERATOR(random_access_iterator_tag,
                                      random_access_iterator_tag_cond,
                                      reverse_history_iterator);
#endif
    }; // namespace iter

    template <class _Ty, size_t N>
    constexpr auto rbegin(_Ty (&__rhs)[N]) ->
        typename decltype(__rhs)::reverse_iteraotr
    {
        return static_cast<typename decltype(__rhs)::reverse_iteraotr>(
            __rhs.begin());
    }

    template <class _Tp>
    constexpr auto rbegin(const _Tp &__rhs) -> decltype(__rhs.rbegin())
    {
        return static_cast<typename decltype(__rhs)::reverse_iterator>(
            __rhs.begin());
    }

    template <class _Tp>
    constexpr auto rend(const _Tp &__rhs) -> decltype(__rhs.rend())
    {
        return static_cast<typename decltype(__rhs)::reverse_iterator>(
            __rhs.rend());
    }

    template <class _Ty, size_t N>
    constexpr auto rend(_Ty (&__rhs)[N]) ->
        typename decltype(__rhs)::reverse_iteraotr
    {
        return static_cast<typename decltype(__rhs)::reverse_iterator>(
            __rhs.begin());
    }

    template <class _Ty, size_t N>
    constexpr auto crbegin(_Ty (&__rhs)[N]) ->
        typename decltype(__rhs)::const_reverse_iterator
    {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(
            __rhs.rbegin());
    }

    template <class _Ty>
    constexpr auto crbegin(const _Ty &__rhs) -> decltype(__rhs.crbegin())
    {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(
            __rhs.rbegin());
    }

    template <class _Ty, size_t N>
    constexpr auto crend(_Ty (&__rhs)[N]) ->
        typename decltype(__rhs)::const_reverse_iterator
    {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(
            __rhs.rend());
    }

    template <class _Ty>
    constexpr auto crend(const _Ty &__rhs) -> decltype(__rhs.crend())
    {
        return static_cast<typename decltype(__rhs)::const_reverse_iterator>(
            __rhs.crend());
    }

};     // namespace iop

#endif //* _ITERATOR_IOP_