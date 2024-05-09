/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef __METACOMPONENT_MPL__
#define __METACOMPONENT_MPL__

#define P_TYPE_INTEGRAL_CONST(_Ty)                                             \
    template <_Ty val> struct integral_const<_Ty, val>                         \
    {                                                                          \
        using value_type = _Ty;                                                \
        using type = integral_const;                                           \
        using init_type = integral_const<_Ty, val>;                            \
        using point_type = integral_const<_Ty, val - 1>;                       \
        using next_type = integral_const<_Ty, val + 1>;                        \
        static constexpr value_type value = val;                               \
        constexpr operator value_type() const noexcept { return value; }       \
    };

/* Comom arithmetic */
#define P_MULT_ARITHMETIC_OP(name, op)                                         \
    template <class N1, class N2, class... Nargs> struct name                  \
    {                                                                          \
        using inner = name<N2, Nargs...>;                                      \
        using value_type =                                                     \
            typename largest_int<typename N1::value_type,                      \
                                 typename inner::value_type>::type;            \
        using type = integral_const<value_type, N1::value op inner::value>;    \
        static constexpr value_type value = type::value;                       \
    };                                                                         \
    template <class N1, class N2> struct name<N1, N2>                          \
    {                                                                          \
        using value_type =                                                     \
            typename largest_int<typename N1::value_type,                      \
                                 typename N2::value_type>::type;               \
        using type = integral_const<value_type, (N1::value op N2::value)>;     \
        static constexpr value_type value = type::value;                       \
    };

// some arithmetic only make non-mult meta-function for such which.
#define P_UNARY_ARITHMETIC_OP(name, op)                                          \
    template <class N1, class N2> struct name                                  \
    {                                                                          \
        using value_type =                                                     \
            typename largest_int<typename N1::value_type,                      \
                                 typename N2::value_type>::type;               \
        using type = integral_const<value_type, (N1::value op N2::value)>;     \
        static constexpr value_type value = type::value;                       \
    };

/* relational operation */
#define P_UNARY_RELATIONAL_OP_BOOL(name, op)                                     \
    template <class N1, class N2> struct name                                  \
    {                                                                          \
        using value_type = bool;                                               \
        using type = integral_const<value_type, (N1::value op N2::value)>;     \
        static constexpr value_type value = type::value;                       \
    };

/* Mult logical */
#define P_MULT_RELATIONAL_OP_BOOL(name, op)                                    \
    template <class _Ty, class _Tys, class... _Args> struct name               \
    {                                                                          \
        using value_type = bool;                                               \
        using liner = name<_Tys, _Args...>;                                    \
        using type = integral_const<value_type, (_Ty::value op liner::value)>; \
        static constexpr value_type value = type::value;                       \
    };                                                                         \
    template <class _Ty, class _Tys> struct name<_Ty, _Tys>                    \
    {                                                                          \
        using value_type = bool;                                               \
        using type = integral_const<value_type, (_Ty::value op _Tys::value)>;  \
        static constexpr value_type value = type::value;                       \
    };

namespace mpls {
    template <class _Ty, _Ty val> struct integral_const
    {};

    template <bool val> struct integral_const<bool, val>
    {
        using value_type = bool;
        using type = integral_const;
        static constexpr value_type value = val;

        constexpr operator value_type() const noexcept { return value; }
    };

    P_TYPE_INTEGRAL_CONST(int);
    P_TYPE_INTEGRAL_CONST(unsigned int);
    P_TYPE_INTEGRAL_CONST(long long);

    template <int N> using int_ = integral_const<int, N>;
    template <unsigned int N> using uint_ = integral_const<unsigned int, N>;
    template <long long N> using ll_ = integral_const<long long, N>;
    template <short N> using short_ = integral_const<short, N>;
    template <bool N> using bool_ = integral_const<bool, N>;

    using true_type = bool_<true>;
    using false_type = bool_<false>;

    template <class...> using __void_t = void;

    template <class> constexpr bool _always_false = false;
    template <class> constexpr bool _always_true = true;

    template <class... Args> struct seq
    {};
    template <class _S, class _T> struct seq_push_back;

    template <class _T, class... Args> struct seq_push_back<seq<Args...>, _T>
    {
        using type = seq<Args..., _T>;
    };

    template <bool Judge, class _T, class _Ty> struct if_bool
    {
        using type = _T;
    };

    template <class _T, class _Ty> struct if_bool<false, _T, _Ty>
    {
        using type = _Ty;
    };

    template <class Judge_traits, class _T, class _Ty> struct if_
    {
        using type = typename if_bool<!!Judge_traits::value, _T, _Ty>::type;
    };

    template <bool _Cond, class _Ty, class _Tp>
    using conditioni_t = typename if_bool<!!_Cond, _Ty, _Tp>::type;

    template <typename T> struct integral_rank;

    template <> struct integral_rank<bool> : uint_<1>
    {};
    template <> struct integral_rank<signed char> : uint_<2>
    {};
    template <> struct integral_rank<char> : uint_<3>
    {};
    template <> struct integral_rank<unsigned char> : uint_<4>
    {};
    template <> struct integral_rank<wchar_t> : uint_<5>
    {};
    template <> struct integral_rank<char16_t> : uint_<6>
    {};
    template <> struct integral_rank<short> : uint_<7>
    {};
    template <> struct integral_rank<unsigned short> : uint_<8>
    {};
    template <> struct integral_rank<char32_t> : uint_<9>
    {};
    template <> struct integral_rank<int> : uint_<10>
    {};
    template <> struct integral_rank<unsigned int> : uint_<11>
    {};
    template <> struct integral_rank<long> : uint_<12>
    {};
    template <> struct integral_rank<unsigned long> : uint_<13>
    {};
    template <> struct integral_rank<long long> : uint_<14>
    {};
    template <> struct integral_rank<unsigned long long> : uint_<15>
    {};

    // since if_ immediate using value such will cannot operator in non-computea
    // field
    template <class T1, class T2>
    using largest_int =
        if_bool<integral_rank<T1>::value >= integral_rank<T2>::value, T1, T2>;

    namespace oper {

        template <class T> struct negate
        {
            using value_type = typename T::value_type;
            using type = integral_const<value_type, -T::value>;
            static constexpr value_type value = type::value;
        };

        P_MULT_ARITHMETIC_OP(plus, +);
        P_MULT_ARITHMETIC_OP(sub, -);
        P_MULT_ARITHMETIC_OP(mult, *);
        P_MULT_ARITHMETIC_OP(bitand_, &);
        P_MULT_ARITHMETIC_OP(bitor_, |);
        P_MULT_ARITHMETIC_OP(bitxor_, ^);

        P_UNARY_ARITHMETIC_OP(minus, *);
        P_UNARY_ARITHMETIC_OP(divides, /);
        P_UNARY_ARITHMETIC_OP(modulus, %);
        P_UNARY_ARITHMETIC_OP(left_shift, <<);
        P_UNARY_ARITHMETIC_OP(right_shift, >>);

        P_UNARY_RELATIONAL_OP_BOOL(equal_to, ==);
        P_UNARY_RELATIONAL_OP_BOOL(not_equal_to, !=);
        P_UNARY_RELATIONAL_OP_BOOL(greater, >);
        P_UNARY_RELATIONAL_OP_BOOL(greater_equal, >=);
        P_UNARY_RELATIONAL_OP_BOOL(less, <);
        P_UNARY_RELATIONAL_OP_BOOL(less_equal, <=);

        P_MULT_RELATIONAL_OP_BOOL(and_, &&);
        P_MULT_RELATIONAL_OP_BOOL(or_, ||);

        template <class _Ty> struct not_
        {
            using value_type = bool;
            using type = integral_const<value_type, !_Ty::value>;
            static constexpr value_type value = type::value;
        };

    }; // namespace oper

}; // namespace mpls

#endif // __METACOMPONENT_MPL__