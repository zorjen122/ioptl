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
#define _FORWARD_
#ifdef _FORWARD_

#include "tycommom.h"
#include "config_def.h"

namespace FVstd {

    template<class _Ty>
    [[__nodiscard__]]
    CCC_CONSTEXPR _Ty&& forward(MPL_L::remove_reference_t<_Ty>& __tl) CCC_NOEXCEPT {
        return static_cast<_Ty&&>(__tl);
    }


    template<class _Ty>
    [[__nodiscard__]]
    CCC_CONSTEXPR _Ty&& forward(MPL_L::remove_reference_t<_Ty>&& __tl) CCC_NOEXCEPT {
        static_assert(!MPL_L::is_lvalue_reference<_Ty>::value,
            "cannot forward an rvalue as an lvalue");

        return static_cast<_Ty&&>(__tl);
    }
};


#endif