#pragma once

#define _IOP_PAIR_H_
#ifdef _IOP_PAIR_H_


namespace iop {

    template <class _Ty, class _Tp> struct pair
    {
        using first_type = _Ty;
        using second_type = _Tp;

        pair() : first(_Ty()), second(_Tp()) {}
        pair(const _Ty &__a, const _Tp &__b) : first(__a), second(__b) {}

        _Ty first;
        _Tp second;
    };
};     // namespace iop


#endif // _IOP_PAIR_H_