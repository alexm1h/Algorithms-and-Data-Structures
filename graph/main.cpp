#include "graph.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

void testSmallGraph() {
    cout << "=== TEST 1: Acyclic Graph (from textbook) ===\n";
    Graph g(4);

    g.addEdge(0, 1, 2.5);
    g.addEdge(1, 2, 1.0);
    g.addEdge(2, 3, 3.0);
    g.addEdge(0, 3, 6.0);

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();

        // Демонстрация кратчайших путей
        cout << "\nExample shortest paths:\n";
        g.printPath(0, 3); cout << endl;
        g.printPath(0, 2); cout << endl;
    }
    cout << "Test 1 completed.\n";
}

void testGraphWithNegativeEdges() {
    cout << "\n=== TEST 2: Graph with Negative Weights ===\n";
    Graph g(4);

    // Граф с отрицательными ребрами, но без отрицательных циклов
    g.addEdge(0, 1, 1.0);
    g.addEdge(0, 2, 4.0);
    g.addEdge(1, 2, 2.0);
    g.addEdge(1, 3, 6.0);
    g.addEdge(2, 3, -3.0);
    g.addEdge(3, 1, 2.0);

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();

        cout << "\nExample shortest paths:\n";
        g.printPath(0, 3); cout << endl;
        g.printPath(0, 1); cout << endl;
    } else {
        cout << "Graph contains a negative cycle!\n";
    }
    cout << "Test 2 completed.\n";
}

void testComplexGraph() {
    cout << "\n=== TEST 3: Complex Graph with Multiple Alternative Paths ===\n";
    Graph g(6);

    // Граф со многими альтернативными путями
    g.addEdge(0, 1, 4.0);
    g.addEdge(0, 2, 2.0);
    g.addEdge(1, 2, 1.0);
    g.addEdge(1, 3, 5.0);
    g.addEdge(2, 1, 3.0);
    g.addEdge(2, 3, 8.0);
    g.addEdge(2, 4, 10.0);
    g.addEdge(3, 4, 2.0);
    g.addEdge(4, 5, 3.0);
    g.addEdge(3, 5, 7.0);
    g.addEdge(1, 5, 12.0);
    g.addEdge(0, 5, 15.0);
    g.addEdge(5, 0, 6.0);

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();

        cout << "\nKey shortest paths with alternatives:\n";
        cout << "From 0 to 5: "; g.printPath(0, 5); cout << endl;
        cout << "From 0 to 4: "; g.printPath(0, 4); cout << endl;
        cout << "From 2 to 5: "; g.printPath(2, 5); cout << endl;
    }
    cout << "Test 3 completed.\n";
}

void testGraphWithNegativeWeightsNoCycle() {
    cout << "\n=== TEST 4: Graph with Negative Weights (No Negative Cycle) ===\n";
    Graph g(5);

    // Граф с отрицательными весами, но без отрицательных циклов
    g.addEdge(0, 1, 3.0);
    g.addEdge(0, 2, 8.0);
    g.addEdge(0, 4, -4.0);
    g.addEdge(1, 3, 1.0);
    g.addEdge(1, 4, 7.0);
    g.addEdge(2, 1, 4.0);
    g.addEdge(3, 0, 2.0);
    g.addEdge(3, 2, -5.0);
    g.addEdge(4, 3, 6.0);

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();

        cout << "\nExample paths with negative weights:\n";
        cout << "From 0 to 2: "; g.printPath(0, 2); cout << endl;
        cout << "From 0 to 3: "; g.printPath(0, 3); cout << endl;
        cout << "From 3 to 1: "; g.printPath(3, 1); cout << endl;
    }
    cout << "Test 4 completed.\n";
}

void testNegativeCycleComplex() {
    cout << "\n=== TEST 5: Graph with Negative Cycle (Complex Case) ===\n";
    Graph g(4);

    // Граф с отрицательным циклом 1→2→3→1
    g.addEdge(0, 1, 1.0);
    g.addEdge(1, 2, 2.0);
    g.addEdge(2, 3, -5.0);
    g.addEdge(3, 1, 1.0);  // Цикл 1→2→3→1 сумма = -2
    g.addEdge(3, 0, 4.0);
    g.addEdge(0, 3, 2.0);

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();
    } else {
        g.printDistanceMatrix();
    }

    cout << "\nChecking reachability through negative cycle:\n";
    cout << "From 0 to 2: "; g.printPath(0, 2); cout << endl;
    cout << "From 0 to 1: "; g.printPath(0, 1); cout << endl;
    cout << "From 1 to 3: "; g.printPath(1, 3); cout << endl;

    cout << "Test 5 completed.\n";
}

void testDenseGraph() {
    cout << "\n=== TEST 6: Dense Graph (10 vertices, many edges) ===\n";

    srand(42); // Фиксированный seed для воспроизводимости
    Graph g(10);

    // Создаем плотный граф
    int edgesAdded = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (i != j && rand() % 3 == 0) { // Примерно 1/3 возможных ребер
                double weight = (rand() % 100 - 20) / 10.0; // Веса от -2.0 до 8.0
                g.addEdge(i, j, weight);
                edgesAdded++;
            }
        }
    }

    cout << "Created dense graph with " << edgesAdded << " edges\n";

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();

        cout << "\nSample paths in dense graph:\n";
        g.printPath(0, 9); cout << endl;
        g.printPath(3, 7); cout << endl;
        g.printPath(5, 1); cout << endl;
    } else {
        cout << "Graph contains a negative cycle!\n";
    }
    cout << "Test 6 completed.\n";
}

void manualInput() {
    cout << "\n=== MANUAL INPUT ===\n";

    int vertices;
    cout << "Enter number of vertices: ";
    cin >> vertices;

    Graph g(vertices);

    cout << "Enter edges (format: from to weight):\n";
    cout << "Enter any non-numeric value to finish\n";

    int u, v;
    double w;
    while (true) {
        cout << "Edge: ";
        if (!(cin >> u >> v >> w)) break;
        if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
            cout << "Invalid vertex numbers! Must be 0.." << vertices-1 << endl;
            continue;
        }
        g.addEdge(u, v, w);
    }
    cin.clear(); // Сброс флагов ошибок ввода

    if (g.computeAllPairsShortestPaths()) {
        g.printDistanceMatrix();

        // Возможность просмотра путей
        char choice;
        do {
            cout << "\nShow path between vertices? (y/n): ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                int i, j;
                cout << "Enter start and end vertices: ";
                cin >> i >> j;
                if (i >= 0 && i < vertices && j >= 0 && j < vertices) {
                    g.printPath(i, j);
                    cout << endl;
                } else {
                    cout << "Invalid vertex numbers!\n";
                }
            }
        } while (choice == 'y' || choice == 'Y');
    } else {
        cout << "Graph contains a negative cycle!\n";
    }
}

int main() {
    cout << "=== ALL-PAIRS SHORTEST PATHS CALCULATOR ===\n";
    cout << "Floyd-Warshall algorithm with negative cycle detection\n";
    cout << "======================================================\n";

    // Выполнение всех тестов
    testSmallGraph();
    testGraphWithNegativeEdges();
    testComplexGraph();
    testGraphWithNegativeWeightsNoCycle();
    testNegativeCycleComplex();
    testDenseGraph();
    manualInput();

    cout << "\nAll tests completed.\n";
    return 0;
}
