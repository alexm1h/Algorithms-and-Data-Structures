#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
private:
    int numVertices;
    std::vector<std::vector<double>> distMatrix;
    std::vector<std::vector<int>> predMatrix; // Матрица предшественников
    bool hasNegativeCycle; // Флаг наличия отрицательных циклов

public:
    Graph(int V);
    void addEdge(int u, int v, double weight);
    bool computeAllPairsShortestPaths(); // Возвращает false при наличии отрицательного цикла
    void printDistanceMatrix() const;
    void printPath(int i, int j) const;
    bool hasNegativeCycles() const { return hasNegativeCycle; }
};

#endif
