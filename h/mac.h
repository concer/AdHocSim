#ifndef MAC_H
#define MAC_H

#include "omnetpp.h"

#include "costants.h"

//buffer for the incoming messages.
//Used to implement the mm1 queue of messages
//toward the routing module
class cBuffer : public cObject
{
    public:
        //the total size of the buffer
	double capacity;

	//the amount of used space
	double used;

	cQueue* queue;
	cBuffer(double);
	~cBuffer();

	//return true if the size of the
	//object to store does fit in the remaining
	//room in the buffer
	bool canStore(int);

	//self explaining...
	bool empty();
	bool insert(cMessage*);

	int  length();

	cMessage* pop();

};

class SimpleMac : public cSimpleModule
{
   private:

	//if false only the messages with correct MAC address will pass
	//this level upward
	bool promisqueMode;

	//decide how long a message will wait
	double elabTime(/*cMessage* */);

	//...
	bool routerBusy;

	//self message used to handle the mm1 queue toward
	//the routig module
	cMessage* endService;

	//input message buffer
	cBuffer* buffer;

   public:

	//Macro that contains the costructor,destructor
	//and other Omnett++ stuff
	Module_Class_Members(SimpleMac, cSimpleModule,0 )

	//statistics...
	int bufferFullDiscard;

	//OMNeT++
	virtual void initialize();
	virtual void handleMessage(cMessage*);
	virtual void finish();
};

#endif
