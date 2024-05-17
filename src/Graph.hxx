#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
// #include "AVLTree.hxx"

using std::vector;
using std::pair;

/**
 * @class DiGraph
 * @brief Represents a directed graph.
 * @tparam V The type of data stored in each vertex.
 * @tparam E The type of data stored in each edge.
 */
template <typename V, typename E>
class DiGraph
{
    private:
        vector<bool> valid;
        vector<V> vertexData;
        vector<std::unordered_map<int, E>> edgeData;

    public:
        int vertexCount = 0, edgeCount = 0;

        /**
         * @brief Get the number of vertices in the graph.
         * @return The number of vertices in the graph.
         */
        int getOrder() const {
            return vertexCount;
        }
        
        /**
         * @brief Get the number of edges in the graph.
         * @return The number of edges in the graph.
         */
        int getSize() const {
            return edgeCount;
        }

        /**
         * @brief Get the indices of the valid vertices in the graph.
         * @return A vector containing the indices of the valid vertices.
         */
        auto getValidVertices() const {
            vector<int> result;
            for (int i = 0; i < valid.size(); ++i) {
                if (valid[i])
                    result.push_back(i);
            }
            return result;
        }

        /**
         * @brief Get the indices of the invalid vertices in the graph.
         * @return A vector containing the indices of the invalid vertices.
         */
        auto getInvalidVertices() const {
            vector<int> result;
            for (int i = 0; i < valid.size(); ++i) {
                if (!valid[i])
                    result.push_back(i);
            }
            return result;
        }

        /**
         * @brief Check if a vertex with the given index exists in the graph.
         * @param u The index of the vertex to check.
         * @return True if the vertex exists, false otherwise.
         */
        bool hasVertex(int u) const {
            return u < vertexCount && valid[u];
        }

        /**
         * @brief Check if an edge exists between two vertices.
         * @param u The index of the first vertex.
         * @param v The index of the second vertex.
         * @return True if the edge exists, false otherwise.
         */
        bool hasEdge(int u, int v) const {
            return u < vertexCount && edgeData[u].count(v) > 0;
        }

        /**
         * @brief Get the in-degree of a vertex.
         * @param u The index of the vertex.
         * @return The in-degree of the vertex.
         */
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

        /**
         * @brief Get the out-degree of a vertex.
         * @param u The index of the vertex.
         * @return The out-degree of the vertex.
         */
        int getOutDegree(int u) const {
            return u < vertexCount ? edgeData[u].size() : 0;
        }

        /**
         * @brief Get the indices of the vertices that have an edge directed towards the given vertex.
         * @param u The index of the vertex.
         * @return A vector containing the indices of the vertices that have an edge directed towards the given vertex.
         */
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

        /**
         * @brief Get the indices of the vertices that have an edge directed away from the given vertex.
         * @param u The index of the vertex.
         * @return A vector containing the indices of the vertices that have an edge directed away from the given vertex.
         */
        std::vector<int> getOutEdges(int u) const {
            if (!hasVertex(u))
                return std::vector<int>();
            std::vector<int> result;
            for (const auto& pair : edgeData[u])
                result.push_back(pair.first);
            return result;
        }

        /**
         * @brief Clear the graph by removing all vertices and edges.
         */
        void clear() {
            valid.clear();
            vertexData.clear();
            edgeData.clear();
            vertexCount = 0;
            edgeCount = 0;
        }

        /**
         * @brief Add a new vertex to the graph.
         * @param newVertex The data associated with the new vertex.
         */
        void addVertex(const V& newVertex = V()) {
            valid.push_back(true);
            vertexData.push_back(newVertex);
            edgeData.emplace_back(std::unordered_map<int, E>());
            vertexCount++;
        }

        /**
         * @brief Add a new edge between two vertices in the graph.
         * @param u The index of the first vertex.
         * @param v The index of the second vertex.
         * @param newEdge The data associated with the new edge.
         */
        void addEdge(int u, int v, const E& newEdge = E()) {
            if (!hasVertex(u) || !hasVertex(v))
                return;
            edgeData[u].emplace(v, newEdge);
            edgeCount++;
        }

        /**
         * @brief Add a new edge between two vertices in the graph if it doesn't already exist.
         * @param u The index of the first vertex.
         * @param v The index of the second vertex.
         * @param newEdge The data associated with the new edge.
         */
        void addEdgeChecked(int u, int v, const E& newEdge = E()) {
            if (!hasVertex(u) || !hasVertex(v) || hasEdge(u, v))
                return;
            edgeData[u].emplace(v, newEdge);
            edgeCount++;
        }

        /**
         * @brief Remove an edge between two vertices in the graph.
         * @param u The index of the first vertex.
         * @param v The index of the second vertex.
         */
        void removeEdge(int u, int v) {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return;
            edgeData[u].erase(v);
            edgeCount--;
        }

        /**
         * @brief Remove all edges directed towards a given vertex.
         * @param u The index of the vertex.
         */
        void removeIncidentEdges(int u) {
            if (!hasVertex(u)) 
                return;
            for (int v : getInEdges(u))
            removeEdge(v, u);
        }

        /**
         * @brief Remove all edges directed away from a given vertex.
         * @param u The index of the vertex.
         */
        void removeOutgoingEdges(int u) {
            if (!hasVertex(u))
                return;
            edgeCount -= edgeData[u].size();
            edgeData[u].clear();
        }

        /**
         * @brief Remove a vertex from the graph.
         * @param u The index of the vertex.
         */
        void removeVertex(int u) {
            if (!hasVertex(u))
                return;
            valid[u] = false;
            vertexData[u] = V();
            removeIncidentEdges(u);
            removeOutgoingEdges(u);
            vertexCount--;
        }

        /**
         * @brief Get the data associated with a vertex.
         * @param u The index of the vertex.
         * @return The data associated with the vertex.
         */
        V getVertexData(int u) const {
            if (!hasVertex(u))
                return V();
            return vertexData[u];
        }

        /**
         * @brief Set the data associated with a vertex.
         * @param u The index of the vertex.
         * @param data The new data to associate with the vertex.
         */
        void setVertexData(int u, const V& data) {
            if (!hasVertex(u))
                return;
            vertexData[u] = data;
        }

        /**
         * @brief Get the data associated with an edge between two vertices.
         * @param u The index of the first vertex.
         * @param v The index of the second vertex.
         * @return The data associated with the edge.
         */
        E getEdgeData(int u, int v) const {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return E();
            return edgeData[u].at(v);
        }

        /**
         * @brief Set the data associated with an edge between two vertices.
         * @param u The index of the first vertex.
         * @param v The index of the second vertex.
         * @param data The new data to associate with the edge.
         */
        void setEdgeData(int u, int v, const E& data) {
            if (!hasVertex(u) || !hasVertex(v) || !hasEdge(u, v))
                return;
            edgeData[u][v] = data;
        }

        /**
         * @brief Get all the edges in the graph.
         * @return A vector containing pairs of vertex indices representing the edges.
         */
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

        /**
         * @brief Overload the << operator to print the graph.
         * @param os The output stream.
         * @param graph The graph to print.
         * @return The output stream.
         */
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



