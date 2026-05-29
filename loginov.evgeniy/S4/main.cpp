#include "UndirectedGraph.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <cctype>
#include <limits>

void printHelp()
{
    std::cout << "\n";
    std::cout << "Available commands:\n";
    std::cout << "  add-vertex              - Add vertex with automatic ID\n";
    std::cout << "  add-node <id>           - Add node with specific ID\n";
    std::cout << "  add-edge <u> <v>        - Add edge between u and v\n";
    std::cout << "  remove-vertex <v>       - Remove vertex and edges\n";
    std::cout << "  remove-edge <u> <v>     - Remove edge between u and v\n";
    std::cout << "  bfs <start>             - Breadth-first search\n";
    std::cout << "  shortest-path <start>   - Shortest paths from start\n";
    std::cout << "  diameter                - Compute graph diameter\n";
    std::cout << "  list-nodes              - List active nodes\n";
    std::cout << "  adjacency-list          - Print adjacency list\n";
    std::cout << "  count                   - Print node count\n";
    std::cout << "  clear                   - Remove all vertices\n";
    std::cout << "  demo                    - Run automated demo\n";
    std::cout << "  help                    - Show this message\n";
    std::cout << "  exit                    - Exit program\n";
    std::cout << "\n";
}

template<typename T>
void printVector(const std::string& title, const T& values)
{
    std::cout << "  " << title << ": {";
    bool first = true;
    for (const auto& val : values) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << val;
        first = false;
    }
    std::cout << "}\n";
}

void printDistances(const std::map<int, int>& dists)
{
    for (const auto& p : dists) {
        std::cout << "    " << p.first << ": " << p.second << "\n";
    }
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, last - first + 1);
}

std::string toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return str;
}

std::string filterAscii(const std::string& str)
{
    std::string result;
    for (char c : str) {
        if (static_cast<unsigned char>(c) <= 127) {
            result += c;
        }
    }
    return result;
}

int main()
{
    UndirectedGraph graph;
    std::string line;

    std::cout << "Undirected Graph - BFS & Diameter (Variant 2.2)\n";
    std::cout << "Type 'help' for available commands.\n";
    std::cout << "Use Ctrl+D (Linux) or Ctrl+Z (Windows) to exit.\n\n";

    std::map<std::string, std::function<void()>> commands;

    commands["add-vertex"] = [&]() {
        int id = graph.addVertex();
        std::cout << "  Vertex added (ID = " << id << ")\n";
    };

    commands["add-node"] = [&]() {
        std::istringstream iss(line);
        std::string cmd;
        int id;
        iss >> cmd >> id;
        try {
            graph.addNode(id);
            std::cout << "  Node " << id << " created successfully\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["add-edge"] = [&]() {
        std::istringstream iss(line);
        std::string cmd;
        int u, v;
        iss >> cmd >> u >> v;
        try {
            graph.addEdge(u, v);
            std::cout << "  Edge " << u << "-" << v << " added\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["remove-vertex"] = [&]() {
        std::istringstream iss(line);
        std::string cmd;
        int v;
        iss >> cmd >> v;
        try {
            graph.removeNode(v);
            std::cout << "  Vertex " << v << " removed\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["remove-edge"] = [&]() {
        std::istringstream iss(line);
        std::string cmd;
        int u, v;
        iss >> cmd >> u >> v;
        try {
            graph.removeEdge(u, v);
            std::cout << "  Edge " << u << "-" << v << " removed\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["bfs"] = [&]() {
        std::istringstream iss(line);
        std::string cmd;
        int start;
        iss >> cmd >> start;
        try {
            auto result = graph.bfsTraversal(start);
            printVector("BFS from " + std::to_string(start), result);
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["shortest-path"] = [&]() {
        std::istringstream iss(line);
        std::string cmd;
        int start;
        iss >> cmd >> start;
        try {
            auto dists = graph.shortestPaths(start);
            std::cout << "  Shortest paths from " << start << ":\n";
            printDistances(dists);
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["shortest-paths"] = commands["shortest-path"];

    commands["diameter"] = [&]() {
        try {
            int diam = graph.computeDiameter();
            std::cout << "  Graph diameter: " << diam << "\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }
    };

    commands["list-nodes"] = [&]() {
        auto nodes = graph.nodes();
        printVector("Nodes", nodes);
    };

    commands["adjacency-list"] = [&]() {
        std::cout << "  Adjacency list:\n";
        graph.printAdjacencyList(std::cout);
    };

    commands["count"] = [&]() {
        std::cout << "  Node count: " << graph.nodeCount() << "\n";
    };

    commands["clear"] = [&]() {
        graph.clear();
        std::cout << "  Graph cleared\n";
    };

    commands["demo"] = [&]() {
        std::cout << "\n=== STARTING AUTOMATED DEMONSTRATION ===\n\n";

        std::cout << "[Step 1] Initial empty state:\n";
        std::cout << "  Node count: " << graph.nodeCount() << "\n";
        printVector("Active nodes", graph.nodes());

        std::cout << "\n[Step 2] Adding 5 vertices:\n";
        for (int i = 0; i < 5; ++i) {
            int id = graph.addVertex();
            std::cout << "  Created vertex " << id << "\n";
        }

        std::cout << "\n[Step 3] Adding edges (0-1, 0-2, 1-3, 2-3, 3-4):\n";
        std::vector<std::pair<int, int>> edges = {
            {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}
        };
        for (const auto& e : edges) {
            graph.addEdge(e.first, e.second);
            std::cout << "  Added edge " << e.first << "-" << e.second << "\n";
        }

        std::cout << "\n[Step 4] Adjacency list:\n";
        graph.printAdjacencyList(std::cout);

        std::cout << "\n[Step 5] BFS from 0:\n";
        try {
            auto res = graph.bfsTraversal(0);
            printVector("BFS order", res);
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }

        std::cout << "\n[Step 6] Shortest paths from 0:\n";
        try {
            auto dists = graph.shortestPaths(0);
            printDistances(dists);
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }

        std::cout << "\n[Step 7] Diameter:\n";
        try {
            int diam = graph.computeDiameter();
            std::cout << "  Diameter: " << diam << "\n";
        } catch (const std::exception& e) {
            std::cout << "  " << e.what() << "\n";
        }

        std::cout << "\n[Step 8] Removing vertex 3:\n";
        try {
            graph.removeNode(3);
            std::cout << "  Vertex 3 removed.\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }

        std::cout << "\n[Step 9] Diameter on disconnected graph:\n";
        try {
            int diam = graph.computeDiameter();
            std::cout << "  Diameter: " << diam << "\n";
        } catch (const std::exception& e) {
            std::cout << "  " << e.what() << "\n";
        }

        std::cout << "\n[Step 10] BFS from 0 after removal:\n";
        try {
            auto res = graph.bfsTraversal(0);
            printVector("BFS order", res);
            std::cout << "  (Note: Vertex 4 is unreachable)\n";
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }

        std::cout << "\n[Step 11] Clearing graph:\n";
        graph.clear();
        std::cout << "  Graph cleared. Node count: " << graph.nodeCount() << "\n";

        std::cout << "\n[Step 12] Sparse nodes (100, 200):\n";
        graph.addNode(100);
        graph.addNode(200);
        std::cout << "  Added nodes 100 and 200.\n";
        graph.addEdge(100, 200);
        std::cout << "  Added edge 100-200.\n";
        printVector("Active nodes", graph.nodes());

        std::cout << "\n[Step 13] BFS on sparse graph:\n";
        try {
            auto res = graph.bfsTraversal(100);
            printVector("BFS order", res);
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << "\n";
        }

        std::cout << "\n[Final] Cleanup:\n";
        graph.clear();
        std::cout << "  Graph cleared. Ready for commands.\n";
        std::cout << "\n=== DEMONSTRATION COMPLETE ===\n\n";
    };

    commands["help"] = [&]() {
        printHelp();
    };

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            std::cout << "\nEOF detected. Exiting...\n";
            break;
        }

        line = filterAscii(line);
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        std::string command = toLower(line);
        size_t firstSpace = command.find(' ');
        if (firstSpace != std::string::npos) {
            command = command.substr(0, firstSpace);
        }

        auto it = commands.find(command);
        if (it != commands.end()) {
            try {
                it->second();
            } catch (const std::exception& e) {
                std::cout << "  Unexpected error: " << e.what() << "\n";
            }
        } else if (command == "exit" || command == "quit") {
            std::cout << "  Exiting...\n";
            break;
        } else {
            std::cout << "  Unknown command: " << command << "\n";
            std::cout << "  Type 'help' for available commands.\n";
        }

        std::cin.clear();
    }

    return 0;
}
