#ifndef NODE_H
#define NODE_H

using namespace std;

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class Node;

struct Edge {
	Node& n1;
	Node& n2;
	int cost;
};

class Node {
public:
	Node(glm::vec3, bool);

	//connects two nodes with an edge of 'edgeCost'
	void addNode(Node& n);
	//links two nodes together via and edge. Updates both nodes edges vector
	inline void addEdge(Edge e) { edges.push_back(e); }
	//determines the opposing node on an edge.
	//if the edge does not contain the current node, it will return the current node
	Node getOtherNode(Edge e);

	//setters
	inline void setCost(int c) { cost = c; }
	inline void setPrev(Node* n) { prev = n; }
	bool isOccupied();

	//getters
	inline glm::vec3 getPosition() { return position; };
	inline int getId() { return id; }
	inline int getCost() const { return cost; }
	inline Node *getPrev() { return prev; }
	inline vector<Edge> getEdges() { return edges; }
	inline void setTowerHere(bool b) { towerHere = b; }

	inline int getWeight() { return weights; }

private:
	//vector containing all edges the connect connects to.
	//this can be used to create a graph with any number of connectivity
	vector<Edge> edges;
	const int id;
	glm::vec3 position;
	int cost;
	Node *prev;
	bool towerHere;
	int weights;
	bool passable;

};
#endif // !NODE_H

