#include "bots.h"


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
	nodeCostStruct neighbourDeltas[] = { (-1,-1, 1.4f), (0, -1, 1), (1, -1, 1.4f), (1, 0, 1), (1, 1, 1.4f), (0, 1, 1), (-1, 1, 1.4f), (-1, 0, 1) };  // Starting top left, goes clockwise.
	bool flag = false;
	while (!flag) {
		for (int row = 0; row < GRIDHEIGHT; row++) {
			for (int col = 0; col < GRIDWIDTH; col++) {
				// 1. Find the grid location with the lowest cost, that is not closed, and is not blocked on the map layout.
				if (!gLevel.isValid(row, col)) { continue; }
				if (graph[row][col].closed == true) { continue; }
				if (graph[row][col].cost < node.cost) {
					node.x = row;
					node.y = col;
					node.cost = graph[row][col].cost;
					//rootNode = (node){.x = row, .y = col, .cost = cost};  // Not running C99?
					// 2. Set this location as closed.
					graph[row][col].closed = true;
					if ((row == gTarget.PositionX()) && (col == gTarget.PositionY())) { flag = true; }
					// 3. Calculate new cost for 8 neighbours. parent+neighbour 1 and 1.4 diag
					for (int index = 0; index < sizeof(neighbourDeltas) / sizeof(*neighbourDeltas); index++) {
						if (!gLevel.isValid(row + neighbourDeltas[index].x, col + neighbourDeltas[index].y)) { continue; }
						if (graph[row + neighbourDeltas[index].x][col + neighbourDeltas[index].y].closed == true) { continue; }
						float newCost = graph[row][col].cost + neighbourDeltas[index].cost;
						// If cost for neighbour is lower than current, update and set link X and Y.
						if (newCost < graph[row + neighbourDeltas[index].x][col + neighbourDeltas[index].y].cost) {
							graph[row + neighbourDeltas[index].x][col + neighbourDeltas[index].y].cost = newCost;
							graph[row + neighbourDeltas[index].x][col + neighbourDeltas[index].y].linkX = row;
							graph[row + neighbourDeltas[index].x][col + neighbourDeltas[index].y].linkY = col;
						}
					}
				}
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
		if ((nextClosedX == bot.PositionX()) && (nextClosedY == bot.PositionY())) done = true;
	}

	completed = true;

}

cDijkstra gDijkstra;