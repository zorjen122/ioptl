/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#define _UTILITY_IOP_DEFINE_
#ifdef _UTILITY_IOP_DEFINE_

#include "./impl/integer_sequence.h"
#include "cppconfig.h"
#include "metacomponent.h"
#include "type_traits.h"

namespace Fiop {

    // forward
    template <class _Ty>
    [[__nodiscard__]] constexpr _Ty &&
    forward(mpls::remove_reference_t<_Ty> &__tl) NOEXCEPT
    {
        return static_cast<_Ty &&>(__tl);
    }

    template <class _Ty>
    [[__nodiscard__]] constexpr _Ty &&
    forward(mpls::remove_reference_t<_Ty> &&__tl) NOEXCEPT
    {
        static_assert(!mpls::is_lvalue_reference<_Ty>::value,
                      "cannot forward an rvalue as an lvalue");

        return static_cast<_Ty &&>(__tl);
    }

    // move
    template <class _Ty>
    [[__nodiscard__]] constexpr auto&&
    move(_Ty&& __tl) NOEXCEPT
    {
        return static_cast<mpls::remove_reference_t<_Ty>&&>(__tl);
    }

    // to_address
    template <class _Ty> [[__nodiscard__]] constexpr _Ty *to_address(_Ty *__ptr)
    {
        static_assert(!mpls::is_function_v<_Ty>,
                      "Error: function is not basic pointer type");

        return __ptr;
    }

    template <class _Ty>
    [[__nodiscard__]] constexpr _Ty &to_address(const _Ty &__ref)
    {
        return __ref.operator->();
    }

    template <class _Ty, _Ty... _V>
    using integer_sequence = integer_sequence_impl<_Ty, _V...>;

    template <class _Ty, _Ty _N>
    using make_integer_sequence =
        typename __make_integer_sequence_check<_Ty, _N>::type;

    template <size_t... _V>
    using index_sequence = integer_sequence<size_t, _V...>;

    template <size_t _N>
    using make_index_sequence = make_integer_sequence<size_t, _N>;

    template <class... _Ty>
    using index_sequence_for = make_index_sequence<sizeof...(_Ty)>;

}; // namespace Fiop

#endif // _UTILITY_IOP_DEFINE_