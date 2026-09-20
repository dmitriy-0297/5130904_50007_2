#include "Graph.h"

#include <iostream>
#include <string>

int main() {
    Graph graph;
    std::string command;

    std::cout << "Minimum Spanning Tree\n";
    std::cout << "Type 'help' to see available commands.\n";

    while (std::cin >> command) {
        if (command == "add-vertex") {
            int vertex;
            std::cin >> vertex;
            graph.addVertex(vertex);
        }
        else if (command == "remove-vertex") {
            int vertex;
            std::cin >> vertex;
            graph.removeVertex(vertex);
        }
        else if (command == "add-edge") {
            int from, to, weight;
            std::cin >> from >> to >> weight;
            graph.addEdge(from, to, weight);
        }
        else if (command == "remove-edge") {
            int from, to;
            std::cin >> from >> to;
            graph.removeEdge(from, to);
        }
        else if (command == "vertices") {
            graph.printVertices();
        }
        else if (command == "edges") {
            graph.printEdges();
        }
        else if (command == "mst") {
            graph.minimumSpanningTree();
        }
        else if (command == "clear") {
            graph.clear();
        }
        else if (command == "help") {
            std::cout << "Available commands:\n";
            std::cout << "add-vertex <vertex>\n";
            std::cout << "remove-vertex <vertex>\n";
            std::cout << "add-edge <from> <to> <weight>\n";
            std::cout << "remove-edge <from> <to>\n";
            std::cout << "vertices\n";
            std::cout << "edges\n";
            std::cout << "mst\n";
            std::cout << "clear\n";
            std::cout << "help\n";
        }
        else {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}
