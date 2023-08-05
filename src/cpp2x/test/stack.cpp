#include "../container/stack_F.h"
#include "../container/vector_F.h"

#include <iostream>
using namespace std;

void stkTop() {

    iop::stack<int> s;
    s.push(2);
    s.push(6);
    s.push(51);

    std::cout << s.size() << " elements on stack\n";
    std::cout << "Top element: " << s.top() << "\n";
    std::cout << s.size() << " elements on stack\n";
    s.pop();
    std::cout << s.size() << " elements on stack\n";
    std::cout << "Top element: " << s.top() << "\n";

}


int main()
{
    iop::stack<int>stk = { 1,2,3 };
    iop::stack<int>cpstk = stk;
    iop::stack<int>dcpstk(cpstk);

    ::std::initializer_list<int> ilist = { 1,2,3 };
    iop::stack<int> rstk(std::move(ilist));
    cout << "ilist move after\n";
    while (!rstk.empty()) {
        cout << rstk.top();
        rstk.pop();
    }


    cout << "\n\n";
    stkTop();

    return 0;
}
