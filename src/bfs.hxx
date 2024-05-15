#pragma once
#include <queue>
#include <vector>
#include "Graph.hxx"

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
