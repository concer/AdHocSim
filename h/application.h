#ifndef APPLICATION_H
#define APPLICATION_H

#include "omnetpp.h"

#include "costants.h"

#include "physic.h"

class Application : public cSimpleModule
{
	Module_Class_Members(Application,cSimpleModule,0);

  private:
	
	//number of packet to send in a request burst
	int pktNum;

	//the destinatary node of a pkt burst
	int dest;
	
	int pktSize;

	int hostNum;

	int rate; // pkt per second

	simtime_t burstInterval;

	//an unique id of the message
	int msgId;
	

	bool active; //does this app generate traffic?

	//pointer tho the module that hold 
	//the vector with all possible destination
	Physic* physic;


	
	
  public:
	
	virtual void initialize();

	virtual void handleMessage(cMessage* msg);

	virtual void finish();
};

#endif
