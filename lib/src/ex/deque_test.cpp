#include<iostream>

#include"../initailzer_list_F.h"
#include"../deque_F.h"

using namespace std;

template<class _Ty>
void display_deque(IOP_STL::deque<_Ty> q) {
    for (auto i = q.begin();i != q.end();++i)
        std::cout << *i << ",";
}

void test() {
    IOP_STL::deque<char> characters;

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

void resize_test() {
    IOP_STL::deque<int> c;
    c.push_back(1);
    c.push_back(2);
    c.push_back(3);

    std::cout << "deque 长度: " << c.size() << endl;
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


int main(int argc, char* argv[]) {

    IOP_STL::deque<int>res;
    cout << res.size() << endl;

    res.push_back(10);
    res.push_back(2);

    cout << res.V_map_size << endl;
    cout << res.size() << endl;

    res.push_front(1);

    IOP_STL::deque<int>rhs(res);

    auto lam = []<class _Ty>(_Ty & __r) {
        for (auto&& i : __r)
            cout << i << ",";
        cout << "\n\n";
    };

    lam(res);
    res.resize(2, 99);
    lam(res);

    resize_test();

    test();


    res.clear();
    cout << "\n\n" << res.empty();

    return 0;
}