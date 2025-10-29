#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <Windows.h>
#include <limits>
#include "Convert.h"
#include "Prim.h"

using namespace std;

class Graph {
private:
    int Nodes;
	int Edges;
    vector<vector<int>> adj;
public:
	Graph(int n) { Nodes = n; adj.resize(n); Edges = 0; }
    void loadGraphManual(vector<vector<int>>& graph, vector<vector<pair<int, int>>>& weightedGraph, bool weighted) {
        int nNodes, nEdges, directedInput;
        cout << "Cantidad de nodos: "; cin >> nNodes; //Leer cantidad de nodos
        cout << "Cantidad de aristas: "; cin >> nEdges; //Leer cantidad de aristas
        cout << "El grafo es dirigido? (1 = Si, 0 = No): ";
        cin >> directedInput;
        bool directed = (directedInput == 1);

        graph.assign(nNodes, vector<int>()); //Inicializar grafo BFS
        weightedGraph.assign(nNodes, vector<pair<int, int>>()); //Inicializar grafo ponderado

        for (int i = 0; i < nEdges; i++) {
            char uNode, vNode; int weight;
            cout << "Arista " << i + 1 << " (formato: u v w): ";
            cin >> uNode >> vNode >> weight;

            int u = nodeToIndex(uNode); //Convertir nodo a índice
            int v = nodeToIndex(vNode); //Convertir nodo a índice

            graph[u].push_back(v); //Agregar arista para BFS
            if (!directed) graph[v].push_back(u); //Grafo no dirigido

            if (weighted) {
                weightedGraph[u].push_back({ v, weight }); //Agregar arista ponderada
                if (!directed) weightedGraph[v].push_back({ u, weight }); //Agregar arista inversa
            }
        }
    }
};
class Heap {
private:
    vector<int> data;   // Vector que representa el montículo
    bool isMaxHeap;     // true = maxHeap, false = minHeap

    // Comparador según el tipo de heap
    bool compare(int a, int b) {
        return isMaxHeap ? (a > b) : (a < b);
    }

    // Mantener la propiedad del heap hacia abajo
    void heapifyDown(int n, int i) {
        int extreme = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && compare(data[left], data[extreme])) {
            extreme = left;
        }
        if (right < n && compare(data[right], data[extreme])) {
            extreme = right;
        }

        if (extreme != i) {
            swap(data[i], data[extreme]);
            heapifyDown(n, extreme);
        }
    }

    // Mantener la propiedad del heap hacia arriba
    void heapifyUp(int i) {
        if (i == 0) return;
        int parent = (i - 1) / 2;

        if (compare(data[i], data[parent])) {
            swap(data[i], data[parent]);
            heapifyUp(parent);
        }
    }
    void InteractivePause() {
        cout << "Presione Enter para continuar...";
	    cin.ignore();
    }
    void PrintStep(int step, int heapSize, const string& msg) {
		cout << "Paso " << step << " (" << msg << ") [ ";
        for (int i = 0; i < data.size(); i++) {
            if (i == heapSize) cout << "| "; // Separador entre heap y parte ordenada
			cout << data[i] << " ";
        }
		cout << "]\n";
		InteractivePause();
    }

public:
    Heap(bool isMax = false) : isMaxHeap(isMax) {} // por defecto MinHeap

    void insertar(int val) {
        data.push_back(val);
        heapifyUp(data.size() - 1);
    }

    int raiz() {
        if (data.empty()) throw runtime_error("Heap vacio");
        return data[0];
    }

    int extraerRaiz() {
        if (data.empty()) throw runtime_error("Heap vacio");
        int root = data[0];
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) heapifyDown(data.size(), 0);
        return root;
    }

    void HeapSort() {
		int n = data.size();
        int Step = 1;
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyDown(n, i);
			PrintStep(Step++, n, "heapify en indice " + to_string(i));
        }
		cout << "Heap inicial construido:\n";
        for (int i = n - 1; i >= 0; i--) {
			swap(data[0], data[i]);
			PrintStep(Step++, i, "swap raiz con indice " + to_string(i));
			heapifyDown(i, 0);
			PrintStep(Step++, i, "Heapify con heapSize= " + to_string(i));
        }
		cout << "HeapSort completado.\n";
    }

    void printHeap() {
        for (int val : data) cout << val << " ";
        cout << endl;
    }

    bool empty() const {
        return data.empty();
    }
};

// ======================= MAIN ============================
int main() {
    cout << "========== Sistema de Lavadero con Prioridad ==========\n";

    int tipo;
    cout << "Seleccione el tipo de Heap:\n";
    cout << "1. Min-Heap (IDs mas bajos tienen prioridad)\n";
    cout << "2. Max-Heap (IDs mas altos tienen prioridad)\n";
    cin >> tipo;

    Heap lavadero(tipo == 2); // true = maxHeap, false = minHeap

    int n;
    cout << "\nIngrese la cantidad de autos a registrar: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int id;
        cout << "Ingrese el ID del auto/cliente " << i + 1 << ": ";
        cin >> id;
        lavadero.insertar(id);
    }

    cout << "\nEstado inicial del lavadero (heap con IDs):\n";
    lavadero.printHeap();

    char opcion;
    do {
       
        cout << "\nOpciones:\n";
        cout << "1. Insertar nuevo auto\n";
        cout << "2. Atender (extraer raiz)\n";
        cout << "3. Mostrar heap actual\n";
		cout << "4. Realizar ordenamiento HeapSort\n";
        cout << "5. Salir\n";
        cout << "Seleccione: ";
        cin >> opcion;

        if (opcion == '1') {
            system("cls");
            int nuevoId;
            cout << "Ingrese el ID del nuevo auto: ";
            cin >> nuevoId;
            lavadero.insertar(nuevoId);
        }
        else if (opcion == '2') {
            system("cls");
            if (!lavadero.empty()) {
                int cliente = lavadero.extraerRaiz();
                cout << "Atendido cliente con ID: " << cliente << endl;
            }
            else {
                cout << "No hay autos en espera.\n";
            }
        }
        else if (opcion == '3') {
            system("cls");
            cout << "Heap actual: ";
            lavadero.printHeap();
        }
        else if (opcion == '4') {
            system("cls");
            cout << "Heap Sort: ";
			lavadero.HeapSort();
            lavadero.printHeap();
        }
    } while (opcion != '5');
    system("cls");
    cout << "\nGracias por usar el sistema de lavadero.\n";
    return 0;
}