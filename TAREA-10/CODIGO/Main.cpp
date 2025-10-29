#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "FB.h"
#include "KMP.h"
#include "BM.h"
#include "cesar.h"
#include "afin.h"
#include "vigenere.h"
#include "playfair.h"
#include "hill.h"
#include "Compression.h"   // contiene huffmanCompressFile() y huffmanDecompressFile()
using namespace std;
using namespace std::chrono;

// ---------------- FUNCIONES AUXILIARES ----------------
string aMinusculas(const string& str) {
    string resultado = str;
    transform(resultado.begin(), resultado.end(), resultado.begin(),
        [](unsigned char c) { return tolower(c); });
    return resultado;
}

// ---------------- REGISTRO ----------------
void registrarUsuario() {
    string nombre, apellido, telefono, email, password;
    ofstream archivoUsuarios("usuarios.txt", ios::app);
    ofstream archivoPasswords("passwords.txt", ios::app);

    system("cls");
    cout << "=== REGISTRO DE USUARIO ===\n";
    cout << "Nombre: ";
    getline(cin, nombre);
    cout << "Apellido: ";
    getline(cin, apellido);
    cout << "Telefono: ";
    getline(cin, telefono);
    cout << "Email: ";
    getline(cin, email);
    cout << "Contrasena: ";
    getline(cin, password);

    archivoUsuarios << nombre << "," << apellido << "," << telefono << "," << email << endl;
    archivoPasswords << email << "," << password << endl;

    archivoUsuarios.close();
    archivoPasswords.close();

    cout << "\nUsuario registrado correctamente.\n";
    system("pause");
}

// ---------------- BUSCAR EMAIL CON ALGORITMO ----------------
bool buscarEmail(const string& email, string& contrasena) {
    ifstream archivo("passwords.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir passwords.txt\n";
        system("pause");
        return false;
    }

    string linea;
    vector<string> lineas;
    while (getline(archivo, linea)) lineas.push_back(linea);
    archivo.close();

    int opcion;
    cout << "\nSeleccione el algoritmo de busqueda para encontrar el email:\n";
    cout << "1) Fuerza Bruta\n";
    cout << "2) Knuth-Morris-Pratt (KMP)\n";
    cout << "3) Boyer-Moore (BM)\n";
    cout << "Opcion: ";
    cin >> opcion;
    cin.ignore();

    bool encontrado = false;
    string emailMin = aMinusculas(email);

    for (const string& registro : lineas) {
        string registroMin = aMinusculas(registro);
        vector<int> resultado;
        int comparaciones = 0;

        auto t0 = high_resolution_clock::now();
        switch (opcion) {
        case 1:
            resultado = fuerzaBrutaBusqueda(registroMin, emailMin, comparaciones);
            break;
        case 2:
            resultado = kmpBusqueda(registroMin, emailMin, comparaciones);
            break;
        case 3:
            resultado = boyerMooreBusqueda(registroMin, emailMin, comparaciones);
            break;
        default:
            cout << "Opcion no valida.\n";
            return false;
        }
        auto t1 = high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(t1 - t0);

        cout << "\n--- INFORME DEL ALGORITMO ---\n";
        cout << "Comparaciones: " << comparaciones << "\n";
        cout << "Tiempo: " << dur.count() << " microsegundos\n";
        cout << "Coincidencias: " << resultado.size() << "\n";

        if (!resultado.empty()) {
            size_t pos = registro.find(',');
            string emailGuardado = registro.substr(0, pos);
            string passGuardada = (pos != string::npos) ? registro.substr(pos + 1) : "";
            if (aMinusculas(emailGuardado) == emailMin) {
                contrasena = passGuardada;
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "No se encontró el email.\n";
        system("pause");
        return false;
    }
    return true;
}

// ---------------- INICIO DE SESION ----------------
void iniciarSesion() {
    string email, password;
    system("cls");
    cout << "=== INICIO DE SESION ===\n";
    cout << "Email: ";
    getline(cin, email);
    cout << "Contrasena: ";
    getline(cin, password);

    string contrasenaGuardada;
    if (!buscarEmail(email, contrasenaGuardada)) return;

    if (password == contrasenaGuardada)
        cout << "\nInicio de sesion exitoso.\n";
    else
        cout << "\nContrasena incorrecta.\n";
    system("pause");
}

// ---------------- CIFRADO Y GUARDADO ----------------
void cifrarYGuardar() {
    string email;
    system("cls");
    cout << "=== CIFRADO DE CONTRASENA ===\n";
    cout << "Ingrese el email para buscar su contrasena: ";
    getline(cin, email);

    string contrasena;
    if (!buscarEmail(email, contrasena)) return;

    cout << "\nContraseña encontrada: [" << contrasena << "]\n";
    system("pause");
    system("cls");

    int opcion1, opcion2, opcion3;
    string cifrada = contrasena;
    do {
        cout << "\nElija el tipo de cifrado:\n";
        cout << "1. Sustitucion\n";
        cout << "2. Poligrafico\n";
        cout << "0. Salir\n";
        cin >> opcion1;
        system("cls");

        switch (opcion1) {
        case 1:
            do {
                cout << "\n Elija el algoritmo:\n";
                cout << "1. Cesar\n2. Afin\n3. Vigenere\n0. Volver\n";
                cin >> opcion2;
                system("cls");
                switch (opcion2) {
                case 1: {
                    int d; cout << "Desplazamiento: "; cin >> d;
                    cifrada = cesar(contrasena, d);
                    break;
                }
                case 2: {
                    int a, b; cout << "a (coprimo 26): "; cin >> a;
                    cout << "b: "; cin >> b;
                    cifrada = afin(contrasena, a, b);
                    break;
                }
                case 3: {
                    string clave; cout << "Clave: "; cin >> clave;
                    cifrada = vigenere(contrasena, clave);
                    break;
                }
                }
            } while (opcion2 != 0);
            break;

        case 2:
            do {
                cout << "\n Elija el algoritmo:\n";
                cout << "1. Playfair\n2. Hill\n0. Volver\n";
                cin >> opcion3;
                system("cls");
                switch (opcion3) {
                case 1: {
                    string clave; cout << "Clave: "; cin >> clave;
                    cifrada = playfair(contrasena, clave);
                    break;
                }
                case 2: {
                    string clave; int n;
                    cout << "Clave Hill: "; cin >> clave;
                    cout << "Tamaño matriz (2 o 3): "; cin >> n;
                    auto k = generarMatrizDesdeClave(clave, n);
                    mostrarMatriz(k);
                    cifrada = cifradoHill(contrasena, k);
                    break;
                }
                }
            } while (opcion3 != 0);
            break;
        }
    } while (opcion1 != 0);

    cout << "\nContraseña cifrada: " << cifrada << endl;

    // Guardar email y contraseña cifrada
    ofstream out("passwords_encriptados.txt", ios::app);
    out << email << "," << cifrada << endl;
    out.close();
    cout << "\nGuardada en 'passwords_encriptados.txt'\n";
    system("pause");
}

// ---------------- COMPRESION ----------------
void comprimirUsuarios() {
    system("cls");
    cout << "=== COMPRESION DE ARCHIVO ===\n";
    if (huffmanCompressFile("usuarios.txt", "usuarios_comprimido.huf"))
        cout << "\nArchivo 'usuarios.txt' comprimido exitosamente.\n";
    else
        cout << "\nError al comprimir el archivo.\n";
    system("pause");
}

// ---------------- MENÚ PRINCIPAL ----------------
int main() {
    int opcion;
    do {
        system("cls");
        cout << "=== SISTEMA DE TRATAMIENTO DE CADENAS ===\n";
        cout << "1) Registrar Usuario\n";
        cout << "2) Iniciar Sesion\n";
        cout << "3) Cifrar y Guardar Contraseña\n";
        cout << "4) Comprimir archivo de usuarios\n";
        cout << "0) Salir\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1: registrarUsuario(); break;
        case 2: iniciarSesion(); break;
        case 3: cifrarYGuardar(); break;
        case 4: comprimirUsuarios(); break;
        case 0: cout << "Saliendo...\n"; break;
        default: cout << "Opcion no valida\n"; system("pause");
        }
    } while (opcion != 0);

    return 0;
}
