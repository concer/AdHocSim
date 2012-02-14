
#include "h/randomWalk.h"


Define_Module_Like(RandomWalk,Mobility);

bool RandomWalk::rebound(int& x, int &y)
{
	bool update=false;

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
bool RandomWalk::torus(int &x, int &y)
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

double RandomWalk::randomWalk(int& x, int& y)
{
 	double step;

	//if the node has covered the defined distance chose a new direction
	//and speed
	if(steps == 0)
	{
		//choose the direction angle; 6.27 is because alfa is in [0,2PI[
		alfa =  genk_uniform(2,0,6.27);

		//choose a random speed
		speed = genk_uniform(1,minSpeed->doubleValue(),maxSpeed->doubleValue());

		//compute a single step length
		step = moveInterval->doubleValue() * speed;

		steps = step > 0  ? (int )( distance->doubleValue() / step) : 1;
		stepsNum += steps;
		partial += steps * speed;
	}
	//copmpute a single step
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


void RandomWalk::initialize()
{

	d("Random Walk Module");
	cGate *g = gate("out");

	//pointer to the physic module that
	//store tje actual position

	physic =(Physic*) g->toGate()->ownerModule();

	minX = 5;
	maxX =(int) par("XRange") - 5;
	minY = 0;
	maxY = (int)par("YRange") - 5;
	steps = 0;


	moveInterval = &par("moveInterval");
	moveKind = &par("movKind");
	minSpeed = &par("minSpeed");
	maxSpeed = &par("maxSpeed");
	distance = &par("distance");

	cMessage *moveMsg = new cMessage("Move");

	//start miving
	scheduleAt(simTime()+0.01, moveMsg);

	//statistical variables
	stepsNum =0;
	partial = 0;

	WATCH(speed);
	WATCH(alfa);

}




void RandomWalk::handleMessage(cMessage *msg)
{
	int x,y;

	d("random Walk");
	//get the current position from the physic module
	physic->getPos(x, y);

	//calcolate the new position
	double time =  randomWalk(x,y);

	cMessage *moveMsg = new cMessage("Move",MOVE);

	moveMsg->addPar("x") = x;
	moveMsg->addPar("y") = y;

	//inform to the physic module about
	//the new position so it can be displayed
	send(moveMsg,"out");

	//tell to the physic module to move
	scheduleAt(simTime()+time, msg);

	d("new x,y: <"<<x<<","<<y<<">");
}



void RandomWalk::finish()
{
	//recordScalar("Speed avarage..........:",partial / stepsNum);
	d("Walk random says bye");

	FILE* fout = fopen("collectedData.dat","a");
	fprintf(fout,"\nSpeed average............... %.2f\n",partial/stepsNum);
	fclose(fout);
}


