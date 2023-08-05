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

#define _FUNC_EXPCEPTION_IOP_DEFINE
#ifdef _FUNC_EXPCEPTION_IOP_DEFINE

#include <stdexcept>
#include <exception>

namespace iop {

    class bad_function_call : public ::std::exception {
    public:
        virtual const char* what() const noexcept
        {
            return "bad function call";
        }
    };

    class bad_range : public ::std::exception {
    public:
        virtual const char* what() const NOEXCEPT {
            return "call out_of_range";
        }
    };

};          // namespace iop

#endif      // _FUNC_EXPCEPTION_IOP_DEFINE