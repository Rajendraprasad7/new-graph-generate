#pragma once 

#include <vector>
#include <utility>
#include <random>
#include <algorithm>

#include "Graph.hxx"

using std::vector;
using std::uniform_int_distribution;
using std::make_pair;
using std::back_inserter;
using std::random_device;
using std::mt19937;
using std::advance;

// Helper function to get a random value from a container
template <typename Container>
auto getRandomElement(Container& container, mt19937& rng) {
    uniform_int_distribution<> dis(0, static_cast<int>(container.size()) - 1);
    auto it = begin(container);
    advance(it, dis(rng));
    return *it;
}

// // Get a random existing edge
// template <typename V, typename E>
//  pair<int, int> getExistingRandomEdge(const DiGraph<V, E>& graph) {
//     static random_device rd;
//     static mt19937 rng(rd());

//     auto edgeList = graph.getAllEdges();
//     if (edgeList.empty()) {
//         return {-1, -1};
//     }
//     return getRandomElement(edgeList, rng);
// }

// Get a vector of random existing edges. If count is greater than total edges, all edges are returned.
template <typename V, typename E>
 vector< pair<int, int>> getExistingRandomEdges(const DiGraph<V, E>& graph, int count) {
    static  random_device rd;
    static  mt19937 rng(rd());

    auto edgeList = graph.getAllEdges();
     vector< pair<int, int>> result;
    result.reserve(count);

    sample(edgeList.begin(), edgeList.end(),  back_inserter(result), count, rng);
    return result;
}

template <typename V, typename E>
std::pair<int, int> getNewRandomEdge(const DiGraph<V, E>& graph) {
    static random_device rd;
    static mt19937 rng(rd());

    auto vertices = graph.getValidVertices();
    if (vertices.empty()) {
        return {-1, -1}; 
    }

    int u = getRandomElement(vertices, rng);
    int v = getRandomElement(vertices, rng);
    return {u, v};
}

template <typename V, typename E>
vector<pair<int, int>> getNewRandomEdges(const DiGraph<V, E>& graph, int count) {
    vector<pair<int, int>> result;
    result.reserve(count);

    for (int i = 0; i < count; ++i) {
        result.push_back(getNewRandomEdge(graph));
    }
    return result;
}

// Get a random edge that doesnt already exist (can loop forever)
template <typename V, typename E>
 pair<int, int> getNewRandomEdgeForcibly(const DiGraph<V, E>& graph) {
    static  random_device rd;
    static  mt19937 rng(rd());

    auto vertices = graph.getValidVertices();
    int u = getRandomElement(vertices, rng);
    int v = u;
    while (v == u || graph.hasEdge(u, v)) {
        v = getRandomElement(vertices, rng);
    }
    return {u, v};
}

template <typename V, typename E>
class GraphDelta {
public:
    vector<pair<int, int>> insertions;
    vector<pair<int, int>> deletions;

    void generateDelta(const DiGraph<V, E>& graph, double alpha, int count) {
        static random_device rd;
        static mt19937 rng(rd());

        int numInsertions = static_cast<int>(alpha * count);
        int numDeletions = count - numInsertions;

        insertions = getNewRandomEdges(graph, numInsertions);
        deletions = getExistingRandomEdges(graph, numDeletions);
    }

    void applyDelta(DiGraph<V, E>& graph) {
        for (const auto& edge: insertions) {
            graph.addEdge(edge.first, edge.second);
        }
        for (const auto& edge: deletions) {
            graph.removeEdge(edge.first, edge.second);
        }
    }

    void printDelta() const {
        for (const auto& edge : insertions) {
            std::cout << "+ (" << edge.first << ", " << edge.second << ")" << std::endl;
        }
        for (const auto& edge : deletions) {
            std::cout << "- (" << edge.first << ", " << edge.second << ")" << std::endl;
        }
    }
};