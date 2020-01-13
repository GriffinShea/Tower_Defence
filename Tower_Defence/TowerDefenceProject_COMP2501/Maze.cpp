#include "Maze.h"

using namespace std;

//class used to compare 2 nodeTuple structs
class compareNode
{
public:
	int operator() (const QNode &n1, const QNode &n2)
	{
		return n1.cost > n2.cost;
	}
};

//main constructor
//takes the width, height of graph, as well as a gameobject used to render each node.
Maze::Maze(int width, int height, string mazeFilePath)
{
	//initializes the 2d nodes array and nodeMap
	nodes = vector<vector<Node>>();
	nodeMap = map<int, Node*>();

	//data for setting node positions on, screen. This works best for a 40x30 graph
	//TODO change these values based on graph size.
	float x_b = -1.0f;
	float y_b = -1.0f;
	float movementX = - x_b / width * 2;
	float movementY = - y_b / height * 2;

	//load maze file
	ifstream mazeFile(mazeFilePath);
	string line;

	//fills the 2d nodes array with nodes.
	for (int i = 0; i < height; i++) {
		vector<Node> nodeRow = vector<Node>();
		getline(mazeFile, line);

		for (int j = 0; j < width; j++) {
			//creates each node, starting at (-4.4, 4.4), topLeft, going down, right
			//with x distance .22, y distance .3
			//std::cout << x_b + j * movementX << "," << y_b + i * movementY << "\n";
			Node newNode = Node(glm::vec3(x_b + j * movementX, y_b + i * movementY, 0), line[j] != '1');//line[j] != '1'returns true if this node should be passable
			nodeRow.push_back(newNode);

			//if this point is a start or end append to the relevant vector
			if (line[j] == 's') {
				mazeStarts.push_back(glm::ivec2(i, j));
			} else if (line[j] == 'e') {
				mazeEnds.push_back(glm::ivec2(i, j));
			}

		}
		nodes.push_back(nodeRow);
	}

	//connects node to each other to form a 4-connected graph with random edge weights
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {

			//if there exists a node to the right of the current node, link them together
			if (j + 1 < nodes.at(i).size()) {
				int randWeight = 10 + (rand() % 6);	//creates a random weight between 10-15

				Node *n1 = &nodes.at(i).at(j);		//referncec to current node in graph.
				Node *n2 = &nodes.at(i).at(j + 1);	//reference to node to the left of the current node.

				n1->addNode(*n2);			//links both nodes together
			}

			//if there exists a node below the current node, link them together
			if (i + 1 < nodes.size()) {
				Node *n1 = &nodes.at(i).at(j);		//referncec to current node in graph.
				Node *n2 = &nodes.at(i + 1).at(j);	//node below the current node.

				n1->addNode(*n2);			//links both nodes together
			}
		}
	}

	//adds all nodes to map with nodeId as key and a pointer to the node
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			nodeMap.insert(pair<int, Node*>(nodes.at(i).at(j).getId(), &nodes.at(i).at(j)));
		}
	}
}

std::stack<Node*> Maze::pathFromPointToPoint(glm::ivec2 start, glm::ivec2 end) {
	//priority queue used in pathfinding.
	//it is created using the NodeTuple struct with a min compare function called compareNode
	priority_queue <QNode, vector<QNode>, compareNode> pq;

	//sets the costs of all nodes to infinity. reset all nodes to be off-path, and unsearched
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.at(i).size(); j++) {
			nodes.at(i).at(j).setCost(INT_MAX);
		}
	}

	//The startnode is added to the pq with cost 0, and marked as searched
	QNode startNode = { getNodeByPosition(start), 0 };
	pq.push(startNode);

	//now that the pq is setup, we can start the algorithm
	//keep in mind that the as the QNode struct has a pointer to the corresponding node
	//some function calls will use pointer syntax (->) 

	//for (int i = 0; i < height; i++) { for (int j = 0; j < height; j++) { cout << getNodeByPosition(glm::ivec2(j, i))->getId(); } }

	while (!pq.empty()) {
		//get the current lowest-cost node in pq
		QNode lowest = pq.top();

		//if the current node is the end node, done!
		if (lowest.node->getId() == getNodeByPosition(end)->getId()) {
			break;
		}

		//OPEN NODE
		vector<Edge> neighbours = lowest.node->getEdges();
		for (int i = 0; i < neighbours.size(); i++) {

			//compute cost to get to neighbouring node
			//cost = the cost to get the corrent node + cost to traverse the edge

			Node *n = getNodeByID(lowest.node->getOtherNode(neighbours.at(i)).getId());
			//Node *n = &(lowest.node->getOtherNode(neighbours.at(i)));

			//computer manhattan distance for A* estimate and calculate node cost
			float manhattan = glm::distance(lowest.node->getPosition(), getNodeByPosition(end)->getPosition());
			int nodeCost = lowest.cost + manhattan + neighbours.at(i).cost;

			//if current node cost is higher than calculated, update node, and add QNode to queue, and mark it as searched
			if (n->getCost() > nodeCost) {
				n->setCost(nodeCost);
				n->setPrev(lowest.node);

				QNode updatedNode = { n, nodeCost };
				pq.push(updatedNode);
			}
		}

		pq.pop();	//REMOVE NODE FROM QUEUE
	}

	//queue is done, go in reverse from END to START to determine path
	Node* currentNode = getNodeByPosition(end);
	//while the current node isn't null, or the begining of the path, add the node to a stack
	stack<Node*> returnStack;
	while (currentNode != NULL && currentNode->getId() != getNodeByPosition(start)->getId()) {
		returnStack.push(currentNode);
		currentNode = currentNode->getPrev();
	}

	return returnStack;
}

std::stack<Node*> Maze::getEnemyPath() {
	//path picked randomly from the pools of starts and ends
	return pathFromPointToPoint(mazeStarts[(rand() % mazeStarts.size())], mazeEnds[(rand() % mazeEnds.size())]);
}

std::stack<Node*> Maze::getWizardPath() {
	//path picked randomly from the pools of ends and starts
	return pathFromPointToPoint(mazeEnds[(rand() % mazeEnds.size())], mazeStarts[(rand() % mazeStarts.size())]);
}

//returns the id of the node at the screen coordinates. If no node exists, it will return -1
Node *Maze::getNodeFromCoords(double _x, double _y) {
	float start_x = -1.0f;
	float start_y = -1.0f;
	float movementX = 0.05;
	float movementY = 0.06666667;

	start_x -= movementX * .5;
	start_y -= movementY * .5;

	int x = (int)((_x - start_x) / movementX);
	int y = (int)((_y - start_y) / movementY);

	//check if the node is 
	if (y >= nodes.size() || y < 0) {
		return nullptr;
	}
	else if (x >= nodes.at(y).size() || x < 0) {
		return nullptr;
	}
	else {
		return &nodes.at(y).at(x);
	}
}