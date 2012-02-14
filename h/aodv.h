//AODV protocol
//
#ifndef AODV_H
#define AODV_H


#include "omnetpp.h"
#include "costants.h"

// element of the precursor list containing the
// address for each its neighbors that are
// likely to use it as a next hop towards
// the destination
class PrecursorElement : public cObject
{
   public:
	int ip; // address of the destination node

	PrecursorElement();

	~PrecursorElement();
};


class RouteTableElement : public cObject
{
   public:
	int destId;

	// sequence number of the route to the sestination
	int seqNum;

	int hopCount;

	//next note on the path toward destId
	int nextHop;

	//precursor list of the destination
	cQueue precList;

	//time to live of the route
	simtime_t expiration;

	//says if the route is active or it
	//is expired
	int active;

	//pointer to the eventMessage that makes
	//the route be deleted.
	//It has to be shifted each time the route
	//is "confirmed"
	cMessage* deleteMessage;

	//look for a entry in the precursor list and
	//performs the actions to be taken

	//insert a new precursor element if it is not
	//alredy stored
	bool updatePrecList(int ip);

	RouteTableElement();

	~RouteTableElement();

};

//list element of the processed RREQ that do not have to be processed
//again
class OldReqs : public cObject
{
   public:
	// ip of the node which generated the RREQ message
	int originator;

	// the id of the RREQ message
	int reqId;

	// the time in which the message was received by the node
	simtime_t time;

	OldReqs();

	~OldReqs();
};

//class that stores the packets waiting for a
//RREP to get to a destination
class WaitingPkt : public cObject
{

  public:

	//address of the node that the RREQ
	//is searching a route for
	int dest;

	//self message that make the RREQ to be
	//re-sended or make the message buffer to be flushed
	cMessage* deleteEvent;

	//counter of RREQ sent toward the same destination
	int trial;

	//counter of the packets that are waiting
	int pktNum;

	int pktSize;

	//reqId of the RREQ sent
	int reqId;

	WaitingPkt();

	~WaitingPkt();
};

//class that stores the host id that is in a
//black list
class BlackListElement : public cObject
{
    public:
	//host id
	int id;

	//list-exit message pointer
	cMessage* removeEvent;

	BlackListElement();

	~BlackListElement();
};

//object that store the waiting RREP that need an ACK
class WaitingRREP : public cObject
{
   public:
	//destinatary of the rrep
	int destId;

	//id of the node expected to send an ack msg
	int nextHopId;

	//
	int trials;

	//a copy of the sent msg
	cMessage* rreqMsg;

	//pointer to the event that care about the time out
	cMessage* espireEvent;

	WaitingRREP();

	~WaitingRREP();
};

//this class is used to collect statistics data
//during the simulation run
class PartialStat: public cObject
{
     public:
	     PartialStat(double,double);
	     ~PartialStat();
	     //partial sums
	     double latencySum;
	     double throughSum;
	     int samples;
};

//collect the statistics of the protocol
class Statistics : public cObject
{
     public:
	     Statistics();
	     ~Statistics();

	     //array to store the per-hop
	     //statistics
	     cArray hopsV;
	     int hopsSum;

	     int sentCtrlPkt;
	     int sentDataPkt;
	     int deliveredDataMsg;
	     int sendDataMsg;

	     //hopV upper bount
	     int maxHop;

	     void collect(cMessage*,double);
};

class AODV : public cSimpleModule
{

	//Macro that contains the constructor,destructor
	//and other Omnett++ stuff
	Module_Class_Members(AODV,cSimpleModule,0)

	~AODV();
    private:

	//send HELLO trigger
	cMessage* helloEvent;

	//seqence number of the mobile host
	//used to prevent message loops
	int sequenceNumber;

	//las tused RREQ id
	int reqId;

	//routing table
	cQueue routeTab;

	//queue of old RREQ already processed
	cQueue oldReqs;

	//buffer for the packets waiting
	//for a route
	cQueue pktBuffer;

	//list of neighbours that are out of range for
	//the node's tx range
	cQueue blackList;

	//list of RREP messages waiting for an ack message
	cQueue waitingRrep;

	cMessage* handleRREQ(cMessage*);

	cMessage* handleRERR(cMessage*);

	cMessage* handleRREP(cMessage*);

	cMessage* handleData(cMessage*);

	//hadle the event of a route expiration
	cMessage* handleDelete(cMessage*);

	//handle a RREQ failure
	cMessage* handleFlush(cMessage*);

	//handle an incoming Hello msg
	void handleHELLO(cMessage*);

	//process the ack msg sent by other nodes
	void handleACK(cMessage*);

	//handle  rrep ack failure trigger
	cMessage* handleESP_ACK(cMessage* );

	//remove a node from the black list
	void handleBLK_LIST(cMessage* );

	//store a new RREQ
	void addNewReq(cMessage* );

	//check if a RREQ has been alredy processed
	bool isNewReq(cMessage* );

	//adds a new route
	RouteTableElement* addNewDestination(int,int,int,int,simtime_t);

	//updates,if necessary, the data in a route table entry
	void updateRouteTable(RouteTableElement*,int,int,int,simtime_t);

	//check the state of the known routes
	cMessage* checkRouteTable(RouteTableElement*, cMessage*,int&);

	//check if the destination passed is unknwon
	RouteTableElement* findNode(int);

	//
	cMessage* copyMessage(cMessage* );

	cMessage* generateRREQmsg(RouteTableElement*, int,int);

	cMessage* generateRREPmsg(cMessage*,int,int);

	//
	cMessage* generateRERRmsg(RouteTableElement*,int);

	cMessage* generateHELLOmsg();

	cMessage* generateDATAmsg(RouteTableElement*,int);

	cMessage* generateACKmsg(cMessage*);

	//send to lower levels
	void broadcast(cMessage *);

	//handle the pakets coming from the
	//applicatio module
	cMessage* sendData(cMessage*);

	//store the message to be trasmitted,
	//while trying to get a route
	cMessage* bufferize(int,int);

	//memorize the rrep message and set up the
	//actions to be taken if an ack don't arrive
	void waitForAck(cMessage*);

	//look for a node in the black list
	bool isInBlackList(int);

	//histogram of the handled pakets kinds
	cLongHistogram pktHistogram;
	cLongHistogram hopsHistogram;

	Statistics statistics;

  public:

	virtual void initialize();

	virtual void handleMessage(cMessage *msg);

	virtual void finish();

};

#endif
