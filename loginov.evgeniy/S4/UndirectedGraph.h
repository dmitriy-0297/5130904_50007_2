#ifndef UNDIRECTED_GRAPH_H
#define UNDIRECTED_GRAPH_H

#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stdexcept>
#include <string>

class UndirectedGraph {
public:
    UndirectedGraph();
    int addVertex();
    void addNode(int node);
    void removeNode(int node);
    void clear() noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] bool hasNode(int node) const noexcept;
    [[nodiscard]] bool hasEdge(int u, int v) const;
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    [[nodiscard]] int nodeCount() const noexcept;
    [[nodiscard]] std::vector<int> nodes() const;
    [[nodiscard]] std::vector<int> bfsTraversal(int start) const;
    [[nodiscard]] std::map<int, int> shortestPaths(int start) const;
    [[nodiscard]] int computeDiameter() const;
    void printAdjacencyList(std::ostream& output) const;

    static constexpr int DISTANCE_UNREACHABLE = -1;
    static constexpr int MAX_NODES_LIMIT = 100000;

private:
    class VertexRecord {
    public:
        VertexRecord() noexcept;
    private:
        friend class UndirectedGraph;
        bool active_;
        std::set<int> neighbors_;
    };

    std::vector<VertexRecord> vertices_;
    std::set<int> activeVertices_;
    std::set<int> freeVertices_;

    void validateActive(int node) const;
    void validateKnown(int node) const;
};

#endif