#include "graph.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stack>

using namespace std;

// Используем стандартные бесконечности из numeric_limits
const double INF = numeric_limits<double>::infinity();
const double NEG_INF = -numeric_limits<double>::infinity();

// Конструктор графа
Graph::Graph(int V) : numVertices(V), hasNegativeCycle(false) {
    // Инициализация матрицы расстояний согласно методичке
    distMatrix.resize(V, vector<double>(V, INF));
    // Инициализация матрицы предшественников
    predMatrix.resize(V, vector<int>(V, -1));

    // dii0 = 0, pii = i (инициализация по методичке)
    for (int i = 0; i < V; ++i) {
        distMatrix[i][i] = 0; // Расстояние от вершины к самой себе = 0
        predMatrix[i][i] = i; // Предшественник самой себе
    }
}

// Добавление ребра в граф
void Graph::addEdge(int u, int v, double weight) {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
        distMatrix[u][v] = weight;
        predMatrix[u][v] = u; // Изначально предшественник - начальная вершина
    }
}

// Алгоритм Флойда-Уоршелла с обнаружением отрицательных циклов
bool Graph::computeAllPairsShortestPaths() {
    // 1. ПОЛНЫЙ алгоритм Флойда-Уоршелла (все 3 вложенных цикла)
    for (int m = 0; m < numVertices; ++m) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                // Проверяем, что оба пути существуют (не INF)
                if (distMatrix[i][m] != INF && distMatrix[m][j] != INF) {
                    double newDist = distMatrix[i][m] + distMatrix[m][j];
                    if (newDist < distMatrix[i][j]) {
                        distMatrix[i][j] = newDist;
                        predMatrix[i][j] = predMatrix[m][j];
                    }
                }
            }
        }
    }

    // 2. Обнаружение отрицательных циклов (ПОСЛЕ выполнения алгоритма)
    for (int i = 0; i < numVertices; ++i) {
        if (distMatrix[i][i] < 0) { // Отрицательный цикл через вершину i
            hasNegativeCycle = true;

            // 3. Помечаем все пары вершин, пути через которые проходят через i
            for (int j = 0; j < numVertices; ++j) {
                for (int k = 0; k < numVertices; ++k) {
                    if (distMatrix[j][i] != INF && distMatrix[i][k] != INF) {
                        distMatrix[j][k] = NEG_INF;
                    }
                }
            }
        }
    }

    return !hasNegativeCycle;
}

// Вывод матрицы расстояний
void Graph::printDistanceMatrix() const {
    cout << "\nDistance Matrix D[" << numVertices << "][" << numVertices << "]:\n";
    cout << "   ";
    for (int i = 0; i < numVertices; ++i) {
        cout << setw(10) << i;
    }
    cout << endl;

    for (int i = 0; i < numVertices; ++i) {
        cout << i << " |";
        for (int j = 0; j < numVertices; ++j) {
            if (distMatrix[i][j] == NEG_INF) {
                cout << setw(10) << "-INF";
            } else if (distMatrix[i][j] == INF) {
                cout << setw(10) << "INF";
            } else {
                cout << setw(10) << fixed << setprecision(2) << distMatrix[i][j];
            }
        }
        cout << endl;
    }

    if (hasNegativeCycle) {
        cout << "\nWARNING: Graph contains negative cycles!\n";
        cout << "-INF means distance is undefined (path through negative cycle)\n";
    }
}

// Восстановление и вывод кратчайшего пути между вершинами
void Graph::printPath(int i, int j) const {
    // Проверка существования пути
    if (distMatrix[i][j] == INF) {
        cout << "No path from " << i << " to " << j;
        return;
    }

    // Проверка на наличие пути через отрицательный цикл
    if (distMatrix[i][j] == NEG_INF) {
        cout << "Path from " << i << " to " << j << " goes through a negative cycle";
        return;
    }

    cout << "Shortest path from " << i << " to " << j << ": ";

    stack<int> path;
    path.push(j);
    int current = j;

    // Восстановление пути по матрице предшественников
    while (current != i) {
        current = predMatrix[i][current];
        if (current == -1) {
            cout << "Error in path reconstruction";
            return;
        }
        path.push(current);
    }

    // Вывод пути
    while (!path.empty()) {
        cout << path.top();
        path.pop();
        if (!path.empty()) cout << " -> ";
    }
    cout << " (length = " << fixed << setprecision(2) << distMatrix[i][j] << ")";
}
