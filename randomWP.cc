
#include "h/randomWP.h"


Define_Module_Like(RandomWP,Mobility);

double RandomWP::randomWaypoint(int& x, int& y)
{
	double distance,angle;
	double speed;
	int a,b,c,d;
	bool pause = false;

	a = x;
	b = y;

	//if the node has reached its destination,define anotherone
	//and rest for pauseTime
	if(steps == 0)
	{
		pause = true;
		//choose the speed of the node
		speed = genk_uniform(1,minSpeed->doubleValue(), maxSpeed->doubleValue());
		//choose the destination point
		c = (int) genk_intuniform(2,minX,maxX);
		d = (int) genk_intuniform(3,minY,maxY);

		distance = sqrt(((c-a)*(c-a))+((d-b)*(d-b))) ;

		d("DISTANCE = "<<distance);
		if ( speed !=0)
		{
			steps = (int)( distance / speed);
			steps++;	//avoid 0!
			dX =((c-a) / steps);
			dY =((d-b) / steps);

		}
		else
		{
			steps = (int) genk_intuniform(4,1,100);
			dX = 0;
			dY = 0;
		}

		stepsNum += steps;
		partial += steps * speed;

	}

	//define new <x,y>
	x = (x + dX) ;
	x = x > maxX ? maxX : x;
	x = x < minX ? minX : x;

	y = (y + dY);
	y = y > maxY ? maxY : y;
	y = y < minY ? minY : y;


	//do not go outside the map

	if(pause)
	    return (double)pauseTime->doubleValue();
	else
	{
	    steps--;
	    return (double)moveInterval->doubleValue();
	}

}


void RandomWP::initialize()
{

	d("Random Way Point");
	cGate *g = gate("out");

	//pointer to the physic module that
	//store tje actual position

	physic =(Physic*) g->toGate()->ownerModule();

	minX = 5;
	maxX = (int)parentModule()->par("Xbound") -5;
	minY = 5;
	maxY = (int)parentModule()->par("Ybound") -5;
	steps = 0;


	moveInterval = &par("moveInterval");
	pauseTime = &par("pauseTime");
	moveKind = &par("movKind");
	maxSpeed = &par("maxSpeed");
	minSpeed = &par("minSpeed");

	cMessage *moveMsg = new cMessage("Move");

	//start miving
	scheduleAt(simTime()+0.01, moveMsg);

	//statistical variables
	stepsNum =0;
	partial =0;

}




void RandomWP::handleMessage(cMessage *msg)
{
	int x,y;

	d(" random WP");
	//get the current position from the physic module
	physic->getPos(x, y);

	//calcolate the new position
	double time =  randomWaypoint(x,y);

	cMessage *moveMsg = new cMessage("Move",MOVE);

	moveMsg->addPar("x") = x;
	moveMsg->addPar("y") = y;

	//inform to the physic module about
	//the new position so it can be displayed
	send(moveMsg,"out");

	//tell to the physic module to move
	scheduleAt(simTime()+time, msg);
}



void RandomWP::finish()
{
	 FILE* fout = fopen("collectedData.dat","a");

	d("Speed avarage..........:"<<partial/stepsNum);
	fprintf(fout,"\nSpeed average............... %.2f\n",partial/stepsNum);

 	fclose(fout);
}


