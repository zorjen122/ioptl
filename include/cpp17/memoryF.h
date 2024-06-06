#ifndef MEMORY_F_H
#define MEMORY_F_H

#include "traits/type_traitsF.h"
#include "util/utilityF.h"

#include "cppconfig.h"

NAMESPACE_BEGIN(iop)

    template <class _T, class... _Args>
    constexpr _T *construct_at(_T *__ptr, _Args &&...__args) noexcept(
        ::std::is_nothrow_constructible_v<_T>)
    {
        return ::new (Fiop::_Voidify_ptr(__ptr))
            _T(Fiop::forward<_Args>(__args)...);
    }

    // destroy
    template <class _T> constexpr void destroy_at(_T *__ptr)
    {
        if constexpr (mpls::is_array_v<_T>) {
            for (auto &elem : *__ptr)
                destroy_at(&elem);
        }
        else
            __ptr->~_T();
    }

    template <class _T> constexpr void destroy_at(_T *__first, _T *__last)
    {
        while (__first != __last)
            destroy_at(Fiop::addressof(*(__first++)));
    }

    template <class _Iter> constexpr void destroy(_Iter iter)
    {
        destroy_at(Fiop::addressof(*(iter)));
    }

    template <class _Iter> constexpr void destroy(_Iter __first, _Iter __last)
    {
        while (__first != __last)
            destroy_at(Fiop::addressof(*(__first++)));
    }

    template <class _ForwardIt, class _Size>
    constexpr _ForwardIt destroy_n(_ForwardIt __first, _Size __n)
    {
        for (; __n > 0; --__n, ++__first)
            destroy_at(Fiop::addressof(*__first));

        return __first;
    }

NAMESPACE_END(iop)

#endif