#pragma once
#ifndef CONVERT_H
#define CONVERT_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>
using namespace std;

static unordered_map<char, int> nodeToIdx;  // Mapeo nodo -> índice
static vector<char> idxToNode;              // Mapeo índice -> nodo

inline int nodeToIndex(char ch) {
    if (nodeToIdx.count(ch)) return nodeToIdx[ch];
    int idx = nodeToIdx.size();
    nodeToIdx[ch] = idx;
    idxToNode.push_back(ch);
    return idx;
}

inline char indexToNode(int x) {
    if (x >= 0 && x < (int)idxToNode.size()) return idxToNode[x];
    return '?'; // nodo desconocido
}

using ll = long long;
const ll INF = 1e9;

#endif
