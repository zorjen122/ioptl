/*
 * Copyright(c) 1997 - zorjen122
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

#ifndef _ALLOCATOR_TRAITS_IOP_
#define _ALLOCATOR_TRAITS_IOP_

#include "allocator_F.h"
#include "cppconfig.h"
#include "metacomponent.h"
#include "type_traits.h"
#include "utilitys.h"
#include "deftype.h"

#include <memory>

#define IOP_ALLOCATOR_TRAITS_TYPE_XXX(NAME, PROPERTIES)                        \
    template <class _Ty, class = void> struct NAME : mpls::false_type          \
    {};                                                                        \
    template <class _Ty>                                                       \
    struct NAME<_Ty, mpls::__void_t<typename _Ty::PROPERTIES>>                 \
        : mpls::true_type                                                      \
    {};


NAMESPACE_BEGIN(iop)

    //* using traits feature
    template <class _Alloc> struct allocator_traits_impl
    {};

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_pointer, pointer);

    template <class _Alloc,
              /*class _RawAlloc = mpls::remove_reference_t<_Alloc>, */
              bool = _has_pointer<_Alloc>::value>
    struct __pointer{
        using type = typename _Alloc::pointer;
    };

    template <class _Alloc> struct __pointer< _Alloc, false>
    {
        using type = typename _Alloc::value_type*;
    };

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_const_pointer, const_pointer);

    template <class _Alloc, bool = _has_const_pointer<_Alloc>::value>
    struct __const_pointer
    {
        using type = typename _Alloc::const_pointer;
    };

    template <class _Alloc>
    struct __const_pointer<_Alloc, false>
    {
        using type = const typename _Alloc::value_type*;
    };

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_void_pointer, void_pointer);

    template <class _Alloc, bool = _has_void_pointer<_Alloc>::value>
    struct __void_pointer
    {
        using type = typename _Alloc::void_pointer;
    };

    template <class _Alloc> struct __void_pointer<_Alloc, false>
    {
        using type = void *;
    };

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_const_void_pointer, const_void_pointer);

    template <class _Alloc,
              bool = _has_const_void_pointer<_Alloc>::value>
    struct __const_void_pointer
    {
        using type = typename _Alloc::const_void_pointer;
    };

    template <class _Alloc>
    struct __const_void_pointer<_Alloc, false>
    {
        using type = const void *;
    };

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_size_type, size_type);

    template <class _Alloc, bool = _has_size_type<_Alloc>::value>
    struct __size_type
    {
        using type = typename _Alloc::size_type;
    };

    template <class _Alloc> struct __size_type<_Alloc, false>
    {
        using type = size_t;
    };

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_alloc_traits_difference_type,
                                  difference_type);

    template <class _Alloc,
              bool = _has_alloc_traits_difference_type<_Alloc>::value>
    struct __difference_type
    {
        using type = typename _Alloc::difference_type;
    };

    template < class _Alloc>
    struct __difference_type<_Alloc, true>
    {
        using type = typename _Alloc::difference_type;
    };

    template <class _Alloc, class _Tp, class = void>
    struct _has_rebind_other : mpls::false_type
    {};

    template <class _Alloc, class _Tp>
    struct _has_rebind_other<
        _Alloc, _Tp, mpls::__void_t<typename _Alloc::template rebind<_Tp>::other>>
        : mpls::true_type
    {};

    template <class _Alloc, class _Tp,
              bool = _has_rebind_other<_Alloc, _Tp>::value>
    struct __allocator_traits_rebind
    {
        static_assert(_has_rebind_other<_Alloc, _Tp>::value,
                      "This allocator has to implement rebind");
        using type = typename _Alloc::template rebind<_Tp>::other;
    };

    //* For ex: one-allocator to mult-allocator, mult-allocator op(iterator) :
    // next/prev ...
    template <template <class, class...> class _Alloc,
              class _Ty, class _Tp, class... _Args>
    struct __allocator_traits_rebind<_Alloc<_Ty, _Args...>, _Tp, false>
    {
        using type = _Alloc<_Tp, _Args...>;
    };

    template <template <class, class...> class _Alloc,
              class _Ty, class _Tp, class... _Args>
    struct __allocator_traits_rebind<_Alloc<_Ty, _Args...>, _Tp, true>
    {
        using type =
            typename _Alloc<_Ty, _Args...>::template rebind<_Tp>::other;
    };

    template <class _Alloc, class _Tp>
    using __allocator_traits_rebind_t =
        typename __allocator_traits_rebind<_Alloc, _Tp>::type;

    template <class, class _Alloc, class... _Args>
    struct _has_construct_impl : mpls::false_type
    {};
    template <class _Alloc, class... _Args>
    struct _has_construct_impl<decltype(mpls::declval<_Alloc>().construct_at(
                                   mpls::declval<_Args>()...)),
                               _Alloc, _Args...> : mpls::true_type
    {};

    template <class _Alloc, class... _Args>
    struct _has_construct : _has_construct_impl<void, _Alloc, _Args...>
    {};

    template <class _Alloc, class _Ptr, class = void>
    struct _has_destroy_impl : mpls::false_type
    {};
    template <class _Alloc, class _Ptr>
    struct _has_destroy_impl<_Alloc, _Ptr,
                             decltype(mpls::declval<_Alloc>().destroy(
                                 mpls::declval<_Ptr>()))> : mpls::true_type
    {};

    template <class _Alloc, class _Ptr>
    struct _has_destroy : _has_destroy_impl<_Alloc, _Ptr, void>
    {};

    //* For everyclass, this meta-function derive if that's base class
    //*     so the same A,B class will not conflict for type.

    template <bool _Cond, class _Unique> struct _non_trivial_if
    {};

    template <class _Unique> struct _non_trivial_if<true, _Unique>
    {
        _non_trivial_if() noexcept{};
    };

    template <class _Alloc>
    class allocator_traits
        : private 
            _non_trivial_if<!mpls::is_void<typename _Alloc::value_type>::value,
                                  allocator_traits<_Alloc>>
    {
      public:
        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<allocator_type>;

        using value_type = typename allocator_type::value_type;
        using pointer = typename __pointer<allocator_type>::type;
        using const_pointer =
            typename __const_pointer<allocator_type>::type;
        using void_pointer =
            typename __void_pointer<allocator_type>::type;
        using const_void_pointer =
            typename __const_void_pointer<allocator_type>::type;
        using difference_type =
            typename __difference_type<allocator_type>::type;
        using size_type =
            typename __size_type<allocator_type>::type;

        constexpr allocator_traits() noexcept = default;
        constexpr
        allocator_traits(const allocator_traits<_Alloc> &) noexcept{};

#if IOP_VERSION > 20
        [[nodiscard]] static IOP_CONSTEXPR_CXX23 allocation_result<value_type>
        allocate_at_least(size_t __n)
        {
            return {_Alloc::allocate(__n), __n};
        }
#endif

        template <class _Up>
        struct rebind
        {
            using other = __allocator_traits_rebind_t<allocator_type, _Up>;
        };

        static constexpr size_type max_size() noexcept
        {
            return size_type(-1) / sizeof(value_type);
        }

        static constexpr pointer allocate(size_type __n)
        {
            return (0 == __n) ? nullptr
                              : _Alloc::allocate(__n * sizeof(value_type));
        }
        static constexpr pointer allocate()
        {
            return _Alloc::allocate(sizeof(value_type));
        }

        static constexpr void deallocate(pointer __p, size_type __n)
        {
            if (0 != __n)
                _Alloc::deallocate(__p, __n * sizeof(value_type));
        }
        static constexpr void deallocate(pointer __p)
        {
            _Alloc::deallocate(__p, sizeof(value_type));
        }

        template <class _Objty, class... _Args>
        static constexpr void construct_at(_Objty *__p,
                                                     _Args &&...__args)
            noexcept(::std::is_nothrow_constructible<_Objty>::value)
        {
            // 以allocator_traits特性，应该设定默认placement new吗？
            if constexpr(_has_construct<allocator_type, _Args...>::value)
                _Alloc::construct_at(__p, Fiop::forward<_Args>(__args)...);
            else
                ::new (Fiop::_Voidify_ptr(__p)) _Objty(Fiop::forward<_Args>(__args)...);
        }

        template <class _Objty, class... _Args>
        static constexpr void
        construct_at(allocator_type &__a, _Objty *__p, _Args &&...__args)
        {
            __a.construct_at(__p, Fiop::forward<_Args>(__args)...);
        }

        template <class _Iter, class = mpls::enable_if_t<
                                   _has_destroy<allocator_type, _Iter>::value>>
        static constexpr void destroy(_Iter __res)
            noexcept(noexcept(_Alloc::destroy(__res)))
        {
            _Alloc::destroy(__res);
        }

        template <class _Iter, class = mpls::enable_if_t<
                                   _has_destroy<allocator_type, _Iter>::value>>
        static constexpr void destroy(allocator_type &__a,
                                                _Iter __res)
            noexcept(noexcept(__a.destroy(__res)))
        {
            __a.destroy(__res);
        }
    };


    template <class _Alloc> class alloc_traits : public allocator_traits<_Alloc>
    {
      public:
        using allocator_type = _Alloc;
        using Base_type = allocator_traits<allocator_type>;

        using value_type = typename Base_type::value_type;
        using pointer = typename Base_type::pointer;
        using const_pointer = typename Base_type::const_pointer;
        using void_pointer = typename Base_type::void_point;
        using const_void_pointer = typename Base_type::const_void_pointer;
        using difference_type = typename Base_type::difference_type;
        using size_type = typename Base_type::size_type;

        using Base_type::allocate;
        using Base_type::allocate_at_least;
        using Base_type::deallocate;

        using Base_type::construct_at;
        using Base_type::destroy;
        using Base_type::max_size;

      private:
        template <class _Ptr>
        using __is_custom_pointer =
            mpls::oper::and_<mpls::is_same<pointer, _Ptr>,
                             mpls::oper::not_<mpls::is_pointer<_Ptr>>>;

      public:
        template <class _Tp, class _Up,
                  class = mpls::enable_if_t<
                      _has_rebind_other<allocator_type, _Tp, _Up>::value>>
        struct rebind
        {
            using other = typename Base_type::template rebind<_Tp, _Up>::other;
        };

        template <class _Objty, class... _Args,
                  class = mpls::enable_if_t<
                      _has_construct<allocator_type, _Args...>::value>>
        static constexpr
            mpls::enable_if_t<__is_custom_pointer<_Alloc>::value>
            construct_at(allocator_type &__a, _Objty __p, _Args &&...__args)
                noexcept(::std::is_nothrow_constructible<_Objty>::value)
        {
            Base_type::construct_at(Fiop::to_address(*__p),
                                    Fiop::forward<_Args>(__args)...);
        }

        template <class _Iter, class = mpls::enable_if_t<
                                   _has_destroy<allocator_type, _Iter>::value>>
        IOP_CONSTEXPR_CXX20
            mpls::enable_if_t<__is_custom_pointer<_Alloc>::value>
            destroy(allocator_type &__a, _Iter __iter)
                noexcept(noexcept(Base_type::destroy(__a,
                                                     Fiop::to_address(__iter))))
        {
            Base_type::destroy(__a, Fiop::to_address(*__iter));
        }
    };

NAMESPACE_END(iop)

#endif