#include <tuple>
#include <iostream>
#include <utility>
#include "../utilitys.h"
#include "../iterator_base_F.h"
#include "../container/array_F.h"
#include "../type_traits.h"

template<typename T, T... ints>
void print_sequence(FViop::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}

template<typename Array, std::size_t... I>
auto a2t_impl(const Array& a, FViop::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template<typename T, std::size_t N, typename Indices = FViop::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}


template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
    const Tuple& t,
    FViop::index_sequence<Is...>)
{
    ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, FViop::index_sequence_for<Args...>{});
    return os << ")";
}

int main()
{
    print_sequence(FViop::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
    print_sequence(FViop::make_integer_sequence<int, 20>{});
    print_sequence(FViop::make_index_sequence<10>{});
    print_sequence(FViop::index_sequence_for<float, std::iostream, char>{});

    std::array<int, 4> array = { 1,2,3,4 };

    auto tuple = a2t(array);
    static_assert(mpls::is_same<decltype(tuple),
        std::tuple<int, int, int, int>>::value, "");

    std::cout << tuple << '\n';

    return 0;
}
