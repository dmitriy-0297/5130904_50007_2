#ifndef GRAPH_H
#define GRAPH_H

#include <cstddef>
#include <map>
#include <utility>
#include <vector>


class Graph
{
public:
    Graph();

    bool hasVertices() const;
    bool containsVertex(int vertex) const;
    std::size_t getVertexCount() const;

    void addVertex(int vertex);
    void removeVertex(int vertex);

    void addDirectedEdge(
        int from,
        int to,
        int weight = 1);

    int removeDirectedEdge(
        int from,
        int to);

    void addUndirectedEdge(
        int vertex1,
        int vertex2,
        int weight = 1);

    int removeUndirectedEdge(
        int vertex1,
        int vertex2);

    bool hasEdge(int from, int to) const;
    int getEdgeWeight(int from, int to) const;

    std::vector<int> depthFirstSearchDirected(
        int startVertex) const;

    std::vector<int> depthFirstSearchUndirected(
        int startVertex) const;

    std::vector<int> topologicalSort() const;

    void print() const;

private:
    std::map<int, std::vector<std::pair<int, int>>> adjacencyList_;

    bool hasVertex(int vertex) const;

    void dfsDirected(
        int vertex,
        std::map<int, bool>& visited,
        std::vector<int>& result) const;

    void dfsUndirected(
        int vertex,
        std::map<int, bool>& visited,
        std::vector<int>& result) const;

    void topologicalSortDfs(
        int vertex,
        std::map<int, int>& state,
        std::vector<int>& result) const;
};

#endif