/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#define _FUNC_EXPCEPTION_IOP_DEFINE
#ifdef _FUNC_EXPCEPTION_IOP_DEFINE

#include <exception>
#include <stdexcept>

namespace iop {

    class bad_function_call : public ::std::exception
    {
      public:
        virtual const char *what() const noexcept
        {
            return "bad function call";
        }
    };

    class bad_range : public ::std::exception
    {
      public:
        virtual const char *what() const noexcept
        {
            return "call out_of_range";
        }
    };

    
    class bad_alloc : public ::std::exception
    {
      public:
        virtual const char *what() const noexcept
        {
            return "bad alloc call";
        }
    };

}; // namespace iop

#endif // _FUNC_EXPCEPTION_IOP_DEFINE