#include <bits/stdc++.h>

namespace qe
{
    namespace pe
    {
        void print()
        {
            std::cout << "qe::pe::print()\n"; 
        }
    }
}

namespace pe
{
    void print()
    {
        std::cout << "pe::print()\n";
    }    
}


namespace qe
{
    auto ptr = pe::print;
}

int main()
{
    qe::ptr();
}