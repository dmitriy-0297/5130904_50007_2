#include "Graph.h"

#include <algorithm>
#include <iostream>
#include <map>

void Graph::addVertex(int vertex) {
    vertices_.insert(vertex);
}

void Graph::removeVertex(int vertex) {
    vertices_.erase(vertex);

    edges_.erase(
        std::remove_if(
            edges_.begin(),
            edges_.end(),
            [vertex](const Edge& edge) {
                return edge.from == vertex || edge.to == vertex;
            }
        ),
        edges_.end()
    );
}

void Graph::addEdge(int from, int to, int weight) {
    if (vertices_.count(from) == 0 || vertices_.count(to) == 0) {
        std::cout << "Vertex does not exist\n";
        return;
    }

    edges_.push_back({from, to, weight});
}

void Graph::removeEdge(int from, int to) {
    edges_.erase(
        std::remove_if(
            edges_.begin(),
            edges_.end(),
            [from, to](const Edge& edge) {
                return (edge.from == from && edge.to == to) ||
                       (edge.from == to && edge.to == from);
            }
        ),
        edges_.end()
    );
}

void Graph::printVertices() const {
    for (int vertex : vertices_) {
        std::cout << vertex << " ";
    }

    std::cout << "\n";
}

void Graph::printEdges() const {
    for (const Edge& edge : edges_) {
        std::cout << edge.from << " - "
                  << edge.to << " : "
                  << edge.weight << "\n";
    }
}

void Graph::minimumSpanningTree() const {
    std::vector<Edge> sortedEdges = edges_;

    std::sort(
        sortedEdges.begin(),
        sortedEdges.end(),
        [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        }
    );

    std::map<int, int> parent;
    std::map<int, int> rank;

    for (int vertex : vertices_) {
        parent[vertex] = vertex;
        rank[vertex] = 0;
    }

    auto find = [&parent](int vertex) {
        int root = vertex;

        while (parent[root] != root) {
            root = parent[root];
        }

        while (parent[vertex] != vertex) {
            int next = parent[vertex];
            parent[vertex] = root;
            vertex = next;
        }

        return root;
    };

    auto unite = [&parent, &rank, &find](int first, int second) {
        first = find(first);
        second = find(second);

        if (first == second) {
            return false;
        }

        if (rank[first] < rank[second]) {
            std::swap(first, second);
        }

        parent[second] = first;

        if (rank[first] == rank[second]) {
            ++rank[first];
        }

        return true;
    };

    std::vector<Edge> result;
    int totalWeight = 0;

    for (const Edge& edge : sortedEdges) {
        if (unite(edge.from, edge.to)) {
            result.push_back(edge);
            totalWeight += edge.weight;

            if (result.size() == vertices_.size() - 1) {
                break;
            }
        }
    }

    if (vertices_.empty()) {
        std::cout << "Graph is empty\n";
        return;
    }

    if (result.size() != vertices_.size() - 1) {
        std::cout << "Graph is not connected\n";
        return;
    }

    std::cout << "Minimum spanning tree:\n";

    for (const Edge& edge : result) {
        std::cout << edge.from << " - "
                  << edge.to << " : "
                  << edge.weight << "\n";
    }

    std::cout << "Total weight: " << totalWeight << "\n";
}

void Graph::clear() {
    vertices_.clear();
    edges_.clear();
}
