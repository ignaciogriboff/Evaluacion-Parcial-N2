#pragma once
#include <vector>
#include <string>

inline std::vector<int> calcularArregloPrefijos(const std::string& patron) {
    int m = patron.size();
    std::vector<int> pi(m, 0);
    int k = 0;
    for (int i = 1; i < m; ++i) {
        while (k > 0 && patron[k] != patron[i])
            k = pi[k - 1];
        if (patron[k] == patron[i])
            ++k;
        pi[i] = k;
    }
    return pi;
}

inline std::vector<int> kmpBusqueda(const std::string& texto, const std::string& patron, int& comparaciones) {
    std::vector<int> posiciones;
    comparaciones = 0;
    if (patron.empty() || texto.empty() || patron.size() > texto.size()) return posiciones;

    std::vector<int> pi = calcularArregloPrefijos(patron);
    int n = texto.size(), m = patron.size(), k = 0;

    for (int i = 0; i < n; i++) {
        while (k > 0) {
            comparaciones++; // comparación patron[k] vs texto[i] en el while
            if (patron[k] == texto[i]) break;
            k = pi[k - 1];
        }
        comparaciones++; // comparación final (o primera) patron[k] == texto[i]
        if (patron[k] == texto[i]) k++;
        if (k == m) {
            posiciones.push_back(i - m + 1);
            k = pi[k - 1];
        }
    }
    return posiciones;
}