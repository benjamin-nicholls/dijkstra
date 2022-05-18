#pragma once
//======================================================================================
//Header file for bots: initially cBotRandom is defined here, but other bot classes
//can go here too
//
//(c) Patrick Dickinson, University of Lincoln, School of Computer Science, 2020
//======================================================================================

#include "botbase.h"
#include <vector>


struct graphNode {
	bool closed = false;			// Whether or not location is closed.
	float cost = 1000000.0f;		// Cost value for each location.
	int linkX = -1;					// Link X coord for each location.
	int linkY = -1;					// Link Y coord for each location.
	bool inPath = false;			// Whether or not a location is in path.
	float heuristic = 1000000.0f;	// Heuristic cost value for each location.

	void reset() {
		closed = false;
		cost = 1000000.0f;
		linkX = -1;
		linkY = -1;
		inPath = false;
		heuristic = 1000000.0f;
	}
};


struct coordsStruct {
	int x = 0;
	int y = 0;
};


class cBotRandom : public cBotBase {
	virtual void ChooseNextGridPosition();
};


class cBotSimple : public cBotBase {
public:
	cBotSimple();
	~cBotSimple();
	virtual void ChooseNextGridPosition();
};


class cBotSimple2 : public cBotBase {
public:
	cBotSimple2();
	~cBotSimple2();
	virtual void ChooseNextGridPosition();
};


class cDijkstra : public cBotBase {
public:
	cDijkstra() { completed = false; };
	~cDijkstra() {};
	virtual void Build(cBotBase& bot);
	virtual void ChooseNextGridPosition();
	//void ShortestPath(cBotBase& bot);
	bool completed;
	bool getInPath(int x, int y) const;
	bool getClosed(int x, int y) const;
protected:
	graphNode graph[GRIDWIDTH][GRIDHEIGHT];
};


class cAStar: public cDijkstra {
public:
	cAStar() { completed = false; };
	~cAStar() {};
	virtual void Build(cBotBase& bot);
	//std::list<coordsStruct> getPath();
	virtual void ChooseNextGridPosition();
	int pathCounter = 0;
	std::vector<coordsStruct> pathVec;
};


extern cDijkstra gDijkstra;
extern cAStar gAStar;
extern int gHeuristic;