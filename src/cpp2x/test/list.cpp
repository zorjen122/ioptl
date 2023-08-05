#include "../container/list_F.h"

#include <iostream>
#include <list>
using namespace std;

template<class Os, class Co>
Os& operator<<(Os& os, const Co& co)
{
    os << "{";
    for (auto const& i : co)
        os << ' ' << i;
    return os << " } ";
}

void listCopy()
{
    iop::list<int> a1{ 1, 2, 3 }, a2{ 4, 5 };

    auto it1 = ++a1.begin();
    auto it2 = ++a2.begin();

    int& ref1 = a1.front();
    int& ref2 = a2.front();

    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';
    a1.swap(a2);
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2 << '\n';

    // 注意交换后迭代器与引用保持与原来的元素关联，
    // 例如尽管 'a1' 中值为 2 的元素被移动到 'a2' 中，
    // 原来指向它的 it1 仍指向同一元素。
}

void listRsize() {
    iop::list<int> c = { 1, 2, 3 };
    std::cout << "list 持有：";
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

void listAssign() {
    iop::list<char> characters;

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


int main() {
    iop::list<int>ls = { 1,2,3 };
    for (auto&& i : ls)
        cout << i << ",";

    cout << "\n" << ls.size() << "\n";

    ls.push_back(10);
    ls.push_front(100);
    cout << "push:\n";
    cout << ls.front() << " " << ls.back() << endl;

    ls.pop_back();
    ls.pop_front();
    cout << "pop:\n";
    cout << ls.front() << " " << ls.back() << endl;

    for (auto&& i : ls)
        cout << i << ",";
    cout << "\n";

    ls.erase(++ls.begin());

    ls.insert(++(ls.begin()), 99);
    for (auto&& i : ls)
        cout << i << ",";

    cout << "\n" << ls.size() << endl;

    cout << "\n\n";

    listRsize();
    cout << "\n\n";

    listCopy();
    cout << "\n\n";

    listAssign();
    cout << "\n\n";

}