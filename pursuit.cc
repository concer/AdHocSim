
#include "h/pursuit.h"


Define_Module_Like(Pursuit,Mobility);


bool Pursuit::rebound(int x, int y)
{
	bool update = false;

	if( (x <= minX) || (x >= maxX) )
	{
		dX *= (-1); // change the sign
		update = true;
	}
	if( (y <= minY) || (y >= maxY) )
	{
		dY *= (-1);
		update = true;
	}
	return update;
}


bool Pursuit::torus(int &x, int &y)
{
	bool update=false;
	if( x < minX)
	{
		x = maxX;
		update=true;
	}
	if( x > maxX)
	{
		x = minX;
		update=true;
	}
	if( y < minY)
	{
		y = maxY;
		update=true;
	}
	if( y > maxY)
	{
		y = minY;
		update=true;
	}
	return update;
}


void Pursuit::pursuit(int& x, int& y)
{
	double distance,alfa;
	int tx,ty; //target coordinates
	double speed = genk_uniform(1,minSpeed->doubleValue(), maxSpeed->doubleValue());

	target->getPos(tx,ty);

	distance = sqrt(((tx-x)*(tx-x))+((ty-y)*(ty-y))) ;

	if(distance > distanceThreshold)
	{
		//choose the speed of the node
	
		//compute the angle of direction
		alfa = asin( (ty-y)/distance );
	
		x = x +(int) ( speed * cos(alfa) * moveInterval->doubleValue() * dX);
		y = y +(int) ( speed * sin(alfa) * moveInterval->doubleValue() * dY);

		//check the x direction, the y one is given by the alfa angle
		dX = tx > x ? 1 : -1;

		//do not go outside the map
		if(0 == (int)moveKind)
			torus(x,y);
		else
			rebound(x,y);
	
		partial +=speed;
		
	}
	movements++;
}


void Pursuit::initialize()
{

	cGate *g = gate("out");
	int targetNode = (int) par("target");
	
	//pointer to the physic module that
	//store tje actual position
	cModule* mod;
	int k = -1;
	int i = 0;
	
	//check if the target module exists
	if(targetNode > (int) parentModule()->par("numHost"))
	{
		ev<<"ERROR: pursued node does not exist!\n";
		exit(1);
	}
	physic =(Physic*) g->toGate()->ownerModule();

	d("Pursuit initialize"<<targetNode);

	//look for the target'physic module 
	while(( i<= simulation.lastModuleId()) && (k != targetNode)  )
        {
		i++;
                mod = (cModule*)simulation.module(i);
                if( strcmp(mod->name(), "physic")  == 0)
                   k++;
        }
	target = (Physic*) mod;
	
	d("following the node:"<<mod->parentModule()->id());
	
	//initialize all the variables
	minX = 0;
	maxX = par("XRange");
	minY = 0;
	maxY = par("YRange");
	
	//direction flag
	dX = 1;
	dY = 1;	
	
	moveInterval = &par("moveInterval");
	moveKind = &par("movKind");
	minSpeed = &par("minSpeed");
	maxSpeed = &par("maxSpeed");

	
	//keep at least this distance
	distanceThreshold = 35;
	
	cMessage *moveMsg = new cMessage("Move");

	//start miving
	scheduleAt(simTime()+0.01, moveMsg);
	
	//statistical variables
	movements = 0;
	partial = 0;

}

void Pursuit::handleMessage(cMessage *msg)
{
	int x,y;
	
	d("pursue module");
	//get the current position from the physic module
	target->getPos(x, y);
	//calcolate the new position
	pursuit(x,y);

	cMessage *moveMsg = new cMessage("Move",MOVE);

	moveMsg->addPar("x") = x;
	moveMsg->addPar("y") = y;

	//inform to the physic module about
	//the new position so it can be displayed
	send(moveMsg,"out");

	//tell to the physic module to move
	scheduleAt(simTime()+moveInterval->doubleValue(), msg);
}



void Pursuit::finish()
{
	d("Pursuit  says bye");
}


