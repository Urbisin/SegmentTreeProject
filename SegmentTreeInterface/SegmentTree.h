#ifndef UNTITLED_SEGMENTTREE_H
#define UNTITLED_SEGMENTTREE_H

#include <vector>
#include <climits>
#include <tuple>

using namespace std;

template<typename T>
class SegmentTree {
private:
    struct Node {
        T sum;
        T min;
        T max;
        int start, end;
        Node *left, *right;

        Node(int s, int e): sum(0), min(INT_MAX), max(INT_MIN), start(s), end(e), left(nullptr), right(nullptr) {}
    };

    Node* root;
    vector<tuple<int, int, T>> tree_structure;

    Node* build(vector<T>& arr, int start, int end) {
        Node* node = new Node(start, end);

        if (start == end) {
            node->sum = arr[start];

            node->min = arr[start];

            node->max = arr[start];
        }
        else {
            int mid = start + (end - start) / 2;
            node->left = build(arr, start, mid);
            node->right = build(arr, mid + 1, end);

            node->sum = node->left->sum + node->right->sum;

            node->min = min(node->left->min, node->right->min);
            node->max = max(node->left->max, node->right->max);
        }
        tree_structure.push_back(make_tuple(node->start, node->end, node->sum));
        return node;
    }

    Node* update(Node* node, int idx, T val) {
        if (node->start == node->end) {
            node->sum = val;

            node->min = val;

            node->max = val;
        }
        else {
            int mid = node->start + (node->end - node->start) / 2;
            if (idx <= mid) {
                node->left = update(node->left, idx, val);
            }
            else {
                node->right = update(node->right, idx, val);
            }

            node->sum = node->left->sum + node->right->sum;

            node->min = min(node->left->min, node->right->min);
            node->max = max(node->left->max, node->right->max);
        }
    }



public:
    SegmentTree(vector<int>& arr) {
        root = build(arr, 0, arr.size() - 1);
    }

    void update(int idx, T val) {
        update(root, idx, val);
    }

    vector<tuple<int, int, int>> getTreeStructure() {
        return tree_structure;
    }

};


#endif //UNTITLED_SEGMENTTREE_H
