#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include "Compression.h"
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left, * right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};
struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void generateCodes(Node* root, string str, unordered_map<char, string>& codes) {
    if (!root) return;
    if (!root->left && !root->right)
        codes[root->ch] = str;
    generateCodes(root->left, str + "0", codes);
    generateCodes(root->right, str + "1", codes);
}

string compress(const string& text, Node*& rootOut) {
    unordered_map<char, int> freq;
    for (char ch : text) freq[ch]++;

    priority_queue<Node*, vector<Node*>, compare> pq;
    for (auto& p : freq) pq.push(new Node(p.first, p.second));

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* internal = new Node('\0', left->freq + right->freq);
        internal->left = left;
        internal->right = right;
        pq.push(internal);
    }

    Node* root = pq.top();
    rootOut = root;

    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    string compressed;
    for (char ch : text)
        compressed += codes[ch];

    return compressed;
}

void DecompressHelper(Node* root, int& index, const string& compressed, string& result) {
    if (!root) return;
    if (!root->left && !root->right) {
        result += root->ch;
        return;
    }
    index++;
    if (index < (int)compressed.size()) {
        if (compressed[index] == '0')
            DecompressHelper(root->left, index, compressed, result);
        else
            DecompressHelper(root->right, index, compressed, result);
    }
}

string decompress(Node* root, const string& compressed) {
    string result;
    int index = -1;
    while (index < (int)compressed.size() - 1)
        DecompressHelper(root, index, compressed, result);
    return result;
}
bool huffmanCompressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in.is_open()) {
        cerr << "Error: no se pudo abrir " << inputFile << endl;
        return false;
    }

    // Leer todo el archivo
    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    if (text.empty()) {
        cerr << "El archivo está vacío.\n";
        return false;
    }

    Node* root = nullptr;
    string compressed = compress(text, root);

    // Guardar en formato textual '0' y '1' (más simple para tu proyecto)
    ofstream out(outputFile, ios::binary);
    if (!out.is_open()) {
        cerr << "Error: no se pudo crear " << outputFile << endl;
        return false;
    }

    // Guardamos primero la longitud original, para referencia
    out << text.size() << "\n";
    out << compressed;
    out.close();

    cout << "Archivo comprimido correctamente -> " << outputFile << endl;
    cout << "Tamaño original: " << text.size() << " bytes\n";
    cout << "Tamaño comprimido: " << compressed.size() / 8 << " bytes (aprox)\n";
    return true;
}

bool huffmanDecompressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in.is_open()) {
        cerr << "Error: no se pudo abrir " << inputFile << endl;
        return false;
    }

    size_t originalSize = 0;
    in >> originalSize; // leemos la primera línea
    in.ignore();        // saltar el salto de línea

    string compressed((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    if (compressed.empty()) {
        cerr << "El archivo comprimido está vacío.\n";
        return false;
    }
}