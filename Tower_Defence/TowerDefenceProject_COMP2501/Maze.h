#ifndef MAZE_H
#define MAZE_H

using namespace std;

#include "Node.h"
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <stack>
#include <fstream>

//struct used to rank various nodes in the priority_queue
struct QNode {
	Node *node;
	int cost;
};

class Maze
{
public:
	Maze(int, int, string);
	
	//finds path from start point to end and returns the path as a stack of nodes
	std::stack<Node*> pathFromPointToPoint(glm::ivec2, glm::ivec2);
	std::stack<Node*> getEnemyPath();
	std::stack<Node*> getWizardPath();
	Node *Maze::getNodeFromCoords(double _x, double _y);

	inline Node *getNodeByPosition(glm::ivec2 p) { return &nodes[p.x][p.y]; };
	inline Node *getNodeByID(int id) { return nodeMap.at(id); };

private:
	//2d vector containing all the nodes in the graph
	vector<vector<Node>> nodes;
	//map used for easy access to nodes via id
	map<int, Node*> nodeMap;


	//maze start and end
	vector<glm::ivec2> mazeStarts;
	vector<glm::ivec2> mazeEnds;

};
#endif // !MAZE_H
