#include "../../container/rb_treeF.h"
#include "../../meta/functionalF.h"

#include<iostream>
using namespace std;


int main(int argc, char* argv[]) {
    using RBTree = iop::rbtree<int, int, iop::identity<int>, iop::less<int, int>>;
    RBTree tree;

    tree.insert_unique(10);
    tree.insert_unique(10);
    tree.insert_unique(20);
    tree.insert_unique(20);
    tree.insert_unique(30);
    tree.insert_unique(30);

    cout << tree << endl;

    // 插入元素
    tree.insert_equal(10);
    tree.insert_equal(30);

    cout << "\n\n\n";


    tree.insert_equal(50);
    tree.insert_equal(20);
    tree.insert_equal(40);

    RBTree etree;
    int i = 0;
    while (i != 10) {
        ++i;
        etree.insert_equal(i);
    }
    tree.insert_equal(etree.begin(), --etree.end());


    for (auto i = etree.begin();i != etree.end();++i)
        cout << *i << " ";
    cout << endl;


    // 遍历输出红黑树的元素
    std::cout << "Elements in the rb_tree: ";

    for (auto it = tree.begin(); it != tree.end();++it) {
        cout << *it << ",";
    }
    cout << endl;
    std::cout << std::endl;

    RBTree ls_tree = { 3,23,12,31,4 };
    for (auto it = ls_tree.begin();it != ls_tree.end();++it)
        cout << *it << ",";
    cout << endl;

    RBTree copy_tree(ls_tree.begin(), --(--ls_tree.end()));

    for (auto it = copy_tree.begin();it != copy_tree.end();++it)
        cout << *it << ",";
    cout << endl;

    return 0;
}