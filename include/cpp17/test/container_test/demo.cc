#include "../../container/listF.h"
#include "../../container/vectorF.h"
#include "../test_.h"

int main()
{
    iop::list<int> ls = {1,2,3,4,5,6};
    test::execBaseFunc(ls);
    
    test::execChangeFunc(ls);

    iop::vector<int> vec = {1,2,3,4,5,6};

    test::execBaseFunc(vec);
    test::execChangeFunc(vec);
}