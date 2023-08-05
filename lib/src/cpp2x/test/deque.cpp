#include "../container/deque_F.h"
#include "../functionals.h"

#include <iostream>
#include <chrono>

using namespace std;

void deqBaseFunc() {
    iop::deque<int>deq = { 1,2,3,0 };
    auto print = [&]() {
        for (auto&& i : deq)
            cout << i << " ";
        cout << endl;
        };
    print();
    cout << "front: " << deq.front() << " back:" << deq.back() << "\n\n";


    cout << deq.size() << endl;

    deq.push_front(97);
    deq.push_back(9);
    cout << "push: " << deq.front() << " " << deq.back() << endl;
    print();

    deq.pop_front();
    deq.pop_back();
    cout << "pop: " << deq.front() << " " << deq.back() << endl;
    print();

    cout << deq.size() << endl;

    deq.insert(++deq.begin(), 21);
    cout << *(++deq.begin()) << endl;
    deq.erase(++deq.begin());
    cout << *(++deq.begin()) << endl;
    cout << "\n\n";

    print();

    // // Error
    // deq.clear();
    // assert(deq.empty());
}

void deqAssign() {
    iop::deque<char> characters;

    auto print_list = [&]()
        {
            for (char c : characters)
                std::cout << c << ' ';
            std::cout << '\n';
        };

    characters.assign(5, 'a');
    print_list();

    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_list();

    characters.assign({ 'C', '+', '+', '1', '1' });
    print_list();

}


void deqResize() {
    iop::deque<int> c = { 1, 2, 3 };
    std::cout << "deque 持有：";
    for (const auto& el : c)
        std::cout << el << ' ';
    std::cout << '\n';

    c.resize(5);
    std::cout << "在增加大小到 5 之后：";
    for (const auto& el : c)
        std::cout << el << ' ';
    std::cout << '\n';

    c.resize(2);
    std::cout << "在减少大小到 2 之后：";
    for (const auto& el : c)
        std::cout << el << ' ';
    std::cout << '\n';

    c.resize(6, 4);
    std::cout << "在增加大小到 6 之后（初始化器 = 4）：";
    for (const auto& el : c)
        std::cout << el << ' ';
    std::cout << '\n';
}

namespace D {
    template<class Os, class Co>
    Os& operator<<(Os& os, const Co& co)
    {
        os << "{";
        for (auto const& i : co)
            os << ' ' << i;
        return os << " } ";
    }

    void deqCopy()
    {
        iop::deque<int> a1{ 1, 2, 3 }, a2{ 4, 5 };

        auto it1 = ++(a1.begin());
        auto it2 = ++(a2.begin());

        int& ref1 = a1.front();
        int& ref2 = a2.front();

        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';

    // TODO self-swap
        a1.swap(a2);
        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';

        // 注意交换后迭代器与引用保持与原来的元素关联，
        // 例如尽管 'a1' 中值为 2 的元素被移动到 'a2' 中，
        // 原来指向它的 it1 仍指向同一元素。
    };
};


void testFunc(iop::function<void()> __f) {
    auto start = std::chrono::high_resolution_clock::now();  // 记录开始时间

    __f();

    auto end = std::chrono::high_resolution_clock::now();  // 记录结束时间
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    cout << duration << " mil\n";
}


int main() {

    testFunc(deqBaseFunc);

    cout << "\n\nAssign\n";
    testFunc(deqAssign);

    cout << "\ndeqCopy\n";
    testFunc(D::deqCopy);

    cout << "\ndeqResize\n";
    testFunc(deqResize);

    {
        iop::deque<int>deq = { 1,2,3 };
        deq.clear();
        assert(deq.empty());
        deq.push_back(10);
    }

    return 0;
};

