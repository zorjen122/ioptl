/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#ifndef _IOP_INITIALIZER_DEF
#define _IOP_INITIALIZER_DEF


#include "../allocatorF.h"
#include "../iteratorF.h"

#include "../traits/allocator_traitsF.h"

namespace iop {

    template <class _Ty> class initializer_list
    {
      public:
        explicit initializer_list() : _Vlist(nullptr), _len(0) {}
        explicit initializer_list(const _Ty *__src, size_t __s)
            : _Vlist(__src), _len(__s)
        {}

      protected:
        const _Ty *_Vlist;
        size_t _len;

      public:
        size_t size() const { return _len; }

        const _Ty *begin() { return _Vlist; }

        const _Ty *end() { return _Vlist + _len; }

        bool empty() { return (begin() == end()); }

        const _Ty *data() { return _Vlist; }
    };

    template <class _T> const _T *begin(const initializer_list<_T> &__r)
    {
        return __r.begin();
    }

    template <class _T> const _T *end(const initializer_list<_T> &__r)
    {
        return __r.end();
    }

}; // namespace iop

#endif // _IOP_INITIALIZER_DEF