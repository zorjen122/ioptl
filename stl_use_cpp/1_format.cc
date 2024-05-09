#include <format>
#include <iostream>
#include <string_view>

using namespace std;

struct Pos{
    int x;
    int y;
    
    Pos(int _x, int _y) : x(_x), y(_y) {}
};


// ERR
template<>
struct std::formatter<Pos>
{
    template<class Ctx>
    constexpr auto parse(Ctx& ctx)
    {
        return ctx.begin();     // default
    }

    template<class Ctx>
    auto format(const Pos& obj, Ctx& ctx)
    {
        return format_to(ctx.out(), "({0:d}, {1:d})", obj.x, obj.y);
    }
};


template<class... _Args>
void print(const string_view fmt_str, _Args&&... args)
{
    auto fmt_args{ make_format_args(args...) };
    string out_str{ vformat(fmt_str, fmt_args) };
    fputs(out_str.c_str(), stdout);
}


int main()
{

    auto s{"world!\n"};
    auto ss = format("Hello, {}", s);
    Pos A1 = {10, 20};
    print("A1: {}", A1);

    cout << ss;
    printf("Hello, %s", s);

    return 0;
}