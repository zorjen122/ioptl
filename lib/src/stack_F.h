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


#pragma once

#define _STACK_IOP_DEFINE_
#ifdef _STACK_IOP_DEFINE_

#include "deque_F.h"
#include "list_F.h"
#include "vector_F.h"


namespace IOP_STL {

    template<class _Ty, class _Sequence = IOP_STL::deque<_Ty>>
    class stack {
    public:
        using value_type = typename _Sequence::value_type;
        using size_type = typename _Sequence::size_type;
        using difference_type = typename _Sequence::difference_type;

        using pointer = typename _Sequence::pointer;
        using const_pointer = typename _Sequence::const_pointer;
        using reference = typename _Sequence::reference;
        using const_reference = typename _Sequence::const_reference;

    protected:

        _Sequence V_stack;

    public:
        stack() : V_stack() {}

        stack(const stack<_Ty, _Sequence>& __res) : V_stack(__res.V_stack) {}
        stack(stack<_Ty, _Sequence>&& __res) { __res.V_stack = FVstd::move(__res); }

        pointer top() { return (*end() - 1); }

        void push(const_reference __v) { V_stack.push_back(__v); }
        void pop() { V_stack.pop_back(); }
        void size() { return V_stack.size(); }
        bool empty() { return V_stack.empty(); }
        void swap(stack<_Ty, _Sequence>& __res) { V_stack.swap(__res.V_stack); }

        stack<_Ty, _Sequence>& operator=(const stack<_Ty, _Sequence>& __res) {
            V_stack = __res.V_stack;
        }

        stack<_Ty, _Sequence>&& operator=(stack<_Ty, _Sequence>&& __res) {
            V_stack = std::move(__res.V_stack);
        }
    }

};      //  namespace IOP_STL

#endif      // _STACK_IOP_DEFINE_