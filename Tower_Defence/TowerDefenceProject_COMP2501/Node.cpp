#include "Node.h"

//static id, used when a new node is created.
static int globalId = 0;

Node::Node(glm::vec3 p, bool pa) : id(globalId++) {
	position = p;
	passable = pa;
	if (!passable) {
		weights = 1000000;
	}
	else {
		weights = 1;
	}
	towerHere = false;
}

//determines the opposing node on an edge.
//if the edge does not contain the current node, it will return the current node
Node Node::getOtherNode(Edge e) {
	if (e.n1.getId() == getId()) {
		return e.n2;
	}
	else if (e.n2.getId() == getId()) {
		return e.n1;
	}
	else {
		return *this;
	}
}

//links two nodes together via and edge. Updates both nodes edges vector
void Node::addNode(Node& n) {
	//creates an edge corresponding to nodes
	Edge e = { *this, n, weights };

	//adds the edge to the vector in both nodes.
	(*this).addEdge(e);
	n.addEdge(e);
}

//checks if any adjacent nodes have a tower or if this node is on the path
//if either return true because there should not be a tower here
bool Node::isOccupied() {
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i].n1.getId() == getId()) {
			if (edges[i].n2.towerHere) { return true; }
		} else {
			if (edges[i].n1.towerHere) { return true; }
		}
	}
	if (passable) { return true; }
	return false;
}