#include <forward_list>
#include <iterator>
#include <iostream>
using namespace std;
template <class _O, class _C>
     _O &operator<<(_O &__cout, _C &__res)
{
    __cout << "{";
    for (auto iter = __res.begin(); iter != __res.end(); ++iter)
        __cout << ' ' << *iter;
    __cout << " }";

    return __cout;
}

void erase_after_test()
{
    std::forward_list<int> ls;

    for(int i = 0; i <= 10; ++i){
        ls.push_front(i);
    }

    ls.push_front(999);

    cout << ls << "\n";


    ls.erase_after(ls.before_begin());

    cout << ls << "\n";

    auto before_end = ls.begin();
    for(;before_end != ls.end(); ++before_end); 

    
    for(auto it = ls.begin(); it != before_end;)
    {
        if((*it) % 2 == 0)
            it = ls.erase_after(it);
        else
            ++it;
    }

    cout << ls << "\n";

    auto start_it = ls.begin();
    auto end_it = ++(++(++start_it));

    auto erase_pos = ls.erase_after(start_it, end_it);

    cout << ls << "\n";

    ls.erase_after(erase_pos, ls.end());
}


struct A
{
    void foo()
    {
        cout << "A\n";
    }

    int b = 10;
protected:
    int a;
private:
    int c;
};

struct B : private A
{
    void foo()
    {
        cout << "B\n";
        a = 10;
        b = 10;
        // c = 10;  err
    }  
};

struct C : B
{
    void foo()
    {
        // b = 20;      if private B, then err
    }
};



int main()
{
    // erase_after_test();

        std::forward_list<char> characters;
 
    characters.assign(5, 'a');
 
    for (char c : characters) {
        std::cout << c << ' ';
    } 
 
    characters.assign({'\n', 'C', '+', '+', '1', '1', 'X'});
 
    for (char c : characters) {
        std::cout << c;
    }

    for(std::forward_list<char>::const_iterator it = characters.begin(); it != characters.end(); ++it)
    cout << *it << " ";
    cout << "\n";

    int a = 10;
    int b = 20;
    int & p = a;
    cout << p << " \n";
    p = b;
    cout << p << " \n";


    B R;
    // R.A::foo();


    std::cout << '\n';
}