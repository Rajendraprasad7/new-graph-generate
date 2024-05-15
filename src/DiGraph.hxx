#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include "AVLTree.hxx"

using std::vector;
using std::pair;

template <typename V, typename E>
class DiGraph
{
    private:
        vector<bool> valid;
        vector<V> vertexData;
        vector< AVLTree<E> > edgeData;

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
            return u < vertexCount && edgeData[u].has(v);
        }

        // Get in-degree of a vertex
        int getInDegree(int u) const {
            if (!hasVertex(u))
                return 0;

            int degree = 0;
            for (int v = 0; v < vertexCount; ++v) {
                if (edgeData[v].has(u))
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
                if (edgeData[v].has(u))
                    result.push_back(v);
            }
            return result;
        }

        // get Outgoing edges of a vertex
        std::vector<int> getOutEdges(int u) const {
            if (!hasVertex(u))
                return std::vector<int>();
            return edgeData[u].getKeys();
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
            edgeData.emplace_back(AVLTree<E>());
            vertexCount++;
        }

        // Add edge
        void addEdge(int u, int v, const E& newEdge = E()) {
            if (!hasVertex(u) || !hasVertex(v))
                return;
            edgeData[u].insertKey(v, newEdge);
            edgeCount++;
        }

        // Add edge only if it doesnt already exist
        void addEdgeChecked(int u, int v, const E& newEdge = E()) {
            if (!hasVertex(u) || !hasVertex(v) || hasEdge(u, v))
                return;
            edgeData[u].insertKey(v, newEdge);
            edgeCount++;
        }

        // Remove edge
        void removeEdge(int u, int v) {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return;
            edgeData[u].removeKey(v);
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
            return edgeData[u].getValue(v);
        }

        // Set edge data
        void setEdgeData(int u, int v, const E& data) {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return;
            edgeData[u].setValue(v, data);
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

        // Print DiGraph
        void print() const {
            for (int u = 0; u < vertexCount; ++u) {
                if (!valid[u])
                    continue;
                std::cout << "Vertex " << u << ": " << vertexData[u] << '\n';
                std::cout << "  Outgoing edges: ";
                std::vector<int> outEdges = getOutEdges(u);
                for (int v : outEdges) {
                    std::cout << "(" << u << ", " << v << ", " << edgeData[u].getValue(v) << ") ";
                }
                std::cout << '\n';
                std::cout << "  Incoming edges: ";
                auto inEdges = getInEdges(u);
                for (auto it = inEdges.begin(); it != inEdges.end(); ++it) {
                    int v = *it;
                    std::cout << "(" << v << ", " << u << ", " << edgeData[v].getValue(u) << ") ";
                }
                std::cout << '\n';
            }
        }
};      



