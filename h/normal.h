
//
#ifndef MOBILITY_H
#define MOBILITY_H

#include "omnetpp.h"
#include "costants.h"

#include "physic.h"

class Normal : public cSimpleModule
{

	//Macro that contains the costructor,destructor
	//and other Omnett++ stuff
	Module_Class_Members(Normal,cSimpleModule,0)

	virtual void initialize();
	virtual void handleMessage(cMessage* );
	virtual void finish();
	//
	
   private:
	//implement the mvement that sims a torus
	bool torus(int&, int&);

	//mplement the rebound movement
	bool rebound(int, int);

	//returns the time intervall
	//to the next move
	double normalMobility(int&, int&);
	

	//quoantum of time between to moves
	//e.g. 1s if the speed is in m/sec
	cPar* moveInterval;
	cPar* pauseTime;
	cPar* minSpeed;
	cPar* maxSpeed;
	
	//pointer of the physic module wich
	//list of the neighbours
	//store the actual <x,y> position
	Physic* physic;

	//size of the movement field
	int minX,maxX,minY,maxY;

	//direction flag
	int dX, dY;

	//former movement angle
	double alfa;
	
	//statistics vars
	int stepsNum;
	double partial;
};
	
#endif	
