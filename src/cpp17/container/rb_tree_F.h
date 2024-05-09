#pragma once

#ifndef _RBTREE_IOP_
#define _RBTREE_IOP_

#include "../allocator_F.h"
#include "../allocator_traits_F.h"
#include "../cppconfig.h"
#include "../functionals.h"
#include "../iterator_F.h"
#include "../type_traits.h"
#include "../utilitys.h"

#include <initializer_list>

namespace iop {

    union __rbtree_color
    {
        static constexpr bool red = 0;
        static constexpr bool black = 1;
    };

    class __rbtree_node_base
    {
      public:
        using color_type = bool;
        using blink_type = __rbtree_node_base *;

        color_type color;
        blink_type left;
        blink_type right;
        blink_type parent;

        blink_type min(blink_type __o)
        {
            while (__o->left != 0)
                __o = __o->left;
            return __o;
        }

        blink_type max(blink_type __o)
        {
            while (__o->right != 0)
                __o = __o->right;
            return __o;
        }
    };

    template <class _V> class __rbtree_node : public __rbtree_node_base
    {
      public:
        using link_type = __rbtree_node<_V> *;

        _V value_m;
    };

    class rbtree_iterator_base
    {
      public:
        using blink_type = __rbtree_node_base::blink_type;
        using iterator_category = iop::bidirectional_iterator_tag;
        using iterator_traits =
            iop::iterator_traits<bidirectional_iterator_tag>;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        blink_type node;

        void increment()
        {
            if (node->right != 0) {
                blink_type p = node->right;
                while (p->left != 0)
                    p = p->left;
                node = p;
            }
            else {
                blink_type p = node->parent;
                while (p->right == node) {
                    node = p;
                    p = p->parent;
                }
                if (node->right != p)
                    node = p;
            }
        }

        void decrement()
        {
            if (node->color == __rbtree_color::red &&
                node->parent->parent == node) {
                node = node->right;
            }
            else if (node->left != 0) {
                blink_type p = node->left;
                while (p->right != 0)
                    p = p->right;
                node = p;
            }
            else {
                blink_type p = node->parent;
                while (p->left == node) {
                    node = p;
                    p = p->parent;
                }
                node = p;
            }
        }
    };

    template <class _Ty, class _Ref, class _Ptr>
    class rbtree_iterator : public rbtree_iterator_base
    {
      public:
        using iterator = rbtree_iterator<_Ty, _Ty &, _Ty *>;
        using const_iterator = rbtree_iterator<_Ty, const _Ty &, const _Ty *>;

        using pointer = _Ptr;
        using const_pointer = const pointer;
        using reference = _Ref;
        using const_reference = const reference;
        using link_type = __rbtree_node<_Ty> *;
        using const_node = const __rbtree_node<_Ty>;
        using Self_ = rbtree_iterator<_Ty, _Ref, _Ptr>;

        rbtree_iterator() { node = nullptr; }
        rbtree_iterator(const_iterator &__r) { node = __r.node; }
        rbtree_iterator(const link_type __r) { node = __r; }

        Self_ &operator++()
        {
            increment();
            return *this;
        }

        Self_ &operator--()
        {
            decrement();
            return *this;
        }

        Self_ operator++(int)
        {
            Self_ h = *this;
            increment();
            return h;
        }

        Self_ operator--(int)
        {
            Self_ h = *this;
            decrement();
            return h;
        }

        reference operator*() { return link_type(node)->value_m; }
        pointer operator->() { return &(operator*()); }

        bool operator==(const rbtree_iterator &__r)
        {
            return (__r.node == node);
        }
        bool operator!=(const rbtree_iterator &__r)
        {
            return !(__r.node == node);
        }
    };

    template <class _K, class _Tv, class _Kv, class _Compare,
              class _Alloc = iop::allocator<__rbtree_node<_Tv>>>
    class rbtree
    {
      protected:
        using link_base_type = __rbtree_node_base *;
        using color_type = bool;
        using allocator_node_type = _Alloc;
        using allocator_traits_type = allocator_traits<_Alloc>;

      public:
        using key_type = _K;
        using value_type = _Tv;
        using link_type = __rbtree_node<_Tv> *;
        using node_type = __rbtree_node<_Tv>;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = rbtree_iterator<value_type, reference, pointer>;
        using const_iterator = typename iterator::const_iterator;

      public:
        rbtree(const _Compare &__r = _Compare())
            : node_size(0), key_compare(__r)
        {
            init();
        }
        ~rbtree()
        {
            clear();
            del_node(header);
        }

        rbtree(const rbtree &__rhs) : rbtree(__rhs.begin(), __rhs.end()) {}

        rbtree(iterator __first, iterator __last)
            : node_size(0), key_compare(_Compare())
        {
            init();
            insert_equal(__first, __last);
        }

        rbtree(const_iterator __first, const_iterator __last)
        : rbtree(__first, __last) {};        

        rbtree(const ::std::initializer_list<value_type> &__rhs)
            : node_size(0), key_compare(_Compare())
        {
            init();
            auto i = __rhs.begin();
            while (i != __rhs.end()) {
                insert_equal(*i);
                ++i;
            }
        }

      protected:
        size_type node_size;
        link_type header;
        _Compare key_compare;

      protected:
        link_type set_node() { return allocator_node_type::allocate(1); }
        void del_node(link_type __p) { allocator_node_type::deallocate(__p); }

        link_type create_node(const_reference __v)
        {
            link_type h = set_node();
            try {
                construct_at(&h->value_m, __v);
            }
            catch (...) {
                del_node(h);
            }

            return h;
        }

        void destroy_node(link_type __p)
        {
            iop::destroy_at(::std::addressof(__p->value_m));
            del_node(__p);
        }

        void init()
        {
            header = set_node();
            color(header) = __rbtree_color::red;

            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }

        template <class _Ty>
        using comom_node_t = mpls::enable_if_t<
            mpls::oper::or_<mpls::is_same<_Ty, link_type>,
                            mpls::is_same<_Ty, __rbtree_node_base *>>::value,
            _Ty>;

      public:
        static reference value(link_type __p) { return __p->value_m; }
        static const _K key(link_type __p) { return _Kv()(value(__p)); }
        static color_type &color(link_type __p)
        {
            return (color_type &)__p->color;
        }
        link_type &root() const { return (link_type &)header->parent; }
        link_type &leftmost() const { return (link_type &)header->left; }
        link_type &rightmost() const { return (link_type &)header->right; }
        static link_type min(link_type __p)
        {
            return (link_type)(__rbtree_node_base::min(__p));
        }
        static link_type max(link_type __p)
        {
            return (link_type)(__rbtree_node_base::max(__p));
        }

        static link_type &left(link_type __p)
        {
            return (link_type &)(__p->left);
        }
        static link_type &right(link_type __p)
        {
            return (link_type &)(__p->right);
        }
        static link_type &parent(link_type __p)
        {
            return (link_type &)(__p->parent);
        }

        static link_type &left(link_base_type __p)
        {
            return (link_type &)(__p->left);
        }
        static link_type &right(link_base_type __p)
        {
            return (link_type &)(__p->right);
        }
        static link_type &parent(link_base_type __p)
        {
            return (link_type &)(__p->parent);
        }
        static reference value(link_base_type __p)
        {
            return ((link_type)__p)->value_m;
        }
        static const _K key(link_base_type __p) { return _Kv()(value(__p)); }
        static bool &color(link_base_type __p)
        {
            return (bool &)((link_type)__p)->color;
        }

        void erase(link_type __x)
        {
            // Erase without rebalancing.
            while (__x != 0) {
                erase(right(__x));
                link_type __y = left(__x);
                destroy_node(__x);
                __x = __y;
            }
        }

        void clear()
        {
            if (node_size != 0) {
                erase(root());

                leftmost() = header;
                root() = 0;
                rightmost() = header;
                node_size = 0;
            }
        }

      public:
        iterator begin() { return static_cast<iterator>(leftmost()); }
        iterator end() { return static_cast<iterator>(header); }
        const_iterator begin() const
        {
            return static_cast<const_iterator>(
                static_cast<iterator>(leftmost()));
        }
        const_iterator end() const
        {
            return static_cast<const_iterator>(static_cast<iterator>(header));
        }

        bool empty() { return node_size == 0; }
        size_type max_size() { return size_type(-1); }
        size_type size() { return node_size; }
        _Compare key_comp() const { return key_compare; }

      public:
        /*  Whether there is a forward child node in the
         *    current opposite direction determines whether
         *    the upper and lower nodes of the has-changed node are exchanged or
         * remain the same.
         */
        void rotate_left(__rbtree_node_base *__p, __rbtree_node_base *&root)
        {
            __rbtree_node_base *h = __p->right;
            __p->right = h->left;

            if (h->left != nullptr)
                h->left->parent = __p;
            h->parent = __p->parent;

            if (__p == root)
                root = h;
            else if (__p->parent->left == __p)
                __p->parent->left = h;
            else
                __p->parent->right = h;

            h->left = __p;
            __p->parent = h;
        }

        void rotate_right(__rbtree_node_base *__p, __rbtree_node_base *&root)
        {
            __rbtree_node_base *h = __p->left;
            __p->left = h->right;

            if (h->right != nullptr)
                h->right->parent = __p;
            h->parent = __p->parent;

            if (__p == root)
                root = h;
            else if (__p->parent->left == __p)
                __p->parent->left = h;
            else
                __p->parent->right = h;

            h->right = __p;
            __p->parent = h;
        }

        void rebalance(__rbtree_node_base *__p, __rbtree_node_base *&root)
        {
            __p->color = __rbtree_color::red;
            while (__p != root && __p->parent->color == __rbtree_color::red) {
                if (__p->parent == __p->parent->parent->left) {
                    __rbtree_node_base *h = __p->parent->parent->right;
                    if (h != nullptr && h->color == __rbtree_color::red) {
                        __p->parent->color = __rbtree_color::black;
                        h->color = __rbtree_color::black;
                        __p->parent->parent->color = __rbtree_color::red;
                        __p = __p->parent->parent;
                    }
                    else {
                        //*  parent node becomes a sibling node and they color
                        // change black.
                        if (__p == __p->parent->right) {
                            __p = __p->parent;
                            rotate_left(__p, root);
                        }
                        __p->parent->color = __rbtree_color::black;
                        __p->parent->parent->color = __rbtree_color::red;
                        rotate_right(__p->parent->parent, root);
                    }
                }
                else {
                    __rbtree_node_base *h = __p->parent->parent->left;
                    if (h != nullptr && h->color == __rbtree_color::red) {
                        __p->parent->color = __rbtree_color::black;
                        h->color = __rbtree_color::black;
                        __p->parent->parent->color = __rbtree_color::red;
                        __p = __p->parent->parent;
                    }
                    else {
                        if (__p == __p->parent->left) {
                            __p = __p->parent;
                            rotate_right(__p, root);
                        }
                        __p->parent->color = __rbtree_color::black;
                        __p->parent->parent->color = __rbtree_color::red;
                        rotate_left(__p->parent->parent, root);
                    }
                }
            }
            root->color = __rbtree_color::black;
        }

        iterator __insert(__rbtree_node_base *__p, __rbtree_node_base *__q,
                          const_reference __v)
        {

            link_type next_node = static_cast<link_type>(__p);
            link_type prev_node = static_cast<link_type>(__q);
            link_type h = create_node(__v);

            if (next_node != nullptr || prev_node == header ||
                key_compare(_Kv()(__v), key(prev_node))) {
                left(prev_node) = h;

                if (prev_node == header) {
                    root() = h;
                    rightmost() = h;
                }
                else if (prev_node == leftmost())
                    leftmost() = h;
            }
            else {
                right(prev_node) = h;
                if (prev_node == rightmost())
                    rightmost() = h;
            }

            parent(h) = prev_node;
            left(h) = 0;
            right(h) = 0;

            rebalance(h, header->parent);
            ++node_size;

            return iterator(h);
        }

        iterator insert_equal(const_reference __v)
        {
            link_type iprev = header;
            link_type inext = root();

            while (inext != 0) {
                iprev = inext;
                inext = key_compare(_Kv()(__v), key(inext)) ? left(inext)
                                                            : right(inext);
            }

            return __insert(inext, iprev, __v);
        }

        iterator insert_equal(iterator __first, iterator __last)
        {
            auto i = __first;
            while (i != __last) {
                insert_equal(*i);
                ++i;
            }
            return insert_unique(*i).first;
        }

        iterator insert_equal(const_iterator __first, const_iterator __last)
        {
            return insert_equal((iterator)__first,(iterator) __last);
        }

        std::pair<rbtree<_K, _Tv, _Kv, _Compare, _Alloc>::iterator, bool>
        insert_unique(const_reference __v)
        {

            bool op = true;
            link_type iprev = header;
            link_type inext = root();

            while (inext != 0) {
                iprev = inext;
                op = key_comp()(_Kv()(__v), key(inext));
                inext = op ? left(inext) : right(inext);
            }

            iterator h = static_cast<iterator>(iprev);
            if (op) {
                if (h == begin())
                    return ::std::pair<iterator, bool>(
                        __insert(inext, iprev, __v), true);
                else
                    --h;
            }
            if (key_comp()(key(h.node), _Kv()(__v)))
                return ::std::pair<iterator, bool>(__insert(inext, iprev, __v),
                                                   true);

            return ::std::pair<iterator, bool>(h, false);
        }

        std::pair<rbtree<_K, _Tv, _Kv, _Compare, _Alloc>::iterator, bool>
        insert_unique(iterator __first, iterator __last)
        {
            try {
                auto pos = __first;
                while (__first != __last)
                    insert_unique(*(__first++));

                return ::std::pair<iterator, bool>(__first, true);
            }
            catch (...) {
                // TODO
            }
        }

        std::pair<rbtree<_K, _Tv, _Kv, _Compare, _Alloc>::iterator, bool>
        insert_unique(const_iterator __first, const_iterator __last)
        { return insert_unique((iterator)__first, (iterator)__last); }
    };

}; // namespace iop

#endif // _RBTREE_IOP_