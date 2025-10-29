#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

inline std::unordered_map<char, int> crearTablaMalaCaracterizacion(const std::string& patron) {
    std::unordered_map<char, int> tabla;
    int m = patron.size();
    for (int i = 0; i < m - 1; i++)
        tabla[patron[i]] = m - i - 1;
    return tabla;
}

inline std::vector<int> crearTablaBuenosSufijos(const std::string& patron) {
    int m = patron.size();
    std::vector<int> tabla(m, m);
    std::vector<int> z(m, 0);
    for (int i = m - 2, l = m - 1, r = m - 1; i >= 0; i--) {
        if (i > l) z[i] = std::min(r - i + 1, z[m - r + i - 1]);
        while (i - z[i] >= 0 && patron[m - z[i] - 1] == patron[i - z[i]]) ++z[i];
        if (i - z[i] < l) l = i - z[i], r = i;
    }
    for (int i = m - 2; i >= 0; i--) tabla[m - z[i] - 1] = i - z[i] + 1;
    return tabla;
}

inline std::vector<int> boyerMooreBusqueda(const std::string& texto, const std::string& patron, int& comparaciones) {
    std::vector<int> posiciones;
    comparaciones = 0;
    int n = texto.size(), m = patron.size();
    if (m == 0 || n == 0 || m > n) return posiciones;

    auto tablaMala = crearTablaMalaCaracterizacion(patron);
    auto tablaSufijos = crearTablaBuenosSufijos(patron);

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0) {
            comparaciones++; // cada comparación patron[j] vs texto[i+j]
            if (patron[j] == texto[i + j]) { j--; continue; }
            else break;
        }
        if (j < 0) {
            posiciones.push_back(i);
            i += tablaSufijos[0];
        }
        else {
            int saltoMalo = (tablaMala.count(texto[i + j]) > 0) ? tablaMala[texto[i + j]] : m;
            i += std::max(saltoMalo, tablaSufijos[j]);
        }
    }
    return posiciones;
}