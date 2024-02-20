#pragma once

#define _IOP_MULTISET_H
#ifdef _IOP_MULTISET_H

#include "../functionals.h"
#include "./rb_tree_F.h"
#include <initializer_list>

namespace iop {

    template <class _Key, class _Compare = iop::less<_Key, _Key>,
              class _Alloc = iop::allocator<_Key>>
    class multiset
    {
      protected:
        using rbtree_type = iop::rbtree<_Key, _Key, iop::identity<_Key>, _Compare>;
        using allocator_type = _Alloc;
        using noconst_iterator = typename rbtree_type::iterator;
        using node_type = typename rbtree_type::node_type;

      public:
        using key_type = _Key;
        using value_type = _Key;
        using key_compare = _Compare;
        using value_compare = _Compare;

        using iterator = typename rbtree_type::const_iterator;
        using const_iterator = typename rbtree_type::const_iterator;
        using pointer = typename rbtree_type::pointer;
        using const_pointer = typename rbtree_type::const_pointer;
        using reference = typename rbtree_type::reference;
        using const_reference = typename rbtree_type::const_reference;

        using size_type = typename rbtree_type::size_type;
        using difference_type = typename rbtree_type::difference_type;

      protected:
        rbtree_type M_rbtree;

      public:
        multiset() : M_rbtree() {}
        multiset(iterator __first, iterator __last) : M_rbtree(__first, __last)
        {}
        multiset(const ::std::initializer_list<value_type> &__rhs)
            : M_rbtree(__rhs)
        {}
        multiset(multiset &__rhs) : M_rbtree(__rhs.begin(), __rhs.end()) {}

        iterator insert(const_reference __v)
        {
            return M_rbtree.insert_equal(__v);
        }
        iterator insert(const_iterator __first, const_iterator __last)
        {
            return M_rbtree.insert_equal(__first, __last);
        }
        iterator erase(const_iterator __pos)
        {
            return M_rbtree.erase(static_cast<noconst_iterator>(__pos));
        }

      public:
        void clear() { return M_rbtree.clear(); }
        iterator begin() { return M_rbtree.begin(); }
        iterator end() { return M_rbtree.end(); }
        const_iterator begin() const { return M_rbtree.begin(); }
        const_iterator end() const { return M_rbtree.end(); }
        size_type size() { return M_rbtree.size(); }
        size_type max_size() { return M_rbtree.max_size(); }
        bool empty() { return M_rbtree.empty(); }
        key_type value_comp() { return M_rbtree.key_comp(); }
        key_type key_comp() { return M_rbtree.key_comp(); }
        value_type min() { return M_rbtree.min(); }
        value_type max() { return M_rbtree.max(); }
    };

}; // namespace iop


#endif // _IOP_MULTISET_H