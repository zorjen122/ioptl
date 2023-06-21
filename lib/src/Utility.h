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

#define _UTILITY_IOP_DEFINE_ 
#ifdef  _UTILITY_IOP_DEFINE_ 

#include "type_trait.h"
#include "metacomponent.h"
#include "config_def.h"
#include "integer_sequence.h"


namespace FVstd {

    template<class _Ty>
    [[__nodiscard__]]
    IOP_CONSTEXPR _Ty&& forward(MPL_L::remove_reference_t<_Ty>& __tl) NOEXCEPT {
        return static_cast<_Ty&&>(__tl);
    }

    template<class _Ty>
    [[__nodiscard__]]
    IOP_CONSTEXPR _Ty&& forward(MPL_L::remove_reference_t<_Ty>&& __tl) NOEXCEPT {
        static_assert(!MPL_L::is_lvalue_reference<_Ty>::value,
            "cannot forward an rvalue as an lvalue");

        return static_cast<_Ty&&>(__tl);
    }

    template<class _Ty>
    [[__nodiscard__]]
    IOP_CONSTEXPR _Ty&& move(MPL_L::remove_reference_t<_Ty>&& __tl) NOEXCEPT {
        return static_cast<_Ty&&>(__tl);
    }


    template<class _Ty, _Ty... _V>
    using integer_sequence = integer_sequence_impl<_Ty, _V...>;

    template<class _Ty, _Ty _N>
    using make_integer_sequence = typename __make_integer_sequence_check<_Ty, _N>::type;

    template<size_t... _V>
    using index_sequence = integer_sequence<size_t, _V...>;

    template<size_t _N>
    using make_index_sequence = make_integer_sequence<size_t, _N>;

    template<class... _Ty>
    using index_sequence_for = make_index_sequence<sizeof...(_Ty)>;


};          // namespace FVstd


#endif      // _UTILITY_IOP_DEFINE_