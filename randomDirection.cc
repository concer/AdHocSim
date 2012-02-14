
#include "h/randomDirection.h"
/*
 *Angles...
 *
 *	       4.71
 *		|
 *		|
 *		|
 *  3.14 ---------------- 0
 *		|
 *		|
 *		|
 *             1.57
 *
 */



Define_Module_Like(RandomDirection,Mobility);


bool RandomDirection::checkBounds(int x, int y)
{
	if( (x <= minX) || (x >= maxX) )
		return true;
	if( (y <= minY) || (y >= maxY) )
		return	true;
	return false;
}


double RandomDirection::randomDirection(int& x, int& y)
{
	bool pause = false;

	//if the node has reached its destination,define anotherone
	//and rest for pauseTime

	//define new <x,y>
	x = (x + dX);
	y = (y + dY);


	if(checkBounds(x,y))
	{
		double alfa;
		double speed;
		double step;

		d("Bound reached, change direction");

		//choose the direction angle;
		if(y <= minY)
		{
			y = minY;
			alfa = genk_uniform(1,0,3.14);
		}
		if(y >= maxY)
		{
			y = maxY;
			alfa = genk_uniform(1,3.14,6.28);
		}

		if(x <= minX)
		{
			//go back to the bound
			x = minX;

			//is it at the alngle "7"?
			if(y <= minY)
				alfa = genk_uniform(1,0,1.57);
			else
			//is it at the angle "1"?
			if(y >= maxY)
				alfa = genk_uniform(1,4.71,6.28);

			else
			//it is only the "4" border
				alfa =  genk_uniform(1,0,1.57) + genk_uniform(1,4.71,6.28) ;
		}
		if(x >= maxX)
		{
			x = maxX;
			//is it at the "9" angle?
			if(y <= minY)
				alfa = genk_uniform(1,1.57,3.14);
			else
			//is it at the "3" andgle?
			if(y >= maxY)
				alfa = genk_uniform(1,3.14,4.71);
			else
			//it is the "6" bound
				alfa = genk_uniform(1,1.57,4.71);
		}


                speed =  genk_uniform(3, minSpeed->doubleValue(), maxSpeed->doubleValue());

                step = moveInterval->doubleValue() * speed;

                dX = (int)(step * cos(alfa));
                dY = (int)(step * sin(alfa));

                stepsNum ++;
                partial +=  speed;

		pause = true;

		d("al:"<<alfa<<" sin:"<<sin(alfa)<<" cos:"<<cos(alfa));
	}


	if(pause)
	    return (double)pauseTime->doubleValue();
	else
	    return (double)moveInterval->doubleValue();

}


void RandomDirection::initialize()
{

	d("Random Direction Model");
	cGate *g = gate("out");

	//pointer to the physic module that
	//store tje actual position

	physic =(Physic*) g->toGate()->ownerModule();

	minX = 1;
	maxX = (int)par("XRange") - 1;
	minY = 1;
	maxY = (int)par("YRange") - 1;

	moveInterval = &par("moveInterval");
	pauseTime = &par("pauseTime");
	minSpeed = &par("minSpeed");
	maxSpeed = &par("maxSpeed");

	//initialize the first move
	double alfa = genk_uniform(1,0,6.27);

	double speed =  genk_uniform(3, minSpeed->doubleValue(), maxSpeed->doubleValue());
        double step = moveInterval->doubleValue() * speed;
        dX = (int)(step * cos(alfa));
        dY = (int)(step * sin(alfa));
        stepsNum ++;
        partial +=  speed;

	cMessage *moveMsg = new cMessage("Move");

	//start miving
	scheduleAt(simTime()+0.01, moveMsg);

	//statistical variables
	stepsNum = 0;
	partial = 0;

}

void RandomDirection::handleMessage(cMessage *msg)
{
	int x,y;

	d("random Direction");
	//get the current position from the physic module
	physic->getPos(x, y);

	//calcolate the new position
	double time =  randomDirection(x,y);

	cMessage *moveMsg = new cMessage("Move",MOVE);

	moveMsg->addPar("x") = x;
	moveMsg->addPar("y") = y;

	//inform to the physic module about
	//the new position so it can be displayed
	send(moveMsg,"out");

	//tell to the physic module to move
	scheduleAt(simTime()+time, msg);
}



void RandomDirection::finish()
{
	d("Drection random says bye");

	FILE* fout = fopen("collectedData.dat","a");
	fprintf(fout,"\nSpeed average............... %.2f\n",partial/stepsNum);
	fclose(fout);
}


