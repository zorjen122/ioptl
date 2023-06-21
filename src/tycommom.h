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

#define _TYCOMMOM_
#ifdef _TYCOMMOM_

#include <cstddef>

#define P_TYPE_INTEGRAL_CONST(_Ty)                                  \
    template<_Ty val>                                               \
    struct integral_const<_Ty,val>{                                 \
    using value_type = _Ty;                                         \
    using type = integral_const;                                    \
    using init_type = integral_const<_Ty,val>;                      \
    using point_type = integral_const<_Ty, val - 1>;                \
    using next_type = integral_const<_Ty, val + 1>;                 \
    static constexpr value_type  value = val;                       \
    constexpr operator value_type() const noexcept {                \
            return value;                                           \
        }                                                           \
    };


/* Comom arithmetic */
#define P_MULT_ARITHMETIC_OP(name, op)                                         \
  template <class N1, class N2, class... Nargs>                                \
  struct name {                                                                \
    using inner = name<N2, Nargs...>;                                          \
    using value_type = typename largest_int<typename N1::value_type,           \
                                            typename inner::value_type>::type; \
    using type = integral_const<value_type, N1::value op inner::value>;        \
    static constexpr value_type value = type::value;                           \
  };                                                                           \
  template <class N1, class N2>                                                \
  struct name<N1, N2> {                                                        \
    using value_type = typename largest_int<typename N1::value_type,           \
                                            typename N2::value_type>::type;    \
    using type = integral_const<value_type, (N1::value op N2::value)>;         \
    static constexpr value_type value = type::value;                           \
  };


// some arithmetic only make non-mult meta-function for such which.
#define P_ONE_ARITHMETIC_OP(name,op)                                        \
  template <class N1, class N2>                                             \
  struct name {                                                             \
    using value_type = typename largest_int<typename N1::value_type,        \
                                            typename N2::value_type>::type; \
    using type = integral_const<value_type, (N1::value op N2::value)>;      \
    static constexpr value_type value = type::value;                        \
  };


/* relational operation */
#define P_ONE_RELATIONAL_OP_BOOL(name, op)                             \
  template <class N1, class N2>                                        \
  struct name {                                                        \
    using value_type = bool;                                           \
    using type = integral_const<value_type, (N1::value op N2::value)>; \
    static constexpr value_type value = type::value;                   \
  };


/* Mult logical */
#define P_MULT_RELATIONAL_OP_BOOL(name,op)                                    \
  template<class _Ty, class _Tys, class... _Args>                             \
  struct name {                                                               \
    using value_type = bool;                                                  \
    using liner = name<_Tys, _Args...>;                                       \
    using type = integral_const<value_type, (_Ty::value op liner::value)>;    \
    static constexpr value_type value = type::value;                          \
  };                                                                          \
  template<class _Ty, class _Tys>                                             \
  struct name<_Ty, _Tys> {                                                    \
      using value_type = bool;                                                \
      using type = integral_const<value_type, (_Ty::value op _Tys::value)>;   \
      static constexpr value_type value = type::value;                        \
  };


namespace MPL_L {

    template<class _Ty, _Ty val>struct integral_const {};

    template<bool val>
    struct integral_const<bool, val> {
        using value_type = bool;
        using type = integral_const;
        static constexpr value_type value = val;

        constexpr  operator value_type() const noexcept {
            return value;
        }
    };


    P_TYPE_INTEGRAL_CONST(int);
    P_TYPE_INTEGRAL_CONST(unsigned int);
    P_TYPE_INTEGRAL_CONST(long long);

    template<int N>    using int_ = integral_const<int, N>;
    template<unsigned int N> using uint_ = integral_const<unsigned int, N>;
    template<long long N> using ll_ = integral_const<long long, N>;
    template<short N>  using short_ = integral_const<short, N>;
    template<bool N>  using bool_ = integral_const<bool, N>;


    using true_type = bool_<true>;
    using false_type = bool_<false>;

    template<class...>using __void_t = void;
    template<class>constexpr bool _Always_false = false;


    template<class... Args> struct seq {};
    template<class _S, class _T> struct seq_push_back;


    template<class _T, class... Args>
    struct seq_push_back<seq<Args...>, _T> {
        using type = seq<Args..., _T>;
    };


    template<bool Judge, class _T, class _Ty>
    struct if_bool_ { using type = _T; };

    template<class _T, class _Ty>
    struct if_bool_<false, _T, _Ty> { using type = _Ty; };

    template<class Judge_traits, class _T, class _Ty>
    struct if_ {
        using type = typename if_bool_<!!Judge_traits::value, _T, _Ty>::type;
    };

    template<bool _Cond, class _Ty, class _Tp>
    using conditioni_t = typename if_bool_<!!_Cond, _Ty, _Tp>::type;


    template <typename T>
    struct integral_rank;

    template <> struct integral_rank<bool> : uint_<1> {};
    template <> struct integral_rank<signed char> : uint_<2> {};
    template <> struct integral_rank<char> : uint_<3> {};
    template <> struct integral_rank<unsigned char> : uint_<4> {};
    template <> struct integral_rank<wchar_t> : uint_<5> {};
    template <> struct integral_rank<char16_t> : uint_<6> {};
    template <> struct integral_rank<short> : uint_<7> {};
    template <> struct integral_rank<unsigned short> : uint_<8> {};
    template <> struct integral_rank<char32_t> : uint_<9> {};
    template <> struct integral_rank<int> : uint_<10> {};
    template <> struct integral_rank<unsigned int> : uint_<11> {};
    template <> struct integral_rank<long> : uint_<12> {};
    template <> struct integral_rank<unsigned long> : uint_<13> {};
    template <> struct integral_rank<long long> : uint_<14> {};
    template <> struct integral_rank<unsigned long long> : uint_<15> {};


    // since if_ immediate using value such will cannot operator in non-computea field
    template <class T1, class T2>
    using largest_int = if_bool_<integral_rank<T1>::value
        >=
        integral_rank<T2>::value, T1, T2>;


    namespace Calculations_L {

        template <class T>
        struct negate {
            using value_type = typename T::value_type;
            using type = integral_const<value_type, -T::value>;
            static constexpr value_type value = type::value;
        };

        P_MULT_ARITHMETIC_OP(plus, +);
        P_MULT_ARITHMETIC_OP(sub, -);
        P_MULT_ARITHMETIC_OP(mult, *);
        P_MULT_ARITHMETIC_OP(bitand_, &);
        P_MULT_ARITHMETIC_OP(bitor_, | );
        P_MULT_ARITHMETIC_OP(bitxor_, ^);

        P_ONE_ARITHMETIC_OP(minus, *);
        P_ONE_ARITHMETIC_OP(divides, / );
        P_ONE_ARITHMETIC_OP(modulus, %);
        P_ONE_ARITHMETIC_OP(left_shift, << );
        P_ONE_ARITHMETIC_OP(right_shift, >> );


        P_ONE_RELATIONAL_OP_BOOL(equal_to, == );
        P_ONE_RELATIONAL_OP_BOOL(not_equal_to, != );
        P_ONE_RELATIONAL_OP_BOOL(greater, > );
        P_ONE_RELATIONAL_OP_BOOL(greater_equal, >= );
        P_ONE_RELATIONAL_OP_BOOL(less, < );
        P_ONE_RELATIONAL_OP_BOOL(less_equal, <= );

        P_MULT_RELATIONAL_OP_BOOL(and_, &&);
        P_MULT_RELATIONAL_OP_BOOL(or_, || );

        template<class _Ty>
        struct not_ {
            using value_type = bool;
            using type = integral_const<value_type, !_Ty::value>;
            static constexpr value_type value = type::value;
        };

    };

    template<class T>
    struct add_lvalue_reference { using type = T&; };
    template<class T> struct add_lvalue_reference<T&> { using type = T&; };

    template<> struct add_lvalue_reference<void> { using type = void; };
    template<> struct add_lvalue_reference<const void> { using type = const void; };
    template<> struct add_lvalue_reference<volatile void> { using type = volatile void; };
    template<> struct add_lvalue_reference<const volatile void> { using type = const volatile void; };

    template<class _Ty>struct add_value_reference : public add_lvalue_reference<_Ty> {
        using Lvalue = _Ty;     using Rvalue = _Ty;
    };
    template<class _Ty>struct add_value_reference<_Ty&> {
        using Lvalue = _Ty&;     using Rvalue = _Ty&&;
    };

    template<class _Ty>
    using add_rvalue_reference_t = typename add_value_reference<_Ty>::Rvalue;
    template<class _Ty>
    using add_lvalue_reference_t = typename add_value_reference<_Ty>::Lvalue;


    template<class _Tp>
    struct is_lvalue_reference : public false_type { };

    template<typename _Tp>
    struct is_lvalue_reference<_Tp&>
        : public true_type { };

    template<typename _Tp>
    struct remove_cv { using type = _Tp; };
    template<typename _Tp>
    struct remove_cv<const _Tp> { using type = _Tp; };
    template<typename _Tp>
    struct remove_cv<volatile _Tp> { using type = _Tp; };
    template<typename _Tp>
    struct remove_cv<const volatile _Tp> { using type = _Tp; };


    template<class _Ty, class _Tp>
    struct is_same : public false_type {};

    template<class _Ty>
    struct is_same<_Ty, _Ty> : public true_type {};

    template<class _Ty>
    struct is_reference : public false_type {};

    template<class _Ty>
    struct is_reference<_Ty&> : public true_type {};

    template<class _Ty>
    struct is_pointer : public false_type {};

    template<class _Ty>
    struct is_pointer<_Ty*> : public true_type {};


    template<class _Ty>
    struct is_void : public false_type {};

    template<>
    struct is_void<void> : public true_type {};

    template<class _Ty>
    struct is_volatile : public false_type {};

    template<class _Ty>
    struct is_volatile<_Ty volatile>
        : public true_type {};


    # ifdef __cpp_char8_t
    #  define _GLIBCXX_USE_CHAR8_T 1
    # endif

    template<typename>
    struct __is_integral_helper
        : public false_type { };

    template<>
    struct __is_integral_helper<bool>
        : public true_type { };

    template<>
    struct __is_integral_helper<char>
        : public true_type { };

    template<>
    struct __is_integral_helper<signed char>
        : public true_type { };

    template<>
    struct __is_integral_helper<unsigned char>
        : public true_type { };


      // We want is_integral<wchar_t> to be true (and make_signed/unsigned to work)
        // even when libc doesn't provide working <wchar.h> and related functions,
        // so don't check _GLIBCXX_USE_WCHAR_T here.

    template<>
    struct __is_integral_helper<wchar_t>
        : public true_type { };

    #ifdef _GLIBCXX_USE_CHAR8_T
    template<>
    struct __is_integral_helper<char8_t>
        : public true_type { };
    #endif

    template<>
    struct __is_integral_helper<char16_t>
        : public true_type { };

    template<>
    struct __is_integral_helper<char32_t>
        : public true_type { };

    template<>
    struct __is_integral_helper<short>
        : public true_type { };

    template<>
    struct __is_integral_helper<unsigned short>
        : public true_type { };

    template<>
    struct __is_integral_helper<int>
        : public true_type { };

    template<>
    struct __is_integral_helper<unsigned int>
        : public true_type { };

    template<>
    struct __is_integral_helper<long>
        : public true_type { };

    template<>
    struct __is_integral_helper<unsigned long>
        : public true_type { };

    template<>
    struct __is_integral_helper<long long>
        : public true_type { };

    template<>
    struct __is_integral_helper<unsigned long long>
        : public true_type { };

    template<class _Ty>
    struct is_integral
        :public __is_integral_helper<_Ty> {};


    template<class>
    struct is_floating_point_cv
        : public false_type { };

    template<>
    struct is_floating_point_cv<float>
        : public true_type { };

    template<>
    struct is_floating_point_cv<double>
        : public true_type { };

    template<>
    struct is_floating_point_cv<long double>
        : public true_type { };

    template<class _Ty>
    struct is_floating_point
        : is_floating_point_cv<typename remove_cv<_Ty>::type> {};


    template<class> struct __is_null_pointer_helper : public false_type {};

    template<>
    struct __is_null_pointer_helper<::std::nullptr_t>
        : public true_type {};

    template<class _Ty>
    struct is_null_pointer
        : public __is_null_pointer_helper<typename remove_cv<_Ty>::type>::type
    { };


    template<class>
    struct is_member_pointer :public false_type {};

    template<class _Ty, class _Cy>
    struct is_member_pointer<_Ty _Cy::*> :true_type {};

    template<class _Ty>
    struct is_array :public false_type {};

    template<class _Ty>
    struct is_array<_Ty[]>
        :public true_type {};

    template<class _Ty, std::size_t _sz>
    struct is_array<_Ty[_sz]>
        :public true_type {};


    template<class _Ty>
    constexpr bool is_reference_v = is_pointer<_Ty>::value;

    template<class _Ty>
    constexpr bool is_pointer_v = is_pointer<_Ty>::value;


    template<class _Ty>
    struct is_arithmetic
        : public Calculations_L::or_<is_integral<_Ty>, is_floating_point<_Ty>>::type
    { };

    template<class _Ty>
    struct is_scalar
        : public Calculations_L
        ::or_<is_arithmetic<_Ty>, /* non-define is_enum<_Ty> ,*/is_pointer<_Ty>,
        is_member_pointer<_Ty>, is_null_pointer<_Ty>>::type
    { };


    template<bool, class _Ty = void> struct enable_if {};

    template<class _Ty>
    struct enable_if<true, _Ty> { using type = _Ty; };


    template<class _Ty1, class _Ty2, class _Ty3>
    struct type_convert {};

    template<class _Ty, class _By_Ty>
    struct type_convert< _Ty, _Ty, _By_Ty> { using type = _By_Ty; };


    template<class _Ty>   struct remove_reference { using type = _Ty; };
    template<class _Ty>   struct remove_reference<_Ty&> { using type = _Ty; };
    template<class _Ty>   struct remove_reference<_Ty&&> { using type = _Ty; };


    template<class _Ty>
    using remove_reference_t = typename remove_reference<_Ty>::type;

    template<bool _Cond, class _Ty = void>
    using enable_if_t = typename enable_if<_Cond, _Ty>::type;

    template<class _Ty, class _Tp, class _Tchange>
    using type_convert_t = typename type_convert<_Ty, _Tp, _Tchange>::type;


    //* since declval is complie-time? so declval<_Ty>.fun() rvalue_reference condition ok?
    template<class _Ty>add_rvalue_reference_t<_Ty> declval() noexcept {
        static_assert(_Always_false<_Ty>, "Calling declval is ill-formed, see N4892 [declval]/2.");
    }


};              //*     namesapce MPL_L

#endif          //*     _TYCOMMOM_