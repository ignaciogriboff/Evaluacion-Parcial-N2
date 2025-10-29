#ifndef HILL_H
#define HILL_H
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

// Convierte letra (A-Z o a-z) en número (A=0 ... Z=25)
int letraANum(char c) {
    return toupper(c) - 'A';
}

// Convierte número (0...25) en letra mayúscula
char numALetra(int n) {
    n = (n % 26 + 26) % 26;  // asegura que el resultado esté entre 0 y 25
    return 'A' + n;
}

// Genera la matriz K a partir de la palabra clave
// Crea una matriz cuadrada n×n a partir de la clave (rellena si es más corta)
vector<vector<int>> generarMatrizDesdeClave(const string& clave, int n) {
    vector<vector<int>> K(n, vector<int>(n));
    int len = clave.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char c = clave[(i * n + j) % len];
            K[i][j] = letraANum(c);  // convierte según A=0
        }
    }
    return K;
}

// Muestra la matriz
void mostrarMatriz(const vector<vector<int>>& K) {
    for (auto& fila : K) {
        for (auto val : fila)
            cout << val << "\t";
        cout << endl;
    }
}

// Cifra un bloque de tamaño n usando Hill
string cifrarBloque(const vector<vector<int>>& K, const string& bloque) {
    int n = K.size();
    vector<int> P(n);

    for (int i = 0; i < n; i++)
        P[i] = letraANum(bloque[i]);

    vector<int> C(n, 0);

    // Multiplicación matricial: C = K * P mod 26
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            C[i] += K[i][j] * P[j];
        C[i] = (C[i] % 26 + 26) % 26;  // garantiza 0 ? C[i] ? 25
    }

    string resultado = "";
    for (int i = 0; i < n; i++)
        resultado += numALetra(C[i]);

    return resultado;
}

// Cifra el texto completo con Hill
string cifradoHill(string texto, const vector<vector<int>>& K) {
    int n = K.size();
    string limpio = "";

    // Elimina espacios y convierte a mayúsculas
    for (char c : texto)
        if (isalpha(c))
            limpio += toupper(c);

    // Si la longitud no es múltiplo de n, se rellena con 'X'
    while (limpio.size() % n != 0)
        limpio += 'X';

    string cifrado = "";
    for (size_t i = 0; i < limpio.size(); i += n) {
        string bloque = limpio.substr(i, n);
        cifrado += cifrarBloque(K, bloque);
    }

    return cifrado;
}

#endif
