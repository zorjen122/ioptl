/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _FUNCTIONAL_IOP_DEFINE_
#define _FUNCTIONAL_IOP_DEFINE_

#include "../allocatorF.h"
#include "../iteratorF.h"
#include "../cppconfig.h"

#include "../traits/allocator_traitsF.h"
#include "../util/exceptionF.h"

#include <typeinfo>   // ?

//* Before c++20, See cppreference.
#define BINARY_ARITHMETIC_FUNC_OP(NAME, IS_A_OBJ, RETURN, OP)                  \
    template <class _Ty, class _Tp = _Ty>                                      \
    struct NAME : public IS_A_OBJ<_Ty, _Tp, RETURN>                            \
    {                                                                          \
        constexpr RETURN operator()(const _Ty &__a,                  \
                                              const _Tp &__b) noexcept         \
        {                                                                      \
            return (__a OP __b);                                               \
        }                                                                      \
    };

#define UNARY_ARITHMETIC_FUNC_OP(NAME, IS_A_OBJ, RETURN, OP)                   \
    template <class _Ty> struct NAME : public IS_A_OBJ<_Ty, RETURN>            \
    {                                                                          \
        constexpr RETURN operator()(const _Ty &__a) noexcept         \
        {                                                                      \
            return OP(__a);                                                    \
        }                                                                      \
    };

namespace iop {

    template <class _F> class function;

    template <class _R, class... _Args> class function<_R(_Args...)>
    {
      private:
        class comom_func_base
        {
          private:
            friend class function;

            virtual ~comom_func_base() = default;
            virtual comom_func_base *clones() const = 0;
            virtual _R invoke(_Args &&...__args) const = 0;
        };

        template <class _F> class comom_func : comom_func_base
        {
          private:
            friend class function;

            comom_func(const comom_func &) = default;

            explicit comom_func(_F &&__f) : f(Fiop::forward<_F>(__f)) {}

            virtual comom_func_base *clones() const noexcept
            {
                //* instantiation *this member f is only _F-null or _F-pfunc.
                return new comom_func(*this);
            }

            virtual _R invoke(_Args &&...__args) const noexcept
            {
                return f(Fiop::forward<_Args>(__args)...);
            }

          protected:
            _F f;
        };

      protected:
        comom_func_base *func__;

      public:
        function() : func__(nullptr) {}

        function(::std::nullptr_t) : func__(nullptr) {}

        ~function() { delete func__; }

        function(const function &__src) : func__(__src.func__->clones()) {}

        function(function &&__src) : func__(__src.func__)
        {
            __src.func__ = nullptr;
        }

        template <class _F>
        function(_F &&__f) : func__(new comom_func<_F>(Fiop::forward<_F>(__f)))
        {}

        constexpr void swap(function &ano)
            noexcept(swap_ptr(::std::declval<function &>(),
                              ::std::declval<function &>()))
        {
            swap_ptr(func__, ano.func__);
        }

        constexpr operator bool() const noexcept
        {
            return func__ != nullptr;
        }

        constexpr _R operator()(_Args &&...__args) const
        {
            if (static_cast<bool>(*this) == false)
                throw bad_function_call();
            else
                return func__->invoke(Fiop::forward<_Args>(__args)...);
        }
    };

    //* The following is removed in c++20 and deprecated in c++17. See
    // cppreference.

    template <class _Ty, class _Tp, class _R> class binary_function
    {
      public:
        using first_type = _Ty;
        using second_type = _Tp;
        using result_type = _R;
    };

    template <class _Ty, class _R> class unary_function
    {
      public:
        using first_type = _Ty;
        using result_type = _R;
    };

    template <class _FOP>
    class bind_unary : public unary_function<typename _FOP::first_type,
                                             typename _FOP::result_type>
    {
      protected:
        _FOP f;
        typename _FOP::first_type v;

      public:
        explicit bind_unary() : f(_FOP()), v() {}
        bind_unary(const _FOP &__p) : f(__p) {}
        bind_unary(const _FOP &__p, typename _FOP::first_type __c)
            : f(__p), v(__c)
        {}

        constexpr typename _FOP::result_type
        operator()(typename _FOP::first_type __c) noexcept
        {
            return f(__c);
        }

        constexpr typename _FOP::result_type operator()() noexcept
        {
            return f(v);
        }
    };

    template <class _FOP>
    class bind_binary : public binary_function<typename _FOP::first_type,
                                               typename _FOP::second_type,
                                               typename _FOP::result_type>
    {
      protected:
        _FOP f;
        typename _FOP::first_type fv;
        typename _FOP::second_type sv;

      public:
        explicit bind_binary() : f(_FOP()), fv(), sv() {}
        bind_binary(const _FOP &__p) : f(__p) {}
        bind_binary(const _FOP &__p, typename _FOP::first_type __a,
                    typename _FOP::second_type __b)
            : f(__p), fv(__a), sv(__b)
        {}

        constexpr typename _FOP::result_type
        operator()(typename _FOP::first_type __a,
                   typename _FOP::second_type __b) noexcept
        {
            return f(__a, __b);
        }

        constexpr typename _FOP::result_type operator()() noexcept
        {
            return f(fv, sv);
        }
    };

    template <class _FOP1, class _FOP2, class _FOP3>
    class double_bind : public unary_function<typename _FOP2::first_type,
                                              typename _FOP1::result_type>
    {
      protected:
        _FOP1 f1;
        _FOP2 f2;
        _FOP3 f3;
        typename _FOP3::first_type v;

      public:
        explicit double_bind() noexcept : f1(_FOP1()),
                                          f2(_FOP2()),
                                          f3(_FOP3()),
                                          v()
        {}

        double_bind(const _FOP1 &__p1, const _FOP2 &__p2, const _FOP3 &__p3,
                    typename _FOP3::first_type __c)
            : f1(__p1), f2(__p2), f3(__p3), v(__c)
        {}

        double_bind(const _FOP1 &__p1, const _FOP2 &__p2, const _FOP3 &__p3)
            : f1(__p1), f2(__p2), f3(__p3)
        {}

        constexpr typename _FOP1::result_type
        operator()(typename _FOP3::first_type __c) noexcept
        {
            return f1(f2(f3(__c)));
        }

        constexpr typename _FOP1::result_type operator()() noexcept
        {
            return f1(f2(f3(v)));
        }
    };

    template <class _R, class _Ty>
    class pointer_unary_function : public unary_function<_Ty, _R>
    {
      protected:
        _R (*f)(_Ty) = nullptr;

      public:
        explicit pointer_unary_function() noexcept {}
        pointer_unary_function(_R (*__p)(_Ty)) : f(__p) {}

        constexpr _R operator()(_Ty &&__v) const noexcept
        {
            return f(Fiop::forward<_Ty>(__v));
        }
    };

    template <class _R, class _Ty, class _Tp>
    class pointer_binary_function : public binary_function<_Ty, _Tp, _R>
    {
      protected:
        _R (*f)(_Ty, _Tp) = nullptr;

      public:
        explicit pointer_binary_function() {}
        pointer_binary_function(_R (*__p)(_Ty, _Tp)) : f(__p) {}

        constexpr _R operator()(_Ty &&__a, _Tp &&__b) const noexcept
        {
            return f(Fiop::forward<_Ty>(__a), Fiop::forward<_Tp>(__b));
        }
    };

    template <class _R, class _Ty>
    constexpr pointer_unary_function<_R, _Ty>
    ptr_fun(_R (*__f)(_Ty)) noexcept
    {
        return pointer_unary_function<_R, _Ty>(__f);
    }

    template <class _R, class _Ty, class _Tp>
    constexpr pointer_binary_function<_R, _Ty, _Tp>
    ptr_fun(_R (*__f)(_Ty, _Tp)) noexcept
    {
        return pointer_binary_function<_R, _Ty, _Tp>(__f);
    }

    template <class _R, class _Ty>
    class mem_fun_t : public unary_function<_Ty *, _R>
    {
      protected:
        _R (_Ty::*f)();

      public:
        explicit mem_fun_t(_R (_Ty::*__p)()) : f(__p) {}

        constexpr _R operator()(_Ty *__p) const noexcept
        {
            return (__p->*f)();
        }
    };

    template <class _R, class _Ty>
    class mem_fun_ref_t : public unary_function<_Ty, _R>
    {
      protected:
        _R (_Ty::*f)();

      public:
        explicit mem_fun_ref_t(_R (_Ty::*__p)()) : f(__p) {}

        constexpr _R &operator()(_Ty &__p) const noexcept
        {
            return (__p.*f)();
        }
    };

    template <class _R, class _Ty>
    class const_mem_fun_t : public unary_function<_Ty *, _R>
    {
      protected:
        _R (_Ty::*f)() const;

      public:
        explicit const_mem_fun_t(_R (_Ty::*__p)() const) : f(__p) {}

        constexpr _R operator()(_Ty *__p) const noexcept
        {
            return (__p->*f)();
        }
    };

    template <class _R, class _Ty>
    class const_mem_fun_ref_t : public unary_function<_Ty, _R>
    {
      protected:
        _R (_Ty::*f)() const;

      public:
        explicit const_mem_fun_ref_t(_R (_Ty::*__p)()) : f(__p) {}

        constexpr _R &operator()(_Ty &__p) const noexcept
        {
            return (__p.*f)();
        }
    };

    UNARY_ARITHMETIC_FUNC_OP(identity, unary_function, _Ty, _Ty);
    UNARY_ARITHMETIC_FUNC_OP(negate, unary_function, _Ty, -);
    UNARY_ARITHMETIC_FUNC_OP(logical_not, unary_function, bool, !);

    BINARY_ARITHMETIC_FUNC_OP(plus, binary_function, _Ty, +);
    BINARY_ARITHMETIC_FUNC_OP(minus, binary_function, _Ty, -);
    BINARY_ARITHMETIC_FUNC_OP(mult, binary_function, _Ty, *);
    BINARY_ARITHMETIC_FUNC_OP(mod, binary_function, _Ty, %);
    BINARY_ARITHMETIC_FUNC_OP(divides, binary_function, _Ty, /);

    BINARY_ARITHMETIC_FUNC_OP(greater, binary_function, bool, >);
    BINARY_ARITHMETIC_FUNC_OP(less, binary_function, bool, <);
    BINARY_ARITHMETIC_FUNC_OP(greater_equal, binary_function, bool, >=);
    BINARY_ARITHMETIC_FUNC_OP(less_equal, binary_function, bool, <=);

    BINARY_ARITHMETIC_FUNC_OP(logical_and, binary_function, bool, &&);
    BINARY_ARITHMETIC_FUNC_OP(logical_or, binary_function, bool, ||);
    BINARY_ARITHMETIC_FUNC_OP(equal_to, binary_function, bool, ==);
    BINARY_ARITHMETIC_FUNC_OP(unequal_to, binary_function, bool, !=);

}; // namespace iop

#endif