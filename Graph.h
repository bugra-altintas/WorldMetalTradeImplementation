#include <list>
#include <stack>
#include <queue>
#include <vector>

#include "HashTable.h"
#include "Edge.h"


/** An adjacency list representation of a directed weighted graph. */
class Graph {

public:
    Graph();
    Graph(const Graph& rhs);
    Graph& operator=(const Graph& rhs);
    ~Graph();
    /** Adds the given node to the graph with vid or country as key,
     * and an empty edge list as value */
    void addNode(const Node& node);
    /** Adds a new edge to the edge list of headNode using tailNode and import data */
    void addConnection(const Node& headNode, const Node& tailNode, int import);
    /** Given a country, returns all adjacent
     * countries of the given country as a list of Node
     * Throw ItemNotFoundException, if the given node does not exist */
    list<Node> getAdjacentNodes(const Node& node);
    /** For the given node, returns the sum of imports */
    long getTotalImports(const Node& node);
    /** Deletes the given country from the graph
     * together with its incident connections(edges) */
    void deleteNode(const Node& node);
    /** Given two countries , returns the least cost path/route between
     * them using import values as the edge weight */
    list<string> findLeastCostPath(const Node& srcNode, const Node& destNode);
    /** Detect whether the graph contains any cycle */
    bool isCyclic();
    /** returns the BFS path from srcNode to destNode as a list of country names */
    list<string> getBFSPath(const Node& srcNode, const Node& destNode);
	
private:
    // Adjacency list representation of the graph
    HashTable<string, list<Edge>> adjList;
	int max_int = -1;
	int get_max();
    // Operator () overloading that compare to egdes by their import values
	struct edge_comparator{
		bool operator()(Edge& edge1, Edge& edge2){
			return edge1.getImport() > edge2.getImport();
		}
	};
	bool hasPath(const string& src,const string& dest,HashTable<string,bool>& visited);
};
// End of header file
