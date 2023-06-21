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

#ifndef __TYPE_TRAITS_
#define __TYPE_TRAITS_

#ifndef __STL_NO_BOOL
#define __STL_TEMPLATE_NULL template<>


namespace Traits_type {
    struct __true_type {};      struct __false_type {};
    template <class _Tp>
    struct __type_traits {
        using  this_dummy_member_must_be_first = __true_type;
        using  has_trivial_default_constructor = __false_type;
        using  has_trivial_copy_constructor = __false_type;
        using  has_trivial_assignment_operator = __false_type;
        using  has_trivial_destructor = __false_type;
        using  is_POD_type = __false_type;

    };

    template<class _Ty>
    struct __type_traits_all_true :__type_traits<_Ty> {
        using  has_trivial_default_constructor = __true_type;
        using  has_trivial_copy_constructor = __true_type;
        using  has_trivial_assignment_operator = __true_type;
        using  has_trivial_destructor = __true_type;
        using  is_POD_type = __true_type;
    };



    __STL_TEMPLATE_NULL struct  __type_traits_all_true<bool> {};

    #endif  /* __STL_NO_BOOL */

    __STL_TEMPLATE_NULL struct __type_traits_all_true<char> {};

    __STL_TEMPLATE_NULL struct __type_traits<signed char> {};

    __STL_TEMPLATE_NULL struct __type_traits<unsigned char> {};

    #ifdef __STL_HAS_WCHAR_T

    __STL_TEMPLATE_NULL struct __type_traits<wchar_t> {};

    #endif /* __STL_HAS_WCHAR_T */

    __STL_TEMPLATE_NULL struct __type_traits<short> {};

    __STL_TEMPLATE_NULL struct __type_traits<unsigned short> {};

    __STL_TEMPLATE_NULL struct __type_traits<int> {};

    __STL_TEMPLATE_NULL struct __type_traits<unsigned int> {};

    __STL_TEMPLATE_NULL struct __type_traits<long> {};

    __STL_TEMPLATE_NULL struct __type_traits<unsigned long> {};

    #ifdef __IOP_STLONG_LONG

    __STL_TEMPLATE_NULL struct __type_traits<long long> {};

    __STL_TEMPLATE_NULL struct __type_traits<unsigned long long> {};

    #endif /* __IOP_STLONG_LONG */

    __STL_TEMPLATE_NULL struct __type_traits<float> {};

    __STL_TEMPLATE_NULL struct __type_traits<double> {};

    __STL_TEMPLATE_NULL struct __type_traits<long double> {};

    #ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

    template <class _Tp>
    struct __type_traits<_Tp*> {};

    #else /* __STL_CLASS_PARTIAL_SPECIALIZATION */

    __STL_TEMPLATE_NULL struct __type_traits<char*> {};

    __STL_TEMPLATE_NULL struct __type_traits<signed char*> {};

    __STL_TEMPLATE_NULL struct __type_traits<unsigned char*> {};

    __STL_TEMPLATE_NULL struct __type_traits<const char*> {};

    __STL_TEMPLATE_NULL struct __type_traits<const signed char*> {};

    __STL_TEMPLATE_NULL struct __type_traits<const unsigned char*> {};

    #endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */


    // The following could be written in terms of numeric_limits.  
    // We're doing it separately to reduce the number of dependencies.

    template <class _Tp> struct _Is_integer {
        using _Integral = __false_type;
    };

    #ifndef __STL_NO_BOOL

    __STL_TEMPLATE_NULL struct _Is_integer<bool> {
        using _Integral = __true_type;
    };

    #endif /* __STL_NO_BOOL */

    __STL_TEMPLATE_NULL struct _Is_integer<char> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<signed char> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<unsigned char> {
        using _Integral = __true_type;
    };

    #ifdef __STL_HAS_WCHAR_T

    __STL_TEMPLATE_NULL struct _Is_integer<wchar_t> {
        using _Integral = __true_type;
    };

    #endif /* __STL_HAS_WCHAR_T */

    __STL_TEMPLATE_NULL struct _Is_integer<short> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<unsigned short> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<int> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<unsigned int> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<long> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<unsigned long> {
        using _Integral = __true_type;
    };

    #ifdef __IOP_STLONG_LONG

    __STL_TEMPLATE_NULL struct _Is_integer<long long> {
        using _Integral = __true_type;
    };

    __STL_TEMPLATE_NULL struct _Is_integer<unsigned long long> {
        using _Integral = __true_type;
    };
    #endif /* __IOP_STLONG_LONG */



};              //* namespace Traits_type


#endif          //* __TYPE_TRAITS_H 
