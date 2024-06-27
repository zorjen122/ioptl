#include <bits/stdc++.h>
using namespace std;


template<class Os, class Co>
Os& operator<<(Os& os, const Co obj)
{
    os << "{ ";
    for(auto it : obj)
        os << it << " ";
    os << "}";

    return os;
}

constexpr void foo()
{
    vector vec = {1,2,3,4};

    vec.push_back(10);
    vec.push_back(10);
    vec.push_back(10);
}


void foo_deq()
{
    deque deq = {1,2,3,4,5,6};

    deq.erase(find(deq.begin(), deq.end(), 2));

    cout << deq << "\n";
}

void foo_ls()
{
    cout << "foo_is():\n";

    list ls = {3,2,1,4,5,6,1};
    list ls2 = {9,2,1,4,5,6,1};

    auto start = ls2.begin();
    ls.merge(ls2);

    for_each(ls.begin(), ls.end(), 
    [=](auto it) { 
        if(it == *start) 
            cout << "\n" << it << " ";
        else
            cout << it << " ";
        }
    ); cout << "\n";


    ls.remove_if([](auto it) { return it % 2 != 0; });
    cout << ls << "\n";


}

int main()
{
    // foo();   

    foo_deq();

    foo_ls();

    return 0;
}