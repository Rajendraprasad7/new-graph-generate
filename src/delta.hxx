#pragma once 

#include <vector>
#include <utility>
#include <random>
#include <algorithm>

#include "Graph.hxx"
#include "utils.hxx"

using std::vector;
using std::pair;
using std::unordered_map;
using std::make_pair;
using std::back_inserter;
using std::advance;
using std::uniform_int_distribution;
using std::random_device;
using std::mt19937;

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
vector<pair<int, int>> getExistingRandomEdges(const DiGraph<V, E>& graph, int count, bool strictDelta) {
    static random_device rd;
    static mt19937 rng(rd());

    if(count >= graph.getSize()){
        return graph.getAllEdges();
    }

    vector<pair<int, int>> result;
    result.reserve(count);

    auto validVertices = graph.getValidVertices();
    if (validVertices.empty()) {
        return result; 
    }

    unordered_map<pair<int, int>, bool, PairHash> mp;

    uniform_int_distribution<> vertexDist(0, static_cast<int>(validVertices.size()) - 1);

    while (result.size() < count) {

        int u = validVertices[vertexDist(rng)];
        auto outEdges = graph.getOutEdges(u);

        if (!outEdges.empty()) {
            uniform_int_distribution<> edgeDist(0, static_cast<int>(outEdges.size()) - 1);
            int v = outEdges[edgeDist(rng)];

            pair<int, int> edge = make_pair(u, v);
            if (mp.count(edge)) continue;
            result.push_back(edge);
            mp[edge] = true;
        }
    }

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

// If the flag strictDelta is set, it is assumed that graph is sparse enough to give new edges quickly
template <typename V, typename E>
vector<pair<int, int>> getNewRandomEdges(const DiGraph<V, E>& graph, int count, bool strictDelta) {
    vector<pair<int, int>> result;
    unordered_map<pair<int, int>, bool, PairHash> mp;
    result.reserve(count);

    int n = graph.getOrder();
    int m = graph.getSize();

    if(strictDelta){
        while(result.size() < std::min(count, n*(n-1) - m)){
            pair<int, int> edge = getNewRandomEdgeForcibly(graph);
            if(mp.count(edge)) continue;
            result.push_back(edge);
            mp[edge] = true;
        }
    }
    else{
        for (int i = 0; i < count; ++i) {
            result.push_back(getNewRandomEdge(graph));
        }
    }
    return result;
}

template <typename V, typename E>
class GraphDelta {
public:
    vector<pair<int, int>> insertions;
    vector<pair<int, int>> deletions;

    // If strictDelta flag is set then each insertion/deletion will surely induce a change to the graph.
    void generateMixedDelta(const DiGraph<V, E>& graph, double alpha, int count, bool strictDelta) {
        static random_device rd;
        static mt19937 rng(rd());

        int numInsertions = static_cast<int>(alpha * count);
        int numDeletions = count - numInsertions;

        insertions = getNewRandomEdges(graph, numInsertions, strictDelta);
        deletions = getExistingRandomEdges(graph, numDeletions, strictDelta);
    }

    // void generatePreferentialAttachmentDelta(const DiGraph<V, E>& graph, int count, bool strictDelta) {
    //     static random_device rd;
    //     static mt19937 rng(rd());

    //     vector<pair<int, int>> result; 


    // }

    void applyCurrentDelta(DiGraph<V, E>& graph) {
        for (const auto& edge: insertions) {
            graph.addEdge(edge.first, edge.second);
        }
        for (const auto& edge: deletions) {
            graph.removeEdge(edge.first, edge.second);
        }
    }

    void clearDelta() {
        insertions.clear();
        deletions.clear();
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