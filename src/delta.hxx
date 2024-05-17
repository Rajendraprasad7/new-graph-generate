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

    void generatePreferentialAttachmentDelta(const DiGraph<V, E>& graph, int count, bool strictDelta, double alpha, double beta, double lambda) {
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
            int u = getRandomElement(vertices, rng);
            int v = vertices[vertexDist(rng)];

            if(strictDelta){
                while (graph.hasEdge(u, v)) {
                    v = vertices[vertexDist(rng)];
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