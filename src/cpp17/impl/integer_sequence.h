/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */


#pragma once

#ifndef _INTEGER_SEQUENCE_IOP_DEFINE_ 
#define _INTEGER_SEQUENCE_IOP_DEFINE_ 

#include "../cppconfig.h"
#include "../type_traits.h"


template<class _Id, _Id... _V>
struct __integer_sequence {
    template <template<class _Ty, _Ty... _Val> class _OthSeq, class _ToTy>
    using __other = _OthSeq<_ToTy, _V...>;
};


template<class _Ty, size_t... _V> struct __repeat;

template<class _Ty, _Ty... _Ny, size_t... _V>
struct __repeat<__integer_sequence<_Ty, _Ny...>, _V...> {
    using type = __integer_sequence<_Ty,
        _Ny...,
        1 * sizeof...(_Ny) + _Ny...,
        2 * sizeof...(_Ny) + _Ny...,
        3 * sizeof...(_Ny) + _Ny...,
        4 * sizeof...(_Ny) + _Ny...,
        5 * sizeof...(_Ny) + _Ny...,
        6 * sizeof...(_Ny) + _Ny...,
        7 * sizeof...(_Ny) + _Ny...,
        _V...>;
};


template<size_t _Np>
struct __parity;

template<size_t _Np>
struct __make : __parity<_Np % 8>::template __pmake<_Np> {};

template<> struct __make<0> { using type = __integer_sequence<size_t>; };
template<> struct __make<1> { using type = __integer_sequence<size_t, 0>; };
template<> struct __make<2> { using type = __integer_sequence<size_t, 0, 1>; };
template<> struct __make<3> { using type = __integer_sequence<size_t, 0, 1, 2>; };
template<> struct __make<4> { using type = __integer_sequence<size_t, 0, 1, 2, 3>; };
template<> struct __make<5> { using type = __integer_sequence<size_t, 0, 1, 2, 3, 4>; };
template<> struct __make<6> { using type = __integer_sequence<size_t, 0, 1, 2, 3, 4, 5>; };
template<> struct __make<7> { using type = __integer_sequence<size_t, 0, 1, 2, 3, 4, 5, 6>; };


template<> struct __parity<0> {
    template<size_t _V> struct __pmake : __repeat<typename __make<_V / 8>::type> {};
};

template<> struct __parity<1> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 1> {};
};
template<> struct __parity<2> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 2, _V - 1> {};
};
template<> struct __parity<3> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 3, _V - 2, _V - 1> {};
};
template<> struct __parity<4> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 4, _V - 3, _V - 2, _V - 1> {};
};
template<> struct __parity<5> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 5, _V - 4, _V - 3, _V - 2, _V - 1> {};
};
template<> struct __parity<6> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 6, _V - 5, _V - 4, _V - 3, _V - 2, _V - 1> {};
};
template<> struct __parity<7> {
    template<size_t _V> struct __pmake
        : __repeat<typename __make<_V / 8>::type, _V - 7, _V - 6, _V - 5, _V - 4, _V - 3, _V - 2, _V - 1> {};
};

template<class _Ty, _Ty... _V>
struct integer_sequence_impl {
    static_assert(mpls::is_integral_v<_Ty>,
        "integer_sequence can only be instantiated with an integral type");

    using value_type = _Ty;
    static constexpr size_t size() noexcept { return sizeof...(_V); }
};


template<class _Ty, _Ty _N>
using __make_integer_sequence_check_impl
= typename __make<_N>::type::template __other<integer_sequence_impl, _Ty>;

template<class _Ty, _Ty _N>
struct __make_integer_sequence_check
{
    static_assert(mpls::is_integral_v<_Ty>,
        "make_integer_sequence can only be instantiated with an integral type");
    static_assert(0 <= _N,
        "make_integer_sequence must have a non - negative sequence length");

 /*
  * In Gcc, Workaround GCC bug by preventing bad installations when 0 <= _N.
  *   see reference https://gcc.gnu.org/bugzilla/show_bug.cgi?id=68929
  * For error examples provided by Eric Fusellier
  *   see reference https://stackoverflow.com/questions/20441293static-assert-does-not-break-compiling-immediately
  */

    using type = __make_integer_sequence_check_impl<_Ty, 0 <= _N ? _N : 0>;
};



#endif      // _INTEGER_SEQUENCE_IOP_DEFINE_