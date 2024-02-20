#include "../../functionals.h"
#include "../../container/vector_F.h"
#include "../../algorithm.h"
#include "../../func_exception.h"
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
    virtual void display() { ::std::cout << "B"; }
};

struct C : public A {
    virtual void display() { ::std::cout << "C"; }
};

struct D : public A {
    virtual void display() { ::std::cout << "D"; }
};


int main(int argc, char* argv[]) {
    iop::function<int(int, int)>func = add;
    iop::function<int(int)> f3 = ::std::bind(add, 7, ::std::placeholders::_1);

    iop::function<int()>funcs = nullptr;

    size_t val = 10;
    auto lam = [&val](int __c) {
        val += __c;
        return val;
        };

    iop::function<int(int)> r = lam;

    ::std::cout << lam(20) << endl;
    ::std::cout << f3(10) << endl;
    ::std::cout << func(10, 20) << endl;

    try {
        funcs();
    }
    catch (const iop::bad_function_call& e) {
        ::std::cout << e.what() << endl;
    }

    ::std::cout << iop::plus<int, int>()(10, 20) << endl;

    iop::bind_unary<iop::logical_not<bool>>v;
    iop::bind_unary vs = iop::logical_not<bool>();
    iop::bind_binary<iop::plus<int, int>> bv = iop::plus<int, int>();

    iop::double_bind<iop::logical_not<bool>, iop::logical_not<bool>,
        iop::logical_not<bool>> multv;

    iop::logical_not<bool> l;

    iop::double_bind multvs(l, l, l);

    iop::pointer_unary_function<int, int>pfunc = ioadd;
    iop::pointer_binary_function<int, int, int>pfuncs = add;

    iop::vector<A*> vec;
    vec.push_back(new B);
    vec.push_back(new C);
    vec.push_back(new D);

    iop::mem_fun_t(&mpls::enable_if_t<is_base_of<A, B>::value, A>::display);

    ::std::cout << pfunc(20) << "\t" << pfuncs(20, 20) << endl;

    ::std::cout << v(true) << " " << vs(false) << endl;
    ::std::cout << bv(10, 20) << endl;
    ::std::cout << multv(false) << endl;
    ::std::cout << multvs(true) << endl;

    return 0;
}
