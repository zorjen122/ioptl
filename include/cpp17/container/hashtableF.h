#ifndef _IOP_HASHTABLE_DEF
#define _IOP_HASHTABLE_DEF

#include "vectorF.h"
#include "../allocatorF.h"
#include "../traits/allocator_traitsF.h"
#include "../util/iterator_baseF.h"
#include "pairF.h"
#include "../cppconfig.h"

NAMESPACE_BEGIN(iop)

namespace hashtable_util
{
    constexpr unsigned long hash_prime_list_num = 35;
    constexpr unsigned long hash_prime_list[hash_prime_list_num] = {
        2,3,5,7ul, 23ul, 53ul, 97ul, 193ul, 389ul, 769ul, 1543ul, 3079ul, 6151ul,
            12289ul, 24593ul, 49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
            1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
            50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
            1610612741ul, 3221225473ul, 4294967291ul
    };

    unsigned long prime_list_next(unsigned long prime)
    {
        unsigned long left = 0, right = hash_prime_list_num, mid = 0;

        while(left < right)
        {
            mid = left + (right - left) / 2;
            if(prime > hash_prime_list[mid])
                left = mid + 1;
            else if(prime < hash_prime_list[mid])
                right = mid - 1;
            else
                return mid < right ? hash_prime_list[mid + 1] : hash_prime_list[mid];
        }

        return hash_prime_list[left];
    }

    template<class T>
    struct hash
    {
        constexpr T operator()(const T& value) 
        {
            return value;
        }
    };
};

template<class T>
struct hashtable_node
{
public:
    hashtable_node() : value(), next(nullptr) {}
    hashtable_node(const hashtable_node& lhs)
        : value(lhs.value), next(lhs.next) {}

public:
    T value;
    hashtable_node* next;

};


template<class Key, class Val, class HashFun, 
        class GetKey, class EqualKey, class Alloc>
class hashtable;

template<class Key, class Val, class HashFun, 
        class GetKey, class EqualKey, class Alloc = allocator<Key>>
class hashtable_iterator
{
protected:
    using hashtable_type = hashtable<Key, Val, HashFun, 
                                GetKey, EqualKey, Alloc>;
public:
    using node_type = hashtable_node<Val>;
    using iterator_category = forward_iterator_tag;
    using iterator = hashtable_iterator<Key, Val, HashFun,
                                    GetKey, EqualKey, Alloc>;
    using const_iterator = hashtable_iterator<const Key, const Val, HashFun,
                                    GetKey, EqualKey, Alloc>;

    using value_type = Val;
    using reference = Val&;
    using pointer = Val*;

public:
    node_type* node;
    hashtable_type ht;

public:
    hashtable_iterator() : ht(), node(nullptr) {}
    hashtable_iterator(node_type* p, const hashtable_type& hashtable) 
        : node(p), ht(hashtable) {}

public:

    reference operator*()
    {
        return node->value;
    }
    pointer operator->()
    {
        return &(node->value);
    }

    iterator& operator++()
    {
        auto cur = node;
        node = node->next;

        if(!node)
        {
            auto idx = ht.bkt_num(cur->value);
            for(auto i = idx + 1; i < ht.bucket.size(); ++i)
            {
                if(ht.bucket[i])
                {
                    node = ht.bucket[i];
                    return *this;
                }
            }
        }

        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp = *this;
        ++(*this);

        return tmp;
    }

    bool operator==(const iterator& iter)
    {
        return node == iter.node;
    }

    bool operator!=(const iterator& iter)
    {
        return !(node == iter.node);
    }
};


template<class Key, class Val, class HashFun, 
        class GetKey, class EqualKey, class Alloc = allocator<Val>>
class hashtable
{
protected:
    using node_type = hashtable_node<Val>;
    using allocator_type = Alloc;
    using allocator_traits_type = allocator_traits<Alloc>;
    using allocator_node_type = typename allocator_traits_type::template 
                                rebind_alloc<node_type>::other;
    using allocator_vector_type = typename allocator_traits_type::template 
                                rebind_alloc<node_type*>::other;
public:
    using hash_type = HashFun;
    using key_type = Key;

    using value_type = Val;
    using reference = value_type&;
    using pointer = value_type*;
    
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ::std::ptrdiff_t;

    using iterator = hashtable_iterator<Key, Val, HashFun, 
                                        GetKey, EqualKey, Alloc>;
    using const_iterator = hashtable_iterator<const Key, const Val,
                        HashFun, GetKey, EqualKey, Alloc>;
public:
    hash_type hash;
    EqualKey equal_key;
    GetKey get_key;

    allocator_node_type __node_allocator;
public:
    vector<node_type*,allocator_vector_type> bucket;
    size_type element_num;

public:
    hashtable() : bucket(), element_num(0) {}

    hashtable(size_type cap)
        : bucket(cap, (node_type*) nullptr), element_num(0)
    {}

    hashtable(size_type cap, HashFun ht, EqualKey ek)
        : hash(ht), equal_key(ek), 
          bucket(cap, (node_type*) nullptr), element_num(0)
    {}

    hashtable(const hashtable& ht)
        : bucket(ht.bucket), element_num(ht.element_num)
    {}
    
protected:

    unsigned long aligned_storage(size_type cap) 
    {
        return hashtable_util::prime_list_next(cap);
    }

    node_type* create_node(const_reference value)
    {
        node_type* node = __node_allocator.allocate(1);
        try
        {
            construct_at(&node->value, value);
            node->next = nullptr;
        }
        catch(...)
        {
            __node_allocator.deallocate(node);
            throw;
        }

        return node;
    }

    void delete_node(node_type* node)
    {
        destroy_at(Fiop::addressof(node->value));
        __node_allocator.deallocate(node);
    }

    void initialize_bucket(size_type cap)
    {
        auto storage = aligned_storage(cap);
        bucket.reserve(storage);

        bucket.insert(bucket.begin(), storage, (node_type*)nullptr);

        element_num = 0;
    }

public:
    pair<iterator, bool> insert_unique(const_reference value)
    {
        resize(element_num + 1);

        return insert_unique_noresize(value);
    }

    pair<iterator, bool> insert_equal(const_reference value)
    {
        resize(element_num + 1);

        return insert_equal_noresize(value);
    }

    void resize(size_type cap)
    {
        if(cap < bucket.size())
            return;
        
        size_type new_storage = aligned_storage(cap);
        vector<node_type*,allocator_vector_type> 
            tmp(new_storage, static_cast<node_type*>(nullptr));
        for(size_type i = 0; i < bucket.size(); ++i)
        {
            auto iter = bucket[i];
            while(iter)
            {
                size_type idx = bkt_num(iter->value);
                
                // we using bucket[i] save next node.
                bucket[i] = iter->next;       

                iter->next = tmp[idx];
                tmp[idx] = iter;

                iter = bucket[i]; 
            }
        }

        bucket.swap(tmp);
    }
    
    pair<iterator, bool> insert_unique_noresize(const_reference value)
    {
        size_type idx = bkt_num(value);

        for(auto iter = bucket[idx]; iter; iter = iter->next)
        {
            if(equal_key(get_key(iter->value), get_key(value)))
            {
                return pair<iterator, bool>(
                    iterator(iter, *this), false
                );
            }
        }

        node_type* new_node = create_node(value);
        new_node->next = bucket[idx];
        bucket[idx] = new_node;
        ++element_num;

        return pair<iterator, bool>(
            iterator(new_node, *this), true
        );
    }

    pair<iterator, bool> insert_equal_noresize(const_reference value)
    {
        size_type idx = bkt_num(value);
        auto new_node = create_node(value);
        ++element_num;

        for(node_type** iter = &bucket[idx]; *iter; *iter = (*iter)->next)
        {
            if(equal_key(get_key((*iter)->value), get_key(value)))
            {
                new_node->next = *iter;
                *iter = new_node;

                return pair<iterator, bool>(
                    iterator(new_node, *this), true
                );
            }
        }

        new_node->next = bucket[idx];
        bucket[idx] = new_node;

        return pair<iterator, bool>(
            iterator(new_node, *this), true
        );
    }

    iterator erase(iterator iter)
    {
        key_type key = get_key(*iter);

        return erase(key);
    }

    iterator erase(iterator first, iterator last)
    {
        while(first != last)
            erase(get_key(*(first++)));

        return last;
    }

    iterator erase(const key_type& key)
    {
        size_type idx = bkt_num_key(key);

        auto cur = bucket[idx];
        auto iter = cur->next;

        if(equal_key(get_key(cur->value),key))
        {
            delete_node(cur);
            bucket[idx] = iter;

            --element_num;
            return iterator(iter, *this);
        }


        for(; iter; iter = iter->next)
        {
            if(equal_key(get_key(iter->value), key))
            {
                cur->next = iter->next;
                delete_node(iter);

                iter = cur->next;
                break;
            }
            else
                cur = cur->next;
        }

        --element_num;
        return iterator(iter, *this);
    }

    void clear()
    {
        for(int i = 0; i < bucket.size(); ++i)
        {
            auto iter = bucket[i];
            while(iter)
            {
                auto next = iter->next;
                delete_node(iter);

                iter = next;
            }

            // 之后若有插入，需要判断节点是否为空，否则段错误
            bucket[i] = nullptr;
        }

        element_num = 0;
    }

    iterator find(const key_type& key)
    {
        int idx = bkt_num_key(key);

        for(auto iter = bucket[idx]; iter; iter = iter->next)
        {
            if(equal_key(get_key(iter->value), key))
            {
                return iterator(iter, *this);
            }
        }

        return iterator((node_type*) nullptr, *this);
    }
    
    iterator begin()
    {
        for(int i = 0; i < bucket.size(); ++i)
        {
            if(bucket[i])
                return iterator(bucket[i], *this);
        }

        return iterator((node_type*) nullptr, *this);
    }

    iterator end()
    {
        return iterator((node_type*) nullptr, *this);
    }

    size_type bkt_num(const_reference value)
    {
        return bkt_num_key(get_key(value));
    }

    size_type bkt_num_key(const key_type& key)
    {
        return hash(key) % bucket.size();
    }

    size_type size()
    {
        return element_num;
    }

    size_type max_size()
    {
        return size_type(~0);
    }


    bool empty()
    {
        return begin() == end();
    }

    size_type count(const key_type& key)
    {
        auto idx = bkt_num_key(key), cnt = 0;

        for(auto iter = bucket[idx]; iter; iter = iter->next)
        {
            if(equal_key(get_key(iter->value), key))
                ++cnt;
        }

        return cnt;
    }

    pair<iterator, iterator> equal_range(const key_type& key)
    {
        auto first = find(key);
        auto last = first;

        if(first.node == nullptr)
            return pair{ first, last };

        for(auto iter = first.node; iter; iter = iter->next)
        {
            if(equal_key(get_key(iter->value), key))
                last.node = iter;
        }

        if(last.node == first.node)
            ++last;

        return pair{ first, last };
    }

    hash_type hash_function() const
    {
        return hash;
    }

    size_type buckets(const key_type& key)
    {
        return bkt_num_key(key);
    }

    template<class... Args>
    pair<iterator, bool> emplace_unique(Args&&... args)
    {
        return insert_unique(value_type{Fiop::forward<Args>(args)...});
    }

    template<class... Args>
    pair<iterator, bool> emplace_equal(Args&&... args)
    {
        return insert_equal(value_type{Fiop::forward<Args>(args)...});
    }

    void swap(const hashtable& lhs)
    {
        if(lhs != *this)
        {
            swap(element_num, lhs.element_num);
            ::std::swap(bucket, lhs.bucket);
            ::std::swap(hash, lhs.hash);
            ::std::swap(equal_key, lhs.equal_key);
            ::std::swap(get_key, lhs.get_key);
        }
    }    

    size_type bucket_size(size_type index)
    {
        size_type cnt = 0;
        for(auto iter = bucket[index]; iter; iter = iter->next)
            ++cnt;

        return cnt;
    }

    size_type bucket_count() const
    {
        return bucket.size();
    }

    size_type max_bucket_count()
    {
        return bucket.max_size();
    }

    reference at(const key_type& key)
    {
        auto ret = find(key);
        if(ret != end())
            return *ret;
        else
            throw;
    }

    void reserve(size_type cap)
    {
        return resize(reserve);
    }
};

NAMESPACE_END(iop)

#endif  // _IOP_HASHTABLE_DEF