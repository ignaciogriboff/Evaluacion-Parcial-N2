#ifndef AFIN_H
#define AFIN_H
#include <iostream>
using namespace std;
string afin(string s, int a, int b) {
	string encriptado = ""; // Cadena para almacenar el texto encriptado
	for (int i = 0; i < s.length(); i++) {
		char c = s[i];
		// Cifrado solo para letras mayusculas y minusculas
		if (isalpha(c)) {
			char limite = isupper(c) ? 'A' : 'a'; // Determina si es mayuscula o minuscula
			c = (a * (c - limite) + b) % 26 + limite; // Aplica la formula del cifrado afin
		}
		encriptado += c; // Agrega el caracter (modificado o no) a la cadena encriptada
	}
	return encriptado;
}
#endif
