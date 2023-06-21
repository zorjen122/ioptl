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

#define _ALLOC_STL_
#ifdef _ALLOC_STL_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <new>
#include <functional>
#include "config_def.h"

#define __STL_THREADS               // non-define
#define __NODE_ALLOCATOR_TRHEADS_DEFAULT false
#define __NODE_ALLOCATOR_THREADS __NODE_ALLOCATOR_TRHEADS_DEFAULT   // non-define

#define __USE_MALLOC                // non-define
#define __USE_DEFAULT_ALLOC         // non-define

#define __DEFAULT_ALLOC_NOBJS_VALUE 20

#define __STL_VOLATILE volatile     //  beacuse maybe is no volatile type
// #define __RESTRICT __restrict__     // maybe no is __restrict__ for STL
#ifndef __RESTRICT
#  define __RESTRICT
#endif

#ifdef __SUNPRO_CC
#  define __PRIVATE public
   // Extra access restrictions prevent us from really making some things
   // private.
#else
#  define __PRIVATE private
#endif

#define __THROW_BAD_ALLOC throw ::std::bad_alloc()

// #ifdef !GENUINE_ALLOCATOR_IS_BUG 



// _MAX_BYTES allocator
template<::std::size_t __sz, class _Ty = void>
class __Malloc_alloc {
public:
    using value_type = void;
    using size_type = ::std::size_t;     using difference_type = ::std::ptrdiff_t;

    using pointer = value_type*;                using const_pointer = const value_type*;

protected:

    // malloc_alloc out-of-memory handling non-define
    #ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG

    CCC_CONSTEXPR static void(*__malloc_alloc_oom_handler)() = 0;

    static value_type(*__set_malloc_handler(value_type(*__f)()))(){
        value_type(*__old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = __f;
        return(__old);
    }
    #endif


    // Optimality for [ma re lloc]
    static pointer _S_oom_malloc(::std::size_t __n) {

        value_type(*__my_malloc_handler)();
        pointer __result;

        for (;;) {
            __my_malloc_handler = __malloc_alloc_oom_handler;
            // for the for(;;) syntax error throw but now non-realization
            if (__my_malloc_handler == nullptr) { __THROW_BAD_ALLOC; }
            (*__my_malloc_handler)();
            __result = malloc(__n);
            if (__result) return(__result);
        }
    }
    static pointer _S_oom_remalloc(pointer __p, ::std::size_t __n) {
        value_type(*__my_malloc_handler)();
        pointer __result;

        for (;;) {
            __my_malloc_handler = __malloc_alloc_oom_handler;
            // for the for(;;) syntax error throw but now non-realization
            if (__my_malloc_handler == nullptr) { __THROW_BAD_ALLOC; }
            (*__my_malloc_handler)();
            __result = realloc(__p, __n);
            if (__result) return(__result);
        }
    }


public:
    static pointer allocate(::std::size_t __n) {
        pointer __result = malloc(__n);
        if (__result == nullptr) _S_oom_malloc(__n);
        return __result;
    }

    // ? : ::std::size_t of requirement
    CCC_CONSTEXPR static void deallocate(pointer __mp, ::std::size_t) { free(__mp); }
    CCC_CONSTEXPR static void deallocate(pointer __mp) { free(__mp); }

    static pointer reallocate(pointer __mp, ::std::size_t __old_n, ::std::size_t __new_n) {
        pointer __result = realloc(__mp, __new_n);
        if (__result == nullptr) __result = _S_oom_remalloc(__mp, __new_n);
        return __result;
    }

};

# ifdef __USE_MALLOC
using malloc_alloc = __Malloc_alloc<0>;

using single_client_alloc = malloc_alloc;

#endif


template<bool __threads, ::std::size_t __sz, class _Ty = void>
class __Default_alloc {
public:
    using value_type = void;
    using size_type = ::std::size_t;     using difference_type = ::std::ptrdiff_t;

    using pointer = value_type*;                using const_pointer = const value_type*;

private:
    /*
     * Really we should use static const int x = N
     * instead of enum { x = N }, but few compilers accept the former.
     */
    #if ! (defined(__SUNPRO_CC) || defined(__GNUC__))
    enum { _ALIGN = 8 };
    enum { _MAX_BYTES = 128 };
    enum { _NFREELISTS = 16 }; // _MAX_BYTES/_ALIGN
    # elif defined(__SUNPRO_CC) || defined(__GNUC__)
        // breaks if we make these template class members:
    enum { _ALIGN = 8 };
    enum { _MAX_BYTES = 128 };
    enum { _NFREELISTS = 16 }; // _MAX_BYTES/_ALIGN
    #endif

    static size_type _S_round_up(size_type __bypes) {
        // __bypes + __bypes Mod 8      
        return (
            (__bypes + static_cast<size_type>(_ALIGN - 1)) & ~(static_cast<size_type>(_ALIGN - 1))
            );
    }

__PRIVATE:
    union _Obj {
        union _Obj* _M_free_list_link;
        char _M_client_data[1];
    };

private:

    static _Obj* __STL_VOLATILE _S_free_list[_NFREELISTS];

    static size_type _S_freelist_index(size_type __bypes) {
       // all-bype change same-bype-index also maybe Mod bype no-eval-to 0 
        return (
            (__bypes + static_cast<size_type>(_ALIGN - 1)) / (static_cast<size_type>(_ALIGN - 1))
            );
    }

    static  pointer _S_refill(size_type __n) {
        size_type __nobjs = __DEFAULT_ALLOC_NOBJS_VALUE;
        char* __chunk = _S_chunk_alloc(__n, __nobjs);
        _Obj* __current_obj;    _Obj* __next_obj;

        _Obj* __STL_VOLATILE* __self_free_list;
        _Obj* __result;
        int __i = 1;        // __nojs > 1 use this 

        if (__nobjs == 1) return __chunk;

        __self_free_list = _S_free_list + _S_freelist_index(__n);

        // non-one-chunk so need return combination of list 
        __result = reinterpret_cast<_Obj*>(__chunk);
        *__self_free_list = __next_obj = reinterpret_cast<_Obj*>(__chunk + __n);
        for (;;++__i) {
            __current_obj = __next_obj;
            __next_obj = reinterpret_cast<_Obj*>((reinterpret_cast<char*>(__next_obj) + __n));
            if (__nobjs - 1 == __i) {
                __current_obj->_M_free_list_link = 0;
                break;
            }
            else {
                __current_obj->_M_free_list_link = __next_obj;
            }
        }
        return __result;
    }

    static char* _return_nomove_pchar(char* __p, char* __ps, const size_type __n) {
        __p = __ps;     __ps += __n;
        return static_cast<char*>(__p);
    }

    static char* _S_chunk_alloc(size_type __size, size_type& __nobjs) {
        char* __result;
        size_type __total_bypes = __size * __nobjs;
        size_type __bytes_left = static_cast<size_type>(_S_end_free - _S_start_free);

        if (__bytes_left >= __total_bypes) {
            return _return_nomove_pchar(__result, _S_start_free, __total_bypes);
        }
        else if (__bytes_left >= __size) {
            __nobjs = (__bytes_left / __size);
            __total_bypes = __size * __nobjs;
            return _return_nomove_pchar(__result, _S_start_free, __total_bypes);
        }
        else {
            size_type __bypes_left_get = 2 * __total_bypes + _S_round_up(_S_heap_size >> 4);

            /*  For thereinbefore Code of the conditioning
             *  now we only > 0 and < 0 and = 0 for such taht type
             */
            if (__bytes_left > 0) {
                _Obj* __STL_VOLATILE* __my_free_list =
                    _S_free_list + _S_freelist_index(__bypes_left_get);

                reinterpret_cast<_Obj*>(_S_start_free)->_M_free_list_link = *__my_free_list;
                *__my_free_list = reinterpret_cast<_Obj*>(_S_start_free);
            }
            _S_start_free = (char*)malloc(__bypes_left_get);

            if (_S_start_free == nullptr) {
                size_type __i;
                _Obj* __STL_VOLATILE* __my_free_list;       _Obj* __p;

                for (__i = __size;
                    __i <= static_cast<size_type>(_MAX_BYTES);
                    __i += static_cast<size_type>(_ALIGN))
                {
                    __my_free_list = _S_free_list + _S_freelist_index(__i);
                    __p = *__my_free_list;
                    if (__p != nullptr) {
                        *__my_free_list = __p->_M_free_list_link;
                        _S_start_free = reinterpret_cast<char*>(__p);
                        _S_end_free = _S_start_free + __i;

                        _return_nomove_pchar(_S_start_free, _S_end_free, __i);
                        return _S_chunk_alloc(__size, __nobjs);
                    }
                }
                _S_end_free = nullptr;
                _S_start_free = (char*)malloc_alloc::allocate(__bypes_left_get);
            }

            _S_heap_size += __bypes_left_get;
            _S_end_free = _S_start_free + __bypes_left_get;

            // call is no one-time beacuse total_bypes momory bigger.
            return _S_chunk_alloc(__size, __nobjs);
        }
    }

    static char* _S_start_free;      static char* _S_end_free;
    static size_type _S_heap_size;

   /*
    * # ifdef __STL_THREADS
    * _STL_mutex_lock _S_node_allocator_lock;
    * # endif
    * class _Lock;
    * friend class _Lock;
    * class _Lock {
    * public:
    *     _Lock() { __NODE_ALLOCATOR_LOCK; }
    *     ~_Lock() { __NODE_ALLOCATOR_UNLOCK; }
    */
public:
    __Default_alloc() {}
    ~__Default_alloc() {}
    static pointer allocate(size_type __n) {
        pointer __ret = nullptr;
        malloc_alloc caller;
        if (__n > static_cast<size_type>(_MAX_BYTES)) {
            __ret = caller.allocate(__n);
        }
        else {
            _Obj* __STL_VOLATILE*
                __self_free_list = _S_free_list + _S_freelist_index(__n);

            _Obj* __RESTRICT  __result = *__self_free_list;
            if (__result == nullptr)
                __ret = _S_refill(_S_round_up(__n));
            else {
                *__self_free_list = __result->_M_free_list_link;
                __ret = __result;
            }
        }
        return __ret;
    }
    static CCC_CONSTEXPR void deallocate(pointer __p, size_type __n) {
        if (__n > _MAX_BYTES) {
            // function for no __n is equivalent
            malloc_alloc::deallocate(__p, __n);
        }
        else {
            // Maybe pointer is null, but we no have thorw error.

            _Obj* __STL_VOLATILE*
                __self_ree_list = _S_free_list + _S_freelist_index(__n);

            _Obj* __rhs = static_cast<_Obj*>(__p);
            __rhs->_M_free_list_link = *__self_ree_list;
            *__self_ree_list = __rhs;
        }
    }
    static pointer reallocate(pointer __p, size_type __old_sz, size_type __new_sz) {
        if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) return __p;

        if (__old_sz + __new_sz > 2 * _MAX_BYTES) {
            return malloc_alloc::reallocate(__p, __old_sz, __new_sz);
        }

        pointer __rhs = allocate(__new_sz);
        size_type __copy_sz = __new_sz > __old_sz ? __old_sz : __new_sz;
        memccpy(__rhs, __p, __copy_sz, 0);
        deallocate(__p, __old_sz);

        return __rhs;
    }

};

#ifdef __USE_DEFAULT_ALLOC
// default coide is using alloc, but beacuse macro non-realization now
template<class>
using default_alloc = __Default_alloc<__NODE_ALLOCATOR_THREADS, 0, void>;
#endif

#endif
// #endif