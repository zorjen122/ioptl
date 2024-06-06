#ifndef _IOP_UNORDERED_MULTISET_DEF
#define _IOP_UNORDERED_MULTISET_DEF

#include "hashtableF.h"
#include "pairF.h"
#include "../allocatorF.h"
#include "../cppconfig.h"
#include "../meta/metautilF.h"

NAMESPACE_BEGIN(iop)

template<class Key,
         class HashFun = hashtable_util::hash<Key>,
         class Alloc = allocator<Key>>
class unordered_multiset
{
private:
    template <typename _Pair>
    struct _Selectlst
    {
        typename _Pair::first_type operator()(const _Pair &__x) const
        {
            return __x.first;
        }
    };
template<class Ta, class Tb = Ta>
struct _equal_to
{
    constexpr bool operator()(const Ta& a, const Tb& b)
    {
        return a == b;
    }
};

protected:
    using hashtable_type = hashtable<Key, pair<Key, Key>, 
                                    HashFun, _Selectlst<pair<Key, Key>>,
                                    _equal_to<Key, Key>, Alloc>;

public:
    using hash_type = typename hashtable_type::hash_type;
    using key_type = typename hashtable_type::key_type;

    using value_type = typename hashtable_type::value_type;
    using iterator = typename hashtable_type::iterator;
    using const_iterator = typename hashtable_type::const_iterator;
    using pointer = typename hashtable_type::pointer;
    using const_pointer = typename hashtable_type::const_pointer;
    using reference = typename hashtable_type::reference;
    using const_reference = typename hashtable_type::const_reference;

    using size_type = typename hashtable_type::size_type;
    using difference_type = typename hashtable_type::difference_type;

protected:
    hashtable_type ht;

public:
    unordered_multiset() : ht(30) {}
    unordered_multiset(const unordered_multiset& lhs) : ht(lhs.ht) {}

public:
    pair<iterator, bool> insert(const_reference value)
    {
        return ht.insert_equal(value);
    }

    pair<iterator, bool> insert_noresize(const_reference value)
    {
        return ht.insert_equal_noresize(value);
    }

    iterator find(const key_type& key)
    {
        return ht.find(key);
    }

    void resize(size_type size)
    {
        return ht.resize(size);
    }

    void clear()
    {
        return ht.clear();
    }

    void erase(const iterator& iter)
    {
        return ht.erase(iter);
    }

    void erase(const iterator& start, const iterator& end)
    {
        return ht.erase(start, end);
    }

    iterator begin()
    {
        return ht.begin();
    }

    iterator end()
    {
        return ht.end();
    }

    size_type size()
    {
        return ht.size();
    }

    size_type max_size()
    {
        return ht.max_size();
    }

    
    void swap(const unordered_multiset& lhs)
    {
        ht.swap(lhs.ht);
    }

    pair<iterator, iterator> equal_range(const key_type& key)
    {
        return ht.equal_range(key);
    }

    template<class... Args>
    pair<iterator, bool> emplace(Args&&... args)
    {
        return ht.emplace_equal(args...);
    }

    size_type count(const key_type& key)
    {
        return ht.count(key);
    }

    size_type buckets(const key_type& key)
    {
        return ht.buckets(key);
    }

    hash_type hash_function()
    {
        return ht.hash_function();
    }

    size_type bucket_size(size_type index)
    {
        return ht.bucket_size(index);
    }

    size_type bucket_count() const
    {
        return ht.bucket_count();
    }

    size_type max_bucket_count() const
    {
        return ht.max_bucket_count();
    }

    bool empty() const
    {
        return ht.empty();
    }

    reference at(const key_type& key)
    {
        return ht.at(key);
    }

    void reserve(size_type cap)
    {
        ht.reserve(cap);
    }
};


NAMESPACE_END(iop)

#endif
