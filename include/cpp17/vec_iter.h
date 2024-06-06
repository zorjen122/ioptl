#pragma once

#include "util/iterator_baseF.h"
#include "util/utilityF.h"
#include "meta/metautilF.h"
#include <iostream>

namespace iop
{
namespace vec_iter
{
template <class _Ty> class trivial_iterator
{
  protected:
    using Traits_type = iterator_traits<_Ty *>;

  public:
    using iterator_category = typename Traits_type::iterator_category;
    using difference_type = typename Traits_type::difference_type;
    using value_type = typename Traits_type::value_type;
    using pointer = typename Traits_type::pointer;
    using reference = typename Traits_type::reference;

  protected:
    pointer ptr;

  public:
    trivial_iterator() : ptr(nullptr)
    {
    }
    trivial_iterator(const pointer &p) : ptr(p)
    {
    }
    trivial_iterator(pointer &&p) : ptr(Fiop::move(p))
    {
    }

    trivial_iterator(const trivial_iterator &src) : ptr(src.ptr)
    {
    }

    trivial_iterator(trivial_iterator &&src) : ptr(Fiop::move(src.ptr)){};

  public:
    constexpr reference operator*() noexcept
    {
        return *ptr;
    }
    constexpr pointer operator->() noexcept
    {
        return ptr;
    }

    constexpr trivial_iterator &operator++() noexcept
    {
        ++ptr;
        return *this;
    }

    constexpr trivial_iterator operator++(int) noexcept
    {
        return trivial_iterator(ptr++);
    }

    constexpr trivial_iterator &operator--() noexcept
    {
        --ptr;
        return *this;
    }

    constexpr trivial_iterator operator--(int) noexcept
    {
        return trivial_iterator(ptr--);
    }

    constexpr trivial_iterator operator-(difference_type n) noexcept
    {
        return ptr - n;
    }

    constexpr trivial_iterator operator+(difference_type n) noexcept
    {
        return ptr + n;
    }

    constexpr difference_type operator-(const trivial_iterator &src) const noexcept
    {
        return static_cast<difference_type>(ptr - src.ptr);
    }

    constexpr trivial_iterator &operator+=(difference_type n) noexcept
    {
        ptr = ptr + n;
        return *this;
    }

    constexpr trivial_iterator &operator-=(difference_type n) noexcept
    {
        ptr = ptr - n;
        return *this;
    }

    constexpr bool operator==(const trivial_iterator &src) noexcept
    {
        return (ptr == src.ptr);
    }

    constexpr bool operator!=(const trivial_iterator &src) noexcept
    {
        return !(ptr == src.ptr);
    }

    constexpr void operator=(const pointer &ptr)
    {
        ptr = ptr;
    }

    constexpr void operator=(pointer &&ptr)
    {
        ptr = ptr;
    }

    constexpr void operator=(const trivial_iterator &src)
    {
        ptr = src.ptr;
    }

    constexpr void operator=(trivial_iterator &&src)
    {
        ptr = src.ptr;
        src.ptr = nullptr;
    }
};
} // namespace vec_iter
} // namespace iop
