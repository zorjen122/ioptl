#ifndef ALLOC_F
#define ALLOC_F

#include <cassert>
#include <cstdlib>

#define MALLOC_BASE_SIZE 8
#define MALLOC_ALIGN 8
#define MALLOC_LIST_MAX_SIZE 16
#define MALLOC_HANDLE_BOUND_BYPES 128

#if defined(__OS400__) || defined(_WIN64)
enum
{
    _ALIGN = 16,
    _ALIGN_SHIFT = 4,
    _MAX_BYTES = 256
};
#else
enum
{
    _ALIGN = 8,
    _ALIGN_SHIFT = 3,
    _MAX_BYTES = 128
};
#endif /* __OS400__ */

#define MALLOC_FREELIST_INDEX(__bytes)                                         \
    ((__bytes - size_t(1)) >> (int)_ALIGN_SHIFT)

namespace iop {

    template <class _Ty> class __malloc_alloc_base
    {
      protected:
        using handle_func_type = _Ty (*)();

      public:
        using pointer = _Ty *;

      protected:
        handle_func_type M_malloc_handle;

      public:
        __malloc_alloc_base() : M_malloc_handle(nullptr) {}

        auto set_malloc_handle(handle_func_type __func)
        {
            auto old_func = M_malloc_handle;
            M_malloc_handle = __func;

            return old_func;
        }

        pointer oom_malloc(size_t __size)
        {
            handle_func_type handle_func = M_malloc_handle;
            for (;;) {
                if (handle_func == nullptr)
                    throw;

                (*handle_func)();
                auto ptr = (pointer)malloc(__size);

                if (ptr != nullptr)
                    return ptr;
            }
        }

        pointer oom_realllc(pointer __ptr, size_t __size)
        {
            handle_func_type handle_func = M_malloc_handle;
            for (;;) {
                if (handle_func == nullptr)
                    throw;

                (*handle_func)();
                realloc(__ptr, __size);

                if (__ptr != nullptr)
                    return __ptr;
            }
        }

        pointer allocate(size_t __size)
        {
            if (__size == 0)
                return nullptr;

            auto ptr = (pointer)malloc(__size);
            if (ptr == nullptr)
                oom_malloc(__size);

            return ptr;
        }

        pointer reallocate(pointer __ptr, size_t __size)
        {
            realloc(__ptr, __size);

            if (__ptr == nullptr)
                oom_realllc(__ptr, __size);
        }

        void deallocate(pointer __ptr, size_t __n = 0) { free(__ptr); }
    };

    template <class _Ty> class __malloc_alloc
    {
      protected:
        using __Base = __malloc_alloc_base<_Ty>;
        union Node
        {
            union Node *link;
            char c[MALLOC_BASE_SIZE];
        };

      public:
        using value_type = _Ty;
        using pointer = _Ty *;
        using const_pointer = const _Ty *;
        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;

      protected:
        __Base M_malloc_handle;
        Node *volatile M_freely_list[MALLOC_LIST_MAX_SIZE];

        char *M_start;
        char *M_end;
        size_t M_heap_size;

      public:
        __malloc_alloc()
            : M_start(nullptr), M_end(nullptr), M_heap_size(), M_malloc_handle()
        {
            for (int i = 0; i < MALLOC_LIST_MAX_SIZE; ++i)
                M_freely_list[i] = 0;
        }

        ~__malloc_alloc() {}

      protected:
        auto round_up(size_t __bytes)
        {
            return (((__bytes) + (size_t)MALLOC_ALIGN - 1) &
                    ~((size_t)MALLOC_ALIGN - 1));
        }

        char *chunk_alloc(size_t &ref_node_size, size_t __aligned_bypes)
        {
            char *result;
            size_t diff_heap = M_end - M_start;
            size_t total_size = ref_node_size * __aligned_bypes;

            if (diff_heap >= total_size) {
                result = M_start;
                M_start += total_size;

                return result;
            }
            else if (diff_heap >= __aligned_bypes) // only one
            {
                ref_node_size = diff_heap / __aligned_bypes;
                result = M_start;
                M_start += ref_node_size * __aligned_bypes;

                return result;
            }
            else {
                Node *volatile *my_freely_list;
                if (diff_heap > 0) {
                    my_freely_list =
                        M_freely_list + MALLOC_FREELIST_INDEX(__aligned_bypes);
                    reinterpret_cast<Node *>(M_start)->link = *my_freely_list;
                    *my_freely_list = reinterpret_cast<Node *>(M_start);
                }

                auto append_size =
                    ref_node_size * __aligned_bypes + (M_heap_size >> 4);
                M_start = (char *)malloc(append_size);

                if (M_start == nullptr) {
                    for (int step = __aligned_bypes;
                         step < MALLOC_HANDLE_BOUND_BYPES;
                         step += MALLOC_ALIGN) {
                        my_freely_list =
                            M_freely_list + MALLOC_FREELIST_INDEX(step);
                        auto ptr = *my_freely_list;
                        if (ptr != nullptr) {
                            *my_freely_list = (*my_freely_list)->link;
                            M_start = reinterpret_cast<char *>(ptr);
                            M_end = M_start + step;

                            return chunk_alloc(ref_node_size, __aligned_bypes);
                        }
                    } // for

                    M_end = nullptr;

                    // This function exist throw
                    M_malloc_handle.allocate(__aligned_bypes);
                }

                M_end = M_start + append_size;
                M_heap_size += append_size;

                return chunk_alloc(ref_node_size, __aligned_bypes);
            }
        }

        char *refill(size_t __aligned_bypes)
        {
            size_t ref_node_size = 20;
            Node *result, *current_obj, *next_obj;

            char *chunk = chunk_alloc(ref_node_size, __aligned_bypes);
            result = reinterpret_cast<Node *>(chunk);

            // chunk_alloc() have update M_freely_list.
            if (ref_node_size == 1)
                return chunk;

            Node *volatile *my_freely_list =
                M_freely_list + MALLOC_FREELIST_INDEX(__aligned_bypes);

            *my_freely_list = next_obj =
                reinterpret_cast<Node *>(chunk + __aligned_bypes);

            for (auto i = 1; i < ref_node_size; ++i) {

                current_obj = next_obj;
                next_obj = reinterpret_cast<Node *>(
                    reinterpret_cast<char *>(next_obj) + __aligned_bypes);

                if (i == ref_node_size - 1)
                    current_obj->link = nullptr;
                else
                    current_obj->link = next_obj;
            }

            return reinterpret_cast<char *>(result);
        }

      public:
        pointer allocate(size_t __size)
        {
            assert(__size > 0);

            // This __size no need ailgned.
            if (__size > MALLOC_HANDLE_BOUND_BYPES)
                return M_malloc_handle.allocate(__size);

            Node *volatile *my_freely_list =
                M_freely_list + MALLOC_FREELIST_INDEX(__size);

            Node *ret = *my_freely_list;
            if (ret == nullptr) {
                auto ptr = reinterpret_cast<pointer>(refill(round_up(__size)));
                return ptr;
            }

            *my_freely_list = (*my_freely_list)->link;

            return reinterpret_cast<pointer>(ret);
        }

        void deallocate(pointer __ptr, size_t __size = 0)
        {
            if (__size > MALLOC_HANDLE_BOUND_BYPES)
                return M_malloc_handle.deallocate(__ptr, __size);

            if (__ptr == nullptr)
                return;

            Node *volatile *my_freely_list =
                M_freely_list + MALLOC_FREELIST_INDEX(__size);

            reinterpret_cast<Node *>(__ptr)->link = *my_freely_list;
            *my_freely_list = reinterpret_cast<Node *>(__ptr);
        }
    };

    template <class _Ty> using alloc = __malloc_alloc<_Ty>;

}; // namespace iop

#endif