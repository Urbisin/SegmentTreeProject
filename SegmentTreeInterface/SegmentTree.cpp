//
// Created by Sebastian Urbina on 1/07/24.
//

#include "SegmentTree.h"
#include <algorithm>
#include <cmath>
#include <climits>

template<typename T>
SegmentTree<T>::Node::Node(int s, int e) : sum(0), maxVal(INT_MIN), minVal(INT_MAX), start(s), end(e), left(nullptr), right(nullptr) {}

template<typename T>
typename SegmentTree<T>::Node* SegmentTree<T>::build(std::vector<T>& arr, int start, int end, int pos) {
    if (pos >= tree_structure.size()) {
        tree_structure.resize(pos + 1, std::make_tuple(-1, -1, T(), T(), T()));
    }

    Node* node = new Node(start, end);

    if (start == end) {
        node->sum = arr[start];
        node->maxVal = arr[start];
        node->minVal = arr[start];
    } else {
        int mid = start + (end - start) / 2;
        node->left = build(arr, start, mid, 2 * pos + 1);
        node->right = build(arr, mid + 1, end, 2 * pos + 2);

        node->sum = node->left->sum + node->right->sum;
        node->maxVal = std::max(node->left->maxVal, node->right->maxVal);
        node->minVal = std::min(node->left->minVal, node->right->minVal);
    }
    tree_structure[pos] = std::make_tuple(node->start, node->end, node->sum, node->maxVal, node->minVal);
    return node;
}

template<typename T>
typename SegmentTree<T>::Node* SegmentTree<T>::update(Node* node, int idx, T val, int pos) {
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
        node->maxVal = std::max(node->left->maxVal, node->right->maxVal);
        node->minVal = std::min(node->left->minVal, node->right->minVal);
    }
    tree_structure[pos] = std::make_tuple(node->start, node->end, node->sum, node->maxVal, node->minVal);
    return node;
}

template<typename T>
SegmentTree<T>::SegmentTree(std::vector<int>& arr) {
    tree_structure.clear();
    int maxSize = 2 * std::pow(2, std::ceil(std::log2(arr.size()))) - 1;
    tree_structure.reserve(maxSize);
    root = build(arr, 0, arr.size() - 1, 0);
}

template<typename T>
void SegmentTree<T>::update(int idx, T val) {
    update(root, idx, val, 0);
}

template<typename T>
std::vector<std::tuple<int, int, T, T, T>> SegmentTree<T>::getTreeStructure() {
    std::vector<std::tuple<int, int, T, T, T>> valid_tree_structure;
    for (const auto& node : tree_structure) {
        if (std::get<0>(node) != -1) {
            valid_tree_structure.push_back(node);
        }
    }
    return valid_tree_structure;
}

