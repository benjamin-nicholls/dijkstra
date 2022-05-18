#include <list>
#include <cmath>
#include "bots.h"

int gHeuristic = 0;  // Global variable to choose heuristic.

struct nodeCostStruct {
	int x = 0;
	int y = 0;
	float cost = 1000000.0f;
};

//======================================================================================
// Dijkstras
//======================================================================================
bool cDijkstra::getInPath(int x, int y) const {
	return graph[x][y].inPath;
}

bool cDijkstra::getClosed(int x, int y) const {
	return graph[x][y].closed;
}

void cDijkstra::ChooseNextGridPosition() {
	// no implementation 
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
		for (int x = 0; x < GRIDWIDTH; x++) {
			for (int y = 0; y < GRIDHEIGHT; y++) {
				// 1. Find the grid location with the lowest cost, that is not closed, and is not blocked on the map layout.
				if (!gLevel.isValid(x, y)) { continue; }  // Has to be a valid location.
				if (graph[x][y].closed == true) { continue; }  // Has to be an open location.
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
		if ((node.x == gTarget.PositionX()) && (node.y == gTarget.PositionY())) {
			flag = true;
			break;
		}
		// 3. Calculate new cost for 8 neighbours. parent+neighbour 1 and 1.4 diag
		for (int index = 0; index < sizeof(neighbourDeltas) / sizeof(*neighbourDeltas); index++) {
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
				//std::cout << "lowest cost node: x=" << node.x << ", y=" << node.y << std::endl;
			}
		}
	}

	bool done = false;  // Set to true when we are back at the bot position.
	int nextClosedX = gTarget.PositionX();  // Start of path.
	int nextClosedY = gTarget.PositionY();  // Start of path.
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


//======================================================================================
// A STAR IMPLEMENTATION
//======================================================================================

void cAStar::Build(cBotBase& bot) {
	// Reset values.
	for (int x = 0; x < GRIDWIDTH; x++) {
		for (int y = 0; y < GRIDHEIGHT; y++) {
			graph[x][y].reset();
		}
	}
	// Set bot position as starting position.
	graph[bot.PositionX()][bot.PositionY()].cost = 0;
	// Stores lowest cost node coordinates and cost.
	nodeCostStruct node;
	// X and Y offsets for neighbours.
	nodeCostStruct neighbourDeltas[8]
		= { {-1, -1, 1.4f},
			{0, -1, 1.0f},
			{1, -1, 1.4f},
			{1, 0, 1.0f},
			{1, 1, 1.4f},
			{0, 1, 1.0f},
			{-1, 1, 1.4f},
			{-1, 0, 1.0f} };  
	
	// Calculate heuristic value for the entire map.
	for (int x = 0; x < GRIDWIDTH; x++) {
		for (int y = 0; y < GRIDHEIGHT; y++) {
			int dx = abs(gTarget.PositionX() - x);
			int dy = abs(gTarget.PositionY() - y);

			float  heuristic = 0.0f;
			if (gHeuristic == 2) {  
				// Diagonal Distance
				// D1*(dx + dy) + (D2 - 2 * D1) * Min(dx , dy). In our case: D1 = 1, D2 = 1.4
				heuristic = (dx + dy) - 0.6 * std::min(dx, dy);
			} else if (gHeuristic == 3) {  
				// Euclidean Distance
				// Sqrt((difference in X)2 + (difference in Y)2)
				heuristic = sqrt(pow(dx, 2) + pow(dy, 2));
			} else { 
				// Manhatten Disance
				// Total difference in X + total difference in Y.
				heuristic = dx + dy;
			}
			graph[x][y].heuristic = heuristic;
		}
	}

	// 1. Find the grid location with the lowest cost, that is not closed, and is not blocked on the map layout.
	bool flag = false;
	while (!flag) {
		node.cost = 1000000.0f;
		for (int x = 0; x < GRIDWIDTH; x++) {
			for (int y = 0; y < GRIDHEIGHT; y++) {
				
				// Has to be a valid location.
				if (!gLevel.isValid(x, y)) { continue; }  
				// Has to be an open location.
				if (graph[x][y].closed == true) { continue; }  
				if (graph[x][y].cost + graph[x][y].heuristic < node.cost) {
					node.x = x;
					node.y = y;
					node.cost = graph[x][y].cost + graph[x][y].heuristic;
					//rootNode = (node){.x = y, .y = x, .cost = cost};  // Not running C99?
				}
			}
		}
		// node --> current lowest cost node
		// 2. Set this location as closed.
		graph[node.x][node.y].closed = true;
		if ((node.x == gTarget.PositionX()) && (node.y == gTarget.PositionY())) {
			flag = true;
		}
		// 3. Calculate new cost for 8 neighbours.
		for (int index = 0; index < sizeof(neighbourDeltas) / sizeof(*neighbourDeltas); index++) {
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

			}
		}
	}

	// Build path.
	bool done = false;  // Set to true when we are back at the bot position.
	int nextClosedX = gTarget.PositionX();  // Start of path.
	int nextClosedY = gTarget.PositionY();  // Start of path.
	coordsStruct nextClosedCoords;
	nextClosedCoords.x = nextClosedX;
	nextClosedCoords.y = nextClosedY;
	pathVec.push_back(nextClosedCoords);
	while (!done) {
		graph[nextClosedX][nextClosedY].inPath = true;
		int tmpX = nextClosedX;
		int tmpY = nextClosedY;
		nextClosedX = graph[tmpX][tmpY].linkX;
		nextClosedY = graph[tmpX][tmpY].linkY;
		nextClosedCoords.x = nextClosedX;
		nextClosedCoords.y = nextClosedY;
		pathVec.push_back(nextClosedCoords);
		if ((nextClosedX == bot.PositionX()) && (nextClosedY == bot.PositionY())) {
			done = true;
		}
	}
	pathCounter = pathVec.size() - 2;  // -2 deals with space at end of vector.
	completed = true;
}


void cAStar::ChooseNextGridPosition() {
	if (!gAStar.completed) { SetNext(PositionX(), PositionY(), gLevel); return; }
	coordsStruct coords;
	try	{
		coords = gAStar.pathVec.at(gAStar.pathCounter);
	} catch (const std::exception&)	{
		coords.x = PositionX();
		coords.y = PositionY();
	}
	SetNext(coords.x, coords.y, gLevel);
	gAStar.pathCounter--;
}

cDijkstra gDijkstra;
cAStar gAStar;
