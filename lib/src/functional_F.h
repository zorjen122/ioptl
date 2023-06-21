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

#define _FUNCTIONAL_IOP_DEFINE_ 
#ifdef _FUNCTIONAL_IOP_DEFINE_ 

#include "iterator_F.h"
#include "allocator_F.h"
#include "allocator_traits_F.h"
#include "config_def.h"
#include "alloc_operator_new_F.h"
#include "func_exception.h"

#include <typeinfo>


//* Before c++20, See cppreference.
#define BINARY_ARITHMETIC_FUNC_OP(NAME,IS_A_OBJ,RETURN,OP)                  \
    template<class _Ty,class _Tp>                                           \
    struct NAME : public IOP_STL::IS_A_OBJ<_Ty, _Tp, RETURN> {              \
        IOP_CONSTEXPR_CXX17 RETURN                                          \
            operator()(const _Ty& __a, const _Tp& __b) NOEXCEPT{            \
            return (__a OP __b);                                            \
        }                                                                   \
    };   

#define UNARY_ARITHMETIC_FUNC_OP(NAME,IS_A_OBJ,RETURN,OP)                   \
    template<class _Ty>                                                     \
    struct NAME : public IOP_STL::IS_A_OBJ<_Ty,  RETURN> {                  \
        IOP_CONSTEXPR_CXX17 RETURN operator()(const _Ty& __a) NOEXCEPT {    \
            return OP(__a);                                                 \
        }                                                                   \
    };                                                              


namespace IOP_STL {

    template<class _F>
    class function;

    template<class _R, class... _Args>
    class function<_R(_Args...)> {
    private:

        class comom_func_base {
        private:
            friend class function;

            virtual ~comom_func_base() = default;
            virtual comom_func_base* clones() const = 0;
            virtual _R invoke(_Args&&... __args) const = 0;
        };

        template<class _F>
        class comom_func : comom_func_base {
        private:
            friend class function;

            comom_func(const comom_func&) = default;

            explicit comom_func(_F&& __f) : f(FVstd::forward<_F>(__f)) {}

            virtual comom_func_base* clones() const NOEXCEPT {
            //* instantiation *this member f is only _F-null or _F-pfunc.
                return new comom_func(*this);
            }

            virtual _R invoke(_Args&&... __args) const NOEXCEPT {
                return f(FVstd::forward<_Args>(__args)...);
            }

        protected:
            _F f;

        };

    protected:

        comom_func_base* func__;

    public:

        function() : func__(nullptr) {}

        function(::std::nullptr_t) : func__(nullptr) {}

        ~function() { delete func__; }

        function(const function& __src) : func__(__src.func__->clones()) {}

        function(function&& __src) : func__(__src.func__) {
            __src.func__ = nullptr;
        }

        template<class _F>
        function(_F&& __f)
            : func__(new comom_func<_F>(FVstd::forward<_F>(__f))) {}

        IOP_CONSTEXPR_CXX17 void swap(function& ano)
            NOEXCEPT(swap_ptr(
                std::declval<function&>(), std::declval<function&>())
            ) {
            swap_ptr(func__, ano.func__);
        }


        IOP_CONSTEXPR_CXX17 operator bool() const NOEXCEPT { return  func__ != nullptr; }

        IOP_CONSTEXPR_CXX17 _R operator()(_Args&&... __args) const {
            if (static_cast<bool>(*this) == false)
                throw bad_function_call();
            else
                return func__->invoke(FVstd::forward<_Args>(__args)...);
        }

    };


//* The following is removed in c++20 and deprecated in c++17. See cppreference.

    template<class _Ty, class _Tp, class _R>
    class binary_function {
    public:
        using first_type = _Ty;
        using second_type = _Tp;
        using result_type = _R;
    };

    template<class _Ty, class _R>
    class unary_function {
    public:
        using first_type = _Ty;
        using result_type = _R;
    };

    template<class _FOP>
    class bind_unary
        : public unary_function<typename _FOP::first_type, typename _FOP::result_type> {
    protected:
        _FOP f;
        typename _FOP::first_type v;

    public:
        explicit bind_unary() : f(_FOP()), v() {}
        bind_unary(const _FOP& __p) : f(__p) {}
        bind_unary(const _FOP& __p, typename _FOP::first_type __c) : f(__p), v(__c) {}

        IOP_CONSTEXPR_CXX17 typename _FOP::result_type
            operator()(typename _FOP::first_type __c)  NOEXCEPT {
            return f(__c);
        }

        IOP_CONSTEXPR_CXX17 typename _FOP::result_type operator()() NOEXCEPT {
            return f(v);
        }

    };


    template<class _FOP>
    class bind_binary
        : public binary_function<typename _FOP::first_type, typename _FOP::second_type,
        typename _FOP::result_type> {
    protected:
        _FOP f;
        typename _FOP::first_type fv;
        typename _FOP::second_type sv;

    public:
        explicit bind_binary() : f(_FOP()), fv(), sv() {}
        bind_binary(const _FOP& __p) : f(__p) {}
        bind_binary(const _FOP& __p,
            typename _FOP::first_type __a, typename _FOP::second_type __b)
            : f(__p), fv(__a), sv(__b) {}

        IOP_CONSTEXPR_CXX17 typename _FOP::result_type operator()
            (typename _FOP::first_type __a, typename _FOP::second_type __b) NOEXCEPT {
            return f(__a, __b);
        }

        IOP_CONSTEXPR_CXX17 typename _FOP::result_type operator()() NOEXCEPT {
            return f(fv, sv);
        }

    };


    template<class _FOP1, class _FOP2, class _FOP3>
    class double_bind
        : public unary_function<typename _FOP2::first_type,
        typename _FOP1::result_type> {
    protected:
        _FOP1 f1;
        _FOP2 f2;
        _FOP3 f3;
        typename _FOP3::first_type v;

    public:
        explicit double_bind() NOEXCEPT : f1(_FOP1()), f2(_FOP2()), f3(_FOP3()), v() {}

        double_bind(const _FOP1& __p1, const _FOP2& __p2, const _FOP3& __p3,
            typename _FOP3::first_type __c) : f1(__p1), f2(__p2), f3(__p3), v(__c) {}

        double_bind(const _FOP1& __p1, const _FOP2& __p2, const _FOP3& __p3)
            : f1(__p1), f2(__p2), f3(__p3) {}


        IOP_CONSTEXPR_CXX17 typename _FOP1::result_type
            operator()(typename _FOP3::first_type __c) NOEXCEPT {
            return f1(f2(f3(__c)));
        }

        IOP_CONSTEXPR_CXX17 typename _FOP1::result_type operator()() NOEXCEPT {
            return f1(f2(f3(v)));
        }
    };

    template<class _R, class _Ty>
    class pointer_unary_function : public unary_function<_Ty, _R> {
    protected:
        _R(*f)(_Ty) = nullptr;
    public:
        explicit pointer_unary_function() NOEXCEPT {}
        pointer_unary_function(_R(*__p)(_Ty)) : f(__p) {}

        IOP_CONSTEXPR_CXX17 _R operator()(_Ty&& __v) const NOEXCEPT {
            return f(FVstd::forward<_Ty>(__v));
        }
    };

    template<class _R, class _Ty, class _Tp>
    class pointer_binary_function : public binary_function<_Ty, _Tp, _R> {
    protected:
        _R(*f)(_Ty, _Tp) = nullptr;
    public:
        explicit pointer_binary_function() {}
        pointer_binary_function(_R(*__p)(_Ty, _Tp)) : f(__p) {}

        IOP_CONSTEXPR_CXX17 _R operator()(_Ty&& __a, _Tp&& __b) const NOEXCEPT {
            return f(FVstd::forward<_Ty>(__a), FVstd::forward<_Tp>(__b));
        }
    };

    template<class _R, class _Ty>
    IOP_CONSTEXPR_CXX17 pointer_unary_function<_R, _Ty>
        ptr_fun(_R(*__f)(_Ty)) NOEXCEPT {
        return pointer_unary_function<_R, _Ty>(__f);
    }

    template<class _R, class _Ty, class _Tp>
    IOP_CONSTEXPR_CXX17 pointer_binary_function<_R, _Ty, _Tp>
        ptr_fun(_R(*__f)(_Ty, _Tp)) NOEXCEPT {
        return pointer_binary_function<_R, _Ty, _Tp>(__f);
    }


    template<class _R, class _Ty>
    class mem_fun_t : public unary_function<_Ty*, _R> {
    protected:
        _R(_Ty::* f)();
    public:
        explicit mem_fun_t(_R(_Ty::* __p)()) :f(__p) {}

        IOP_CONSTEXPR_CXX17 _R operator()(_Ty* __p) const NOEXCEPT { return (__p->*f)(); }
    };

    template<class _R, class _Ty>
    class mem_fun_ref_t : public unary_function<_Ty, _R> {
    protected:
        _R(_Ty::* f)();
    public:
        explicit mem_fun_ref_t(_R(_Ty::* __p)()) :f(__p) {}

        IOP_CONSTEXPR_CXX17 _R& operator()(_Ty& __p) const NOEXCEPT { return (__p.*f)(); }
    };

    template<class _R, class _Ty>
    class const_mem_fun_t : public unary_function<_Ty*, _R> {
    protected:
        _R(_Ty::* f)() const;
    public:
        explicit const_mem_fun_t(_R(_Ty::* __p)() const) :f(__p) {}

        IOP_CONSTEXPR_CXX17 _R operator()(_Ty* __p) const NOEXCEPT { return (__p->*f)(); }
    };

    template<class _R, class _Ty>
    class const_mem_fun_ref_t : public unary_function<_Ty, _R> {
    protected:
        _R(_Ty::* f)() const;
    public:
        explicit const_mem_fun_ref_t(_R(_Ty::* __p)()) :f(__p) {}

        IOP_CONSTEXPR_CXX17 _R& operator()(_Ty& __p) const NOEXCEPT { return (__p.*f)(); }
    };



    UNARY_ARITHMETIC_FUNC_OP(negate, unary_function, bool, -);
    UNARY_ARITHMETIC_FUNC_OP(logical_not, unary_function, bool, !);
    UNARY_ARITHMETIC_FUNC_OP(identity, unary_function, bool, _Ty);



    BINARY_ARITHMETIC_FUNC_OP(plus, binary_function, _Ty, +);
    BINARY_ARITHMETIC_FUNC_OP(minus, binary_function, _Ty, -);
    BINARY_ARITHMETIC_FUNC_OP(mult, binary_function, _Ty, *);
    BINARY_ARITHMETIC_FUNC_OP(mod, binary_function, _Ty, %);
    BINARY_ARITHMETIC_FUNC_OP(divides, binary_function, _Ty, / );


    BINARY_ARITHMETIC_FUNC_OP(greater, binary_function, bool, > );
    BINARY_ARITHMETIC_FUNC_OP(less, binary_function, bool, < );
    BINARY_ARITHMETIC_FUNC_OP(greater_equal, binary_function, bool, >= );
    BINARY_ARITHMETIC_FUNC_OP(less_equal, binary_function, bool, <= );


    BINARY_ARITHMETIC_FUNC_OP(logical_and, binary_function, bool, &&);
    BINARY_ARITHMETIC_FUNC_OP(logical_or, binary_function, bool, || );
    BINARY_ARITHMETIC_FUNC_OP(equal_to, binary_function, bool, == );
    BINARY_ARITHMETIC_FUNC_OP(unequal_to, binary_function, bool, != );

};      // namespace IOP_STL

#endif