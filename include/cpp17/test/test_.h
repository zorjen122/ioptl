#ifndef _IOP_TEST_
#define _IOP_TEST_


#include <chrono>
#include <iostream>
#include "../meta/functionalF.h"

namespace test
{

    namespace __FormatCout
    {
        template<class Container>
        void print(Container& cont)
        {
            printf("$element: "); 
            for(auto it : cont) 
                printf("%d ", it); 
            printf("\n");
        }
    };

    void __execFunc(::iop::function<void()> __f) {
        auto start = std::chrono::high_resolution_clock::now(); // 记录开始时间

        __f();

        auto end = std::chrono::high_resolution_clock::now(); // 记录结束时间
        auto duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                .count();

        std::cout << duration << " mil\n";
    }

    /* container */

    #define __EXEC_BLOCK_TEST(EXEC_CODE) std::cout << "\n-------------------------------------------\n";\
                                            EXEC_CODE                                                   \
                                        std::cout << "\n-------------------------------------------\n"; 


    template<class Container>
    void execBaseFunc(Container& cont)
    {

        __EXEC_BLOCK_TEST(

            __FormatCout::print(cont);

            printf("front/begin        back/end - 1\n");
            printf("{%d}                    {%d}\n\n", cont.front(), cont.back());

            printf("size        classSize       max_size\n");
            printf("{%ld}            {%ld}           {%ld}\n", cont.size(), sizeof(cont), cont.max_size());

            printf("empty: {%s}\n", cont.empty() ? "true" : "false");

        );
    }

    template<class Container>
    void execChangeFunc(Container& cont)
    {

        __EXEC_BLOCK_TEST(
            __EXEC_BLOCK_TEST(
                
                __FormatCout::print(cont);
                printf("push_back(19)/push_front(91) after:\n");
                cont.push_front(19);
                cont.push_back(91);
                __FormatCout::print(cont);
            );
        );
    }

};

#endif