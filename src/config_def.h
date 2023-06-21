/*
 * Copyright (c) 1997
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


#define _CONSTEXPR_VERSION_DEF_
#ifdef _CONSTEXPR_VERSION_DEF_

#ifdef __cplusplus

#  ifndef CCC_VERSION 
#    if __cplusplus <= 201103L
#      define  CCC_VERSION 11
#    elif __cplusplus <= 201402L
#      define  CCC_VERSION 14
#    elif __cplusplus <= 201703L
#      define  CCC_VERSION 17
#    elif __cplusplus <= 202002L
#      define  CCC_VERSION 20

#    else

// Expected release year of the next C++ standard
#      define  CCC_VERSION 23
#    endif
#  endif // CCC_VERSION 


#    if CCC_VERSION < 11
#    define CCC_NOEXCEPT throw()

#    else

#    define CCC_NOEXCEPT_(x) noexcept(x)
#    define CCC_NOEXCEPT noexcept
#    define CCC_CONSTEXPR constexpr

#endif




//* for constexpr if expression is speical case, sometime which effect of the code inline .
#  if CCC_VERSION <= 11
#    define CCC_EXPLICIT_AFTER_CXX11
#  else
#    define CCC_EXPLICIT_AFTER_CXX11 explicit
#  endif

#  if CCC_VERSION > 11
#    define CCC_CONSTEXPR_CXX14 constexpr
#  else
#    define CCC_CONSTEXPR_CXX14 inline
#  endif

#  if CCC_VERSION > 14
#    define CCC_CONSTEXPR_CXX17 constexpr
#    define CCC_SPECIAL_IF_COSTEXPR_CXX17 if constexpr
#  else
#    define CCC_CONSTEXPR_CXX17 inline
#    define CCC_SPECIAL_IF_COSTEXPR_CXX17 if
#  endif

#  if CCC_VERSION > 17
#    define CCC_CONSTEXPR_CXX20 constexpr
#    define CCC_SPECIAL_IF_COSTEXPR_CXX20 if constexpr
#  else
#    define CCC_CONSTEXPR_CXX20 inline
#    define CCC_SPECIAL_IF_COSTEXPR_CXX20 if
#  endif

#  if CCC_VERSION > 20
#    define CCC_CONSTEXPR_CXX23 constexpr
#    define CCC_SPECIAL_IF_COSTEXPR_CXX23 if constexpr
#  else
#    define CCC_CONSTEXPR_CXX23 inline
#    define CCC_SPECIAL_IF_COSTEXPR_CXX23 if
#  endif


#endif               //* __cplusplus

#endif             //* _CONSTEXPR_VERSION_DEF_