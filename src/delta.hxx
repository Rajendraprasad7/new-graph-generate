#pragma once

#include <vector>
#include <random>
#include <unordered_map>
#include <iostream>
#include "Graph.hxx"
#include "edge.hxx"

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
 * @class GraphDelta
 * @brief Represents the changes to a graph (insertions and deletions).
 * @tparam V The type of the vertices in the graph.
 * @tparam E The type of the edges in the graph.
 */
template <typename V, typename E>
class GraphDelta {
public:
    vector<pair<int, int>> insertions;
    vector<pair<int, int>> deletions;

    /**
     * @brief Generates a mixed delta of edge insertions and deletions.
     * @param graph The original graph.
     * @param epsilon The proportion of insertions in the delta.
     * @param count The total number of changes in the delta.
     * @param strictDelta Flag indicating if each insertion/deletion should induce a change to the graph.
     */
    void generateMixedDelta(const DiGraph<V, E>& graph, double epsilon, int count, bool strictDelta) {
        static random_device rd;
        static mt19937 rng(rd());
        int numInsertions = static_cast<int>(epsilon * count);
        int numDeletions = count - numInsertions;
        insertions = getNewRandomEdges(graph, numInsertions, strictDelta);
        deletions = getExistingRandomEdges(graph, numDeletions, strictDelta);
    }

    /**
     * @brief Generates an insertion delta using preferential attachment function.
     * @param graph The original graph.
     * @param count The total number of changes in the delta.
     * @param alpha The alpha parameter of the preferential attachment function.
     * @param beta The beta parameter of the preferential attachment function.
     * @param lambda The lambda parameter of the preferential attachment function.
     * @param strictPreferential Flag indicating if both the source and target vertices are preferentially chosen.
     * @param strictDelta Flag indicating if each insertion should induce a change to the graph.
     */
    void generatePreferentialAttachmentDelta(const DiGraph<V, E>& graph, int count, double alpha, double beta, double lambda, bool strictPreferential, bool strictDelta) {
        static random_device rd;
        static mt19937 rng(rd());
        vector<pair<int, int>> result;
        unordered_map<pair<int, int>, bool, PairHash> mp;
        result.reserve(count);
        auto vertices = graph.getValidVertices();
        int n = vertices.size();
        vector<double> f(n);
        double total_f = 0.0;
        for (int i = 0; i < n; ++i) {
            double d = graph.getInDegree(vertices[i]);
            double wt = exp(alpha + beta * log(1 + d)) - lambda;
            if (wt < 0) wt = 0; 
            f[i] = wt;
            total_f += wt;
        }
        for (double& p : f) {
            p /= total_f;
        }
        discrete_distribution<int> vertexDist(f.begin(), f.end());
        while (result.size() < std::min(count,n*n)) {
            int u,v;
            if(strictPreferential){
                u = vertices[vertexDist(rng)];
                v = vertices[vertexDist(rng)];
            }
            else{
                u = getRandomElement(vertices, rng);
                v = vertices[vertexDist(rng)];
            }
            if(strictDelta){
                if(strictPreferential){
                    while (graph.hasEdge(u, v)) {
                        u = vertices[vertexDist(rng)];
                        v = vertices[vertexDist(rng)];
                    }
                }
                else{
                    while (graph.hasEdge(u, v)) {
                        u = getRandomElement(vertices, rng);
                        v = vertices[vertexDist(rng)];
                    }
                }
                pair<int, int> edge = make_pair(u, v);
                if (mp.count(edge)) continue;
                result.push_back(edge);
                mp[edge] = true;
            }
            else{
                pair<int, int> edge = make_pair(u, v);
                result.push_back(edge);
            }
        }
        insertions = result;
    }

    /**
     * @brief Generates a deletion delta using preferential function.
     * @param graph The original graph.
     * @param count The total number of changes in the delta.
     * @param alpha The alpha parameter of the preferential function.
     * @param beta The beta parameter of the preferential function.
     * @param lambda The lambda parameter of the preferential function.
     * @param strictPreferential Flag indicating if both the source and target vertices are preferentially chosen.
     * @param strictDelta Flag indicating if each deletion should induce a change to the graph.
     */
    void generatePreferentialDetachmentDelta(const DiGraph<V, E>& graph, int count, double alpha, double beta, double lambda, bool strictPreferential, bool strictDelta) {
        static random_device rd;
        static mt19937 rng(rd());
        vector<pair<int, int>> result;
        unordered_map<pair<int, int>, bool, PairHash> mp;
        result.reserve(count);
        auto vertices = graph.getValidVertices();
        int n = vertices.size();
        int numEdges = graph.getSize();
        vector<double> f(n);
        double total_f = 0.0;
        for (int i = 0; i < n; ++i) {
            double d = graph.getInDegree(vertices[i]);
            double wt = exp(alpha + beta * log(1 + d)) - lambda;
            if (wt < 0) wt = 0; 
            f[i] = wt;
            total_f += wt;
        }
        for (double& p : f) {
            p /= total_f;
        }
        discrete_distribution<int> vertexDist(f.begin(), f.end());
        while (result.size() < std::min(count,numEdges)) {
            int u,v;
            if(strictPreferential){
                u = vertices[vertexDist(rng)];
                v = vertices[vertexDist(rng)];
            }
            else{
                u = getRandomElement(vertices, rng);
                v = vertices[vertexDist(rng)];
            }
            if(strictDelta){
                if(strictPreferential){
                    while (!graph.hasEdge(u, v)) {
                        u = vertices[vertexDist(rng)];
                        v = vertices[vertexDist(rng)];
                    }
                }
                else{
                    while (!graph.hasEdge(u, v)) {
                        u = getRandomElement(vertices, rng);
                        v = vertices[vertexDist(rng)];
                    }
                }
                pair<int, int> edge = make_pair(u, v);
                if (mp.count(edge)) continue;
                result.push_back(edge);
                mp[edge] = true;
            }
            else{
                pair<int, int> edge = make_pair(u, v);
                result.push_back(edge);
            }
        }
        deletions = result;
    }

    /** 
     * @brief Generates a mixed delta of edge insertions and deletions using preferential function.
     * @param graph The original graph.
     * @param count The total number of changes in the delta.
     * @param alpha The alpha parameter of the preferential attachment function.
     * @param beta The beta parameter of the preferential attachment function.
     * @param lambda The lambda parameter of the preferential attachment function.
     * @param strictPreferential Flag indicating if both the source and target vertices are preferentially chosen.
     * @param strictDelta Flag indicating if each insertion/deletion should induce a change to the graph.
     * @param epsilon The fraction of insertions in the delta.
     */
    void generatePreferentialMixedDelta(const DiGraph<V, E>& graph, int count, double alpha, double beta, double lambda, bool strictPreferential, bool strictDelta, double epsilon) {
        static random_device rd;
        static mt19937 rng(rd());
        int numInsertions = static_cast<int>(epsilon * count);
        int numDeletions = count - numInsertions;
        generatePreferentialAttachmentDelta(graph, numInsertions, alpha, beta, lambda, strictPreferential, strictDelta);
        generatePreferentialDetachmentDelta(graph, numDeletions, alpha, beta, lambda, strictPreferential, strictDelta);
    }
        
    /**
     * @brief Applies the current delta to a graph.
     * @param graph The graph to apply the delta to.
     */
    void applyCurrentDelta(DiGraph<V, E>& graph) {
        for (const auto& edge: insertions) {
            graph.addEdge(edge.first, edge.second);
        }
        for (const auto& edge: deletions) {
            graph.removeEdge(edge.first, edge.second);
        }
    }

    /**
     * @brief Clears the current delta.
     */
    void clearDelta() {
        insertions.clear();
        deletions.clear();
    }

    /**
     * @brief Overloads the << operator to print the delta.
     * @param os The output stream.
     * @param delta The delta to print.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const GraphDelta<V, E>& delta) {
        for (const auto& edge : delta.insertions) {
            os << "+ (" << edge.first << ", " << edge.second << ")" << std::endl;
        }
        for (const auto& edge : delta.deletions) {
            os << "- (" << edge.first << ", " << edge.second << ")" << std::endl;
        }
        return os;
    }
};