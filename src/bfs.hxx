#pragma once
#include <queue>
#include <vector>
#include "Graph.hxx"

/**
 * @brief Performs breadth-first search on a directed graph starting from a given vertex.
 * @tparam V The type of the vertex in the graph.
 * @tparam E The type of the edge in the graph.
 * @param graph The directed graph to perform breadth-first search on.
 * @param start The starting vertex for the breadth-first search.
 * @return A vector containing the vertices in the order they are visited during the traversal.
 */
template <typename V, typename E>
std::vector<int> breadthFirstSearch(const DiGraph<V, E>& graph, int start) {
    std::vector<int> bfsOrder;
    if (!graph.hasVertex(start))
        return bfsOrder;
    vector<bool> visited(graph.getOrder(), false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        bfsOrder.push_back(u);
        for (int v : graph.getOutEdges(u)) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return bfsOrder;
}
