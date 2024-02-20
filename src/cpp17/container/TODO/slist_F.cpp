#include "../allocator_F.h"
#include "../allocator_traits_F.h"
#include "../iterator_F.h"
#include "../iterator_base_F.h"

#include <initializer_list>

namespace iop {
    struct slist_node_base
    {
        slist_node_base *next;
    };

    template <class _Ty> struct slist_node : public slist_node_base
    {
        _Ty data;
    };

    class slist_iterator_base
    {
      public:
        slist_iterator_base(slist_node_base *__rhs) : node(__rhs) {}
        slist_iterator_base(const slist_iterator_base &__rhs) : node(__rhs.node)
        {}

        void incr() { node = node->next; }
        bool operator==(const slist_iterator_base &__rhs)
        {
            return node == __rhs.node;
        }
        bool operator!=(const slist_iterator_base &__rhs)
        {
            return !(node == __rhs.node);
        }

      protected:
        slist_node_base *node;
    };

    template <class _Ty, class _Ref, class _Ptr>
    class slist_iterator : public slist_iterator_base
    {
      public:
        using iterator = slist_iterator<_Ty, _Ty &, _Ty *>;
        using const_iterator = slist_iterator<_Ty, const _Ty &, const _Ty *>;

        using value_type = _Ty;
        using pointer = _Ty *;
        using const_pointer = const _Ty *;
        using reference = _Ty &;
        using const_reference = const _Ty &;
        using node_type = slist_node<_Ty>;
        using const_node = const slist_node<const _Ty>;
        using self_type = slist_iterator<_Ty, _Ref, _Ptr>;

      public:
        slist_iterator() : slist_iterator_base(nullptr) {}
        slist_iterator(const slist_iterator_base &__rhs)
            : slist_iterator_base(__rhs)
        {}
        slist_iterator(const slist_iterator &__rhs)
            : slist_iterator_base(__rhs.node)
        {}
        slist_iterator(const slist_node<_Ty> &__rhs)
            : slist_iterator_base(__rhs.node)
        {
            ((slist_node<_Ty> *)(this->node))->data = __rhs.data;
        }

        self_type operator++()
        {
            incr();
            return *this;
        }
        self_type operator++(int)
        {
            auto self = *this;
            incr();
            return self;
        }

        reference operator*()
        {
            return static_cast<slist_node<_Ty> *>(node)->data;
        }
        pointer operator->() { return &(*this); }
    };

    template <class _Ty, class _Alloc = iop::allocator<slist_node<_Ty>>>
    class slist
    {
      protected:
        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<_Alloc>;

      public:
        using node_type = slist_node<_Ty>;
        using const_node_type = slist_node<const _Ty>;

        using value_type = _Ty;
        using size_type = typename allocator_traits_type::size_type;
        using difference_type = typename allocator_traits_type::difference_type;

        using pointer = typename allocator_traits_type::pointer;
        using const_pointer = typename allocator_traits_type::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;

        using iterator = slist_iterator<_Ty, _Ty &, _Ty *>;
        using const_iterator = slist_iterator<_Ty, const _Ty &, const _Ty *>;

      protected:
        slist_node_base head;
        [[no_unique_address]] allocator_type __slist_allocator;

      protected:
        slist_node_base *__slist_make_link(slist_node_base *prev_node,
                                           slist_node_base *add_node)
        {
            add_node->next = prev_node->next;
            prev_node->next = add_node;

            return add_node;
        }
        size_t __slist_get_size(slist_node_base *node)
        {
            auto cnt = 0;
            while (node->next) {
                ++cnt;
                node = node->next;
            }
            return cnt;
        }

        node_type *__create_node(const_reference __val)
        {
            auto new_node = __slist_allocator.allocate(1);
            try {
                construct_at(&new_node->data, __val);
                new_node->next = nullptr;
            }
            catch (...) {
                __slist_allocator.deallocate(new_node);
                throw;
            }
            return new_node;
        }

      public:
        slist() : head() { head.next = nullptr; }
        ~slist() { clear(); }
        explicit slist(size_type __len, const_reference __val)
        {
            head.next = nullptr;
            for (size_type i = 0; i != __len; ++i) {
                push_front(__val);
            }
        }

        void push_front(const_reference __val)
        {
            auto ptr = __create_node(__val);
            __slist_make_link(&head, ptr);
        }

        void pop_front()
        {
            auto ptr = static_cast<slist_node<_Ty> *>(head.next);
            head.next = ptr->next;
            destroy_at(&ptr->data);
            __slist_allocator.deallocate(ptr);
        }

        void clear()
        {
            if (empty())
                return;

            while (head.next != nullptr) {
                auto iter = (slist_node<_Ty> *)head.next;
                head.next = (slist_node_base *)iter->next;
                destroy_at(&iter->data);
                __slist_allocator.deallocate(iter);
            }
        }

        void swap(slist<_Ty> &__rhs)
        {
            auto tmp = head.next;
            head.next = __rhs.head.next;
            __rhs.head.next = tmp;
        }

        iterator begin()
        {
            return static_cast<iterator>(
                static_cast<slist_node<_Ty> *>(head.next));
        }
        iterator end() { return iterator(nullptr); }

        size_type size() { return __slist_get_size(); }
        bool empty() { return head.next == nullptr; }
        reference front() { return *begin(); }

        // slist no back(), back() need O(n) time.
        reference back()
        {
            auto iter = head.next;
            while (iter != nullptr) {
                ++iter;
            }
            return static_cast<slist_node<_Ty> *>(iter)->data;
        }
    };

}; // namespace iop

template <class _O, class _C> _O &operator<<(_O &__cout, _C &__res)
{
    __cout << "{";
    for (auto iter = __res.begin(); iter != __res.end(); ++iter)
        __cout << ' ' << *iter;
    __cout << " }";

    return __cout;
}

#include <iostream>
using namespace std;

int main()
{
    iop::slist<int> ls1;
    iop::slist<int> ls2;
    iop::slist<int> ls3(3, 10);

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0)
            ls1.push_front(i);
        else
            ls2.push_front(i);
    }

    cout << ls1 << "\n" << ls2 << "\n";
}