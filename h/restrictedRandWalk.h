
//
#ifndef RES_RANDOM_WALK_H
#define RES_RANDOM_WALK_H

#include "omnetpp.h"
#include "costants.h"

#include "physic.h"
// element of the precursor list containing the IP
// address for each its neighbors that are
// likely to use it as a next hop towards
// the destination


class RestrictedRandWalk : public cSimpleModule
{

	//Macro that contains the costructor,destructor
	//and other Omnett++ stuff
	Module_Class_Members(RestrictedRandWalk, cSimpleModule,0)

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
	double resRandomWalk(int&, int&);
	//
//	virtual pursueModel();


	//quoantum of time between to moves
	//e.g. 1s if the speed is in m/sec
	cPar* moveInterval;
	cPar* moveKind;
	cPar* distance;
	cPar* minSpeed;
	cPar* maxSpeed;
	cPar* speedVar;
	//epoch duration mean (for the expnential

	//pointer of the physic module wich
	//list of the neighbours
	//store the actual <x,y> position
	Physic* physic;

	double speed;
	double alfa;
	//size of the movement field
	int minX,maxX,minY,maxY;

	//direction flag
	int dX, dY;

	//number of steps to reach
	//the destination
	int steps;

	//statistics vars
	int stepsNum;
	double partial;
};
	
#endif	
