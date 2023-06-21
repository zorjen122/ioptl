#include "../lib/src/iterator_F.h"
#include "../lib/src/allocator_F.h"
#include "../lib/src/allocator_traits_F.h"
#include "../lib/src/algobase_F.h"

#include<iostream>
#include<cmath>
using namespace std;

#include "../lib/src/alloc_operator_new_F.h"

#define _OPERATOR_THAN(op)                              \
    bool operator op(array& __rhs) {                    \
        value_type __iter = 0;                          \
        for (auto __it = __rhs.begin();                 \
            __it != __rhs.end();++__it, ++_Varray) {    \
            __iter += ((*__it) - (*__it));              \
        }                                               \
        return (__iter op 0);                           \
    }                                                   \

namespace CCC_STL {


    template<class _Ty, ::std::size_t _N>
    class array {
    public:
        using allocator_type = CCC_allocator<_Ty>;
        using allocator_traits_type = allocator_traits<_Ty, allocator_type>;

        using value_type = _Ty;     using size_type = allocator_traits_type::size_type;
        using difference_type = allocator_traits_type::difference_type;

        using pointer = allocator_traits_type::pointer;
        using const_pointer = allocator_traits_type::const_pointer;
        using reference = value_type&;         using const_reference = const value_type&;

        using iterator = pointer;
        using const_iterator = const iterator;
        using reverse_iterator = Iterator_STL::reverse_iterator<iterator>;
        using const_reverse_iterator = Iterator_STL::reverse_iterator<const_iterator>;


    protected:
        allocator_type V_data_allocator;
        pointer _Varray;   size_type _Size;

    public:
        array() : _Size(_N) {
            _Varray = V_data_allocator.allocate(_N + 1);
        }

        template<class _Tp, size_type _Np>
        array(const array<_Tp, _Np>& __rhs) {
            _Varray = V_data_allocator.allocate(_N);
            this->_Size = __rhs._Size;

            pointer __res = this->_Varray;

            for (auto __iter = __rhs.begin();__iter != __rhs.end(); ++__iter) {
                construct(__res, *__iter);
            }
        }


        template<class _Tp, size_type _Np>
        array(array<_Tp, _Np>& __rhs) {
            _Varray = V_data_allocator.allocate(_N);
            this->_Size = __rhs._Size;

            pointer __res = this->_Varray;

            for (auto __iter = __rhs.begin();__iter != __rhs.end(); ++__iter) {
                construct(__res, *__iter);
            }
        }

        ~array() {
            destroy(this->begin(), this->end());
        }

        size_type size() {
            return this->_Size;
        }
        pointer data() NOEXCEPT(NOEXCEPT(assert(this->_Varray != nullptr))) {
            return this->_Varray;
        }

        iterator end() {
            return static_cast<iterator>(_Varray + (this->_Size));
        }
        iterator begin() {
            return static_cast<iterator>(_Varray);
        }

        const_iterator cend()const {
            return static_cast<const_iterator>(_Varray + (this->_Size));
        }
        const_iterator cbegin()const {
            return static_cast<const_iterator>(_Varray);
        }

        reference back() {
            return *(this->end() - 1);
        }
        reference front() {
            return *(this->begin());
        }

        void fiil(const value_type& __val) {
            pointer __res = this->_Varray;
            for (;__res != this->end();++__res) {
                *__res = __val;
            }
        }

        void swap(array& __rhs)noexcept(
            noexcept(swap_ptr(
                std::declval<reference>(), std::declval<reference>()))
            ) {
            pointer __res = this->_Varray;
            for (;__res != this->end();++__res, ++__rhs)
                swap_ptr(__res, __rhs);

        }

        bool empty() {
            return (this->_Varray == nullptr);
        }

    public:
        value_type operator[](size_type __idx) {
            return *(_Varray + __idx + 1);
        }

        bool operator=(array& __rhs) { return (__rhs == *this); }
        bool operator!=(array& __rhs) { return (__rhs != *this); }

        bool operator>(array& __rhs) {
            value_type __iter = 0;
            pointer __res = __rhs.data();
            for (auto __it = this->begin();
                __it != this->end(); ++__it, ++__res) {
                __iter += ((*__it) - (*__res));
            }
            return (__iter > 0);
        }

        _OPERATOR_THAN(< );
        _OPERATOR_THAN(<= );
        _OPERATOR_THAN(>= );
    };


};


int main(int argc, char* argv[]) {
    using namespace std;


    CCC_STL::array<int, 10>arr;
    CCC_STL::array<int, 10>arrs;

    for (int i = 0;i < arr.size();++i) {
        int number = rand();
        *(arr.data() + i) = number;
    }

    for (int i = 0;i < arr.size();++i) {
        int number = rand();
        *(arrs.data() + i) = number;
    }

    std::cout << endl;

    std::cout << (arr > arrs) << endl;
    std::cout << (arr < arrs) << endl;
    std::cout << (arr <= arrs) << endl;
    std::cout << (arr >= arrs) << endl;


    std::cout << endl;

    ::std::cout << *arr.begin() << "\tend\t" << *arr.end()
        << endl << endl;

    for (auto __it : arr) {
        std::cout << __it << ",";
    }

    std::cout << endl << "index 0: " << arr[0] << endl;

    ::std::cout << "size: " << arr.size() << endl;
    ::std::cout << "front: " << arr.back() << endl;
    ::std::cout << "back: " << arr.front() << endl;

    ::std::cout << "empty: " << arr.empty() << endl;

    // ::std::array<int>ary;

    return 0;
}

