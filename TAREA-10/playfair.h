#ifndef PLAYFAIR_h
#define PLAYFAIR_h
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;
// Elimina letras repetidas y fusiona 'J' con 'I'
string prepararClave(string clave) {
    string resultado;
    vector<bool> usado(26, false);

    for (char& c : clave) {
        c = toupper(c);
        if (c == 'J') c = 'I';
        int pos = c - 'A';
        if (!usado[pos] && isalpha(c)) {
            usado[pos] = true;
            resultado += c;
        }
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue; // se omite la J
        if (!usado[c - 'A']) {
            resultado += c;
        }
    }
    return resultado;
}
// Crea la matriz 5x5 para el cifrado Playfair
vector<vector<char>> crearMatriz(const string& clave) {
    vector<vector<char>> matriz(5, vector<char>(5));
    for (int i = 0; i < 25; i++) {
        matriz[i / 5][i % 5] = clave[i];
    }
    return matriz;
}
// Buscar posicion de una letra en la matriz
pair<int, int> buscarPos(const vector<vector<char>>& matriz, char c) {
    if (c == 'J') c = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matriz[i][j] == c) return { i, j };
        }
    }
    return { -1, -1 };
}
// Prepara el texto eliminando espacios, duplicados y agregando x
string prepararTexto(string texto) {
    string res;
    for (char c : texto) {
        if (isalpha(c)) {
            res += toupper(c);
        }
    }

    string preparado;
    for (int i = 0; i < res.size(); i++) {
        preparado += res[i];
        if (i + 1 < res.size() && res[i] == res[i + 1])
            preparado += 'X';
    }
    if (preparado.size() % 2 != 0)
        preparado += 'X';
    return preparado;
}
// Cifra el texto usando la matriz Playfair
string playfair(string texto, string clave) {
    string clavePreparada = prepararClave(clave);
    auto matriz = crearMatriz(clavePreparada);
    string textoPreparado = prepararTexto(texto);
    string resultado;

    for (int i = 0; i < textoPreparado.size(); i += 2) {
        char a = textoPreparado[i];
        char b = textoPreparado[i + 1];
        std::pair<int, int> posA = buscarPos(matriz, a);
        std::pair<int, int> posB = buscarPos(matriz, b);

        int filaA = posA.first;
        int colA = posA.second;
        int filaB = posB.first;
        int colB = posB.second;


        if (filaA == filaB) {
            // misma fila ? siguiente letra
            resultado += matriz[filaA][(colA + 1) % 5];
            resultado += matriz[filaB][(colB + 1) % 5];
        }
        else if (colA == colB) {
            // misma columna ? letra debajo
            resultado += matriz[(filaA + 1) % 5][colA];
            resultado += matriz[(filaB + 1) % 5][colB];
        }
        else {
            // rectángulo ? intercambiar columnas
            resultado += matriz[filaA][colB];
            resultado += matriz[filaB][colA];
        }
    }
    return resultado;
}
#endif
