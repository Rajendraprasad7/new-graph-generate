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
using std::discrete_distribution;
using std::random_device;
using std::mt19937;

/**
 * Get a random element from a container.
 * @tparam Container The type of the container.
 * @param container The container.
 * @param rng The random number generator.
 * @return A random element from the container.
 */
template <typename Container>
auto getRandomElement(Container& container, mt19937& rng) {
    uniform_int_distribution<> dis(0, static_cast<int>(container.size()) - 1);
    auto it = begin(container);
    advance(it, dis(rng));
    return *it;
}

/**
 * Get a vector of random existing edges. If count is greater than total edges, all edges are returned.
 * @tparam V The vertex type of the graph.
 * @tparam E The edge type of the graph.
 * @param graph The directed graph.
 * @param count The number of random edges to retrieve.
 * @param strictDelta strictDelta flag indicating if each insertion/deletion should induce a change to the graph.
 * @return A vector of random existing edges.
 */
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

/**
 * Get a random new edge.
 * @tparam V The vertex type of the graph.
 * @tparam E The edge type of the graph.
 * @param graph The directed graph.
 * @return A pair of integers representing the new edge.
 */
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

/**
 * Get a random edge that doesn't already exist.
 * @tparam V The vertex type of the graph.
 * @tparam E The edge type of the graph.
 * @param graph The directed graph.
 * @return A pair of integers representing the new edge.
 */
template <typename V, typename E>
pair<int, int> getNewRandomEdgeForcibly(const DiGraph<V, E>& graph) {
    static  random_device rd;
    static  mt19937 rng(rd());
    auto vertices = graph.getValidVertices();
    int u,v;
    do{
        u = getRandomElement(vertices, rng);
        v = getRandomElement(vertices, rng);
    }while (graph.hasEdge(u, v)); 
    return {u, v};
}

/**
 * Get a vector of random new edges.
 * @tparam V The vertex type of the graph.
 * @tparam E The edge type of the graph.
 * @param graph The directed graph.
 * @param count The number of random edges to retrieve.
 * @param strictDelta strictDelta flag indicating if each insertion/deletion should induce a change to the graph.
 * @return A vector of random new edges.
 */
template <typename V, typename E>
vector<pair<int, int>> getNewRandomEdges(const DiGraph<V, E>& graph, int count, bool strictDelta) {
    vector<pair<int, int>> result;
    unordered_map<pair<int, int>, bool, PairHash> mp;
    result.reserve(count);
    int n = graph.getOrder();
    int m = graph.getSize();
    if(strictDelta){
        while(result.size() < std::min(count, n*n - m)){
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