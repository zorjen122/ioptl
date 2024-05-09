#include "../../container/deque_F.h"
#include "../../functionals.h"

#include <chrono>
#include <iostream>

using namespace std;

void deqBaseFunc() {
    iop::deque<int> deq = {1, 2, 3, 0}, swap_deq = { 0, 3, 2, 1 };
    deq.swap(swap_deq);

    auto print = [&]() {
        for (auto &&i : deq)
            cout << i << " ";
        cout << "\t | size: " << deq.size() << "\n";
    };

    print();
    cout << "front: " << deq.front() << " back:" << deq.back()
         << " size: " << deq.size() << "\n\n";

    deq.push_front(97);
    deq.push_back(9);
    deq.push_back(8);
    cout << "push after: ";
    print();

    cout << "\n\npop after: ";
    deq.pop_front();
    deq.pop_back();
    print();
    cout << "\n\n";

    cout << "insert: ";
    deq.insert(++deq.begin(), 21);
    cout << *(++deq.begin()) << endl;
    print();
    cout << "\n\n";

    cout << "erase: ";
    deq.erase(++deq.begin());
    print();
    cout << "\n\n";

    deq.clear();
    assert(deq.empty());
}

void deqAssign() {
    iop::deque<char> characters;

    auto print_list = [&]() {
        for (char c : characters)
            std::cout << c << ' ';
        std::cout << '\n';
    };

    characters.assign(5, 'a');
    print_list();

    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_list();

    characters.assign({'C', '+', '+', '1', '1'});
    print_list();
}

void deqResize() {
    iop::deque<int> c = {1, 2, 3};
    std::cout << "deque 持有：";
    for (const auto &el : c)
        std::cout << el << ' ';
    std::cout << '\n';

    c.resize(5);
    std::cout << "在增加大小到 5 之后：";
    for (const auto &el : c)
        std::cout << el << ' ';
    std::cout << '\n';

    c.resize(2);
    std::cout << "在减少大小到 2 之后：";
    for (const auto &el : c)
        std::cout << el << ' ';
    std::cout << '\n';

    c.resize(6, 4);
    std::cout << "在增加大小到 6 之后（初始化器 = 4）：";
    for (const auto &el : c)
        std::cout << el << ' ';
    std::cout << '\n';
}

template <class Os, class Co> Os &operator<<(Os &os, const Co &co) {
    os << "{";
    for (auto const &i : co)
        os << ' ' << i;
    return os << " } ";
}

void deqCopy() {
    iop::deque<int> a1 = {1, 2, 3};
    iop::deque<int> a2 = {4, 5};

    auto it1 = ++(a1.begin());
    auto it2 = ++(a2.begin());

    int &ref1 = a1.front();
    int &ref2 = a2.front();

    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2
              << '\n';

    // TODO self-swap
    a1.swap(a2);
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2
              << '\n';

    // 注意交换后迭代器与引用保持与原来的元素关联，
    // 例如尽管 'a1' 中值为 2 的元素被移动到 'a2' 中，
    // 原来指向它的 it1 仍指向同一元素。
};


template<class Tp>
struct NAlloc
{
    typedef Tp value_type;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    using pointer = value_type*;
    using const_pointer = const value_type*;
    NAlloc() = default;
 
    template<class T> NAlloc(const NAlloc<T>&) {}
 
    Tp* allocate(std::size_t n)
    {
        n *= sizeof(Tp);
        std::cout << "分配 " << n << " 个字节\n";
        return static_cast<Tp*>(::operator new(n));
    }
 
    void deallocate(Tp* p, std::size_t n)
    {
        std::cout << "解分配 " << n*sizeof*p << " 个字节\n";
        ::operator delete(p);
    }
};
template<class T, class U>
bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
template<class T, class U>
bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }

// void deq_shrink_to_fit(){
//     std::cout << "默认构造的 deque:\n";
//     iop::deque<int, NAlloc<int>> deq;
 
//     std::cout << "\n添加 300 个元素:\n";
//     for (int i = 1000; i < 1300; ++i)
//         deq.push_back(i);
 
//     std::cout << "\n弹出 100 个元素:\n";
//     for (int i = 0; i < 100; ++i)
//         deq.pop_front();
 
//     std::cout << "\n运行 shrink_to_fit:\n";
//     deq.shrink_to_fit();
 
//     std::cout << "\ndeque 随离开作用域而销毁:\n";
// }


#include "../../type_traits.h"
#include "../test_.h"
int main() {


    testFunc(deqBaseFunc);

    cout << "\n\nSwap\n";
    testFunc(deqCopy);

    // cout << "\n\nAssign\n";
    // testFunc(deqAssign);

    // deq_shrink_to_fit();

    iop::deque<int> deq = {2,3,4,5};
    for(auto it : deq)
        cout << it << " ";
    cout << endl;

    for(int i = 0; i < 10; ++i)
    {
        deq.push_back(i);
    }

    for(auto it : deq)
        cout << it << " ";
    cout << endl;

    return 0;
};
