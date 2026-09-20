#include "Graph.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


void printResult(const std::vector<int>& result)
{
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        if (i > 0)
        {
            std::cout << " ";
        }

        std::cout << result[i];
    }

    std::cout << "\n";
}


void printHelp()
{
    std::cout << "Available commands:\n";
    std::cout << "  add_vertex <vertex>\n";
    std::cout << "  remove_vertex <vertex>\n";
    std::cout << "  add_edge <from> <to> <weight>\n";
    std::cout << "  remove_edge <from> <to>\n";
    std::cout << "  add_undirected <vertex1> <vertex2> <weight>\n";
    std::cout << "  dfs_directed <start>\n";
    std::cout << "  dfs_undirected <start>\n";
    std::cout << "  topological_sort\n";
    std::cout << "  print\n";
    std::cout << "  help\n";
}


void processCommand(
    const std::string& line,
    Graph& graph)
{
    std::istringstream input(line);
    std::string command;

    input >> command;

    if (command.empty())
    {
        return;
    }

    if (command == "add_vertex")
    {
        int vertex;

        if (!(input >> vertex))
        {
            throw std::invalid_argument(
                "Usage: add_vertex <vertex>");
        }

        graph.addVertex(vertex);

        std::cout << "OK\n";
        return;
    }

    if (command == "remove_vertex")
    {
        int vertex;

        if (!(input >> vertex))
        {
            throw std::invalid_argument(
                "Usage: remove_vertex <vertex>");
        }

        graph.removeVertex(vertex);

        std::cout << "OK\n";
        return;
    }

    if (command == "add_edge")
    {
        int from;
        int to;
        int weight;

        if (!(input >> from >> to >> weight))
        {
            throw std::invalid_argument(
                "Usage: add_edge <from> <to> <weight>");
        }

        graph.addDirectedEdge(from, to, weight);

        std::cout << "OK\n";
        return;
    }

    if (command == "remove_edge")
    {
        int from;
        int to;

        if (!(input >> from >> to))
        {
            throw std::invalid_argument(
                "Usage: remove_edge <from> <to>");
        }

        int weight = graph.removeDirectedEdge(from, to);

        std::cout << "OK weight=" << weight << "\n";
        return;
    }

    if (command == "add_undirected")
    {
        int vertex1;
        int vertex2;
        int weight;

        if (!(input >> vertex1 >> vertex2 >> weight))
        {
            throw std::invalid_argument(
                "Usage: add_undirected "
                "<vertex1> <vertex2> <weight>");
        }

        graph.addUndirectedEdge(
            vertex1,
            vertex2,
            weight);

        std::cout << "OK\n";
        return;
    }

    if (command == "dfs_directed")
    {
        int start;

        if (!(input >> start))
        {
            throw std::invalid_argument(
                "Usage: dfs_directed <start>");
        }

        std::vector<int> result =
            graph.depthFirstSearchDirected(start);

        std::cout << "DFS directed: ";
        printResult(result);
        return;
    }

    if (command == "dfs_undirected")
    {
        int start;

        if (!(input >> start))
        {
            throw std::invalid_argument(
                "Usage: dfs_undirected <start>");
        }

        std::vector<int> result =
            graph.depthFirstSearchUndirected(start);

        std::cout << "DFS undirected: ";
        printResult(result);
        return;
    }

    if (command == "topological_sort")
    {
        std::vector<int> result =
            graph.topologicalSort();

        std::cout << "Topological order: ";
        printResult(result);
        return;
    }

    if (command == "print")
    {
        graph.print();
        return;
    }

    if (command == "help")
    {
        printHelp();
        return;
    }

    throw std::invalid_argument(
        "Unknown command. Use 'help'.");
}


int main()
{
    Graph graph;
    std::string line;

    std::cout << "Graph command interface.\n";
    std::cout << "Enter 'help' to see available commands.\n";

    while (std::getline(std::cin, line))
    {
        try
        {
            processCommand(line, graph);
        }
        catch (const std::exception& exception)
        {
            std::cerr << "Error: "
                << exception.what()
                << "\n";
        }
    }

    std::cout << "\nEnd of input.\n";

    return 0;
}