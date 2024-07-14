#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

#include <vector>
#include <tuple>

template<typename T>
class SegmentTree {
private:
    struct Node {
        T sum;
        T maxVal;
        T minVal;
        int start, end;
        Node *left, *right;

        Node(int s, int e);
    };

    Node* root;
    std::vector<std::tuple<int, int, T, T, T>> tree_structure;

    Node* build(std::vector<T>& arr, int start, int end, int pos);
    Node* update(Node* node, int idx, T val, int pos);

public:
    SegmentTree(std::vector<int>& arr);
    void update(int idx, T val);
    std::vector<std::tuple<int, int, T, T, T>> getTreeStructure();
};

#endif // SEGMENTTREE_H
