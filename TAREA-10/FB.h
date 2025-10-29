#pragma once
#include <string>
#include <vector>

inline std::vector<int> fuerzaBrutaBusqueda(const std::string& texto, const std::string& patron, int& comparaciones) {
    std::vector<int> posiciones;
    comparaciones = 0;
    int n = texto.size();
    int m = patron.size();
    if (m == 0 || n == 0 || m > n) return posiciones;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m) {
            comparaciones++; // cada comparación de caracteres
            if (texto[i + j] != patron[j]) break;
            j++;
        }
        if (j == m) posiciones.push_back(i);
    }
    return posiciones;
}