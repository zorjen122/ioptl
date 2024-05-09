/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _DEFTYPE_
#define _DEFTYPE_

#include "cppconfig.h"
#include "metacomponent.h"
#include "type_traits.h"

#include <memory>

namespace Fiop{

    template <class _Ty>
    constexpr void *_Voidify_ptr(_Ty __p) noexcept
    {
        if constexpr(mpls::is_pointer_v<_Ty>)
        {
            return const_cast<void *>(static_cast<const volatile void *>(__p));
        }
        else
        {
            return const_cast<void *>(
                static_cast<const volatile void *>(::std::addressof(__p)));
        }
    }

};  // namespace Fiop

#endif // *_DEFTYPE_