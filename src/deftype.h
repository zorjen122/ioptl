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

#define _DEFTYPE_
#ifdef _DEFTYPE_

#include <memory>
#include "tycommom.h"
#include "config_def.h"

template<class _Iter>
CCC_CONSTEXPR_CXX17 void* _Voidify_iter(_Iter _it) noexcept {
    CCC_SPECIAL_IF_COSTEXPR_CXX17(MPL_L::is_pointer_v<_Iter>) {
        return const_cast<void*>(static_cast<const volatile void*>(_it));
    }
    else {
        return const_cast<void*>(static_cast<const volatile void*>(::std::addressof(*_it)));
    }
}



#endif          // *_DEFTYPE_