#include "UndirectedGraph.h"
#include <iostream>

UndirectedGraph::VertexRecord::VertexRecord() noexcept
    : active_(false)
{
}

UndirectedGraph::UndirectedGraph()
{
}

int UndirectedGraph::addVertex()
{
    int id = 0;

    if (!freeVertices_.empty()) {
        id = *freeVertices_.begin();
        freeVertices_.erase(freeVertices_.begin());
    } else {
        if (vertices_.size() >= MAX_NODES_LIMIT) {
            throw std::overflow_error("Maximum number of vertices reached");
        }
        id = static_cast<int>(vertices_.size());
        vertices_.push_back(VertexRecord());
    }

    vertices_[id].active_ = true;
    activeVertices_.insert(id);
    return id;
}

void UndirectedGraph::addNode(int node)
{
    if (node < 0) {
        throw std::invalid_argument("Node ID must be non-negative");
    }
    if (node > MAX_NODES_LIMIT) {
        throw std::out_of_range("Node ID exceeds maximum allowed limit");
    }
    if (hasNode(node)) {
        throw std::logic_error("Node already exists");
    }

    while (static_cast<int>(vertices_.size()) <= node) {
        int id = static_cast<int>(vertices_.size());
        vertices_.push_back(VertexRecord());
        freeVertices_.insert(id);
    }

    freeVertices_.erase(node);
    vertices_[node].active_ = true;
    activeVertices_.insert(node);
}

void UndirectedGraph::removeNode(int node)
{
    validateActive(node);

    std::vector<int> neighbors;
    for (int neighbor : vertices_[node].neighbors_) {
        neighbors.push_back(neighbor);
    }

    for (int neighbor : neighbors) {
        removeEdge(node, neighbor);
    }

    vertices_[node].active_ = false;
    vertices_[node].neighbors_.clear();
    activeVertices_.erase(node);
    freeVertices_.insert(node);
}

void UndirectedGraph::clear() noexcept
{
    vertices_.clear();
    activeVertices_.clear();
    freeVertices_.clear();
}

bool UndirectedGraph::isEmpty() const noexcept
{
    return activeVertices_.empty();
}

bool UndirectedGraph::hasNode(int node) const noexcept
{
    return node >= 0
        && node < static_cast<int>(vertices_.size())
        && vertices_[node].active_;
}

bool UndirectedGraph::hasEdge(int u, int v) const
{
    validateActive(u);
    validateActive(v);
    return vertices_[u].neighbors_.count(v) > 0;
}

void UndirectedGraph::addEdge(int u, int v)
{
    validateActive(u);
    validateActive(v);

    if (u == v) {
        throw std::invalid_argument("Self-loops are not allowed");
    }
    if (hasEdge(u, v)) {
        return;
    }

    vertices_[u].neighbors_.insert(v);
    vertices_[v].neighbors_.insert(u);
}

void UndirectedGraph::removeEdge(int u, int v)
{
    validateActive(u);
    validateActive(v);

    if (!hasEdge(u, v)) {
        throw std::logic_error("Edge does not exist");
    }

    vertices_[u].neighbors_.erase(v);
    vertices_[v].neighbors_.erase(u);
}

int UndirectedGraph::nodeCount() const noexcept
{
    return static_cast<int>(activeVertices_.size());
}

std::vector<int> UndirectedGraph::nodes() const
{
    return std::vector<int>(
        activeVertices_.begin(),
        activeVertices_.end());
}

std::vector<int> UndirectedGraph::bfsTraversal(int start) const
{
    validateActive(start);

    std::vector<int> result;
    std::set<int> visited;
    std::queue<int> queue;

    queue.push(start);
    visited.insert(start);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        result.push_back(current);

        for (int neighbor : vertices_[current].neighbors_) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }

    return result;
}

std::map<int, int> UndirectedGraph::shortestPaths(int start) const
{
    validateActive(start);

    std::map<int, int> distances;
    for (int v : activeVertices_) {
        distances[v] = DISTANCE_UNREACHABLE;
    }
    distances[start] = 0;

    std::queue<int> queue;
    queue.push(start);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int neighbor : vertices_[current].neighbors_) {
            if (distances[neighbor] == DISTANCE_UNREACHABLE) {
                distances[neighbor] = distances[current] + 1;
                queue.push(neighbor);
            }
        }
    }

    return distances;
}

int UndirectedGraph::computeDiameter() const
{
    if (isEmpty()) {
        return 0;
    }

    int maxDist = 0;

    for (int start : activeVertices_) {
        auto dists = shortestPaths(start);
        for (const auto& pair : dists) {
            if (pair.second == DISTANCE_UNREACHABLE) {
                throw std::domain_error(
                    "Graph is disconnected: diameter is undefined");
            }
            if (pair.second > maxDist) {
                maxDist = pair.second;
            }
        }
    }

    return maxDist;
}

void UndirectedGraph::printAdjacencyList(std::ostream& output) const
{
    if (isEmpty()) {
        output << "<EMPTY>\n";
        return;
    }

    for (int u : activeVertices_) {
        output << u << ": ";

        if (vertices_[u].neighbors_.empty()) {
            output << "<none>";
        } else {
            bool first = true;
            for (int v : vertices_[u].neighbors_) {
                if (!first) {
                    output << ", ";
                }
                output << v;
                first = false;
            }
        }
        output << "\n";
    }
}

void UndirectedGraph::validateActive(int node) const
{
    validateKnown(node);

    if (!vertices_[node].active_) {
        throw std::logic_error("Node does not exist");
    }
}

void UndirectedGraph::validateKnown(int node) const
{
    if (node < 0
        || node >= static_cast<int>(vertices_.size())) {
        throw std::out_of_range("Node is unknown");
    }
}
