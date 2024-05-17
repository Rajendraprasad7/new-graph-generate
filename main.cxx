#include "src/main.hxx"

using namespace std;

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
    }

    DiGraph<int, int> graph = handleFile<int, int>(argc, argv);


    cout << graph.getSize() << ' ' << graph.getOrder() << endl;

    // cout << breadthFirstSearch(graph, 999).size() << endl;

    // GraphDelta<int, int> del; 

    // del.generateDelta(graph, 0, 1000);

    // del.printDelta();

    // // graph.print();

    // del.applyDelta(graph);
    
    // graph.print();
    // cout << graph.getSize() << ' ' << graph.getOrder() << endl;

    // int inEdges = 0, outEdges = 0;

    // for(int i=0; i<graph.getOrder(); i++)
    // {
    //     inEdges += graph.getInDegree(i);
    //     outEdges += graph.getOutDegree(i);
    // }

    // cout << "Indegree: " << inEdges << ' ' << "Outdegree: " << outEdges << endl;
    
    // for (int i=0; i<graph.getOrder(); i++)
    // {
    //     cout << graph.getOutDegree(i) << endl;
    // }
    
    // for (auto it: graph.getOutEdges(0))
    // {
    //     cout<<it<<endl;
    // }

    // int startVertex = 999;
    // std::vector<int> bfsOrder = breadthFirstSearch(graph, startVertex);
    // std::cout << "BFS starting from vertex " << startVertex << ":\n";
    // for (int v : bfsOrder) {
    //     std::cout << v << " ";
    // }
    // std::cout << std::endl;

    // long long timeTaken = measureTime(breadthFirstSearch<int, int>, graph, 0);
    // std::cout << "Time taken: " << timeTaken << " milliseconds" << std::endl;

    // long long timeTaken = measureTime([&graph](){ graph.print(); });
    // std::cout << "Time taken to print the graph: " << timeTaken << " milliseconds" << std::endl;


    // DiGraph<int, int> graph;

    // // Adding vertices
    // graph.addVertex(0);
    // graph.addVertex(1); 
    // graph.addVertex(2);
    // graph.addVertex(3); 
    // graph.addVertex(4);
    // graph.addVertex(5);

    // // Adding edges
    // graph.addEdge(4, 2, 1);
    // graph.addEdge(0, 5, 1);
    // graph.addEdge(1, 2, 1);
    // graph.addEdge(0, 0, 1);
    // graph.addEdge(2, 4, 1);
    // graph.addEdge(3, 2, 1);

    // GraphDelta<int, int> del; 

    // std::cout<<graph;
    // // del.generateMixedDelta(graph, 0, 3, true);
    // del.generatePreferentialAttachmentDelta(graph, 10, false, 0, 1, 0);

    // // del.printDelta();

    // std::cout<<del;

    // del.applyCurrentDelta(graph);

    // std::cout<<graph;

    // std::cout<<del;
    // // Testing vertex and edge existence
    // std::cout << "Vertex 2 exists: " << graph.hasVertex(2) << std::endl;
    // std::cout << "Edge (2, 3) exists: " << graph.hasEdge(2, 3) << std::endl;
    // std::cout << "Edge (3, 2) exists: " << graph.hasEdge(3, 2) << std::endl;

    // // Testing getInDegree and getOutDegree
    // std::cout << "In-degree of vertex 1: " << graph.getInDegree(1) << std::endl;
    // std::cout << "Out-degree of vertex 3: " << graph.getOutDegree(3) << std::endl;

    // Testing BFS
    // int startVertex = 0;
    // std::vector<int> bfsOrder = breadthFirstSearch(graph, startVertex);
    // std::cout << "BFS starting from vertex " << startVertex << ":\n";
    // for (int v : bfsOrder) {
    //     std::cout << v << " ";
    // }
    // std::cout << std::endl;

    // // Testing vertex data functions
    // graph.setVertexData(2, 42);
    // std::cout << "Vertex data for vertex 2: " << graph.getVertexData(2) << std::endl;

    // // Testing edge data functions
    // graph.setEdgeData(2, 3, 99);
    // std::cout << "Edge data for edge (2, 3): " << graph.getEdgeData(2, 3) << std::endl;

    // // Testing getOrder and getSize
    // std::cout << "Graph order (vertex count): " << graph.getOrder() << std::endl;
    // std::cout << "Graph size (edge count): " << graph.getSize() << std::endl;

    // // Testing getValidVertices
    // std::vector<int> validVertices = graph.getValidVertices();
    // std::cout << "Valid vertices: ";
    // for (int v : validVertices) {
    //     std::cout << v << " ";
    // }
    // std::cout << std::endl;

    // //Testing print 
    // graph.print();

    // // Testing removeEdge
    // graph.removeEdge(2, 3);
    // std::cout << "Edge (2, 3) exists after removal: " << graph.hasEdge(2, 3) << std::endl;

    // // Testing removeVertex
    // graph.removeVertex(3);
    // std::cout << "Vertex 3 exists after removal: " << graph.hasVertex(3) << std::endl;
    // graph.print();

    // // Testing clear
    // graph.clear();
    // std::cout << "Graph order after clear: " << graph.getOrder() << std::endl;
    // std::cout << "Graph size after clear: " << graph.getSize() << std::endl;

    // DiGraph<int, int> graph;

    // for(int i=0; i<100000; i++)
    // {
    //     graph.addVertex(i);
    // }

    // for(int i=1; i<100000; i++)
    // {
    //     graph.addEdge(0, i, 69);
    // }

    // std::cout<< "outDegree of 0: " << graph.getOutDegree(0) << endl;

    // vector<int> outEdges = graph.getOutEdges(0);

    // for(auto i: outEdges)
    // {
    //     std::cout << i << std::endl;
    // }

    // vector<int> trav = breadthFirstSearch(graph, 0);

    // for(auto i: trav)
    // {
    //     std::cout << i << std::endl;
    // }

    // std::cout << graph.getOrder() << ' ' << graph.getSize() << std::endl;
    return 0;
}