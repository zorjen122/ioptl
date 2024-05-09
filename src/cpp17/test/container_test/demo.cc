#include <iostream>
#include "../test_.h"
#include "../../allocator_traits_F.h"
#include "../../allocator_F.h"

using std::cout;

namespace iop{


    struct Student
    {
        Student() {
            age = 0;
            name = nullptr;
        }


        Student(const char* name, int age)
        {
            this->name = name;
            this->age = age;
        }


        void print()
        {
            cout << "name: " << this->name << "\t age: " <<age << "\n";
        }

        int age;
        const char* name;
    };

    void test()
    {
        using type = allocator<int>;
        using ReAlloc = typename allocator_traits<type>::template rebind<Student>::other;

        cout << "bool: ";
        cout << mpls::is_same<ReAlloc, allocator<Student>>::value << "\n";

        ReAlloc __alloc;

        using traits_type = allocator_traits<type>;
        Student* st = __alloc.allocate(1);
        traits_type::construct_at(st, "musk", 18);
        
        st->print();
    }
}

int main()
{
    iop::test();
    
}