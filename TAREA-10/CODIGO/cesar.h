#ifndef CESAR_H
#define CESAR_H
#include <iostream>
using namespace std;
string cesar(string s, int desplazamiento) {
	string encriptado = ""; // Cadena para almacenar el texto encriptado
	for (int i = 0; i < s.length(); i++) {
		char c = s[i];
		// Cifrado solo para letras mayusculas y minusculas
		if (isalpha(c)) {
			char limite = isupper(c) ? 'A' : 'a'; // Determina si es mayuscula o minuscula
			c = (c - limite + desplazamiento) % 26 + limite; // Aplica el desplazamiento con wrap-around
		}
		encriptado += c; // Agrega el caracter (modificado o no) a la cadena encriptada
	}
	return encriptado;
}
#endif