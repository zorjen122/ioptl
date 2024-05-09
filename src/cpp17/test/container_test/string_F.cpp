#include "../../container/string_F.h"
#include "../../algorithm.h"

#include <iostream>
#include <iomanip>
using namespace std;

void strErase() {
    iop::string s = "This Is An Example";
    std::cout << "1) " << s << '\n';

    s.erase(7, 3); // 使用重载 (1) 擦除 " An"
    std::cout << "2) " << s << '\n';

    s.erase(iop_algo::find(s.begin(), s.end(), ' ')); // 使用重载 (2) 擦除第一个 ' '
    std::cout << "3) " << s << '\n';

    s.erase(s.find(' ')); // 使用重载 (1) 截掉从 ' ' 到字符串结尾的部分
    std::cout << "4) " << s << '\n';

    auto it = s.begin() + s.find('s'); // 获取指向第一个 's' 的迭代器
    s.erase(it, it + 2); // 使用重载 (3) 擦除 "sI"
    std::cout << "5) " << s << '\n';

    // assert(static_cast<size_t>(s.end() - s.begin()) - s.size() == 1);
}


void strRvalue() {
    iop::string source = "Hello";
    iop::string destination(std::move(source));

    std::cout << "Destination string: " << destination << std::endl;
    std::cout << "Source string after move: " << source << std::endl;

}
#define TEST(expr) \
    do { \
        cout << "Testing " #expr " ... "; \
        if (expr) { \
            cout << "Passed" << endl; \
        } else { \
            cout << "Failed" << endl; \
        } \
    } while(0)


void strAssign() {
    {
        iop::basic_string<char> str1;
        str1.assign("Hello");
        TEST(str1 == "Hello");

        iop::basic_string<char> str2;
        str2.assign("Hello");
        TEST(str2 == "Hello");
    }

    //  assign(const iop::basic_string& str)
    {
        iop::basic_string<char> str3 = "Hello";
        iop::basic_string<char> str4;
        str4.assign(str3);
        TEST(str4 == "Hello");

        iop::basic_string<char> str5 = "Hello";
        iop::basic_string<char> str6(5);
        str6.assign(str5);
        TEST(str6 == "Hello");
    }

    //  assign(std::initializer_list<const_pointer> ilist)
    {
        iop::basic_string<char> str7;
        str7.assign({ "Hello", "World" });
        TEST(str7 == "World");

        iop::basic_string<char> str8(10);
        str8.assign({ "Hello", "World" });
        TEST(str8 == "World");
    }

    //  assign 函数对特殊情况的处理
    {
        iop::basic_string<char> str9(5);
        str9.assign("Hello World");
        TEST(str9 == "Hello World");

        iop::basic_string<wchar_t> str10;
        str10.assign(L"word");
        TEST(str10 == L"word");
    }

};

void strAppend() {
    iop::basic_string<char> str = "string";
    const char* cptr = "C-string";
    const char carr[] = "Two and one";

    iop::string output;

    // 1) 后附 char 3 次。
    // 注意，这是仅有的接受 char 的重载。
    output.append(3, '*');
    std::cout << "1) " << output << "\n";
        //  2) 后附整个字符串
    output.append(str);
    std::cout << "2) " << output << "\n";

    // 3) 后附字符串的一部分（下例后附最后 3 个字母）
    output.append(str, 3, 3);
    std::cout << "3) " << output << "\n";

    // 4) 后附 C 字符串的一部分
    // 注意，因为 `append` 返回 *this，我们能一同链式调用
    output.append(1, ' ').append(carr, 4);
    std::cout << "4) " << output << "\n";

    // 5) 后附整个 C 字符串
    output.append(cptr);
    std::cout << "5) " << output << "\n";

    // 6) 后附范围
    // using p = decltype(std::begin(carr));
    output.append(std::begin(carr) + 3, std::end(carr));
    std::cout << "6) " << output << "\n";

    // 7) 后附初始化器列表
    output.append({ ' ', 'l', 'i', 's', 't' });
    std::cout << "7) " << output << "\n";
}

void strResever()
{
    iop::string s;
    const iop::string::size_type new_capacity{ 100u };
    std::cout << "变更前：" << s.capacity() << "\n";

    s.reserve(new_capacity);
    std::cout << "变更后：" << s.capacity() << "\n";
    assert(new_capacity <= s.capacity());

    // 观察容量的增长因数
    auto cap{ s.capacity() };
    for (int check{}; check != 4; ++check)
    {
        while (cap == s.capacity())
            s += '$';
        cap = s.capacity();
        std::cout << "新容量：" << cap << '\n';
    }

    // s.reserve(); // C++20 中弃用，应使用：
    // s.shrink_to_fit();
    // std::cout << "变更后：" << s.capacity() << "\n";
}

void test()
{
    iop::basic_string<char>vals("pps");
    iop::basic_string<char>val(vals);
    iop::string ss = { "sadsad","ioopppppooooas pp p p" ,"sadsad" };

    {
        for (auto&& i : ss)
            cout << i;
        cout << "\n\n";
        cout << ss << "\n";
        cout << ss.size() << "\n";
    }
    
    {
        val.insert(val.begin() + 1, "-vvq-");
        val.append("-hello-");
        std::cout << "{ c_str() || data() } :" << val.c_str() << "\n";

        std::cout << *val.begin() << " begin, end " << *val.end() << endl;
        std::cout << "size " << val.size() << "\n\n";
        assert(static_cast<size_t>(val.end() - val.begin()) - val.size() == 1);
    }

    {
        iop::string const s("Emplary");
        assert(s.size() == strlen(s.c_str()));
        assert(std::equal(s.begin(), s.end(), s.c_str()));
        assert(std::equal(s.c_str(), s.c_str() + s.size(), s.begin()));
        assert(0 == *(s.c_str() + s.size()));
    }

    {
        iop::string s1 = "Hello";
        iop::string s2 = "world";
        cout << s1 + ' ' + s2 + "\n";
        cout << ' ' + s1 + (" Hello " + s2) + "!\n\n\n";
    }

    {
        cout<< "\n\n strErase: \n";
        strErase();

        cout<< "\n\n strRvalue: \n";    
        strRvalue();

        cout << "\n\n strAssign: \n";
        strAssign();

        cout << "\n\n strAppend: \n";
        strAppend();

        cout << "\n\n strResever: \n";
        strResever();
    }

}

int main(int argc, char* argv[]) {

    std::char_traits<char> Ms;

    iop::string s = "hello";
    auto str = s.data();

    const char* val = "hello world";
    iop::string scp(val, Ms.length(val));
    cout<< scp << endl;

    auto len = Ms.length(str);
    cout<< "len: "<< len << endl;

    {
        cout<< "\n\ntest:";
        test();
    }

    return 0;
}


// #endif          // _STRING_STL_DEFINE_
