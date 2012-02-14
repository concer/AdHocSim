
#include "h/restrictedRandWalk.h"


Define_Module_Like(RestrictedRandWalk,Mobility);

bool RestrictedRandWalk::rebound(int& x, int &y)
{
	bool update=false;
	int sigX = dX >=  0 ? 1 : -1;
	int sigY = dY >=  0 ? 1 : -1;

	d("rebound");
	if( x < minX)
	{
		dX *= (-1); // change the sign
		x = minX;

		alfa = 3.14 - alfa;
		update=true;
	}
	if( x > maxX)
	{
		x = maxX;
		dX *= (-1);
		update=true;
		alfa = 3.14 - alfa;
	}
	if( y < minY)
	{
		dY *= -1;
		y = minY;

		alfa = 6.28 - alfa ;


		update=true;
	}
	if( y > maxY)
	{
		dY *= -1;
		y = maxY;
		alfa = 6.28 - alfa;

		update=true;
	}
	return update;
}

bool RestrictedRandWalk::torus(int &x, int &y)
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

double RestrictedRandWalk::resRandomWalk(int& x, int& y)
{
 	double step;

	//if the node has covered the defined distance chose a new direction
	//and speed
	if(steps == 0)
	{
		//choose the direction angle; considering the former direction,
		//chose a new one among [alfa - PI/4, alfa + PI/4]
		alfa = genk_uniform(1,( alfa - 0.785), ( alfa + 0.785));
		//alfa = alfa + 0.785;

		speed =  genk_uniform(3, speed - speedVar->doubleValue(), speed + speedVar->doubleValue() );

		if( (speed > maxSpeed->doubleValue() ) || ( speed < minSpeed->doubleValue()) )
			speed = genk_uniform(3,  minSpeed->doubleValue() ,   maxSpeed->doubleValue() );


		//compute a single step length
		step = moveInterval->doubleValue() * speed;

		steps = step > 0  ? (int )( distance->doubleValue() / step) : 1;
		stepsNum += steps;
		partial += steps * speed;
	}

	step = moveInterval->doubleValue() * speed;
	dX = (int) (step * cos(alfa));
	dY = (int) (step * sin(alfa));


	//do not go outside the map
	if(0 == moveKind->boolValue())
	{
		//define new <x,y>
		x = (x + dX);
		y = (y + dY);
		torus(x,y);
	}
	else
	{

		//define new <x,y>
		x = (x + dX);
		y = (y + dY);
		rebound(x,y);
	}

	steps--;
	return (double)moveInterval->doubleValue();
}


void RestrictedRandWalk::initialize()
{

	cGate *g = gate("out");

	//pointer to the physic module that
	//store tje actual position

	physic =(Physic*) g->toGate()->ownerModule();

	minX = 5;
	maxX = (int)par("XRange") -5;
	minY = 5;
	maxY = (int)par("YRange") -5;
	steps = 0;


	moveInterval = &par("moveInterval");
	moveKind = &par("movKind");
	minSpeed = &par("minSpeed");
	maxSpeed = &par("maxSpeed");
	distance = &par("distance");
	speedVar = &par("speedVariator");

	//initialize the speed and the direction angle  that the node
	//will use as initial settings
	speed = genk_uniform(3, minSpeed->doubleValue(), maxSpeed->doubleValue());
  	alfa = genk_uniform(1,0,6.27);

	cMessage *moveMsg = new cMessage("Move");

	//start miving
	scheduleAt(simTime()+0.01, moveMsg);

	//statistical variables
	stepsNum =0;
	partial = 0;
	WATCH(dX);
	WATCH(dY);
	WATCH(speed);
	WATCH(minX);
	WATCH(maxX);
	WATCH(minY);
	WATCH(maxY);
	WATCH(alfa);

}


void RestrictedRandWalk::handleMessage(cMessage *msg)
{
	int x,y;

	d("Restricted Random Mobility Model");
	//get the current position from the physic module
	physic->getPos(x, y);

	//calcolate the new position
	double time = resRandomWalk(x,y);

	cMessage *moveMsg = new cMessage("Move",MOVE);

	moveMsg->addPar("x") = x;
	moveMsg->addPar("y") = y;

	//inform to the physic module about
	//the new position so it can be displayed
	send(moveMsg,"out");

	//tell to the physic module to move
	scheduleAt(simTime() + time, msg);
}



void RestrictedRandWalk::finish()
{

	d("Restricted random says bye");
	FILE* fout = fopen("collectedData.dat","a");
	fprintf(fout,"\nSpeed average............... %.2f\n",partial/stepsNum);
	fclose(fout);
}


