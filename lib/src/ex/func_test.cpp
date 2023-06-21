#include "../functional_F.h"
#include "../vector_F.h"
#include "../algorithm.h"
#include "../func_exception.h"

#include <iostream>
#include <functional>

using namespace std;

int add(int a, int b) {
    return a + b;
}


int ioadd(int a) {
    return ++a;
}

struct A {
    virtual void display() = 0;
};

struct B : public A {
    virtual void display() { std::cout << "B"; }
};

struct C : public A {
    virtual void display() { std::cout << "C"; }
};

struct D : public A {
    virtual void display() { std::cout << "D"; }
};


int main(int argc, char* argv[]) {
    IOP_STL::function<int(int, int)>func = add;
    IOP_STL::function<int(int)> f3 = std::bind(add, 7, std::placeholders::_1);

    IOP_STL::function<int()>funcs = nullptr;

    std::size_t val = 10;
    auto lam = [&val](int __c) {
        val += __c;
        return val;
        };

    IOP_STL::function<int(int)> r = lam;

    ::std::cout << lam(20) << endl;
    ::std::cout << f3(10) << endl;
    ::std::cout << func(10, 20) << endl;

    try {
        funcs();
    }
    catch (const IOP_STL::bad_function_call& e) {
        ::std::cout << e.what() << endl;
    }

    ::std::cout << IOP_STL::plus<int, int>()(10, 20) << endl;

    IOP_STL::bind_unary<IOP_STL::logical_not<bool>>v;
    IOP_STL::bind_unary vs = IOP_STL::logical_not<bool>();
    IOP_STL::bind_binary<IOP_STL::plus<int, int>> bv = IOP_STL::plus<int, int>();

    IOP_STL::double_bind<IOP_STL::logical_not<bool>, IOP_STL::logical_not<bool>,
        IOP_STL::logical_not<bool>> multv;

    IOP_STL::logical_not<bool> l;

    IOP_STL::double_bind multvs(l, l, l);

    IOP_STL::pointer_unary_function<int, int>pfunc = ioadd;
    IOP_STL::pointer_binary_function<int, int, int>pfuncs = add;

    IOP_STL::vector<A*> vec;
    vec.push_back(new B);
    vec.push_back(new C);
    vec.push_back(new D);

    IOP_STL::mem_fun_t(&MPL_L::enable_if_t<is_base_of<A, B>::value, A>::display);

    ::std::cout << pfunc(20) << "\t" << pfuncs(20, 20) << endl;

    ::std::cout << v(true) << " " << vs(false) << endl;
    ::std::cout << bv(10, 20) << endl;
    ::std::cout << multv(false) << endl;
    ::std::cout << multvs(true) << endl;

    return 0;
}