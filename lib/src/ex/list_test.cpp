#include<bits/stdc++.h>
#include "../list_F.h"

using namespace std;

template<class _Ty>
void display(IOP_STL::list<_Ty>& __res) {
    for (auto&& it : __res)
        std::cout << it << ",";
    std::cout << endl;
}

int main() {

    IOP_STL::list<int>rhs;
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

    rhs.__swap(++rhs.begin(), (--rhs.end()));

    std::cout << "Swap after--> " << *(++rhs.begin()) << "\t" << *(--rhs.end()) << endl;



    rhs.clear();

    rhs.insert(rhs.begin());
    rhs.insert(++rhs.begin());
    rhs.insert(++rhs.begin(), 20);
    rhs.remove(20);

    for (IOP_STL::list<int>::iterator i = rhs.begin();i != rhs.end();++i) {
        std::cout << *i << ",";
    }
    std::cout << "\n\n";


    IOP_STL::list<int>opt(999, 4);
    IOP_STL::list<int>P(342, 8);
    IOP_STL::list<int>P_null;

    std::cout << "list:" << endl;
    for (IOP_STL::list<int>::iterator i = P.begin();i != P.end();++i) {
        std::cout << *i << ",";
    }

    opt.push_back(10);
    opt.push_back(11);
    IOP_STL::list<int>::iterator z = IOP_STL::find(opt.begin(), opt.end(), 10);

    std::cout << "find:" << endl;
    for (IOP_STL::list<int>::iterator i = z;i != opt.end();++i) {
        std::cout << *i << ",";
    }

    rhs.splice(z, P);
    std::cout << "split:" << endl;

    for (IOP_STL::list<int>::iterator i = opt.begin();i != opt.end();++i) {
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

    cout << "\nnull merge: " << endl;
    P_null.merge(P);
    display(P_null);


    cout << "\n\nreverse P: ";
    P.reverse();
    display(P);

    std::cout << "\n\n";
    opt = P;
    cout << "operator opt=";
    display(opt);

    return 0;
}