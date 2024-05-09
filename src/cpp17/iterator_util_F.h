/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _ITERATOR_UTIL_IOP_
#define _ITERATOR_UTIL_IOP_

#include "cppconfig.h"
#include "metacomponent.h"

#include <cstddef>  // ::std::ptrdiff_t
#include <type_traits> // is_convertible

#define ITERAOTR_TAG_DEFINE(NAME, IS_A_OBJ)                                    \
    template <class _Ty, class _Distance>                                      \
    struct NAME : public IS_A_OBJ<_Ty, _Distance>                              \
    {                                                                          \
        using iterator_category = NAME##_tag;                                  \
        using value_type = _Ty;                                                \
        using pointer = _Ty *;                                                 \
        using reference = _Ty &;                                               \
        using difference_type = _Distance;                                     \
    };

#define IS_ITERATOR_TAG_TOTAL(ITERATOR_TAG)                                    \
    template <class _Ty>                                                       \
    using is_##ITERATOR_TAG = ::std::is_convertible<_Ty, ITERATOR_TAG>;        \
    template <class _Ty>                                                       \
    using is_##ITERATOR_TAG##_t = typename is_##ITERATOR_TAG<_Ty>::type;       \
    template <class _Ty>                                                       \
    constexpr bool is_##ITERATOR_TAG##_v = is_##ITERATOR_TAG<_Ty>::value;

#define IS_ITERATOR_CATEGORY_ARGS_V(IS_ITERATOR_TAG)            \
    template<class... _Args>                                    \
    constexpr bool IS_ITERATOR_TAG##_args_v                     \
        = (util::IS_ITERATOR_TAG##_v<_Args> && ...)

namespace iop {

    // Iterator type
    struct input_iterator_tag
    {};
    struct output_iterator_tag
    {};
    struct forward_iterator_tag : public input_iterator_tag
    {};
    struct bidirectional_iterator_tag : public forward_iterator_tag
    {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag
    {};

    // Iterator
    template <class _Category, class _Ty, class _Distance = ::std::ptrdiff_t,
              class _Pointer = _Ty *, class _Reference = _Ty &>
    class iterator
    {
      public:
        using iterator_category = _Category;

        using value_type = _Ty;
        using pointer = _Ty *;
        using reference = _Ty &;
        using difference_type = _Distance;
    };

    template <class _Ty, class _Distance> struct input_iterator
    {
        using iterator_category = input_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty *;
        using reference = _Ty &;
        using difference_type = _Distance;
    };

    struct output_iterator
    {
        using iterator_category = output_iterator_tag;

        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
    };

    ITERAOTR_TAG_DEFINE(forward_iterator, input_iterator);
    ITERAOTR_TAG_DEFINE(bidirectional_iterator, forward_iterator);
    ITERAOTR_TAG_DEFINE(random_access_iterator, bidirectional_iterator);

    // iterator traits
    template <class _Iterator> struct iterator_traits
    {
        using iterator_category = typename _Iterator::iterator_category;

        using value_type = typename _Iterator::value_type;
        using pointer = typename _Iterator::pointer;
        using reference = typename _Iterator::reference;
        using difference_type = typename _Iterator::difference_type;
    };

    template <class _Tp> struct iterator_traits<_Tp *>
    {
        using iterator_category = random_access_iterator_tag;

        using value_type = _Tp;
        using difference_type = ::std::ptrdiff_t;
        using pointer = _Tp *;
        using reference = _Tp &;
    };

    template <class _Tp> struct iterator_traits<const _Tp *>
    {
        using iterator_category = random_access_iterator_tag;

        using value_type = _Tp;
        using difference_type = ::std::ptrdiff_t;
        using pointer = const _Tp *;
        using reference = const _Tp &;
    };

    template <class I>
    constexpr typename iterator_traits<I>::iterator_category
    iterator_category(const I &)
    {
        using category = typename iterator_traits<I>::iterator_category;
        return category();
    }

    template<class T>
    using iterator_category_t = typename iterator_traits<T>::iterator_category;

    template <class _Iterty>
    using difference_type_t =
        typename iterator_traits<_Iterty>::difference_type;

    namespace util {
        /* macro ex:   is_input_iterator_tag 
                    *  is_input_iterator_tag_t 
                    *  is_input_iterator_tag_v   */
        IS_ITERATOR_TAG_TOTAL(input_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(output_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(forward_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(bidirectional_iterator_tag);
        IS_ITERATOR_TAG_TOTAL(random_access_iterator_tag);

        template<class T>
        using is_non_output_iterator_tag = mpls::conditioni_t<is_input_iterator_tag_v<T> 
                                            || is_forward_iterator_tag_v<T>
                                            || is_bidirectional_iterator_tag_v<T>
                                            || is_random_access_iterator_tag_v<T>
                                            , mpls::true_type
                                            , mpls::false_type
                                            >;


        template<class T> 
        using is_non_output_iterator_tag_t 
            = typename is_non_output_iterator_tag<T>::type;
        template<class T> constexpr bool 
        is_non_output_iterator_tag_v = is_non_output_iterator_tag<T>::value;


        template<class T>
        using is_iterator_tag 
            = mpls::conditioni_t<is_non_output_iterator_tag_v<T> 
                                || is_output_iterator_tag_v<T>
                                , mpls::true_type
                                , mpls::false_type
                                >;

        template<class T> 
        using is_iterator_tag_t = typename is_iterator_tag<T>::type;
        template<class T> constexpr bool 
        is_iterator_tag_v = is_iterator_tag<T>::value;

    
        // is_iterator-categoty args version.
        template<class... _Args>
        constexpr bool is_input_iterator_tag_args_v 
            = (is_input_iterator_tag_v<_Args> && ...);
        IS_ITERATOR_CATEGORY_ARGS_V(is_output_iterator_tag);
        IS_ITERATOR_CATEGORY_ARGS_V(is_forward_iterator_tag);
        IS_ITERATOR_CATEGORY_ARGS_V(is_bidirectional_iterator_tag);
        IS_ITERATOR_CATEGORY_ARGS_V(is_random_access_iterator_tag);
        IS_ITERATOR_CATEGORY_ARGS_V(is_non_output_iterator_tag);
        IS_ITERATOR_CATEGORY_ARGS_V(is_iterator_tag);
    }; // namespace iop::util


}; // namespace iop

#endif //*  _ITERATOR_UTIL_IOP_
