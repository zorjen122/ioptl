#include <algorithm>
#include <array>
#include <initializer_list>
#include <iostream>

#include "../../container/arrayF.h"

using namespace std;

namespace D {
    template <class Os, class V> Os &operator<<(Os &os, const V &v)
    {
        os << "{";
        for (auto i : v)
            os << ' ' << i;
        return os << " } ";
    }

    void arrayCopy()
    {

        iop::array<int, 3> a1{1, 2, 3}, a2{4, 5, 6};

        auto it1 = a1.begin();
        auto it2 = a2.begin();
        int &ref1 = a1[1];
        int &ref2 = a2[1];

        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' '
                  << ref2 << '\n';
        a1.swap(a2);
        std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' '
                  << ref2 << '\n';

        // 注意交换后迭代器与引用保持与原 array 关联，
        // 例如 `it1` 仍指向元素 a1[0] ， `ref1` 仍指代 a1[1] 。
    }
}; // namespace D

void arrayFill()
{

    constexpr int xy = 4;

    using Cell = iop::array<unsigned char, 8>;

    iop::array<Cell, xy * xy> board;

    board.fill({{0xE2, 0x96, 0x84, 0xE2, 0x96, 0x80, 0, 0}}); // "▄▀";

    int O = 1;
    for (auto &c : board) {
        std::cout << c.data() << ((O++ % xy) ? "" : "\n");
    }
}

void arrayGet()
{
    iop::array<int, 3> lvarr;
    iop::array<int, 3> rvarr = {1, 2, 3};

    iop::get<0>(lvarr) = 1;
    iop::get<1>(lvarr) = 2;
    iop::get<2>(lvarr) = 3;

    // lvalue
    std::cout << "(" << iop::get<0>(lvarr) << ", " << iop::get<1>(lvarr) << ", "
              << iop::get<2>(lvarr) << ")\n";

    // rvalue
    std::cout << "(" << iop::get<0>(Fiop::move(rvarr)) << ", "
              << iop::get<1>(Fiop::move(rvarr)) << ", "
              << iop::get<2>(Fiop::move(rvarr)) << ")\n";
}

struct alignSz
{
    size_t a;
    int *p;
};

int main()
{

    iop::array<int, 4> arr = {1, 2, 3, 4};
    std::array<int, 4> arrstd = {4, 3, 2, 1};

    assert(sizeof(arr) + sizeof(alignSz) == 2 * sizeof(arrstd));

    for (auto &&it : arr)
        cout << it << ",";
    cout << "\n";

    const ::std::initializer_list<int> res = {9, 2, 2, 3};
    iop::array<int, 4> ares = res;
    for (auto &&it : ares)
        cout << it << ",";
    cout << "\n\n";

    arrayFill();

    cout << "\n\n";

    D::arrayCopy();

    cout << "\n\n";

    arrayGet();

    return 0;
}
