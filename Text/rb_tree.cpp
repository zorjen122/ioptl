#include "../lib/src/iterator_F.h"
#include "../lib/src/allocator_F.h"
#include "../lib/src/allocator_traits_F.h"
#include "../lib/src/initailzer_list_F.h"
#include "../lib/src/config_def.h"
#include "../lib/src/type_trait.h"
#include "../lib/src/alloc_operator_new_F.h"
#include "../lib/src/functional_F.h"

#include <utility>
#include <bits/stl_tree.h>
#include <iostream>

namespace CCC_STL {

    struct __rbtree_color {
        static constexpr bool black = 1;
        static constexpr bool red = 0;
    };

    class __rbtree_node_base {
    public:
        using color_type = bool;
        using blink_type = __rbtree_node_base*;

        color_type color;
        blink_type left;
        blink_type right;
        blink_type parent;

        blink_type min(blink_type __o) {
            while (__o->left != 0)
                __o = __o->left;
            return __o;
        }

        blink_type max(blink_type __o) {
            while (__o->right != 0)
                __o = __o->right;
            return __o;
        }
    };

    template<class _V>
    class __rbtree_node : public __rbtree_node_base {
    public:
        using link_type = __rbtree_node<_V>*;

        _V value_m;

    };

    class rbtree_iterator_base {
    public:
        using blink_type = __rbtree_node_base::blink_type;
        using iteartor_category = CCC_STL::bidirectional_iterator_tag;
        using iterator_traits = CCC_STL::iterator_traits<bidirectional_iterator_tag>;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        blink_type node;

        void increment() {
            if (node->right != 0)
            {
                node = node->right;
                while (node->left != 0)
                    node = node->left;
            }
            else {
                blink_type b = node->parent;
                while (b->right == node) {
                    node = b;
                    b = b->parent;
                }
                if (node->right == b->parent)
                    node = b;
            }
        }

        void decrement() {
            if (node->color == __rbtree_color::red
                && node->parent->parent == node)
            {
                node = node->right;
            }
            else if (node->right != 0)
            {
                node = node->left;
                while (node->right != 0)
                    node = node->right;
            }
            else {
                blink_type b = node->parent;
                while (b->left == node) {
                    node = b;
                    b = b->parent;
                }
                if (node->left == b->parent)
                    node = b;
            }
        }

    };

    template<class _Ty, class _Ref, class _Ptr>
    class rbtree_iterator : public rbtree_iterator_base {
    public:
        using iterator = rbtree_iterator<_Ty, _Ty&, _Ty*>;
        using const_iterator = rbtree_iterator<_Ty, const _Ty&, const _Ty*>;

        using pointer = _Ty*;
        using const_pointer = const _Ty*;
        using reference = _Ty&;
        using const_reference = const _Ty&;
        using link_type = __rbtree_node<_Ty>*;
        using const_node = const __rbtree_node<_Ty>;
        using Self_ = rbtree_iterator<_Ty, _Ref, _Ptr>;


        rbtree_iterator() { node = nullptr; }
        rbtree_iterator(const iterator& __r) { node = __r.node; }
        rbtree_iterator(const link_type __r) { node = __r; }


        Self_ operator++() {
            increment();
            return *this;
        }

        Self_ operator--() {
            decrement();
            return *this;
        }

        Self_ operator++(int) {
            Self_ h = *this;
            increment();

            return h;
        }

        Self_ operator--(int) {
            Self_ h = *this;
            decrement();

            return h;
        }

        reference operator*() { return link_type(node)->value_m; }
        link_type operator->() { return &(operator*()); }

        bool operator==(const rbtree_iterator& __r) { return (__r.node == __r.node); }
        bool operator!=(const rbtree_iterator& __r) { return !(__r.node == __r.node); }
    };

    template<class _K, class _Tv, class _Kv, class _Compare,
        class _Alloc = CCC_allocator<__rbtree_node<_Tv>>>
    class rbtree {
    protected:
        using link_base_type = typename __rbtree_node_base::blink_type;
        using color_type = bool;
        using node_type = __rbtree_node<_Tv>;
        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<node_type, _Alloc>;

    public:
        using key_type = _K;
        using value_type = _Tv;
        using link_type = __rbtree_node<_Tv>*;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        using iterator = rbtree_iterator<value_type, reference, pointer>;
        using const_iterator = const iterator;
        using reverse_iterator = Iterator_STL::reverse_iterator<iterator>;
        using const_reverse_iterator = Iterator_STL::reverse_iterator<const_iterator>;

    public:
        explicit rbtree() : node_size(), key_compare() { init(); }
        explicit rbtree(const _Compare& __r) : node_size(), key_compare(__r) { init(); }
        ~rbtree() { del_node(header); clear(); }

    protected:
        size_type node_size;
        link_type header;
        _Compare key_compare;
        allocator_type V_allocator;

    public:
        link_type set_node() { return V_allocator.allocate(); }
        void del_node(link_type __p) { V_allocator.deallocate(__p); }

        link_type create_node(const_reference __v) {
            link_type h = set_node();
            try {
                construct(&h->value_m, __v);
            }
            catch (...) {
                destroy_node(h);
            }

            return h;
        }

        void destroy_node(link_type __p) {
            destroy(&__p->value_m);
            del_node(__p);
        }


        template<class _Ty>
        using comom_node_t = MPL_L::enable_if_t<MPL_L::oper::or_
            <MPL_L::is_same<_Ty, link_type>, MPL_L::is_same<_Ty, __rbtree_node_base*>>::value, _Ty>;


        static link_type& left(link_type __p) { return (link_type&)(__p->left); }

        static link_type& right(link_type __p) { return (link_type&)(__p->right); }

        static link_type& parent(link_type __p) { return (link_type&)(__p->parent); }

        static reference value(link_type __p) { return __p->value_m; }

        static const _K key(link_type __p) { return _Kv()(value(__p)); }

        static bool& color(link_type __p) { return (bool&)__p->color; }


        link_type& root() const { return (link_type&)header->parent; }
        link_type& leftmost() const { return (link_type&)header->left; }
        link_type& rightmost() const { return (link_type&)header->right; }
        static link_type min(link_type __p) { return (link_type)(__rbtree_node_base::min(__p)); }
        static link_type max(link_type __p) { return (link_type)(__rbtree_node_base::max(__p)); }

        void init() {
            header = set_node();
            color(header) = __rbtree_color::red;
            root() = 0;
            leftmost() = 0;
            rightmost() = 0;
        }

        void erase(link_type __x)
        {
          // Erase without rebalancing.
            while (__x != 0)
            {
                erase(right(__x));
                link_type __y = left(__x);
                destroy_node(__x);
                __x = __y;
            }
        }

        void clear() {
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
        bool empty() { return node_size == 0; }
        size_type max_size() { return size_type(-1); }
        _Compare& key_comp()const { key_compare; }

    public:

        /*  Whether there is a forward child node in the
         *    current opposite direction determines whether
         *    the upper and lower nodes of the has-changed node are exchanged or remain the same.
         */
        void rotate_left(__rbtree_node_base* __p, __rbtree_node_base*& root) {
            __rbtree_node_base* h = __p->right;
            __p->right = h->left;

            if (h->left != 0)
                h->left->parent = __p->parent;
            h->parent = __p;

            bool o = (__p->parent->left == __p) ? 1 : 0;

            if (__p == root)
                root = __p;
            else if (o)
                __p->parent->left = h;
            else
                __p->parent->right = h;

            h->left = __p;
            __p->left = h;
        }

        void rotate_right(__rbtree_node_base* __p, __rbtree_node_base*& root) {
            __rbtree_node_base* h = __p->left;
            __p->left = h->right;

            if (__p->right != 0)
                h->right->parent = __p->parent;
            h->parent = __p;

            bool o = (__p->parent->right == __p) ? 1 : 0;

            if (__p == root)
                root = __p;
            else if (o)
                __p->parent->right = h;
            else
                __p->parent->left = h;

            h->right = __p;
            __p->right = h;
        }

        void rebalance(__rbtree_node_base* __p, __rbtree_node_base*& root) {
            __p->color = __rbtree_color::red;
            while (__p != root && __p->parent->color == __rbtree_color::red) {
                if (__p->parent == __p->parent->parent->left) {
                    __rbtree_node_base* h = __p->parent->right;
                    if (h != 0 && h->color == __rbtree_color::red) {
                        __p->parent->color = __rbtree_color::black;
                        h->color = __rbtree_color::black;
                        __p = __p->parent;
                    }
                    else {
                    //*  parent node becomes a sibling node and they color change black.
                        if (__p == __p->parent->parent->right) {
                            __p = __p->parent;
                            rotate_left(__p, root);
                        }
                        __p->parent->color = __rbtree_color::black;
                        __p->parent->parent->color = __rbtree_color::black;
                        rotate_right(__p->parent->parent, root);
                    }
                }
                else {
                    if (__p == __p->parent->right) {
                        __rbtree_node_base* h = __p->parent->left;
                        if (h != 0 && h->color == __rbtree_color::red) {
                            h->color = __rbtree_color::black;
                            __p->parent->color = __rbtree_color::black;
                            __p = __p->parent;
                        }
                    }
                    else {
                        if (__p == __p->parent->left) {
                            __p = __p->parent;
                            rotate_right(__p, root);
                        }
                        __p->parent->color = __rbtree_color::black;
                        __p->parent->parent->color = __rbtree_color::black;
                        rotate_left(__p->parent->parent, root);
                    }
                }
            }
            root->color = __rbtree_color::black;
        }

        iterator __insert(__rbtree_node_base* __p, __rbtree_node_base* __q, const_reference __v) {

            link_type p = static_cast<link_type>(__p);
            link_type q = static_cast<link_type>(__q);
            link_type h = create_node(__v);

            if (__p != 0 || q == header || key_compare(_Kv()(__v), key(q))) {
                left(q) = h;

                if (q == header) {
                    root() = h;
                    rightmost() = h;
                }
                else if (q == leftmost())
                    leftmost() = h;
            }
            else {
                right(q) == h;
                if (q = rightmost())
                    rightmost() = h;
            }

            parent(h) = q;
            left(h) = 0;
            right(h) = 0;

            rebalance(h, header->parent);
            ++node_size;

            return iterator(h);
        }

        iterator insert_equal(const_reference __v) {
            link_type q = header;
            link_type p = root();

            while (p != nullptr) {
                q = p;
                p = key_compare(_Kv()(__v), key(q)) ? left(p) : right(p);
            }

            return __insert(p, q, __v);
        }

        std::pair<rbtree<_K, _Tv, _Kv, _Compare, _Alloc>::iterator, bool> insert_unique(const_reference __v) {

            bool o = true;
            link_type q = header;
            link_type p = (link_type)root();

            while (p != nullptr) {
                q = p;
                o = key_compare(_Kv()(__v), key(p));
                p = o ? left(p) : right(p);
            }

            iterator h = static_cast<iterator>(q);
            if (o)
                if (h == begin())
                    return ::std::pair(__insert(p, q, __v), true);
                else --h;

            if (key_compare(key(h.node), _Kv()(h)))
                return ::std::pair(__insert(p, q, __v), true);

            return ::std::pair(h, false);
        }

    };


};

template<class _T>
struct identity {
    bool operator()(const _T& __a) { return __a; }
};

template<class _T>
struct less {
    bool operator()(const _T& __a, const _T& __b) { return __a < __b; }
};


int main(int argc, char* argv[]) {
    CCC_STL::rbtree<int, int, CCC_STL::identity<int>, CCC_STL::less<int, int>> itree;
    itree.insert_equal(10);
    itree.insert_equal(11);
    itree.insert_equal(12);
    for (auto i = itree.begin();i != itree.end();++i)
        std::cout << *i << ",";

    return 0;
}