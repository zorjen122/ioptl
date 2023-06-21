#include "../vector_F.h"
#include "../alloc_operator_new_F.h"

#include <iostream>
using namespace std;


void test() {
    IOP_STL::vector<int> vec;

    vec.push_back(10);
    vec.push_back(10);
    vec.push_back(10);

    vec.push_front(100);

    ::std::cout << "A" << endl;

    for (size_t i = 0;i != vec.size();++i) {
        std::cout << vec[i] << ",";
    }

    cout << endl;

    cout << vec.size() << endl;
    cout << vec.max_size() << endl;
    cout << vec.empty() << endl;
    cout << vec.at(1) << endl;
    cout << vec.front() << endl;
    cout << vec.back() << endl;

}

int main(int argc, char* argv[]) {
    test();

    return 0;
}