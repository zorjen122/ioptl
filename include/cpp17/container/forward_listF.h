#ifndef _IOP_FORWARD_LIST_DEF
#define _IOP_FORWARD_LIST_DEF

#include "../allocatorF.h"
#include "../iteratorF.h"
#include "../traits/allocator_traitsF.h"
#include "../util/iterator_baseF.h"

#include <initializer_list>


namespace iop
{

template <class _Ty> struct slist_node
{
    slist_node() : data(), next(nullptr)
    {
    }

    bool operator==(const slist_node &slist)
    {
        return next == slist.next;
    }

    _Ty data;
    slist_node *next;
};

template <class _Ty, class _Ref, class _Ptr> class slist_iterator
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

  public:
    node_type *node;

  public:
    slist_iterator() : node(nullptr)
    {
    }
    slist_iterator(const iterator &__iter) : node(__iter.node)
    {
    }
    slist_iterator(const const_iterator &__iter) : node(__iter.node)
    {
    }
    slist_iterator(node_type *__lhs) : node(__lhs)
    {
    }

    slist_iterator &operator++()
    {
        node = node->next;
        return *this;
    }
    slist_iterator operator++(int)
    {
        auto ret = *this;
        node = node->next;
        return ret;
    }

    reference operator*()
    {
        return node->data;
    }
    pointer operator->()
    {
        return &(node->data);
    }

    bool operator==(const iterator &__lhs)
    {
        return node == __lhs.node;
    }

    bool operator!=(const iterator &__lhs)
    {
        return !(node == __lhs.node);
    }
};

template <class _Ty, class _Alloc = iop::allocator<_Ty>> class slist
{
  protected:
    using allocator_type = _Alloc;
    using allocator_traits_type = allocator_traits<_Alloc>;
    using allocator_node_type = typename allocator_traits_type::template rebind_alloc<slist_node<_Ty>>::other;

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
    node_type head;
    [[no_unique_address]] allocator_node_type __slist_allocator;

  protected:
    node_type *__slist_make_link(node_type *prev_node, node_type *add_node)
    {
        add_node->next = prev_node->next;
        prev_node->next = add_node;

        return add_node;
    }
    size_t __slist_get_size(node_type *node)
    {
        auto cnt = 0;
        while (node->next)
        {
            ++cnt;
            node = node->next;
        }
        return cnt;
    }

    node_type *__create_node(const_reference __val)
    {
        auto new_node = __slist_allocator.allocate(1);
        try
        {
            construct_at(&new_node->data, __val);
            new_node->next = nullptr;
        }
        catch (...)
        {
            __slist_allocator.deallocate(new_node);
            throw;
        }
        return new_node;
    }

    void __init()
    {
        head.next = nullptr;
    }

  public:
    slist()
    {
        __init();
    }
    ~slist()
    {
        clear();
    }
    explicit slist(size_type count)
    {
        __init();
        insert_after(before_begin(), count, value_type());
    }
    template <class InputIt, class = typename mpls::enable_if_t<!mpls::is_integral_v<InputIt>>>
    slist(InputIt first, InputIt last)
    {
        __init();
        insert_after(before_begin(), first, last);
    }
    slist(const slist &other)
    {
        __init();
        insert_after(before_begin(), other.begin(), other.end());
    }
    slist(slist &&other)
    {
        head = other.head;
        other.head.next = nullptr;
    }

    slist(const std::initializer_list<value_type> &ilist) : head()
    {
        __init();
        insert_after(before_begin(), ilist.begin(), ilist.end());
    }

    explicit slist(size_type __len, const_reference __val)
    {
        __init();
        for (size_type i = 0; i != __len; ++i)
        {
            push_front(__val);
        }
    }

    void assign(size_type count, const value_type &value)
    {
        iterator iter = before_begin();

        for (; count > 0; --count)
        {
            if (iter.node->next == nullptr)
                break;
            else
                *(++iter) = value;
        }

        if (count > 0)
            insert_after(iter, count, value);
        else
            erase_after(iter, end());
    }

    template <class InputIt, class = typename mpls::enable_if_t<!mpls::is_integral_v<InputIt>>>
    void assign(InputIt first, InputIt last)
    {
        iterator iter = before_begin();

        for (; first != last;)
        {
            if (iter.node->next == nullptr)
                break;
            else
                *(++iter) = *(first++);
        }

        if (first != last)
            insert_after(iter, first, last);
        else
            erase_after(iter, end());
    }

    void assign(std::initializer_list<value_type> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }

    void push_front(const_reference __val)
    {
        auto node = __create_node(__val);
        __slist_make_link(&head, node);
    }

    template <class... Args> reference emplace_front(Args &&...args)
    {
        push_front(value_type(Fiop::forward<Args>(args)...));

        return front();
    }

    void pop_front()
    {
        auto ptr = static_cast<node_type *>(head.next);
        head.next = ptr->next;
        destroy_at(&ptr->data);
        __slist_allocator.deallocate(ptr);
    }

    //* insert_after
    iterator insert_after(const_iterator pos, const value_type &value)
    {
        return iterator(__slist_make_link(pos.node, __create_node(value)));
    }

    iterator insert_after(const_iterator pos, value_type &&value)
    {
        auto node = __create_node(Fiop::move(value));
        return iterator(__slist_make_link(pos.node, node));
    }

    iterator insert_after(const_iterator pos, size_type count, const value_type &value)
    {
        auto iter = pos;
        for (; count > 0; --count)
            iter = insert_after(iter, value);

        return iter;
    }

    template <class InputIt, class = typename mpls::enable_if_t<!mpls::is_integral_v<InputIt>>>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last)
    {
        auto iter = pos;
        for (; first != last; ++first)
            iter = insert_after(iter, *first);

        return iter;
    }

    iterator insert_after(const_iterator pos, const std::initializer_list<value_type> &ilist)
    {
        return insert_after(pos, ilist.begin(), ilist.end());
    }

    // erase_after
    iterator erase_after(const_iterator pos)
    {
        // cpprefrence-std say if delete pos is nullptr, then return end()
        // <==> iterator(nullptr) but in gcc, we dont see this std. for
        // example: for(it = before_begin();
        // it != end()) it = erase_after(it) is error.
        if (pos.node == nullptr || pos.node->next == nullptr)
            return iterator(nullptr);

        auto del_node = static_cast<node_type *>(pos.node->next);
        pos.node->next = del_node->next;

        destroy_at(&del_node->data);
        __slist_allocator.deallocate(del_node);

        return ++pos;
    }
    iterator erase_after(const_iterator first, const_iterator last)
    {
        auto stop = first;
        while (stop != last)
            stop = erase_after(first);

        return first;
    }

    template <class... _Args> iterator emplace_after(const_iterator pos, _Args &&...args)
    {
        auto node = __create_node(value_type(Fiop::forward<_Args>(args)...));

        return iterator(__slist_make_link(pos.node, node));
    }

    void resize(size_type count, const value_type &value)
    {
        iterator iter = before_begin();

        for (; count > 0; --count)
        {
            if (iter.node->next == nullptr)
                break;
            else
                ++iter;
        }

        // size() - count
        if (count > 0)
            insert_after(iter, count, value);
        else
            erase_after(iter, end());
    }

    void resize(size_type count)
    {
        resize(count, value_type());
    }

    void clear()
    {
        if (empty())
            return;

        while (head.next != nullptr)
        {
            auto iter = (node_type *)head.next;
            head.next = (node_type *)iter->next;
            destroy_at(&iter->data);
            __slist_allocator.deallocate(iter);
        }
    }

    void swap(slist &__rhs)
    {
        ::std::swap(__rhs.head, head);
    }

    void sort()
    {
    }

    template <class Compare> void merge(slist &other, Compare comp)
    {
    }

    void merge(slist &other)
    {
        if (other.empty())
            return;
    }

    void merge(slist &&other)
    {
    }

    iterator before_begin()
    {
        return static_cast<iterator>(static_cast<node_type *>(Fiop::addressof(head)));
    }
    const_iterator before_begin() const
    {
        return (const_iterator) static_cast<iterator>(static_cast<node_type *>(Fiop::addressof(head)));
    }

    iterator begin()
    {
        return static_cast<iterator>(head.next);
    }
    const_iterator begin() const
    {
        return (const_iterator) static_cast<iterator>(static_cast<node_type *>(head.next));
    }
    iterator end()
    {
        return iterator(nullptr);
    }
    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    size_type size()
    {
        return __slist_get_size(static_cast<node_type *>(&head));
    }
    size_type max_size()
    {
        return size_type(-1);
    }
    bool empty()
    {
        return head.next == nullptr;
    }
    reference front()
    {
        return *begin();
    }

    // slist no back(), back() need O(n) time.
    reference back()
    {
        auto iter = &head;
        while (iter->next != nullptr)
        {
            iter = iter->next;
        }
        return static_cast<node_type *>(iter)->data;
    }

    // TODO
    //  resize/swap/....

    bool operator==(const slist &__lhs)
    {
        return head == __lhs.head;
    }
};

template <class _Ty, class _Alloc = iop::allocator<slist_node<_Ty>>> using forward_list = slist<_Ty, _Alloc>;

}; // namespace iop

#endif
