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

#define _DEFTYPE_
#ifdef _DEFTYPE_

#include "type_traits.h"
#include "metacomponent.h"
#include "cppconfig.h"

#include <memory>

template<class _Ty>
IOP_CONSTEXPR_CXX17 void* _Voidify_ptr(_Ty __p) noexcept {
    IOP_SPECIAL_IF_COSTEXPR_CXX17(mpls::is_pointer_v<_Ty>) {
        return const_cast<void*>(static_cast<const volatile void*>(__p));
    }
    else {
        return const_cast<void*>(static_cast<const volatile void*>(::std::addressof(__p)));
    }
}



#endif          // *_DEFTYPE_