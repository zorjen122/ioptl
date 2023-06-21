#include "../lib/src/iterator_F.h"
#include "../lib/src/allocator_F.h"
#include "../lib/src/allocator_traits_F.h"
#include<iostream>
#include<typeinfo>

using namespace std;

#include "../lib/src/alloc_operator_new_F.h"

namespace CCC_STL {

    struct list_node_base {
        list_node_base* V_next;       list_node_base* V_prev;
    };

    template<class _Ty>
    struct list_node : public list_node_base { _Ty V_data; };


    class list_iterator_base {
    public:
        using iteartor_category = CCC_STL::bidirectional_iterator_tag;
        using iterator_traits = CCC_STL::iterator_traits<iteartor_category>;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        list_node_base* V_node;

        explicit list_iterator_base(list_node_base* __rhs) : V_node(__rhs) {}
        explicit list_iterator_base() {}

        void V_incr() { V_node = V_node->V_next; }
        void V_decr() { V_node = V_node->V_prev; }


        CCC_CONSTEXPR bool operator==(const list_iterator_base& __res)
            const NOEXCEPT {
            return this->V_node == __res.V_node;
        }
        CCC_CONSTEXPR bool operator!=(const list_iterator_base& __res)
            const NOEXCEPT {
            return this->V_node != __res.V_node;
        }

    };


    template<class _Ty, class _Ref, class _Ptr>
    class list_iterator : public list_iterator_base {
    public:
        using iterator = list_iterator<_Ty, _Ty&, _Ty*>;
        using const_iterator = list_iterator<_Ty, const _Ty&, const _Ty*>;

        using pointer = _Ty*;
        using const_pointer = const _Ty*;
        using reference = _Ty&;
        using const_reference = const _Ty&;
        using node = list_node<_Ty>;
        using const_node = const list_node<const _Ty>;
        using _Self = list_iterator<_Ty, _Ref, _Ptr>;


        explicit list_iterator() : list_iterator_base() {}
        explicit list_iterator(node* __rhs) : list_iterator_base(__rhs) {}
        explicit list_iterator(const_iterator& __rhs) : list_iterator_base(__rhs.V_node) {}

        CCC_CONSTEXPR_CXX17 reference operator*() const NOEXCEPT {
            return static_cast<node*>(V_node)->V_data;
        }

        CCC_CONSTEXPR_CXX17 _Self& operator++()NOEXCEPT {
            V_incr();
            return *this;
        }

        CCC_CONSTEXPR_CXX17 _Self& operator++(int) NOEXCEPT {
            _Self& _inner = *this;
            V_incr();
            return _inner;
        }

        CCC_CONSTEXPR_CXX17 _Self& operator--()NOEXCEPT {
            V_decr();
            return *this;
        }

        CCC_CONSTEXPR_CXX17 _Self& operator--(int) NOEXCEPT {
            _Self& _inner = *this;
            V_decr();
            return _inner;
        }


    };

    template<class _Ty, class _Alloc = CCC_allocator<list_node<_Ty>>>
    class list {
    protected:
        using allocator_type = _Alloc;

    public:
        using allocator_traits_type = allocator_traits<_Ty, _Alloc>;
        using value_type = _Ty;     using size_type = allocator_traits_type::size_type;
        using difference_type = allocator_traits_type::difference_type;
        using node = list_node<_Ty>;        using const_node = list_node<const _Ty>;

        using pointer = allocator_traits_type::pointer;
        using const_pointer = allocator_traits_type::const_pointer;
        using reference = value_type&;         using const_reference = const value_type&;

        using iterator = list_iterator<_Ty, _Ty&, _Ty*>;
        using const_iterator = list_iterator<_Ty, const _Ty&, const _Ty*>;


        void V_initializer_list() {
            this->V_data_allocator = allocator_type();
            this->V_node = V_data_allocator.allocate(1);
            this->V_node->V_next = this->V_node;
            this->V_node->V_prev = this->V_node;
        }

        void V_uninitializer_copy(const list<_Ty, _Alloc>& __res) {
            node* __rhs = static_cast<node*>(__res.V_node->V_next);

            try {
                while (__rhs != __res.V_node) {
                    this->push_back(__rhs->V_data);
                    __rhs = static_cast<node*>(__rhs->V_next);
                }
            }
            catch (...) {
                while (__rhs != __res.V_node) {
                    node* tmp = __rhs;
                    tmp = static_cast<node*>(tmp->V_prev);
                    destroy(__rhs);
                    __rhs = tmp;
                }
                throw;
            }
        }

        explicit list() NOEXCEPT {
            V_initializer_list();
        }

        explicit list(const value_type __v, const size_type __sz) NOEXCEPT {
            V_initializer_list();

            for (size_type i = 0;i < __sz - 1;++i)
                push_back(__v);
        }

        explicit list(list<_Ty, _Alloc>&& __res) NOEXCEPT {
            this->V_data_allocator = __res.V_data_allocator;
            this->V_node = __res.V_node;

            __res.V_data_allocator = allocator_type();
            __res.V_node->V_prev = nullptr;
            __res.V_node->V_next = nullptr;
        }

        explicit list(const list<_Ty, _Alloc>& __res) NOEXCEPT {
            this->V_data_allocator = allocator_type();
            V_uninitializer_copy(__res);
        }

        explicit list(iterator __first, iterator __last) NOEXCEPT {
            insert(end(), __first, __last);
        }

        ~list() {
            if (this->V_node != nullptr) {
                this->clear();
                this->V_data_allocator.deallocate(this->V_node, 1);
            }
        }


    protected:
        node* V_node;
        allocator_type V_data_allocator;

    public:
        CCC_CONSTEXPR node* V_unit_create(const _Ty& __val) NOEXCEPT {
            node* __inner = V_data_allocator.allocate(1);
            construct(&__inner->V_data, __val);
            return __inner;
        }

        CCC_CONSTEXPR void clear() {
            node* __res = static_cast<node*>(this->V_node->V_next);
            while (__res != this->V_node) {
                node* __inner = __res;
                __res = static_cast<node*>(__res->V_next);
                destroy(&__inner->V_data);

                this->V_data_allocator.deallocate(__inner, 1);
            }
            this->V_node->V_next = this->V_node;
            this->V_node->V_prev = this->V_node;
        }

        CCC_CONSTEXPR iterator insert(iterator __postition, value_type __val) {
            node* __inner = V_unit_create(__val);
            __inner->V_next = __postition.V_node;
            __inner->V_prev = __postition.V_node->V_prev;

            __postition.V_node->V_prev->V_next = __inner;
            __postition.V_node->V_prev = __inner;       // prev end.

            return static_cast<iterator>(__inner);
        }

        CCC_CONSTEXPR iterator insert(iterator __postition) {
            return static_cast<iterator>(this->insert(__postition, _Ty()));
        }

        CCC_CONSTEXPR void insert(iterator __pos, iterator __first, iterator __last) {
            iterator i = __first;
            for (;i != __last;++i)
                insert(__pos, *i);
        }


        // copy to delete
        CCC_CONSTEXPR iterator erase(iterator __position) {
            list_node_base* __next_node = __position.V_node->V_next;
            list_node_base* __prev_node = __position.V_node->V_prev;

            node* __res = static_cast<node*>(__position.V_node);

            __next_node->V_prev = __prev_node;
            __prev_node->V_next = __next_node;

            destroy(&__res->V_data);
            V_data_allocator.deallocate(__res, 1);

            return static_cast<iterator>(static_cast<node*>(__next_node));
        }

        CCC_CONSTEXPR iterator erase(iterator __frist, iterator __last) {

            if (__frist.V_node == __last.V_node) {
                return this->erase(static_cast<iterator>(__frist.V_node));
            }

            node* __res = __frist.V_node;
            while (__res->V_next != __last.V_node) {
                node* __inner = __res;
                __res = static_cast<node*>(__res->V_next);
                erase(static_cast<iterator>(__inner));
            }

            return static_cast<iterator>(__res);

        }

        CCC_CONSTEXPR bool empty() NOEXCEPT { return (this->V_node->V_next == this->V_node); }

        CCC_CONSTEXPR bool empty() const NOEXCEPT { return (this->V_node->V_next == this->V_node); }

        CCC_CONSTEXPR size_type size() NOEXCEPT {
            size_type __count = 0;
            for (iterator __iter = this->begin();
                __iter != this->end();++__iter, ++__count);
            return __count;
        }

        CCC_CONSTEXPR void swap(iterator& __q, iterator& __p) {
            CCC_STL::swap(__q, __p);
        }

        CCC_CONSTEXPR size_type max_size() const NOEXCEPT {
            return size_type(-1);
        }

        CCC_CONSTEXPR value_type front() const {
            return *this->begin();
        }

        CCC_CONSTEXPR value_type back() const {
            return *(--this->end());
        }


        iterator begin() NOEXCEPT { return static_cast<iterator>(static_cast<node*>(this->V_node->V_next)); }
        const_iterator begin() const NOEXCEPT { return static_cast<const_iterator>(this->V_node->V_next); }

        iterator end() NOEXCEPT { return static_cast<iterator>(this->V_node); }
        const_iterator end() const NOEXCEPT { return static_cast<const_iterator>(this->V_node); }

        iterator push_back(const _Ty& __val) NOEXCEPT { return insert(this->end(), __val); }
        const_iterator push_back(const _Ty& __val) const NOEXCEPT {
            return static_cast<const_iterator>(insert(this->end(), __val));
        }

        iterator push_front(const _Ty& __val) NOEXCEPT { return insert(this->begin(), __val); }
        const_iterator push_front(const _Ty& __val) const NOEXCEPT {
            return static_cast<const_iterator>(insert(this->begin(), __val));
        }

        iterator pop_back() NOEXCEPT(NOEXCEPT(!this->empty())) {
            return erase(--this->end());
        }
        const_iterator pop_back()
            const NOEXCEPT(NOEXCEPT(!this->empty())) {
            return static_cast<const_iterator>(erase(this->end()));
        }

        iterator pop_front() NOEXCEPT(NOEXCEPT(!this->empty())) {
            return erase(this->begin());
        }

        const_iterator pop_front()
            const NOEXCEPT(NOEXCEPT(assert(!this->empty()))) {
                {
                    return static_cast<const_iterator>(erase(this->begin()));
                }
        }

        void remove(const value_type& __v) {
            if (!empty()) {
                iterator it = begin();
                while (it != end()) {
                    iterator r = it;
                    ++r;
                    if (*it == __v)
                        erase(it);
                    it = r;
                }
            }
        }

        void unique() {
            if (!empty()) {
                iterator it = begin();
                iterator nit = it;
                while (++nit != end()) {
                    if (*it == *nit)
                        erase(nit);
                    else {
                        it = nit;
                    }
                    nit = it;
                }
            }
        }

        void transfer(iterator __pos, iterator __first, iterator __last) {
            if (__pos != __last) {
              // Remove [first, last) from its old position.
                __last.V_node->V_prev->V_next = __pos.V_node;
                __first.V_node->V_prev->V_next = __last.V_node;
                __pos.V_node->V_prev->V_next = __first.V_node;

                // Splice [first, last) into its new position.
                list_node_base* __tmp = __pos.V_node->V_prev;
                __pos.V_node->V_prev = __last.V_node->V_prev;
                __last.V_node->V_prev = __first.V_node->V_prev;
                __first.V_node->V_prev = __tmp;
            }
        }

        void splice(iterator __pos, list<_Ty>& __rhs) {
            if (!__rhs.empty()) {
                transfer(__pos, __rhs.begin(), __rhs.end());
            }
        }


        CCC_CONSTEXPR void sort() NOEXCEPT {

            node* __it = this->V_node;
            while (__it->V_next != this->V_node) {
                __it = static_cast<node*>(__it->V_next);
                node* __subit = static_cast<node*>(__it->V_next);
                while (__subit != this->V_node) {
                    if (__it->V_data > __subit->V_data) {
                        value_type __tmp = __it->V_data;
                        __it->V_data = __subit->V_data;
                        __subit->V_data = __tmp;
                    }
                    __subit = static_cast<node*>(__subit->V_next);
                }
            }
        }

        CCC_CONSTEXPR void merge(list<_Ty, _Alloc>& __res) NOEXCEPT {
            if (empty()) {
                this->V_node = std::move(__res.V_node);
                return;
            }

            if (!__res.empty()) {
                iterator it = this->begin();
                iterator eit = __res.begin();

                iterator rpos = __res.begin();
                for (;eit != __res.end();) {
                    if (*eit >= *it && it != this->end())
                        ++it;
                    else {
                        insert(it, *eit);
                        ++eit;
                    }
                }

            }

        }

        CCC_CONSTEXPR void reverse() NOEXCEPT {
            if (!empty()) {
                size_type i = size() - (size() / 2);
                iterator rit = end();
                for (iterator it = this->begin();i != 0;--i, ++it) {
                    --rit;
                    swap(it, rit);
                }
            }
        }

        CCC_CONSTEXPR list<_Ty, _Alloc>& operator=(const list<_Ty, _Alloc>& __res) {
            if (&__res == this) { return *this; }

            clear();
            V_uninitializer_copy(__res);

            return *this;
        }

        CCC_CONSTEXPR list<_Ty, _Alloc>&& operator=(const list<_Ty, _Alloc>&& __res) {
            if (&__res == this) { return std::move(*this); }

            clear();

            V_node = __res.V_node;
            __res.V_node->V_prev = nullptr;
            __res.V_node->V_next = nullptr;

            return std::move(*this);
        }

        bool operator!=(const list<_Ty, _Alloc>& __res) {
            return !(__res.V_node == V_node);
        }

        bool operator==(const list<_Ty, _Alloc>& __res) {
            return (__res.V_node == V_node);
        }

    };
};

template<class _Ty>
void display(CCC_STL::list<_Ty>& __res) {
    for (auto&& it : __res)
        std::cout << it << ",";
    std::cout << endl;
}

int main() {

    CCC_STL::list<int>rhs;
    rhs.push_back(120);
    rhs.push_back(102);
    std::cout << rhs.empty() << endl;
    rhs.push_back(103);
    rhs.push_back(1111);
    rhs.push_back(20);
    rhs.push_front(1200);

    for (auto&& iter : rhs) {
        ::std::cout << iter << ",";
    }
    ::std::cout << "\n This is sort function:\n";
    rhs.sort();

    for (auto&& iter : rhs) {
        ::std::cout << iter << ",";
    }


    rhs.pop_back();
    rhs.pop_front();

    cout << "\nNew list" << endl;
    for (auto&& iter : rhs) {
        ::std::cout << iter << ",";
    }


    std::cout << "Size -> " << rhs.size() << endl;

    std::cout << "Swap before--> " << *(++rhs.begin()) << "\t" << *(--rhs.end()) << endl;

    rhs.swap(++rhs.begin(), (--rhs.end()));

    std::cout << "Swap after--> " << *(++rhs.begin()) << "\t" << *(--rhs.end()) << endl;



    rhs.clear();

    rhs.insert(rhs.begin());
    rhs.insert(++rhs.begin());
    rhs.insert(++rhs.begin(), 20);
    rhs.remove(20);

    for (CCC_STL::list<int>::iterator i = rhs.begin();i != rhs.end();++i) {
        std::cout << *i << ",";
    }
    std::cout << "\n\n";


    CCC_STL::list<int>opt(999, 4);
    CCC_STL::list<int>P(342, 8);
    std::cout << "list:" << endl;
    for (CCC_STL::list<int>::iterator i = P.begin();i != P.end();++i) {
        std::cout << *i << ",";
    }

    opt.push_back(10);
    opt.push_back(11);
    CCC_STL::list<int>::iterator z = CCC_STL::find(opt.begin(), opt.end(), 10);

    std::cout << "find:" << endl;
    for (CCC_STL::list<int>::iterator i = z;i != opt.end();++i) {
        std::cout << *i << ",";
    }

    rhs.splice(z, P);
    std::cout << "split:" << endl;

    for (CCC_STL::list<int>::iterator i = opt.begin();i != opt.end();++i) {
        std::cout << *i << ",";
    }

    std::cout << "\nSize -> " << rhs.size() << "\n\n";


    std::cout << "merge:" << endl;
    P.push_back(2);
    P.push_back(9);
    P.push_back(1);
    P.push_front(23);
    display(opt);
    opt.sort();
    std::cout << "opt sort: " << endl;
    display(opt);

    std::cout << "\n\n";
    display(P);
    P.sort();
    std::cout << "P sort: " << endl;
    display(P);

    std::cout << "\n\n";
    std::cout << "back is " << P.back() << endl;

    P.merge(opt);

    cout << "P merge: " << "\n";
    display(P);


    cout << "\nopt merge: " << endl;
    opt.merge(P);
    display(opt);

    cout << "\n\nreverse P: ";
    P.reverse();
    display(P);

    std::cout << "\n\n";
    opt = P;
    cout << "operator opt=";
    display(opt);

    return 0;
}