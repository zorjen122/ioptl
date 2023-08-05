#include "../iterator_base_F.h"
#include "../type_traits.h"
#include "../container/vector_F.h"

#include <iostream>
using namespace std;

template<class _Ty>
concept is_same_tag_v = mpls::is_same<
    typename iop::iterator_traits<_Ty>::iterator_category,
    iop::input_iterator_tag
>::values;

struct S {
    int x;
    float y;
    double z;

    S(int x, float y, double z) : x{ x }, y{ y }, z{ z } { ::std::cout << "S::S();\n"; }

    ~S() { ::std::cout << "S::~S();\n"; }

    void print() const {
        ::std::cout << "S { x=" << x << "; y=" << y << "; z=" << z << "; };\n";
    }
};


void ConstructFunc() {
    alignas(S) unsigned char storage[sizeof(S)];

    S* ptr = iop::construct_at(reinterpret_cast<S*>(storage), 42, 2.71828f, 3.1415);
    ptr->print();

    iop::destroy_at(ptr);
}

void UninitcopyFunc() {
    const char* v[] = { "This", "is", "an", "example" };

    auto sz = ::std::size(v);

    if (void* pbuf = ::std::aligned_alloc(alignof(::std::string), sizeof(::std::string) * sz))
    {
        try
        {
            auto first = static_cast<::std::string*>(pbuf);
            auto last = iop::uninitialized_copy(::std::begin(v), ::std::end(v), first);

            for (auto it = first; it != last; ++it)
                ::std::cout << *it << '_';
            ::std::cout << '\n';

            iop::destroy(first, last);
        }
        catch (...) {}
        ::std::free(pbuf);
    }
}

void uninitcopyN() {
    iop::vector<int> source = { 1, 2, 3, 4, 5 };
    iop::vector<int> destination(5);
    assert(destination.size() == 5);

    iop::uninitialized_copy_n(source.begin(), 3, destination.begin());

    std::cout << "Copied elements: ";
    for (const auto& element : destination) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

}


int main(int arcc, char* argv[]) {

    // using type = iop::iterator_traits<int*>::iterator_category;
    static_assert(!is_same_tag_v<int*>);

    int* p = new int[10];
    for (int i = 0;i != 10;++i) {
        p[i] = i + (i - 1);
    }

    int* z = p;
    for (int i = 0;i != 10;++i) {
        cout << *z << ",";
        ++z;
    }
    cout << "\n";

    int k = 0;
    cout << k << "\t";

    int* q = p + 5;
    iop::distance(p, q, k);
    cout << k << "\n";

    cout << *q << "\t";
    iop::advance(q, 2);
    cout << *q << "\n";

    int* r = new int[9];
    int* rit = iop::copy_backward(p, q, r + (size_t)(q + 1 - p));

    for (int i = 0;i != 9;++i) {
        cout << *rit << ",";
        ++rit;
    }

    cout << "\n";
    int* rits = r;
    for (int i = 0;i != 9;++i) {
        cout << *rits << ",";
        ++rits;
    }
    cout << "\n\nConstruct:";

    ConstructFunc();

    cout << "\nuninitcopy:";
    UninitcopyFunc();

    cout << "pairUninitcopy:\n";
    uninitcopyN();

    return 0;
}