#ifndef _IOP_MAP_DEF
#define _IOP_MAP_DEF

#include "../meta/functionalF.h"
#include "rb_treeF.h"
#include "pairF.h"

#include <initializer_list>

namespace iop {

    template <class _Key, class _Tv, class _Compare = iop::less<_Key, _Key>,
              class _Alloc = iop::allocator<iop::pair<const _Key, _Tv>>>
    class map
    {
      public:
        using key_type = _Key;
        using value_type = iop::pair<const _Key, _Tv>;
        using key_compare = _Compare;

      protected:
        template <typename _Pair>
        struct _Selectlst
            : public unary_function<_Pair, typename _Pair::first_type>
        {
            typename _Pair::first_type operator()(const _Pair &__x) const
            {
                return __x.first;
            }
        };

        using allocator_type = _Alloc;
        using rbtree_type =
            iop::rbtree<key_type, value_type, _Selectlst<value_type>, _Compare>;

      public:
        using iterator = typename rbtree_type::iterator;
        using const_iterator = typename rbtree_type::const_iterator;
        using pointer = typename rbtree_type::pointer;
        using const_pointer = typename rbtree_type::const_pointer;
        using reference = typename rbtree_type::reference;
        using const_reference = typename rbtree_type::const_reference;

        using size_type = typename rbtree_type::size_type;
        using difference_type = typename rbtree_type::difference_type;

      protected:
        rbtree_type c;

      public:
        map() : c() {}
        map(iterator __first, iterator __last) : c(__first, __last) {}
        map(const ::std::initializer_list<value_type> &__rhs) : c(__rhs) {}
        map(map &__rhs) : c(__rhs.begin(), __rhs.end()) {}

      protected:
        class value_compare
            : public binary_function<value_type, value_type, bool>
        {
            friend class map<_Key, _Tv, _Compare, _Alloc>;

          protected:
            _Compare comp;
            value_compare(_Compare c) : comp(c) {}

          public:
            bool operator()(const value_type &__p1, const value_type &__p2)
            {
                return comp(__p1.first, __p2.first);
            }
        };

      public:
        iterator insert(const value_type &__v) { return c.insert_unique(__v).first; }

        iterator begin() { return c.begin(); }
        iterator end() { return c.end(); }
        const_iterator begin() const { return c.begin(); }
        const_iterator end() const { return c.end(); }

        key_compare key_comp() { return c.key_comp(); }
        value_compare value_comp() { return value_compare(c.key_comp()); }

        _Tv &operator[](const key_type &__k)
        {
            return (*(c.insert_unique(value_type(__k, _Tv())).first)).second;
        }
    };

}; // namespace iop

#endif // _IOP_MAP_DEF