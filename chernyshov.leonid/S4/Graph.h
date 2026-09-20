#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>
#include <string>

struct Edge {
    int from;
    int to;
    int weight;
};

class Graph {
private:
    std::set<int> vertices_;
    std::vector<Edge> edges_;

public:
    void addVertex(int vertex);
    void removeVertex(int vertex);

    void addEdge(int from, int to, int weight);
    void removeEdge(int from, int to);

    void printVertices() const;
    void printEdges() const;

    void minimumSpanningTree() const;

    void clear();
};

#endif
