
//
#ifndef RANDOM_WALK_H
#define RANDOM_WALK_H

#include "omnetpp.h"
#include "costants.h"

#include "physic.h"

class RandomWalk : public cSimpleModule
{

	//Macro that contains the costructor,destructor
	//and other Omnett++ stuff
	Module_Class_Members(RandomWalk,cSimpleModule,0)

	virtual void initialize();
	virtual void handleMessage(cMessage* );
	virtual void finish();
	//
	
   private:
	//implement the mvement that sims a torus
	bool torus(int&, int&);

	//mplement the rebound movement
	bool rebound(int&, int&);

	//returns the time intervall
	//to the next move
	double randomWalk(int&, int&);

	//quoantum of time between to moves
	//e.g. 1s if the speed is in m/sec
	cPar* moveInterval;
	cPar* pauseTime;
	cPar* moveKind;
	cPar* minSpeed;
	cPar* maxSpeed;
	cPar* distance;
	
	//pointer of the physic module wich
	//list of the neighbours
	//store the actual <x,y> position
	Physic* physic;

	//size of the movement field
	int minX,maxX,minY,maxY;

	//direction flag
	int dX, dY;
	
	//direction angle 
	double alfa;

	double speed;
	//number of steps to reach
	//the destination
	int steps;

	//statistics vars
	int stepsNum;
	double partial;
};
	
#endif	
