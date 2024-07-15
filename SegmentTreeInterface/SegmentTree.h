#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

#include <vector>
#include <tuple>

enum Type{
    MIN_QUERY, MAX_QUERY, SUM_QUERY
};

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
    int rangeMinimumQuery(int start, int end, Node* node);
    int rangeMaximumQuery(int start, int end, Node* node);
    int rangeSumQuery(int start, int end, Node* node);

public:
    SegmentTree(std::vector<int>& arr);
    void update(int idx, T val);
    int rangeQuery(int start, int end, Type type);
    std::vector<std::tuple<int, int, T, T, T>> getTreeStructure();
};

#endif // SEGMENTTREE_H
