
#include "h/normal.h"


Define_Module_Like(Normal,Mobility);

bool Normal::torus(int &x, int &y)
{
	bool update=false;
	d("toru");
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

double Normal::normalMobility(int& x, int& y)
{
	double distance, duration;
	double speed;
	//choose the direction angle
	alfa +=  genk_normal(1,0,0.54); // PI/6

	speed = genk_uniform(2,minSpeed->doubleValue(), maxSpeed->doubleValue());
		//choose the movement duration
	distance = moveInterval->doubleValue() * speed;

	dX = (int)(distance * cos(alfa));
	dY = (int)(distance * sin(alfa));

	d("al:"<<alfa<<" d:"<<distance );

	//define new <x,y>
	x = (x + dX);
	y = (y + dY);

	//do not go outside the map
	torus(x,y);

	stepsNum++;
	partial+= speed;


  	return (double)moveInterval->doubleValue();
}


void Normal::initialize()
{

	cGate *g = gate("out");

	//pointer to the physic module that
	//store tje actual position

	physic =(Physic*) g->toGate()->ownerModule();

	alfa = 0;
	minX = 0;
	maxX = par("XRange");
	minY = 0;
	maxY = par("YRange");
	//steps = 0;


	moveInterval = &par("moveInterval");
	minSpeed = &par("minSpeed");
	maxSpeed = &par("maxSpeed");

	cMessage *moveMsg = new cMessage("Move");

	//start moving
	scheduleAt(simTime()+0.01, moveMsg);

	//statistical variables
	stepsNum =0;
	partial =0;

}




void Normal::handleMessage(cMessage *msg)
{
	int x,y;

	d("Normal Markowian Mobility");
	//get the current position from the physic module
	physic->getPos(x, y);

	//calcolate the new position
	double time = normalMobility(x,y);

	cMessage *moveMsg = new cMessage("Move",MOVE);

	moveMsg->addPar("x") = x;
	moveMsg->addPar("y") = y;

	//inform to the physic module about
	//the new position so it can be displayed
	send(moveMsg,"out");

	//tell to the physic module to move
	scheduleAt(simTime()+time, msg);
}



void Normal::finish()
{
	d("Normal random says bye");
	FILE* fout = fopen("collectedData.dat","a");
	fprintf(fout,"\nSpeed average............... %.2f\n",partial/stepsNum);
  	fclose(fout);
}


