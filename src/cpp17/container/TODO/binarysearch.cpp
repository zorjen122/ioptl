#include "../allocator_F.h"
#include "../functionals.h"
#include <iostream>

using namespace std;

namespace iop {

    template <class _Ty> struct __btree_node
    {
        __btree_node *left;
        __btree_node *right;
        _Ty value;

        __btree_node(const __btree_node &__res)
        {
            left = __res.left, right = __res.right, value = __res.value;
        }
    };

    template <class _Ty, class _Comp = iop::less<_Ty, _Ty>,
              class _Allocator = iop::allocator<__btree_node<_Ty>>>
    class BinarySearchTree
    {
      public:
        using node_type = __btree_node<_Ty>;
        using allocator_type = _Allocator;
        using Compare = _Comp;

        using value_type = _Ty;
        using reference = _Ty &;
        using const_reference = const _Ty &;

        // protected:
        node_type *root;
        [[no_unique_address]] allocator_type __allocator;
        [[no_unique_address]] Compare __comp;

      public:
        explicit BinarySearchTree() { set_node(value_type()); }
        explicit BinarySearchTree(const BinarySearchTree &__res) {}

      public:
        node_type *set_node(const_reference __val)
        {
            try {
                node_type *new_node = __allocator.allocate(1);
                new_node->value = __val;
                new_node->left = nullptr;
                new_node->right = nullptr;

                return new_node;
            }
            catch (...) {
                throw;
            }
        }

        node_type *find(const_reference __pos)
        {
            if (root == nullptr)
                return nullptr;

            auto iter = root;
            while (iter != nullptr) {
                bool comp = __comp(iter->value, __pos);
                if (iter->value == __pos)
                    return iter;
                else if (comp)
                    iter = iter->right;
                else
                    iter = iter->left;
            }

            return nullptr;
        }

        node_type *findMin(node_type *__start)
        {
            if (__start == nullptr)
                return __start;

            auto iter = __start;
            while (iter->left != nullptr)
                iter = iter->left;

            return iter;
        }

        node_type *findMax(node_type *__start)
        {
            if (__start == nullptr)
                return __start;

            auto iter = __start;
            while (iter->right != nullptr)
                iter = iter->right;

            return iter;
        }

        node_type *findMin() { return findMin(root); }
        node_type *findMax() { return findMax(root); }

        bool compare(node_type *__a, node_type *__b)
        {
            return __comp(__a->value, __b->value);
        }

        void insert(node_type *__node)
        {
            node_type *iter = root;

            while (iter != nullptr)
                iter = (iter < __node) ? iter->right : iter->left;

            iter = __node;
        }

        void insert(const_reference __val)
        {
            node_type *ist = set_node(__val);
            insert(ist);
        }

        void erase(node_type *__node)
        {
            if (__node == nullptr)
                return;

            node_type *iter = root;

            while (iter != nullptr) {
                bool comp = compare(iter, __node);
                if (iter->value == __node->value) {
                    if (iter->left != nullptr && iter->right != nullptr) {
                        auto ist_node = findMin(iter->right);
                        ist_node->left = set_node(__node->value);

                        return;
                    }

                    iter = iter->left ? iter->left : iter->right;
                    iter = set_node(__node->value);

                    return;
                }
                else if (comp == 1)
                    iter = iter->right;
                else
                    iter = iter->left;
            } // while
        }
    };

}; // namespace iop

int main()
{
    iop::BinarySearchTree<int> btree;

    btree.insert(10);

    // Err
    cout << btree.findMax()->value << endl;
    cout << btree.findMin()->value << endl;

    // cout << btree.find(10)->value << endl;

    // for(int i = 0; i < 10; ++i)
    //     btree.insert(i);

    // for(int i = 0; i < 10; ++i)
    //     cout<< btree.find(i)->value << endl;

    return 0;
}