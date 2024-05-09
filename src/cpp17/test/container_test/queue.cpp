#include "../../container/queue_F.h"
#include<deque>

#include<iostream>
using namespace std;


template <class Os, class Co> Os &operator<<(Os &os, const Co &co) {
    os << "{";
    for (auto const &i : co)
        os << ' ' << i;
    return os << " } ";
}

int main() {
    iop::deque<int> d = { 1,2,3,4 };


    // ERR
    for (int i = 5; i < 10; ++i)
    {
        d.push_back(i);

    }



    cout << d << "\n";

    
    return 0;
}