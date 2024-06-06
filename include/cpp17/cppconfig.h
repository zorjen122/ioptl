/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _CONSTEXPR_VERSION_DEF_
#define _CONSTEXPR_VERSION_DEF_

#ifdef __cplusplus

#ifndef IOP_VERSION
#if __cplusplus <= 201103L
#define IOP_VERSION 11
#elif __cplusplus <= 201402L
#define IOP_VERSION 14
#elif __cplusplus <= 201703L
#define IOP_VERSION 17
#elif __cplusplus <= 202002L
#define IOP_VERSION 20

#else

// Expected release year of the next C++ standard
#define IOP_VERSION 23
#endif
#endif // IOP_VERSION

#if IOP_VERSION < 11
#define NOEXCEPT throw()

#else

#define NOEXCEPT_(x) noexcept(x)
#define IOP_NOEXCEPT noexcept
#define NOEXCEPT IOP_NOEXCEPT
#define IOP_CONSTEXPR constexpr

#endif

//* for constexpr if expression is speical case, sometime which effect of the
// code inline .
#if IOP_VERSION <= 11
#define IOP_EXPLICIT_AFTER_CXX11
#else
#define IOP_EXPLICIT_AFTER_CXX11 explicit
#endif

#if IOP_VERSION > 11
#define IOP_CONSTEXPR_CXX14 constexpr
#else
#define IOP_CONSTEXPR_CXX14 inline
#define IOP_STATIC_CONSTEXPR_CXX14 static inline
#endif

#if IOP_VERSION > 14
#define IOP_CONSTEXPR_CXX17 constexpr
#define IOP_STATIC_CONSTEXPR_CXX17 static constexpr
#define IOP_SPECIAL_IF_COSTEXPR_CXX17 if constexpr
#else
#define IOP_CONSTEXPR_CXX17 inline
#define IOP_SPECIAL_IF_COSTEXPR_CXX17 if
#endif

#if IOP_VERSION > 17
#define IOP_CONSTEXPR_CXX20 constexpr
#define IOP_STATIC_CONSTEXPR_CXX20 static constexpr
#define IOP_SPECIAL_IF_COSTEXPR_CXX20 if constexpr
#else
#define IOP_CONSTEXPR_CXX20 inline
#define IOP_SPECIAL_IF_COSTEXPR_CXX20 if
#endif

#if IOP_VERSION > 20
#define IOP_CONSTEXPR_CXX23 constexpr
#define IOP_STATIC_CONSTEXPR_CXX23 static constexpr
#define IOP_SPECIAL_IF_COSTEXPR_CXX23 if constexpr
#else
#define IOP_CONSTEXPR_CXX23 inline
#define IOP_SPECIAL_IF_COSTEXPR_CXX23 if
#endif

#endif //* __cplusplus


#define NAMESPACE_BEGIN(name) namespace name{
#define NAMESPACE_END(name) }; // namespace iop


#endif //* _CONSTEXPR_VERSION_DEF_