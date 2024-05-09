#include "../../container/vector_F.h"
#include <vector>

#include <iostream>
using namespace std;

namespace D {
    template <class Os, class Co> Os &operator<<(Os &os, const Co &co)
    {
        os << "{";
        for (auto const &i : co)
            os << ' ' << i;
        return os << " } ";
    }

    void vecSwap()
    {
        iop::vector<int> a1{1, 2, 3}, a2{4, 5};

        auto it1 = std::next(a1.begin());
        auto it2 = std::next(a2.begin());

        int &ref1 = a1.front();
        int &ref2 = a2.front();

        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' '
                  << ref2 << '\n';
        a1.swap(a2);
        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' '
                  << ref2 << '\n';

        // 注意交换后迭代器与引用保持与原来的元素关联，
        // 例如尽管 'a1' 中值为 2 的元素被移动到 'a2' 中，
        // 原来指向它的 it1 仍指向同一元素。
    }
}; // namespace D

namespace Q {
    struct Student
    {
        string name;
        int age;
        Student() : name(), age() {}
        Student(string __name, int __age) : name(__name), age(__age)
        {};

        void print()
        {
            cout << "name: " << name << "age: " << age <<endl;
        }
    };

    void vecEmplace()
    {
        Student s1("xm", 18);
        iop::vector<Student> vec = {s1};

        vec.emplace_back("xh", 19);
        vec.emplace_back("xq", 19);

        for(auto it : vec)
            it.print();
    }
}; // namespace Q


void vecCap()
{
    int sz = 200;
    iop::vector<int> v1;

    auto cap = v1.capacity();
    std::cout << "initial capacity=" << cap << '\n';

    for (int n = 0; n < sz; ++n) {
        v1.push_back(n);
        if (cap != v1.capacity()) {
            cap = v1.capacity();
            std::cout << "new capacity=" << cap << '\n';
        }
    }

    std::cout << "final size=" << v1.size() << '\n';
    std::cout << "final capacity=" << v1.capacity() << '\n';
}

void print_container(const std::vector<int> &c)
{
    for (int i : c)
        std::cout << i << " ";
    std::cout << '\n';
}

void vecErase()
{
    std::vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_container(c);

    c.erase(c.begin());
    print_container(c);

    c.erase(c.begin() + 2, c.begin() + 5);
    print_container(c);

    // 移除所有偶数
    for (std::vector<int>::iterator it = c.begin(); it != c.end();) {
        if (*it % 2 == 0)
            it = c.erase(it);
        else
            ++it;
    }
    print_container(c);
}
template <class Tp> struct NAlloc
{
    typedef Tp value_type;

    NAlloc() = default;
    template <class T> NAlloc(const NAlloc<T> &) {}

    Tp *allocate(std::size_t n)
    {
        n *= sizeof(Tp);
        Tp *p = static_cast<Tp *>(::operator new(n));
        std::cout << "在 " << p << " 分配 " << n << " 个字节\n";
        return p;
    }

    void deallocate(Tp *p, std::size_t n)
    {
        std::cout << "在 " << p << "解分配 " << n * sizeof *p << " 个字节\n";
        ::operator delete(p);
    }
};

namespace R {
    template <class T, class U>
    bool operator==(const NAlloc<T> &, const NAlloc<U> &)
    {
        return true;
    }

    template <class T, class U>
    bool operator!=(const NAlloc<T> &, const NAlloc<U> &)
    {
        return false;
    }

    void vecReverse()
    {
        constexpr int max_elements = 32;

        std::cout << "使用 reserve：\n";
        {
            std::vector<int, NAlloc<int>> v1;
            v1.reserve(
                max_elements); // 预留至少 max_elements * sizeof(int) 个字节

            for (int n = 0; n < max_elements; ++n)
                v1.push_back(n);
        }

        std::cout << "不使用 reserve：\n";
        {
            std::vector<int, NAlloc<int>> v1;

            for (int n = 0; n < max_elements; ++n) {
                if (v1.size() == v1.capacity())
                    std::cout << "size() == capacity() == " << v1.size()
                              << '\n';
                v1.push_back(n);
            }
        }
    }
}; // namespace R

void vecShrink_to_fit()
{
    std::vector<int> v;
    std::cout << "Default-constructed capacity is " << v.capacity() << '\n';
    v.resize(100);
    std::cout << "Capacity of a 100-element vector is " << v.capacity() << '\n';
    v.resize(50);
    std::cout << "Capacity after resize(50) is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
    v.clear();
    std::cout << "Capacity after clear() is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
    for (int i = 1000; i < 1300; ++i)
        v.push_back(i);
    std::cout << "Capacity after adding 300 elements is " << v.capacity()
              << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
}

void vecRvalue()
{
    iop::vector<int> source = {1, 2, 3, 4, 5};
    iop::vector<int> destination(std::move(source));

    std::cout << "Destination vector: ";
    for (const auto &element : destination) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    std::cout << "Source vector after move: ";
    for (const auto &element : source) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main()
{
    iop::vector<int> vec = {1, 2, 3};
    iop::vector<int> oer = {2, 3, 1, 1, 2};
    iop::vector<int> vecp = vec;
    vecp = oer;

    iop::vector<int> per = {1,2,3,4,5};

    per.insert(decltype(per)::iterator(per.begin()), 5);
    for(auto it : per)
        cout << it << " ";
    cout << "\n";

    iop::vector<int> dcpvec(vecp);
    for (auto &&it : dcpvec)
        cout << it << ",";
    cout << "\n\n";

    vecp.insert(vecp.begin(), 10);
    vecp.insert(vecp.end(), 10);

    for (int i = 0; i != 10; ++i)
        vecp.push_back(i);

    vecp.erase(vecp.begin());
    vecp.erase(vecp.begin());

    for (auto &&it : vecp)
        cout << it << ",";

    cout << "iterator: " << sizeof(vec.begin()) << "V_alloc: " << sizeof(vecp)
         << "\n";

    cout << "\n\n";
    vecCap();

    cout << "\n\nswap\n";
    D::vecSwap();

    cout << "\n\n";
    Q::vecEmplace();
    cout << "\n\n";

    vecErase();
    cout << "\n\n";

    R::vecReverse();
    cout << "\n\n";
    vecShrink_to_fit();

    vecp.clear();
    assert(vecp.empty());

    cout << "\n\n";
    vecRvalue();

    cout << "\n\n";


}
