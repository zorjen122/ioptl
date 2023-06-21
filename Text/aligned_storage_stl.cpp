/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * This file is part of Project:XXXX
 * See NeverlandC.TXT for licensing information.
 * Copyleft 2022, NeverlandC. All wrongs reserved.
 *
 */


#include "type_trait.h"
#include "metacomponent.h"
#include "config_def.h"
#include <typeinfo>

struct __nat {};

template<class _Ty, class _Tp>
struct __type_list {
    using head = _Ty;   using tail = _Tp;
};

template<class _Tlist, ::std::size_t _Size,
    bool = _Size <= sizeof(typename _Tlist::head)>
struct __frist_type_list {};

template<class _Ty, class _Tpnext, ::std::size_t _Size>
struct __frist_type_list<__type_list<_Ty, _Tpnext>, _Size, true> {
    using type = _Ty;
};


//* if no one cond is true : error
template<class _Ty, class _Tpnext, ::std::size_t _Size>
struct __frist_type_list<__type_list<_Ty, _Tpnext>, _Size, false> {
    using type = typename __frist_type_list<_Tpnext, _Size>::type;
};


template<class _Ty, ::std::size_t _Align>struct __find_pod;

template<class _Ty, class _Tp, ::std::size_t _Align>
struct __find_pod<__type_list<_Ty, _Tp>, _Align> {
    using type = MPL_L::conditioni_t < _Align == _Ty::value,
        typename _Ty::type, typename __find_pod<_Tp, _Align>::type>;
};
