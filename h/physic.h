//
//
#ifndef PH_H
#define PH_H

#include "omnetpp.h"

#include "costants.h"
//in order to avoid the scan of every module,
//I keep the ids of evry mobile host in an array

//object that stores the neighbours data. It is a subclass of
//cObject in order ot use a cArray to store and handle it
class cNeighbour : public cObject
{
   public:
        int ph;
        int mobHost;

	int gateIndex;

	cNeighbour();
	~cNeighbour();
};

class Physic : public cSimpleModule
{
   private:
	//position of the host itself in the hosts array
	int myI;

	int posX, posY;

	//number of hosts in the map
	int numHost ;

	//array to achieve a faster acces to the other modules
	cArray hosts ;

	//array of pointers to hosts that are within the range of
	//trasmission
	cArray  nbList ;

	//neighbour counter
	int nbCount;

	//fulfill the above vector
	void detectNeighbours();

	//check whether the modules passed
	//are within the eachother range of trasmission
	bool isReachable(int, int );

	//create the gates and the connections
	//returns the gate index of the physic gate
	int connectModules(int,int);

	//disconnect the gates:
	//physic->mobile, mobile->mobile, mobile->physic
	//deleting the gates
	bool disconnectFrom(int, int);

	//returns the index of the gates that connect
	//the two modules passed by parameter
	bool getGateIndex(cModule*,cModule*,int&,int&,int&,int&);

	//adds a gate to the module passe by parameter.
	//it try to "recycle" empty slots within the gate vector
	//before create a new istance of a gate
	int addNewGate(cModule *mod, char* name, char type);

	//send a copy of the input message to each neighbour
	void broadcast(cMessage*);

   public:

	//Macro that contains the costructor,destructor
	//and other Omnett++ stuff
	Module_Class_Members(Physic, cSimpleModule,0 )

	//check whether the other hosts are still
	//reachable
	void updateConnections();
	bool checkConnection(int i);

	//device trasmission power,it affect directly the tx range
	double power;

	//threshold that part the noise form an actual message
	double rxThreshold;

	//channel parameters
	/*cPar* delay;
	cPar* error;
	cPar* dataRate;*/
	double delay;
	double error;
	double dataRate;

	//Trasmission range of the module
	double range;

	//gives the i-th element of the hosts array
	void getVect(int,int&,int&);

	//gates counter
	int gatesNum ;

	//set up a cannection between the physic module and
	//sthe compuntd module, then returns the index of
	//the external gate (by the referece variable)
	void setUpConn(char,int&,int&);

	//delete the link between two modules
	bool putDownConn(int,int,int,int);

	//OMNeT++ methods
	virtual void handleMessage(cMessage*);
	virtual void initialize();
	virtual void finish();

	//set and return the host actual position
	void getPos(int &x, int &y);
	void setPos(int x, int y);

	//statistical variable: erroneus message received
	int msgWithErr;

};

#endif
