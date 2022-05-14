#include "bots.h"
#include <iostream>

struct nodeCostStruct {
	int x = 0;
	int y = 0;
	float cost = 1000000.0f;
};

struct coordsStruct {
	int x = 0;
	int y = 0;
};


cDijkstra::cDijkstra() {
	completed = false;
};

cDijkstra::~cDijkstra() {};


bool cDijkstra::getInPath(int x, int y) const {
	return graph[x][y].inPath;
}

bool cDijkstra::getClosed(int x, int y) const {
	return graph[x][y].closed;
}


void cDijkstra::Build(cBotBase& bot) {
	graph[bot.PositionX()][bot.PositionY()].cost = 0;
 
	nodeCostStruct node;
	nodeCostStruct neighbourDeltas[8]
		= { {-1, -1, 1.4f},
			{0, -1, 1.0f},
			{1, -1, 1.4f},
			{1, 0, 1.0f},
			{1, 1, 1.4f},
			{0, 1, 1.0f},
			{-1, 1, 1.4f},
			{-1, 0, 1.0f} };  // Starting top left, goes clockwise.
	bool flag = false;
	
	while (!flag) {
		node.cost = 1000000.0f;
		//std::cout << "entering while !flag" << std::endl;
		for (int x = 0; x < GRIDWIDTH; x++) {
			//std::cout << "x" << std::endl;
			//std::cout << "entering x " << x << "\n = = = = = = = = = = = = = = = = = = \n = = = = = = = = = = = = = = = = = = \n = = = = = = = = = = = = = = = = = = \n = = = = = = = = = = = = = = = = = = \n = = = = = = = = = = = = = = = = = = \n = = = = = = = = = = = = = = = = = = \n = = = = = = = = = = = = = = = = = = " << std::endl;
			for (int y = 0; y < GRIDHEIGHT; y++) {
				//std::cout << "entering y" << y << std::endl;
				//std::cout << "valid: " << gLevel.isValid(x, y) << std::endl;
				///std::cout << "closed: " << graph[x][y].closed << std::endl;
				//std::cout << "cost: " << graph[x][y].cost << std::endl;
				// 1. Find the grid location with the lowest cost, that is not closed, and is not blocked on the map layout.
				if (!gLevel.isValid(x, y)) { continue; }  // Has to be a valid location.
				if (graph[x][y].closed == true) { continue; }  // Has to be an open location.
				//if (x == 13 && y == 20) {
					//std::cout << "here" << std::endl;
				//}
				if (graph[x][y].cost < node.cost) {
					node.x = x;
					node.y = y;
					node.cost = graph[x][y].cost;
					//rootNode = (node){.x = y, .y = x, .cost = cost};  // Not running C99?
				}
			}
		}
		// node --> current lowest cost node
		// 2. Set this location as closed.
		graph[node.x][node.y].closed = true;
		//std::cout << "x: " << node.x << ". PositionX: " << gTarget.PositionX() << ". y: " << node.y << ". PositionY: " << gTarget.PositionY() << std::endl;
		if ((node.x == gTarget.PositionX()) && (node.y == gTarget.PositionY())) {
			flag = true;
		}
		// 3. Calculate new cost for 8 neighbours. parent+neighbour 1 and 1.4 diag
		for (int index = 0; index < sizeof(neighbourDeltas) / sizeof(*neighbourDeltas); index++) {
			//std::cout << "entering neighbours" << std::endl;
			int tmpX = node.x + neighbourDeltas[index].x;
			int tmpY = node.y + neighbourDeltas[index].y;
			if (!gLevel.isValid(tmpX, tmpY)) { continue; }
			if (graph[tmpX][tmpY].closed == true) { continue; }
			float newCost = graph[node.x][node.y].cost + neighbourDeltas[index].cost;
			// If cost for neighbour is lower than current, update and set link X and Y.
			if (newCost < graph[tmpX][tmpY].cost) {
				graph[tmpX][tmpY].cost = newCost;
				graph[tmpX][tmpY].linkX = node.x;
				graph[tmpX][tmpY].linkY = node.y;
				std::cout << "lowest cost node: x=" << node.x << ", y=" << node.y << std::endl;
			}
		}
	}

	bool done = false; //set to true when we are back at the bot position
	int nextClosedX = gTarget.PositionX(); //start of path
	int nextClosedY = gTarget.PositionY(); //start of path
	while (!done)
	{
		graph[nextClosedX][nextClosedY].inPath = true;
		int tmpX = nextClosedX;
		int tmpY = nextClosedY;
		nextClosedX = graph[tmpX][tmpY].linkX;
		nextClosedY = graph[tmpX][tmpY].linkY;
		if ((nextClosedX == bot.PositionX()) && (nextClosedY == bot.PositionY())) {
			done = true;
		}
	}

	completed = true;

}

cDijkstra gDijkstra;