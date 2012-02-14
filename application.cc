#include "h/application.h"

Define_Module(Application);

//applicaton sends data to a mobile host chosen
//randomly among the numHost device.
//the actual address of the chosen host is defined by
//the routing module,that can gain the number accessing
//the hsots arry in the physic module.

void Application::initialize()
{
	//store parameters for a faster access
	rate = (int) par("rate");
	pktSize = (int) par("pktSize");
	hostNum = (int) par("hostNum");
	active = (int) par("active");
	burstInterval = (simtime_t) par("burstInterval");
	d("active = "<<active);
	physic = (Physic*)parentModule()->submodule("physic");
	//initialize the work variables
	pktNum = 0;
	dest = 0;


	//let's go
	if (active)
 	  scheduleAt(simTime()+2+exponential(1),new cMessage("newAppBurst"));
}

void Application::handleMessage(cMessage* msg)
{
	int i,m;
	d("APPLication");	
	cMessage *pkt = new cMessage("Data",DATA,pktSize);

	if(pktNum == 0)
	{
		pktNum = 64;// (int)intuniform(40,530);

		//chose destination
		do
		{
			//chose the destination of the pkt burst
			i = (int) intuniform(0,hostNum -1);

			physic->getVect(i,m,dest);

		}while (dest == parentModule()->id());
		d("new Burst of "<<pktNum<<" pkts to dest:"<<dest);
	}

	pkt->addPar("msgId") = msgId++;
	pkt->addPar("dest") = dest;

	//send the paket
	pktNum --;
	send(pkt,"out");

	if(pktNum == 0) // end of the burst
		scheduleAt(simTime() + burstInterval, new cMessage("New data burst"));
	else
	{
		scheduleAt(simTime()+0.25, new cMessage("App:send data pkt"));
	}
	delete msg;
}


void Application::finish(){d("Applicarion says bye");}




