#define __DEFINE_QUEUE_H
#ifdef __DEFINE_QUEUE_H

#include "deque_F.h"

namespace iop {
    template <class _Ty, class Sequence = iop::deque<_Ty>> class queue
    {
      public:
        using value_type = Sequence::value_type;
        using pointer = Sequence::pointer;
        using reference = Sequence::reference;
        using size_type = Sequence::size_type;
        using difference_type = Sequence::difference_type;
        using const_pointer = Sequence::const_pointer;
        using const_reference = Sequence::const_reference;

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

#endif // __DEFINE_QUEUE_H