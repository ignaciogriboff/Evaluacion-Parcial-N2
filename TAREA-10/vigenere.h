#ifndef VIGENERE_H
#define VIGENERE_H
#include <iostream>
using namespace std;
string vigenere(string s, string clave) {
	string encriptado = ""; // Cadena para almacenar el texto encriptado
	int j = 0; // Indice para la clave
	for (int i = 0; i < s.length(); i++) {
		char c = s[i];
		// Cifrado solo para letras mayusculas y minusculas
		if (isalpha(c)) {
			char limite = isupper(c) ? 'A' : 'a'; // Determina si es mayuscula o minuscula
			int desplazamiento = toupper(clave[j % clave.length()]) - 'A'; // Calcula el desplazamiento basado en la clave
			c = (c - limite + desplazamiento) % 26 + limite; // Aplica el desplazamiento con wrap-around
			j++; // Incrementa el indice de la clave solo si se ha cifrado una letra
		}
		encriptado += c; // Agrega el caracter (modificado o no) a la cadena encriptada
	}
	return encriptado;
}
#endif