/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _TYPE_MPL_TRAITS_
#define _TYPE_MPL_TRAITS_

#include "metacomponent.h"
#include <cstddef>

namespace mpls {

    // add_lvalue_reference
    template <class T> struct add_lvalue_reference
    {
        using type = T &;
    };
    template <class T> struct add_lvalue_reference<T &>
    {
        using type = T &;
    };

    template <> struct add_lvalue_reference<void>
    {
        using type = void;
    };
    template <> struct add_lvalue_reference<const void>
    {
        using type = const void;
    };
    template <> struct add_lvalue_reference<volatile void>
    {
        using type = volatile void;
    };
    template <> struct add_lvalue_reference<const volatile void>
    {
        using type = const volatile void;
    };

    // add_value_reference
    template <class _Ty>
    struct add_value_reference : public add_lvalue_reference<_Ty>
    {
        using Lvalue = _Ty;
        using Rvalue = _Ty;
    };
    template <class _Ty> struct add_value_reference<_Ty &>
    {
        using Lvalue = _Ty &;
        using Rvalue = _Ty &&;
    };
    template <class _Ty>
    using add_rvalue_reference_t = typename add_value_reference<_Ty>::Rvalue;
    template <class _Ty>
    using add_lvalue_reference_t = typename add_value_reference<_Ty>::Lvalue;

    // dereference
    template <class _Ty> struct dereference
    {};
    template <class _Ty> struct dereference<_Ty *>
    {
        using type = _Ty;
    };
    template <class _Ty> using dereference_t = typename dereference<_Ty>::type;

    // remove_cv
    template <typename _Tp> struct remove_cv
    {
        using type = _Tp;
    };
    template <typename _Tp> struct remove_cv<const _Tp>
    {
        using type = _Tp;
    };
    template <typename _Tp> struct remove_cv<volatile _Tp>
    {
        using type = _Tp;
    };
    template <typename _Tp> struct remove_cv<const volatile _Tp>
    {
        using type = _Tp;
    };
    template <class _Ty> using remove_cv_t = typename remove_cv<_Ty>::type;

    // remove_reference
    template <class _Ty> struct remove_reference
    {
        using type = _Ty;
    };
    template <class _Ty> struct remove_reference<_Ty &>
    {
        using type = _Ty;
    };
    template <class _Ty> struct remove_reference<_Ty &&>
    {
        using type = _Ty;
    };
    template <class _Ty>
    using remove_reference_t = typename remove_reference<_Ty>::type;

    template <class _Tp> struct is_lvalue_reference : public false_type
    {};
    template <class _Tp> struct is_lvalue_reference<_Tp &> : public true_type
    {};

    template <class _Ty, class _Tp> struct is_same : public false_type
    {};
    template <class _Ty> struct is_same<_Ty, _Ty> : public true_type
    {};

    template <class _Ty> struct is_reference : public false_type
    {};
    template <class _Ty> struct is_reference<_Ty &> : public true_type
    {};

    template <class _Ty> struct is_pointer : public false_type
    {};
    template <class _Ty> struct is_pointer<_Ty *> : public true_type
    {};

    template <class _Ty> struct is_const : public false_type
    {};
    template <class _Ty> struct is_const<const _Ty> : public true_type
    {};

    template <class _Ty> struct is_void : public false_type
    {};
    template <> struct is_void<void> : public true_type
    {};

    template <class _Ty> struct is_volatile : public false_type
    {};
    template <class _Ty> struct is_volatile<_Ty volatile> : public true_type
    {};

#ifdef __cpp_char8_t
#define _GLIBCXX_USE_CHAR8_T 1
#endif

    // is_integral
    template <typename> struct __is_integral_helper : public false_type
    {};

    template <> struct __is_integral_helper<bool> : public true_type
    {};

    template <> struct __is_integral_helper<char> : public true_type
    {};

    template <> struct __is_integral_helper<signed char> : public true_type
    {};

    template <> struct __is_integral_helper<unsigned char> : public true_type
    {};

    // We want is_integral<wchar_t> to be true (and make_signed/unsigned to
    // work) even when libc doesn't provide working <wchar.h> and related
    // functions, so don't check _GLIBCXX_USE_WCHAR_T here.

    template <> struct __is_integral_helper<wchar_t> : public true_type
    {};

#ifdef _GLIBCXX_USE_CHAR8_T
    template <> struct __is_integral_helper<char8_t> : public true_type
    {};
#endif

    template <> struct __is_integral_helper<char16_t> : public true_type
    {};
    template <> struct __is_integral_helper<char32_t> : public true_type
    {};
    template <> struct __is_integral_helper<short> : public true_type
    {};
    template <> struct __is_integral_helper<unsigned short> : public true_type
    {};
    template <> struct __is_integral_helper<int> : public true_type
    {};
    template <> struct __is_integral_helper<unsigned int> : public true_type
    {};
    template <> struct __is_integral_helper<long> : public true_type
    {};
    template <> struct __is_integral_helper<unsigned long> : public true_type
    {};
    template <> struct __is_integral_helper<long long> : public true_type
    {};
    template <>
    struct __is_integral_helper<unsigned long long> : public true_type
    {};

    template <class _Ty> struct is_integral : public __is_integral_helper<_Ty>
    {};

    template <class _Ty>
    using is_intergral_pointer = is_integral<dereference_t<_Ty>>;

    // is_floating_point_cv、is_floating_point
    template <class> struct is_floating_point_cv : public false_type
    {};

    template <> struct is_floating_point_cv<float> : public true_type
    {};

    template <> struct is_floating_point_cv<double> : public true_type
    {};

    template <> struct is_floating_point_cv<long double> : public true_type
    {};

    template <class _Ty>
    struct is_floating_point
        : is_floating_point_cv<typename remove_cv<_Ty>::type>
    {};

    // is_null_pointer
    template <class> struct __is_null_pointer_helper : public false_type
    {};

    template <>
    struct __is_null_pointer_helper<::std::nullptr_t> : public true_type
    {};

    template <class _Ty>
    struct is_null_pointer
        : public __is_null_pointer_helper<typename remove_cv<_Ty>::type>::type
    {};

    // is_member_pointer
    template <class> struct is_member_pointer : public false_type
    {};
    template <class _Ty, class _Cy>
    struct is_member_pointer<_Ty _Cy::*> : true_type
    {};

    // is_array
    template <class _Ty> struct is_array : public false_type
    {};
    template <class _Ty> struct is_array<_Ty[]> : public true_type
    {};
    template <class _Ty, size_t _sz>
    struct is_array<_Ty[_sz]> : public true_type
    {};

    // is_basic_char
    template <typename T> struct is_basic_char : false_type
    {};
    template <> struct is_basic_char<char> : true_type
    {};
    template <> struct is_basic_char<wchar_t> : true_type
    {};

    template <> struct is_basic_char<signed char> : true_type
    {};
    template <> struct is_basic_char<unsigned char> : true_type
    {};
#if IOP_VERSION > 17
    template <> struct is_basic_char<char8_t> : true_type
    {};
    template <> struct is_basic_char<char16_t> : true_type
    {};
    template <> struct is_basic_char<char32_t> : true_type
    {};
#endif

    // is_function
    // since ref type is non-const type, so we need to check the is_reference.
    template <class _Ty>
    struct is_function
        : public oper::and_<is_const<const _Ty>, is_reference<_Ty>>
    {};

    template <class _Ty>
    struct is_arithmetic
        : public oper::or_<is_integral<_Ty>, is_floating_point<_Ty>>::type
    {};

    template <class _Ty>
    struct is_scalar
        : public oper::or_<is_arithmetic<_Ty>,
                           /* non-define is_enum<_Ty> ,TODO */ is_pointer<_Ty>,
                           is_member_pointer<_Ty>, is_null_pointer<_Ty>>::type
    {};

    // Type Traits of the is_xxx_v
    template <class _Tp> constexpr bool is_void_v = is_void<_Tp>::value;

    template <class _Tp>
    constexpr bool is_null_pointer_v = is_null_pointer<_Tp>::value;

    template <class _Tp> constexpr bool is_integral_v = is_integral<_Tp>::value;

    template <class _Tp>
    constexpr bool is_floating_point_v = is_floating_point<_Tp>::value;

    template <class _Tp> constexpr bool is_array_v = is_array<_Tp>::value;

    template <class _Tp> constexpr bool is_pointer_v = is_pointer<_Tp>::value;

    template <class _Tp> constexpr bool is_function_v = is_function<_Tp>::value;

    template <class _Tp>
    constexpr bool is_reference_v = is_reference<_Tp>::value;

    template <class _Tp>
    constexpr bool is_arithmetic_v = is_arithmetic<_Tp>::value;

    template <class _Tp> constexpr bool is_scalar_v = is_scalar<_Tp>::value;

    template <class _Tp>
    constexpr bool is_member_pointer_v = is_member_pointer<_Tp>::value;

    template <class _Tp> constexpr bool is_const_v = is_const<_Tp>::value;

    template <class _Tp> constexpr bool is_volatile_v = is_volatile<_Tp>::value;

    // Type Traits of the is_xxx_t
    template <class _Tp> using is_void_t = typename is_void<_Tp>::value_type;

    template <class _Tp>
    using is_null_pointer_t = typename is_null_pointer<_Tp>::value_type;

    template <class _Tp>
    using is_integral_t = typename is_integral<_Tp>::value_type;

    template <class _Tp>
    using is_floating_point_t = typename is_floating_point<_Tp>::value_type;

    template <class _Tp> using is_array_t = typename is_array<_Tp>::value_type;

    template <class _Tp>
    using is_pointer_t = typename is_pointer<_Tp>::value_type;

    template <class _Tp>
    using is_function_t = typename is_function<_Tp>::value_type;

    template <class _Tp>
    using is_reference_t = typename is_reference<_Tp>::value_type;

    template <class _Tp>
    using is_arithmetic_t = typename is_arithmetic<_Tp>::value_type;

    template <class _Tp>
    using is_scalar_t = typename is_scalar<_Tp>::value_type;

    template <class _Tp>
    using is_member_pointer_t = typename is_member_pointer<_Tp>::value_type;

    template <class _Tp> using is_const_t = typename is_const<_Tp>::value_type;

    template <class _Tp>
    using is_volatile_t = typename is_volatile<_Tp>::value_type;

    // Other meta func
    template <bool, class _Ty = void> struct enable_if
    {};
    template <class _Ty> struct enable_if<true, _Ty>
    {
        using type = _Ty;
    };

    template <class _Ty1, class _Ty2, class _Ty3> struct type_convert
    {};
    template <class _Ty, class _By_Ty> struct type_convert<_Ty, _Ty, _By_Ty>
    {
        using type = _By_Ty;
    };

    template <bool _Cond, class _Ty = void>
    using enable_if_t = typename enable_if<_Cond, _Ty>::type;

    template <class _Ty, class _Tp, class _Tchange>
    using type_convert_t = typename type_convert<_Ty, _Tp, _Tchange>::type;

    //* since declval is complie-time? so declval<_Ty>.fun() rvalue_reference
    // condition ok?
    template <class _Ty> add_rvalue_reference_t<_Ty> declval() noexcept
    {
        static_assert(_always_false<_Ty>,
                      "Calling declval is ill-formed, see N4892 [declval]/2.");
    }

    
    template<class From, class To>
    constexpr To convertible_to() { return declval<From>(); }

    template<class From, class To>
    using is_convertible = mpls::is_same<To, decltype(convertible_to<From, To>())>;
    
    template<class From, class To>
    using is_convertible_t = typename is_convertible<From, To>::type;

    template<class From, class To>
    constexpr bool is_convertible_v = is_convertible<From, To>::value;

}; // namespace mpls

#endif //*     _TYPE_MPL_TRAITS_