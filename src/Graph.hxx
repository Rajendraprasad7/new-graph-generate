#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
// #include "AVLTree.hxx"

using std::vector;
using std::pair;

template <typename V, typename E>
class DiGraph
{
    private:
        vector<bool> valid;
        vector<V> vertexData;
        vector<std::unordered_map<int, E>> edgeData;

    public:
        int vertexCount = 0, edgeCount = 0;

        // get vertex count 
        int getOrder() const {
            return vertexCount;
        }
        
        // get edge count
        int getSize() const {
            return edgeCount;
        }

        // get valid vertices
        auto getValidVertices() const {
            vector<int> result;
            for (int i = 0; i < valid.size(); ++i) {
                if (valid[i])
                    result.push_back(i);
            }
            return result;
        }

        // get invalid vertices
        auto getInvalidVertices() const {
            vector<int> result;
            for (int i = 0; i < valid.size(); ++i) {
                if (!valid[i])
                    result.push_back(i);
            }
            return result;
        }

        // does vertex exist?
        bool hasVertex(int u) const {
            return u < vertexCount && valid[u];
        }

        // does edge exist between u and v?
        bool hasEdge(int u, int v) const {
            return u < vertexCount && edgeData[u].count(v) > 0;
        }

        // Get in-degree of a vertex
        int getInDegree(int u) const {
            if (!hasVertex(u))
                return 0;

            int degree = 0;
            for (int v = 0; v < vertexCount; ++v) {
                if (edgeData[v].count(u) > 0)
                    ++degree;
            }
            return degree;
        }

        // get out-degree of a vertex
        int getOutDegree(int u) const {
            return u < vertexCount ? edgeData[u].size() : 0;
        }

        // get incident edges of a vertex
        std::vector<int> getInEdges(int u) const {
            std::vector<int> result;
            if (!hasVertex(u))
                return result; 

            for (int v = 0; v < vertexCount; ++v) {
                if (edgeData[v].count(u) > 0)
                    result.push_back(v);
            }
            return result;
        }

        // get Outgoing edges of a vertex
        std::vector<int> getOutEdges(int u) const {
            if (!hasVertex(u))
                return std::vector<int>();
            
            std::vector<int> result;
            for (const auto& pair : edgeData[u])
                result.push_back(pair.first);
            
            return result;
        }

        // Clear DiGraph
        void clear() {
            valid.clear();
            vertexData.clear();
            edgeData.clear();
            vertexCount = 0;
            edgeCount = 0;
        }

        // Add vertex
        void addVertex(const V& newVertex = V()) {
            valid.push_back(true);
            vertexData.push_back(newVertex);
            edgeData.emplace_back(std::unordered_map<int, E>());
            vertexCount++;
        }

        // Add edge
        void addEdge(int u, int v, const E& newEdge = E()) {
            if (!hasVertex(u) || !hasVertex(v))
                return;
            edgeData[u].emplace(v, newEdge);
            edgeCount++;
        }

        // Add edge if it doesnt already exist
        void addEdgeChecked(int u, int v, const E& newEdge = E()) {
            if (!hasVertex(u) || !hasVertex(v) || hasEdge(u, v))
                return;
            edgeData[u].emplace(v, newEdge);
            edgeCount++;
        }

        // Remove edge
        void removeEdge(int u, int v) {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return;
            edgeData[u].erase(v);
            edgeCount--;
        }

        // Remove incident edges
        void removeIncidentEdges(int u) {
            if (!hasVertex(u)) 
                return;
            for (int v : getInEdges(u))
            removeEdge(v, u);
        }

        // Remove outgoing edges
        void removeOutgoingEdges(int u) {
            if (!hasVertex(u))
                return;
            edgeCount -= edgeData[u].size();
            edgeData[u].clear();
        }

        // Remove vertex
        void removeVertex(int u) {
            if (!hasVertex(u))
                return;
            valid[u] = false;
            vertexData[u] = V();
            removeIncidentEdges(u);
            removeOutgoingEdges(u);
            vertexCount--;
        }

        // Get vertex data
        V getVertexData(int u) const {
            if (!hasVertex(u))
                return V();
            return vertexData[u];
        }

        // Set vertex data
        void setVertexData(int u, const V& data) {
            if (!hasVertex(u))
                return;
            vertexData[u] = data;
        }

        // Get edge data
        E getEdgeData(int u, int v) const {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return E();
            return edgeData[u].at(v);
        }

        // Set edge data
        void setEdgeData(int u, int v, const E& data) {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return;
            edgeData[u][v] = data;
        }

        // Get all edges
        vector<pair<int, int>> getAllEdges() const {
            vector<pair<int, int>> edges;
            for (int u = 0; u < vertexCount; ++u) {
                if (!valid[u])
                    continue;
                for (int v : getOutEdges(u)) {
                    edges.push_back({u, v});
                }
            }
            return edges;
        }

        friend std::ostream& operator<<(std::ostream& os, const DiGraph<V, E>& graph) {
            for (int u = 0; u < graph.vertexCount; ++u) {
                if (!graph.valid[u])
                    continue;
                os << "Vertex " << u << ": " << graph.vertexData[u] << '\n';
                os << "  Outgoing edges: ";
                std::vector<int> outEdges = graph.getOutEdges(u);
                for (int v : outEdges) {
                    os << "(" << u << ", " << v << ", " << graph.edgeData[u].at(v) << ") ";
                }
                os << '\n';
                os << "  Incoming edges: ";
                auto inEdges = graph.getInEdges(u);
                for (auto it = inEdges.begin(); it != inEdges.end(); ++it) {
                    int v = *it;
                    os << "(" << v << ", " << u << ", " << graph.edgeData[v].at(u) << ") ";
                }
                os << '\n';
            }
            return os;
        }
};      



