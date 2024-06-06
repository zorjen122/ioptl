#ifndef _IOP_QUEUE_DEF
#define _IOP_QUEUE_DEF

#include "dequeF.h"

namespace iop {
    template <class _Ty, class Sequence = iop::deque<_Ty>> class queue
    {
      public:
        using value_type = typename Sequence::value_type;
        using pointer = typename Sequence::pointer;
        using reference = typename Sequence::reference;
        using size_type = typename Sequence::size_type;
        using difference_type = typename Sequence::difference_type;
        using const_pointer = typename Sequence::const_pointer;
        using const_reference = typename Sequence::const_reference;

      private:
        Sequence c;

      public:
        queue() : c() {}

        void push(const_reference __value) { c.push_back(__value); }
        void pop() { c.pop_front(); }

        void swap(queue &__res) { c.swap(__res); }

        size_type size() { return c.size(); }
        bool empty() { return c.empty(); }

        reference front() { return c.front(); }
        reference back() { return c.back(); }
    };
}; // namespace iop

#endif // _IOP_QUEUE_DEF