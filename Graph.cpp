#include "Graph.h"

// Default consructor
Graph::Graph() {
    
}

// Copy constructor
Graph::Graph(const Graph& rhs) {
	adjList = rhs.adjList;
}

// Operator = overloading
Graph& Graph::operator=(const Graph& rhs) { 
    this->adjList = rhs.adjList;
}

// Destructor
Graph::~Graph() {

}

 // A function adds the given node to the graph with country as key, and an empty edge list as value
void Graph::addNode(const Node &node) {
	list<Edge> connections;
	adjList.Insert(node.getCountry(),connections);
}

/* A function adds a new edge with given tailNode and import data to the edge list of 
   headNode by using country as a key.*/
void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {
	Edge newEdge(tailNode,import);
	adjList.Get(headNode.getCountry()).push_back(newEdge);
	if(import > max_int)
		max_int = import;
}

/* A function gets a node as a parameter and returns all adjacent nodes of the given node as a list of
   Node, considering only the given node's adjacency list. Therefore, only the edges that are originated
   from the given node are considered. 
   If the input parameter does not exist in the graph, throw ItemNotFoundException(). */
list<Node> Graph::getAdjacentNodes(const Node& node) {
	list<Node> adjacentNodes;
	list<Edge> connections;
	connections = adjList.Get(node.getCountry());
	while(!connections.empty()){
		adjacentNodes.push_back(connections.front().getTailNode());
		connections.pop_front();
	}
	return adjacentNodes;
}

// A function gets a node as a parameter and returns the total imports from the given node.
long Graph::getTotalImports(const Node& node) {
	list<Edge> connections;
	connections = adjList.Get(node.getCountry());
	int sum = 0;
	while(!connections.empty()){
		sum += connections.front().getImport();
		connections.pop_front();
	}
	return sum;
}

/* A function deletes the given node from the graph together with its incident connections (edges).
Also, it deletes the given node from their adjacency lists, by using "getKeys" function of "HashTable" to 
retrieve all active keys. */
void Graph::deleteNode(const Node& node) {
	string countries[adjList.Size()];
	adjList.getKeys(countries);
	int i;
	for(i=0;i<adjList.Size();i++){
		list<Edge> &connections = adjList.Get(countries[i]);
		int x = connections.size();
		while(x){
			if(connections.front().getTailNode().getCountry() == node.getCountry()){
				connections.pop_front();
			}
			else{	
				connections.push_back(connections.front());
				connections.pop_front();
			}
			x--;
		}
		adjList.Insert(countries[i],connections);
	}
	adjList.Delete(node.getCountry());
}

/* A function gets two nodes as parameter, calculates the least cost path from srcNode to destNode and 
returns the calculated path as a list of country names from srcNode to destNode. Import values are considered
as the edge weights. Implemented by Dijkstra's weighted shortest path algorithm using priority queue. */
list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
	list<string> path;
	HashTable<string, string> previous;
	HashTable<string, int> imports;
	priority_queue<Edge, vector<Edge>,edge_comparator> pq;
	string countries[adjList.Size()];
	int new_imp;
	adjList.getKeys(countries);
	for(int i = 0; i < adjList.Size();i++){
		imports.Insert(countries[i],get_max());
		previous.Insert(countries[i],"-");
	}
	imports.Insert(srcNode.getCountry(),0);
	int size = adjList.Get(srcNode.getCountry()).size();
	
	while(size != 0){
		pq.push(adjList.Get(srcNode.getCountry()).front());
		imports.Insert(adjList.Get(srcNode.getCountry()).front().getTailNode().getCountry(),adjList.Get(srcNode.getCountry()).front().getImport());
		previous.Insert(adjList.Get(srcNode.getCountry()).front().getTailNode().getCountry(),srcNode.getCountry());
		adjList.Get(srcNode.getCountry()).push_back(adjList.Get(srcNode.getCountry()).front());
		adjList.Get(srcNode.getCountry()).pop_front();
		size--;
	}
	while(!pq.empty()){
		string v = pq.top().getTailNode().getCountry();
		pq.pop();
		list<Edge> cons;
		cons = adjList.Get(v);
		while(!cons.empty()){
			Edge w(cons.front().getTailNode(),cons.front().getImport());
			new_imp = imports.Get(v) + w.getImport();
			if(new_imp < imports.Get(w.getTailNode().getCountry())){
				imports.Insert(w.getTailNode().getCountry(),new_imp);
				previous.Insert(w.getTailNode().getCountry(),v);
				w.setImport(new_imp);
				pq.push(w);
			}
			cons.pop_front();
		}
	}
	//I have the path in "previous" HashTable
	string p = destNode.getCountry();
	path.push_front(p);
	p = previous.Get(p);
	while(p != "-"){
		path.push_front(p);
		p = previous.Get(p);
	}
	return path;
}

/* A function detects whether the graph contains any cycle by using utilty function hasPath.
   For each country, it checks whether there is a path from the country to itself.
*/
bool Graph::isCyclic() {
	string countries[adjList.Size()];
	adjList.getKeys(countries);
	for(int i = 0; i<adjList.Size();i++){
		HashTable<string,bool> visited;
		visited.Insert(countries[i],true);
		if(hasPath(countries[i],countries[i],visited)){
			return true;
		}
	}
	return false;
}

// A utility function checks whether there is a path between two given parameter src and dest. If there is, it returns true.
bool Graph::hasPath(const string& src, const string& dest, HashTable<string, bool>& visited){
	list<Edge> connections = adjList.Get(src);
	while(!connections.empty()){
		string v = connections.front().getTailNode().getCountry();
		if(v==dest)
			return true;
		try{
			if(visited.Get(v)){
				connections.pop_front();
				return false;
			}
		}
		catch(ItemNotFoundException err){
			visited.Insert(v,true);
			if(hasPath(v,dest,visited)){
				return true;
			}
			else{
				connections.pop_front();
			}
		}
	}
	return false;
}

/* A function takes srcNode and destNode as parameters and returns the BFS path from srcNode to destNode
   as a list of country names by using queue */
list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {
	list<string> path;
	HashTable<string, bool> mark;
	string b;
	queue<string> q;
	int i,n;
	string neighbour;
	q.push(srcNode.getCountry());
	mark.Insert(srcNode.getCountry(),true);
	while(!q.empty()){
		b = q.front();
		q.pop();
		if(b == destNode.getCountry())//reached to destination
			break;
		n = adjList.Get(b).size();
		for(i=0;i<n;i++){
			neighbour = adjList.Get(b).front().getTailNode().getCountry();
			try{
				if(mark.Get(neighbour)){
				}
			}
			catch(ItemNotFoundException f){
				path.push_back(neighbour);
				if(neighbour == destNode.getCountry()){
					path.push_front(srcNode.getCountry());
					return path;
				}
				mark.Insert(neighbour,true);
				q.push(neighbour);
			}
			adjList.Get(b).push_back(adjList.Get(b).front());
			adjList.Get(b).pop_front();
		}
	}
	return path;
}
int Graph::get_max(){
	return max_int;
}