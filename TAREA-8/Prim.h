#pragma once
#ifndef PRIM_H
#define PRIM_H
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include "Convert.h"
using namespace std;
//============================================ Prim: MST (costo total mínimo para concectar todos los nodos) ========================================
struct EdgePrim {
    int u, v;
    ll w;
};
//Prim sobre weightedGraph (vector<vector<pair<int,int>>>). Devuelve {peso_total, lista_aristas}
pair<ll, vector<EdgePrim>> prim(const vector<vector<pair<int, int>>>& g, int start = 0) {
    int n = (int)g.size();
    vector<bool> visited(n, false);
    vector<EdgePrim> mst;
    ll total = 0;

    using State = tuple<int, int, int>; // (peso, nodo, padre)
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push(State(0, start, -1));

    while (!pq.empty() && mst.size() < static_cast<size_t>(n > 0 ? n - 1 : 0)) {
        int w; int u; int p;
        tie(w, u, p) = pq.top(); pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        if (p != -1) {
            mst.push_back({ p, u, static_cast<ll>(w) });
            total += w;
        }
        for (const auto& vw : g[u]) {
            int v = vw.first; int w2 = vw.second;
            if (!visited[v]) pq.emplace(w2, v, u);
        }
    }

    return { total, mst };
}
#endif