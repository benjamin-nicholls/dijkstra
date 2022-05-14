#pragma once
//======================================================================================
//Header file for bots: initially cBotRandom is defined here, but other bot classes
//can go here too
//
//(c) Patrick Dickinson, University of Lincoln, School of Computer Science, 2020
//======================================================================================

#include "botbase.h"
#include <list>


struct graphNode {
	bool closed = false;  // Whether or not location is closed.
	float cost = 1000000.0f;  // Cost value for each location.
	int linkX = -1;  // Link X coord for each location.
	int linkY = -1;  // Link Y coord for each location.
	bool inPath = false;  // Whether or not a location is in path.
};


class cBotRandom : public cBotBase
{
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


class cDijkstra {
private:
	graphNode graph[GRIDWIDTH][GRIDHEIGHT];
public:
	cDijkstra();
	~cDijkstra();
	virtual void Build(cBotBase& bot);
	//void ShortestPath(cBotBase& bot);
	bool completed;
	bool getInPath(int x, int y) const;
	bool getClosed(int x, int y) const;
};


extern cDijkstra gDijkstra;

