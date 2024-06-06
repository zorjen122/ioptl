/*
 * Copyright(c) 1997     zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 *that both that copyright notice and this permission notice appear
 * in supporting documentation.Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.It is provided "as is" without express or implied warranty.
 */

#ifndef _IOP_STACK_DEF
#define _IOP_STACK_DEF

#include "dequeF.h"
#include "../util/utilityF.h"

#include <initializer_list>

namespace iop {

    template <class _Ty, class _Container = iop::deque<_Ty>> class stack
    {
      public:
        using value_type = typename _Container::value_type;
        using size_type = typename _Container::size_type;
        using difference_type = typename _Container::difference_type;

        using pointer = typename _Container::pointer;
        using const_pointer = typename _Container::const_pointer;
        using reference = typename _Container::reference;
        using const_reference = typename _Container::const_reference;

      protected:
        _Container V_stack;

      public:
        stack() : V_stack() {}

        stack(const stack &__res) : V_stack(__res.V_stack) {}
        stack(stack &&__res) : V_stack(__res.V_stack) {}
        stack(const ::std::initializer_list<_Ty> &__res) : V_stack(__res) {}

        reference top() { return V_stack.back(); }

        constexpr void push(const_reference __v)
        {
            V_stack.push_back(__v);
        }
        constexpr void pop() { V_stack.pop_back(); }
        constexpr void swap(stack &__res)
        {
            V_stack.swap(__res.V_stack);
        }
        constexpr size_type size() { return V_stack.size(); }
        constexpr bool empty() { return V_stack.empty(); }

        stack &operator=(const stack &__res) { V_stack = __res.V_stack; }

        stack &&operator=(stack &&__res)
        {
            V_stack = Fiop::move(__res.V_stack);
        }
    };

}; //  namespace iop

#endif // _IOP_STACK_DEF