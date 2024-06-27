#include "../../container/vectorF.h"
#include "../../traits/type_traitsF.h"
#include "../../util/iterator_baseF.h"

#include <iostream>
using namespace std;

struct S
{
    int x;
    float y;
    double z;

    S(int x, float y, double z) : x{x}, y{y}, z{z}
    {
        ::std::cout << "S::S();\n";
    }

    ~S() { ::std::cout << "S::~S();\n"; }

    void print() const
    {
        ::std::cout << "S { x=" << x << "; y=" << y << "; z=" << z << "; };\n";
    }
};

void ConstructFunc()
{
    alignas(S) unsigned char storage[sizeof(S)];

    S *ptr =
        iop::construct_at(reinterpret_cast<S *>(storage), 42, 2.71828f, 3.1415);
    ptr->print();

    iop::destroy_at(ptr);
}

void UninitcopyFunc()
{
    const char *v[] = {"This", "is", "an", "example"};

    auto sz = ::std::size(v);

    if (void *pbuf = ::std::aligned_alloc(alignof(::std::string),
                                          sizeof(::std::string) * sz)) {
        try {
            auto first = static_cast<::std::string *>(pbuf);
            auto last =
                iop::uninitialized_copy(::std::begin(v), ::std::end(v), first);

            for (auto it = first; it != last; ++it)
                ::std::cout << *it << '_';
            ::std::cout << '\n';

            iop::destroy_at(first, last);
        }
        catch (...) {
        }
        ::std::free(pbuf);
    }
}

void uninitcopyN()
{
    iop::vector<int> source = {1, 2, 3, 4, 5};
    iop::vector<int> destination(5);
    assert(destination.size() == 5);

    iop::uninitialized_copy_n(source.begin(), 3, destination.begin());

    std::cout << "Copied elements: ";
    for (const auto &element : destination) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main(int arcc, char *argv[])
{

    using namespace iop;

    using alloc_type = iop::allocator<int>;
    alloc_type __alloc;

    iop::vec_iter::trivial_iterator<int> ptr = __alloc.allocate(10);

    ptr = __alloc.allocate(10);

    for (int i = 0; i < 10; ++i) {
        *(ptr + i) = i;
    }

    cout << "\n\nConstruct:";

    ConstructFunc();

    cout << "\nuninitcopy:";
    UninitcopyFunc();

    cout << "pairUninitcopy:\n";
    uninitcopyN();

    return 0;
}