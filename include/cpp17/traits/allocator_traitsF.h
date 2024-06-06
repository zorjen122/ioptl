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

#include "../cppconfig.h"

#include "../meta/metautilF.h"
#include "../traits/type_traitsF.h"
#include "../util/utilityF.h"


NAMESPACE_BEGIN(iop)

    template <class Alloc>
    struct __pointer{
        using type = typename mpls::traits_pointer<typename Alloc::value_type*,
                                        Alloc>::type;
    };


    template <class Alloc>
    struct __const_pointer
    {
        using type = typename mpls::traits_const_pointer<const typename Alloc::value_type*,
                                                Alloc>::type;
    };


    template <class Alloc>
    struct __void_pointer
    {
        using type = typename mpls::traits_void_pointer<void*,
                                              Alloc>::type;
    };



    template <class Alloc>
    struct __const_void_pointer
    {
        using type = typename mpls::traits_const_void_pointer<const void*,
                                                Alloc>::type;
    };


    template <class Alloc>
    struct __size_type
    {
        using type = typename mpls::traits_size_type<size_t, Alloc>::type;
    };



    template <class Alloc>
    struct __difference_type
    {
        using type = typename mpls::traits_difference_type<::std::ptrdiff_t,
                                                Alloc>::type;
    };

    template <class Alloc, class Tp, class =void>
    struct _has_rebind_other : mpls::false_type
    {};

    template <class Alloc, class Tp>
    struct _has_rebind_other<Alloc,
            Tp,
            mpls::__void_t<typename Alloc::template rebind_alloc<Tp>::other>>
        : mpls::true_type
    {};

    template <class Alloc, class Tp,
              bool = _has_rebind_other<Alloc, Tp>::value>
    struct __allocator_traits_rebind_alloc
    {
        using type = typename Alloc::template rebind_alloc<Tp>::other;
    };

    template <template <class, class...> class Alloc,
            class _Ty, class Tp, class... Args>
    struct __allocator_traits_rebind_alloc<Alloc<_Ty, Args...>, Tp, true>
    {
        using type = typename Alloc<_Ty, Args...>::template rebind_alloc<Tp>::other;
    };

    template <template <class, class...> class Alloc,
              class _Ty, class Tp, class... Args>
    struct __allocator_traits_rebind_alloc<Alloc<_Ty, Args...>, Tp, false>
    {
        using type = Alloc<Tp>;
    };


    template <class Alloc, class T>
    using __allocator_traits_rebind_alloc_t =
        typename __allocator_traits_rebind_alloc<Alloc, T>::type;

    template <class, class Alloc, class... Args>
    struct _has_construct_impl : mpls::false_type
    {};
    template <class Alloc, class... Args>
    struct _has_construct_impl<decltype(mpls::declval<Alloc>().construct(
                                   mpls::declval<Args>()...)),
                               Alloc, Args...> : mpls::true_type
    {};

    template <class Alloc, class... Args>
    struct _has_construct : _has_construct_impl<void, Alloc, Args...>
    {};

    template <class Alloc, class Ptr, class = void>
    struct _has_destroy_impl : mpls::false_type
    {};
    template <class Alloc, class Ptr>
    struct _has_destroy_impl<Alloc, Ptr,
                             decltype(mpls::declval<Alloc>().destroy(
                                 mpls::declval<Ptr>()))> : mpls::true_type
    {};

    template <class Alloc, class Ptr>
    struct _has_destroy : _has_destroy_impl<Alloc, Ptr, void>
    {};

    //* For everyclass, this meta-function derive if that's base class
    //*     so the same A,B class will not conflict for type.

    template <bool _Cond, class _Unique> struct _non_trivial_if
    {};

    template <class _Unique> struct _non_trivial_if<true, _Unique>
    {
        _non_trivial_if() noexcept{};
    };

    template <class Alloc>
    class allocator_traits
        // : private 
        //     _non_trivial_if<!mpls::is_void<typename Alloc::value_type>::value,
        //                           allocator_traits<Alloc>>
    {
      public:
        using allocator_type = Alloc;
        using allocator_traits_type = allocator_traits<Alloc>;

        using value_type = typename Alloc::value_type;
        using pointer = typename __pointer<Alloc>::type;
        using const_pointer = typename __const_pointer<Alloc>::type;
        using void_pointer = typename __void_pointer<Alloc>::type;
        using const_void_pointer = typename __const_void_pointer<Alloc>::type;
        using difference_type = typename __difference_type<Alloc>::type;
        using size_type = typename __size_type<Alloc>::type;
        
#if IOP_VERSION > 20
        [[nodiscard]] static IOP_CONSTEXPR_CXX23 allocation_result<value_type>
        allocate_at_least(size_t __n)
        {
            return { Alloc::allocate(__n), __n };
        }
#endif

        template <class T>
        struct rebind_alloc
        {
            using other = __allocator_traits_rebind_alloc_t<allocator_type, T>;
        };

        template <class T>
        struct rebind_traits
        {
            using other = allocator_traits<T>;
        };

        static constexpr size_type max_size() noexcept
        {
            return size_type(-1) / sizeof(value_type);
        }

        static constexpr pointer allocate(size_type __n)
        {
            return (0 == __n) ? nullptr
                              : Alloc::allocate(__n * sizeof(value_type));
        }
        static constexpr pointer allocate()
        {
            return Alloc::allocate(sizeof(value_type));
        }

        static constexpr void deallocate(pointer __p, size_type __n)
        {
            if (0 != __n)
                Alloc::deallocate(__p, __n * sizeof(value_type));
        }
        static constexpr void deallocate(pointer __p)
        {
            Alloc::deallocate(__p, sizeof(value_type));
        }

        template <class Objty, class... Args>
        static constexpr void construct(Objty *__p, Args &&...__args)
        {
            if constexpr(_has_construct<allocator_type, Args...>::value)
                Alloc::construct(__p, Fiop::forward<Args>(__args)...);
            else
                ::new (Fiop::_Voidify_ptr(__p)) Objty(Fiop::forward<Args>(__args)...);
        }

        template <class Objty, class... Args>
        static constexpr void
        construct(allocator_type &__a, Objty *__p, Args &&...__args)
        {
            __a.construct(__p, Fiop::forward<Args>(__args)...);
        }

        template <class _Iter>
        static constexpr void destroy(_Iter __res)
        {
            if constexpr(_has_destroy<allocator_type, _Iter>::value)
                Alloc::destroy(__res);
            else
                __res->~_Iter();
        }

        template <class _Iter>
        static constexpr void destroy(allocator_type &__a,
                                                _Iter __res)
            noexcept(noexcept(__a.destroy(__res)))
        {
            __a.destroy(__res);
        }
    };


    template <class Alloc> class alloc_traits : public allocator_traits<Alloc>
    {
      public:
        using allocator_type = Alloc;
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
        template <class Ptr>
        using __is_custom_pointer =
            mpls::oper::and_<mpls::is_same<pointer, Ptr>,
                             mpls::oper::not_<mpls::is_pointer<Ptr>>>;

      public:
        template <class Objty, class... Args,
                  class = mpls::enable_if_t<
                      _has_construct<allocator_type, Args...>::value>>
        static constexpr
            mpls::enable_if_t<__is_custom_pointer<Alloc>::value>
            construct_at(allocator_type &__a, Objty __p, Args &&...__args)
        {
            Base_type::construct_at(Fiop::to_address(*__p),
                                    Fiop::forward<Args>(__args)...);
        }

        template <class _Iter, class = mpls::enable_if_t<
                                   _has_destroy<allocator_type, _Iter>::value>>
        IOP_CONSTEXPR_CXX20
            mpls::enable_if_t<__is_custom_pointer<Alloc>::value>
            destroy(allocator_type &__a, _Iter __iter)
                noexcept(noexcept(Base_type::destroy(__a,
                                                     Fiop::to_address(__iter))))
        {
            Base_type::destroy(__a, Fiop::to_address(*__iter));
        }
    };

NAMESPACE_END(iop)

#endif