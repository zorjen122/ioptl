// #pragma once

#define _STRING_CCC_DEFINE_ 
#ifdef _STRING_CCC_DEFINE_ 
#endif

#include "../lib/src/iterator_F.h"
#include "../lib/src/allocator_F.h"
#include "../lib/src/allocator_traits_F.h"
#include "../lib/src/character_traits_F.h"

#include <iostream>

#ifdef ALLOC_CCC_BUG
#include "alloc_F.h"
#else
#include "../lib/src/alloc_operator_new_F.h"
template<class _Ty>
using CCC_allocator = __Default_alloc<_Ty>;
#endif


// TODO:  need abstruct iterator to const char* type;
namespace CCC_STL {

    template<class _CharT,
        class _Traits = char_traits<_CharT>,
        class _Alloc = CCC_allocator<_CharT>>
        class string {
        protected:
            using allocator_type = _Alloc;
            using allocator_traits_type = allocator_traits<_CharT, _Alloc>;

        public:
            using traits_type = _Traits;
            using value_type = _CharT;
            using size_type = typename allocator_traits_type::size_type;
            using difference_type = typename allocator_traits_type::difference_type;

            using pointer = typename allocator_traits_type::pointer;
            using const_pointer = typename allocator_traits_type::const_pointer;
            using reference = value_type&;
            using const_reference = const value_type&;

            using iterator = pointer;
            using const_iterator = const iterator;
            using reverse_iterator = Iterator_STL::reverse_iterator<iterator>;
            using const_reverse_iterator = Iterator_STL::reverse_iterator<const_iterator>;

        protected:
            size_type len;
            pointer _Vstring;
            allocator_type V_data_allocator;
            traits_type V_data_traits;

        public:
            string() {
                V_data_allocator = allocator_type();
                _Vstring = V_data_allocator.allocate(1);
                *_Vstring = '\0';
                len = 0;
            }

            string(const size_type& __sz) {
                len = __sz;
                V_data_allocator = allocator_type();
                _Vstring = V_data_allocator.allocate(len + 1);
                *(_Vstring + len + 1) = '\0';
            }

            string(string& __rhs) {
                len = __rhs.len;
                V_data_allocator = decltype(__rhs.V_data_allocator)();
                _Vstring = V_data_allocator.allocate(len + 1);

                iterator iter = __rhs.begin();
                uninitialized_copy(iter, __rhs.end(), _Vstring);
                *(_Vstring + len + 1) = '\0';
            }

            string(const char* __val) {
                len = V_data_traits.length(__val);
                V_data_allocator = allocator_type();

                _Vstring = V_data_allocator.allocate(len + 1);

                try {
                    for (size_type i = 0;i < len;++i)
                        *(_Vstring + i) = __val[i];
                }
                catch (std::exception __e) {
                    throw __e;
                }

                *(_Vstring + len + 1) = '\0';
            }

            string(const char* __val, size_type __len) {
                len = __len;
                V_data_allocator = allocator_type();

                _Vstring = V_data_allocator(len + 1);
                iterator __iter = __val;

                uninitialized_copy(__iter, __val + len, _Vstring);
                *(_Vstring + len + 1) = '\0';
            }

        public:
            string& insert(const_iterator __pos, const_pointer __val) {

                size_type sup = V_data_traits.length(__val);

                pointer res = V_data_allocator.allocate(sup + len + 1);

                iterator iter = begin();
                const_pointer viter = __val;

                const_iterator start_pos = uninitialized_copy(iter, __pos, res);

                const_iterator add_pos = uninitialized_copy(viter, (viter + sup), start_pos);

                uninitialized_copy(__pos, end(), add_pos);

                destroy(this->begin(), this->end());
                V_data_allocator.deallocate(_Vstring);

                _Vstring = res;
                len += sup;

                return *this;
            }

            string& push_back(const_pointer __val) {

                size_type sup = traits_type::length(__val);
                insert(this->end() - 1, __val);

                return *this;
            }

            string& push_front(const_pointer __val) {

                size_type sup = traits_type::length(__val);
                insert(this->begin(), __val);

                return *this;
            }


            const_pointer c_str() { return _Vstring; }

            iterator begin() { return static_cast<iterator>(_Vstring); }
            iterator end() { return static_cast<iterator>(_Vstring + len + 1); }
            size_type size() { return len; }

        public:
            string& operator=(const string& __rhs) {

                destroy(begin(), end());
                len = __rhs.len;
                V_data_allocator = allocator_type();
                _Vstring = V_data_allocator.allocate(len + 1);

                iterator iter = __rhs.begin();
                uninitialized_copy(iter, __rhs.end(), _Vstring);

                return *this;
            }

            reference operator[](size_type __sz) {
                return *(_Vstring + __sz);
            }

            // string& operator""s(const char* __str, size_type __len) { string(__str, __len); }
    };

};              // _STRING_CCC_DEFINE_


int main(int argc, char* argv[]) {

    using namespace std;

    CCC_STL::string<char>vals("pps");
    CCC_STL::string<char>val(vals);
    val.insert(val.begin() + 1, "-vvq-");
    val.push_back("-hello-");
    val.push_front("-word-");

    std::cout << val.c_str() << endl;

    std::cout << *val.begin() << " begin, end " << *val.end() << endl;
    std::cout << "size " << val.size();


    return 0;
}


// #endif          // _STRING_CCC_DEFINE_
