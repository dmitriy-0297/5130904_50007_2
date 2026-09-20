#include "Graph.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>


Graph::Graph()
{
}


bool Graph::hasVertex(int vertex) const
{
    return adjacencyList_.find(vertex) != adjacencyList_.end();
}


bool Graph::hasVertices() const
{
    return !adjacencyList_.empty();
}


bool Graph::containsVertex(int vertex) const
{
    return hasVertex(vertex);
}


std::size_t Graph::getVertexCount() const
{
    return adjacencyList_.size();
}


void Graph::addVertex(int vertex)
{
    if (hasVertex(vertex))
    {
        throw std::invalid_argument(
            "Vertex already exists.");
    }

    adjacencyList_[vertex] = {};
}


void Graph::removeVertex(int vertex)
{
    if (!hasVertex(vertex))
    {
        throw std::out_of_range(
            "Vertex does not exist.");
    }

    adjacencyList_.erase(vertex);

    for (auto& item : adjacencyList_)
    {
        auto& edges = item.second;

        edges.erase(
            std::remove_if(
                edges.begin(),
                edges.end(),
                [vertex](const std::pair<int, int>& edge)
                {
                    return edge.first == vertex;
                }),
            edges.end());
    }
}


void Graph::addDirectedEdge(
    int from,
    int to,
    int weight)
{
    if (!hasVertex(from) || !hasVertex(to))
    {
        throw std::out_of_range(
            "One or both vertices do not exist.");
    }

    for (auto& edge : adjacencyList_[from])
    {
        if (edge.first == to)
        {
            edge.second = weight;
            return;
        }
    }

    adjacencyList_[from].push_back({ to, weight });
}


int Graph::removeDirectedEdge(
    int from,
    int to)
{
    if (!hasVertex(from) || !hasVertex(to))
    {
        throw std::out_of_range(
            "One or both vertices do not exist.");
    }

    auto& edges = adjacencyList_[from];

    auto iterator = std::find_if(
        edges.begin(),
        edges.end(),
        [to](const std::pair<int, int>& edge)
        {
            return edge.first == to;
        });

    if (iterator == edges.end())
    {
        throw std::runtime_error(
            "Edge does not exist.");
    }

    int weight = iterator->second;
    edges.erase(iterator);

    return weight;
}


void Graph::addUndirectedEdge(
    int vertex1,
    int vertex2,
    int weight)
{
    if (!hasVertex(vertex1) || !hasVertex(vertex2))
    {
        throw std::out_of_range(
            "One or both vertices do not exist.");
    }

    addDirectedEdge(vertex1, vertex2, weight);

    if (vertex1 != vertex2)
    {
        addDirectedEdge(vertex2, vertex1, weight);
    }
}


int Graph::removeUndirectedEdge(
    int vertex1,
    int vertex2)
{
    if (!hasVertex(vertex1) || !hasVertex(vertex2))
    {
        throw std::out_of_range(
            "One or both vertices do not exist.");
    }

    int weight = getEdgeWeight(vertex1, vertex2);

    removeDirectedEdge(vertex1, vertex2);

    if (vertex1 != vertex2)
    {
        removeDirectedEdge(vertex2, vertex1);
    }

    return weight;
}


bool Graph::hasEdge(int from, int to) const
{
    if (!hasVertex(from) || !hasVertex(to))
    {
        return false;
    }

    for (const auto& edge : adjacencyList_.at(from))
    {
        if (edge.first == to)
        {
            return true;
        }
    }

    return false;
}


int Graph::getEdgeWeight(int from, int to) const
{
    if (!hasVertex(from) || !hasVertex(to))
    {
        throw std::out_of_range(
            "One or both vertices do not exist.");
    }

    for (const auto& edge : adjacencyList_.at(from))
    {
        if (edge.first == to)
        {
            return edge.second;
        }
    }

    throw std::runtime_error(
        "Edge does not exist.");
}


void Graph::dfsDirected(
    int vertex,
    std::map<int, bool>& visited,
    std::vector<int>& result) const
{
    visited[vertex] = true;
    result.push_back(vertex);

    for (const auto& edge : adjacencyList_.at(vertex))
    {
        int neighbour = edge.first;

        if (!visited[neighbour])
        {
            dfsDirected(neighbour, visited, result);
        }
    }
}


std::vector<int> Graph::depthFirstSearchDirected(
    int startVertex) const
{
    if (!hasVertex(startVertex))
    {
        throw std::out_of_range(
            "Starting vertex does not exist.");
    }

    std::map<int, bool> visited;
    std::vector<int> result;

    for (const auto& item : adjacencyList_)
    {
        visited[item.first] = false;
    }

    dfsDirected(startVertex, visited, result);

    return result;
}


void Graph::dfsUndirected(
    int vertex,
    std::map<int, bool>& visited,
    std::vector<int>& result) const
{
    visited[vertex] = true;
    result.push_back(vertex);

    for (const auto& edge : adjacencyList_.at(vertex))
    {
        int neighbour = edge.first;

        if (!visited[neighbour])
        {
            dfsUndirected(neighbour, visited, result);
        }
    }
}


std::vector<int> Graph::depthFirstSearchUndirected(
    int startVertex) const
{
    if (!hasVertex(startVertex))
    {
        throw std::out_of_range(
            "Starting vertex does not exist.");
    }

    std::map<int, bool> visited;
    std::vector<int> result;

    for (const auto& item : adjacencyList_)
    {
        visited[item.first] = false;
    }

    dfsUndirected(startVertex, visited, result);

    return result;
}


void Graph::topologicalSortDfs(
    int vertex,
    std::map<int, int>& state,
    std::vector<int>& result) const
{
    state[vertex] = 1;

    for (const auto& edge : adjacencyList_.at(vertex))
    {
        int neighbour = edge.first;

        if (state[neighbour] == 1)
        {
            throw std::runtime_error(
                "Graph contains a cycle.");
        }

        if (state[neighbour] == 0)
        {
            topologicalSortDfs(
                neighbour,
                state,
                result);
        }
    }

    state[vertex] = 2;
    result.push_back(vertex);
}


std::vector<int> Graph::topologicalSort() const
{
    std::map<int, int> state;
    std::vector<int> result;

    for (const auto& item : adjacencyList_)
    {
        state[item.first] = 0;
    }

    for (const auto& item : adjacencyList_)
    {
        int vertex = item.first;

        if (state[vertex] == 0)
        {
            topologicalSortDfs(
                vertex,
                state,
                result);
        }
    }

    std::reverse(result.begin(), result.end());

    return result;
}


void Graph::print() const
{
    if (!hasVertices())
    {
        std::cout << "Graph is empty.\n";
        return;
    }

    for (const auto& item : adjacencyList_)
    {
        std::cout << item.first << ": ";

        for (const auto& edge : item.second)
        {
            std::cout << edge.first
                << "(" << edge.second << ") ";
        }

        std::cout << "\n";
    }
}