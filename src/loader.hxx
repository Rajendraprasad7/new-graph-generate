#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>


#include "Graph.hxx"

DiGraph<int, int> loadMtxGraphFromFile(const std::string& fileName) {
    DiGraph<int,int> graph;

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return graph;
    }

    int numVertices, numEdges;
    std::string line;
    getline(file, line);
    while (getline(file, line) && line[0] == '%') {
    }

    std::istringstream iss(line);
    iss >> numVertices >> numVertices >> numEdges;

    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(i);
    }

    while (getline(file, line)) {
        std::istringstream iss(line);
        int u, v;
        iss >> u >> v;
        graph.addEdge(u - 1, v - 1); // Adjust indices since MTX format is 1-based
    }

    file.close();

    return graph;
}

template <typename V, typename E>
DiGraph<V, E> handleFile(int argc, char* argv[]) {

    std::string filename = argv[1];
    std::filesystem::path filePath(filename);

    if (filePath.extension() == ".mtx") {
        return loadMtxGraphFromFile(filePath);
    } else {
        std::cout << "Invalid file extension. Expected .mtx file.\n";
        return DiGraph<V, E>();
    }
}
