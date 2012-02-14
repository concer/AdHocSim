
#include "h/mac.h"
#include <string.h>

Define_Module_Like(SimpleMac,Mac);

cBuffer::cBuffer(double size)
{
	capacity = size;
	used = 0;
	queue = new cQueue();
}

cBuffer::~cBuffer(){}

bool cBuffer::canStore(int s)
{
	return (used + s) < capacity;
}


bool cBuffer::empty()
{
	return used == 0;
	//return queue->empty();
}


bool cBuffer::insert(cMessage* m)
{
	if( (used + (int)m->length()) > capacity)
		return false;

	used += m->length();
	queue->insert( (cMessage*) m);
	return true;
}


cMessage* cBuffer::pop()
{
	if(queue->empty())
		return NULL;
	cMessage* m = (cMessage*) queue->pop();
	used -= m->length();
	return m;
}

int cBuffer::length()
{
	return queue->length();
}


void SimpleMac::initialize()
{

	bool promisqueMode =(bool) par("promisqueMode");

	d("initialize");

	//buffer of input pakets. If the buffer is full the incomming
	//messages will be dropped
	buffer = new cBuffer( (double)par("inBufferSize") );

	//flag to menage the forwarding of messages toward the route module
	routerBusy = false;

	bufferFullDiscard =0;

	//self message used to simulate the mm1 queue toward
	//the routing module
	endService = new cMessage("endService");

}

void SimpleMac::handleMessage(cMessage* msg)
{
	d("handle");
	//send the messages to the neighbours
	if ( msg->arrivedOn("fromRoute") )
	{
		d("msg from Route");

		//the message will be deleted by the physic
		//module
		send(msg,"toPh");
	}
	else
	//if the router has finished his work,send another message
	if (msg == endService)
	{
		d("end service");
		if(!buffer->empty())
		{
			//send up a waiting message
			//this->takeOwnership(false);
			cMessage* m = buffer->pop();

			send(m, "toRoute");

			//this->takeOwnership(false);
			scheduleAt(elabTime() ,endService);
		}
		else
		{
			//set the host as idle
			d("no messages...waiting");
			routerBusy = false;
		}
	}
	else
	{
		//send to higher levels... if....
		if( ( (int)msg->par("mac") != parentModule()->id() ) &&
		    ( (int)msg->par("mac") != BROADCAST ) &&
		    ( ! promisqueMode) )
		{
			d("message not for this node, discarding");
			delete msg;
		}
		else
		{
			//this node can handle the message
			d("got message from "<<msg->par("source"));
			//if the host was idle
			if( (!routerBusy) && (buffer->empty()) )
			{
				routerBusy = true;

				//this->takeOwnership(false)
				send(msg,"toRoute");
				//this->takeOwnership(true);
				scheduleAt( elabTime(), endService);

			}
			else
			{
				if(buffer->canStore(msg->length()))
				{
					d("host busy, will put the msg in the buffer!");
					buffer->insert(msg);
				}
				else
				{
					d("input buffer full!!! discarding pkts");
					bufferFullDiscard++;
					delete msg;
				}
			}
		}
	}
}

// No need to pass msg!
double SimpleMac::elabTime(/*cMessage* msg*/)
{
	//use a mormal distribution truncated to get non negative values
	double x = truncnormal(NODE_TRAVERSAL_TIME/2 , NODE_TRAVERSAL_TIME/4);
	return  simTime()+ x;
}

void SimpleMac::finish()
{
	d("Mac says bye");

	FILE* fout = fopen("collectedData.dat","a");
	fprintf(fout,"Buffer full discards...........  %d\n",bufferFullDiscard);
 	fclose(fout);

	d("Buffer full discards...."<<bufferFullDiscard);
}

