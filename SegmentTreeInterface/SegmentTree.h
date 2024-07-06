#ifndef UNTITLED_SEGMENTTREE_H
#define UNTITLED_SEGMENTTREE_H

#include <vector>
#include <climits>
#include <tuple>
#include <cmath>

using namespace std;

template<typename T>
class SegmentTree {
private:
    struct Node {
        T sum;
        T maxVal;
        T minVal;
        int start, end;
        Node *left, *right;

        Node(int s, int e) : sum(0), maxVal(INT_MIN), minVal(INT_MAX), start(s), end(e), left(nullptr), right(nullptr) {}
    };

    Node* root;
    vector<tuple<int, int, T, T, T>> tree_structure;

    Node* build(vector<T>& arr, int start, int end, int pos) {
        if (pos >= tree_structure.size()) {
            tree_structure.resize(pos + 1, make_tuple(-1, -1, T(), T(), T()));
        }

        Node* node = new Node(start, end);

        if (start == end) {
            node->sum = arr[start];
            node->maxVal = arr[start];
            node->minVal = arr[start];
        }
        else {
            int mid = start + (end - start) / 2;
            node->left = build(arr, start, mid, 2 * pos + 1);
            node->right = build(arr, mid + 1, end, 2 * pos + 2);

            node->sum = node->left->sum + node->right->sum;
            node->maxVal = max(node->left->maxVal, node->right->maxVal);
            node->minVal = min(node->left->minVal, node->right->minVal);
        }
        tree_structure[pos] = make_tuple(node->start, node->end, node->sum, node->maxVal, node->minVal);
        return node;
    }

    Node* update(Node* node, int idx, T val, int pos) {
        if (node->start == node->end) {
            node->sum = val;
            node->maxVal = val;
            node->minVal = val;
        } else {
            int mid = node->start + (node->end - node->start) / 2;
            if (idx <= mid) {
                node->left = update(node->left, idx, val, 2 * pos + 1);
            } else {
                node->right = update(node->right, idx, val, 2 * pos + 2);
            }

            node->sum = node->left->sum + node->right->sum;
            node->maxVal = max(node->left->maxVal, node->right->maxVal);
            node->minVal = min(node->left->minVal, node->right->minVal);
        }
        tree_structure[pos] = make_tuple(node->start, node->end, node->sum, node->maxVal, node->minVal);
        return node;
    }

public:
    SegmentTree(vector<int>& arr) {
        tree_structure.clear();
        int maxSize = 2 * pow(2, ceil(log2(arr.size()))) - 1;
        tree_structure.reserve(maxSize);
        root = build(arr, 0, arr.size() - 1, 0);
    }

    void update(int idx, T val) {
        update(root, idx, val, 0);
    }

    vector<tuple<int, int, T, T, T>> getTreeStructure() {
        vector<tuple<int, int, T, T, T>> valid_tree_structure;
        for (const auto& node : tree_structure) {
            if (get<0>(node) != -1) {
                valid_tree_structure.push_back(node);
            }
        }
        return valid_tree_structure;
    }
};

#endif // UNTITLED_SEGMENTTREE_H
