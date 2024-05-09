#include "../../algorithm.h"
#include "../../iterator_base_F.h"
#include "../../container/list_F.h"

#include <iostream>
#include <list>
using namespace std;

template <class Os, class Co> Os &operator<<(Os &os, const Co &co) {
    os << "{";
    for (auto const &i : co)
        os << ' ' << i;
    return os << " } ";
}

void listCopy() {
    iop::list<int> a1{1, 2, 3}, a2{4, 5};

    auto it1 = ++a1.begin();
    auto it2 = ++a2.begin();

    int &ref1 = a1.front();
    int &ref2 = a2.front();

    cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2
         << '\n';
    a1.swap(a2);
    cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2
         << '\n';

    // 注意交换后迭代器与引用保持与原来的元素关联，
    // 例如尽管 'a1' 中值为 2 的元素被移动到 'a2' 中，
    // 原来指向它的 it1 仍指向同一元素。
}

void listRsize() {
    iop::list<int> c = {1, 2, 3};
    cout << "list 持有：";
    cout << c << "\n";
    cout << '\n';

    c.resize(5);
    cout << "在增加大小到 5 之后：";
    cout << c << "\n";

    c.resize(2);
    cout << "在减少大小到 2 之后：";
    cout << c << "\n";

    c.resize(6, 4);
    cout << "在增加大小到 6 之后（初始化器 = 4）：";
    cout << c << "\n";
}

void listAssign() {
    iop::list<char> characters;

    auto print_list = [&]() {
        for (char c : characters)
            cout << c << ' ';
        cout << '\n';
    };

    characters.assign(5, 'a');
    print_list();

    const std::string extra(6, 'b');

    characters.assign(extra.begin(), extra.end());
    print_list();

    characters.assign({'C', '+', '+', '1', '1'});
    print_list();
}

void list_erase() {
    iop::list<int> ls = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    cout << ls << "\n";

    ls.erase(ls.begin());
    cout << ls << "\n";

    auto end = ls.size();
    auto range_begin = ls.begin();
    auto range_end = ls.begin();

    iop::advance(range_begin, end - 3);
    iop::advance(range_end, end - 1);

    cout << ls << "\n";
    for (auto it = ls.begin(); it != ls.end();) {
        if (*it % 2 == 0)
            ls.erase(it++);
        else
            ++it;
    }
    cout << ls << "\n";
}

void list_insert() {

    auto print = [](int id, iop::list<int> &container) {
        cout << id << ". ";
        for (auto x : container)
            cout << x << ' ';
        cout << "\n";
    };

    iop::list<int> c1(100, 3);
    print(1, c1);

    auto it = c1.begin();
    it = c1.insert(it, 200);
    print(2, c1);

    c1.insert(it, 2, 300);
    print(3, c1);

    // 将 it 重新指向开头
    it = c1.begin();

    iop::list<int> c2(400, 2);
    c1.insert(iop::next(it, 2), c2.begin(), c2.end());
    print(4, c1);

    int arr[] = {501, 502, 503};
    int arr_size = sizeof(arr) / sizeof(int);
    c1.insert(c1.begin(), arr, arr + arr_size);
    print(5, c1);

    c1.insert(c1.end(), {601, 602, 603});
    print(6, c1);
    
}



int main() {
    iop::list<int> ls = {1, 2, 3};
    std::list<int> ls_std = {1, 2, 3};

    cout << "ls " << sizeof(ls) << ", ls_std " << sizeof(ls_std) << "\n";

    cout << "list element: " << ls << "\n";
    cout << "list base func: \n"
         << "front: " << ls.front() << "\t back : " << ls.back()
         << "\t size : " << ls.size() << "\t empty : " << ls.empty() << "\n";
    
    {
        auto it = iop_algo::find(ls.begin(), ls.end(), 2);
        cout << "list find(2) : " << *it << "\n\n";
    }

    {
        cout << "list_insert: \n";
        list_insert();
        cout << "\n\n";
    }
    {
        cout << "list_erase: \n";
        list_erase();
        cout << "\n\n";
    }

    {
        cout << "listResize: \n";
        listRsize();
        cout << "\n\n";
    }

    {
        cout << "listCopy: \n";
        listCopy();
        cout << "\n\n";
    }

    {
        cout << "listAssign: \n";
        listAssign();
        cout << "\n\n";
    }
}