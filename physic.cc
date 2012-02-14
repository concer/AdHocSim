
#include "h/physic.h"
#include <string.h>

//define the string that define the host display
#define displayS "p=%d,%d;b=12,12,oval;o=%s,,1"

Define_Module(Physic);

cNeighbour::cNeighbour(){};
cNeighbour::~cNeighbour(){};


void Physic::initialize()
{
	cModule *parent = parentModule();
	cModule *mod;
	char str[90];

	posX = (int)parent->par("x");
	posY = (int)parent->par("y");

	numHost = (int)parent->par("numHost");

	power = (double) par("txPower");
	rxThreshold =  (double) par("rxThreshold");

 	/*delay = &par("channelDelay");
	error = &par("channelError");
	dataRate = &par("channelDatarate");
	*/
	delay = (double)par("channelDelay");
	error = (double)par("channelError");
	dataRate = (double)par("channelDatarate");

	gatesNum = 0;
	nbCount = 0;
	msgWithErr =0;

	// nbList must not take ownership of pointers in hosts list
	nbList.takeOwnership(false);

	//show the node on the map
	sprintf(str,displayS,posX,posY,"red");
	//parentModule()->setDisplayString(0,str,true);
	parent->displayString().parse(str);
	//fullfill the hosts vector
	detectNeighbours();
}

void Physic::handleMessage(cMessage* msg)
{
	d("\t--- physic ---");
	char str[50];

	//update position and go on
	if ( msg->arrivedOn("fromMobility") )
	{
		d("msg from mobility");
		posX = (int)msg->par("x");
		posY= (int)msg->par("y");

		//update the position
		sprintf(str,displayS,posX, posY,"red");
		//parentModule()->setDisplayString(0,str,true);
		parentModule()->displayString().parse(str);
		//compute the trasmission range that might have been changed
		range = sqrt(power/rxThreshold);
		//check which hosts are reachable
		updateConnections();
		delete msg;
	}
	else

	//send the messages to the neighbours
	if ( msg->arrivedOn("fromMac") )
	{
		d("msg from Mac");

		//compute the trasmission range
		range = sqrt(power/rxThreshold);

		//send the message to all the neighbours
		broadcast(msg);

		/*if( msg!= NULL );		//No more needed, broadcast()
			delete msg;*/		// sends also the original msg --> make less copies
	}
	else
	{
		//arrived from outside the module
		d("msg from outside");
		if(msg->hasBitError())
		{
			d("received message with errors...discarding!");
			msgWithErr++;
			delete msg;
		}
		else
		{
			d("got message: "<<msg->name()<<" from: "<<msg->par("source")<<" sending to mac");
			//in this way there is not the need to copy	// C'e' veramente bisogno di copiarlo?
			//the message, saving CPU time.
			//this->takeOwnership(false);
			send(msg,"toMac");
			//this->takeOwnership(true);

		}
	}
}


void Physic::broadcast(cMessage*msg)
{
	//cNeighbour* n = NULL;
	d("broadcast Out");

	int items = nbList.items();

	for(int i =0; i < items-1 ;i++)
	{
		if(nbList[i])
		{
			//a new copy of message has to be made
			cMessage* m = new cMessage(*msg);
			send(m, ((cNeighbour*)nbList[i])->gateIndex);
		}
	}

	// Send the original message at last
	if(nbList[items-1])
		send(msg, ((cNeighbour*)nbList[items-1])->gateIndex);
	else
		delete msg;


}

/*
 *	Init hosts array
 */
void Physic::detectNeighbours()
{
	//pointer to the modules that are to be scanned
	cModule* mod;
	//stores the index returned by cArray.add()
	int pos = 0;


	d("detectNeigh");

	for(int i=1; i<= simulation.lastModuleId(); i++)
  	{
		//scan the simulation module vector
		mod = (cModule*)simulation.module(i);

	   	if( strcmp(mod->name(), "physic")  == 0)
	   	{
		   cNeighbour *n = new cNeighbour();

		   n->ph = mod->id();
		   //remember its parent module too
		   n->mobHost = mod->parentModule()->id();
		   n->gateIndex = -1;//not connected

		//hosts.takeOwnership(false);
		   pos =  hosts.add(n);
		   if(i == id())
			   myI = pos;
	   	}
	}
}

void Physic::updateConnections()
{
	d("updateConnections");
	cNeighbour* n;
	for(int i=0; i< numHost ; i++)
	{
		//check the link avoiding to analyze itself
		if ( ( i != myI) && checkConnection(i)){

			n = (cNeighbour*)hosts[i];
			d("link changed between this host and:"<<n->mobHost);

			//make the other node do the check
			if (n->mobHost < parentModule()->id())
			{
				d("notify the disconnection to the remote node-");
				Physic* remote = (Physic*)simulation.module(n->ph);

				// the index of this host on the remote hosts vector
				remote->checkConnection( myI );
			}
		}
	}
}

/*
 *	If host i is reachable, connect to it and add to the neighbours array,
 *	otherwise disconnect from it and remove the entry.
 */
//returns true if the connection has changed state
bool Physic::checkConnection(int i){

	d("checkConnection");
	cNeighbour* n = (cNeighbour*)hosts[i];

	//avoid the parent to this module parent
	if( (n->ph != id()) &&  isReachable(id(),n->ph) )
	{
		//if it is not known
		if (nbList.find(n) == -1 )
		{
			d("node "<<i<<" connecting to "<<n->mobHost<<". n->ph="<<n->ph);
			//cNeighbour *newN = new cNeighbour(*n);

			nbList.add(n);
			nbCount++;

			n->gateIndex= connectModules(n->ph,n->mobHost);
		}
		return true;
	}
	else
	{
		//check if it was connected
	 	int a = nbList.find(n);
		d("host not reachable "<<n->mobHost);
		if(a!= -1)
		{
			d("disconnect from neighbour:"<<n->mobHost);
			cNeighbour* tmp;

			//remove does not delete the object from the
			//memory! the list stores only a pointer to the
			//original host object.
			nbList.remove(a);
			//avoid holes in the array
			if( (nbCount>1) && (a!=nbCount-1))
			{
				tmp = (cNeighbour*)nbList.remove(nbCount-1);
				nbList.addAt(a,tmp);
			}
			nbCount--;
			n->gateIndex = -1;
				//disconnect
			disconnectFrom(n->ph,n->mobHost);

			return true;
		}
		else
		{
			d("but "<<n->ph<<" was not connected");
			return false;
		}
	}
}


int Physic::connectModules(int rPhysic,int rMod)
{
	d("connectModules");
	//a->b->c->d
	int a,b,c,d;

	//pointer to the remote physical layer
	Physic *ph = (Physic*)simulation.module(rPhysic);
	//pointer to the remote host object
	cModule* mod = (cModule*) simulation.module(rMod);

	//create the gates and the connection whithin these modules
	setUpConn('O',a,b);

	//do the same for the remote node. Note that 'c' will be used later for the conn
	//between the two mobile hosts.
	ph->setUpConn('I',d,c);
	if( (a == -1) || (b == -1) || (c == -1) || (d == -1) )
	{
		ev << "Error in the connectModules function \n";
		wait(2);
	}
	d(a<<"->"<<b<<"-->"<<c<<"->"<<d<<" OK!");

	//mobile->mobile connection
	//CHANNEL
	//cLinkType* etere = findLink( "etere" );

	//connect((cModule*)parentModule(), b,(cLinkType *) etere,(cModule*) mod,c);
	//now deprecated!
	//cSimpleChannel *channel = new cSimpleChannel("channel");
	cBasicChannel *channel = new cBasicChannel("channel");

	channel->setDelay(delay);
	channel->setDatarate(dataRate);
	channel->setError(error);

	parentModule()->gate(b)->connectTo(mod->gate(c), channel);

	//draw the link
	//cGate *g = parentModule()->gate(b);
	//g->displayString();// g->displayString(), true);

	d("b-->c connected --> end actual connect");
	return a;
}

//kind = 'O' || 'I'
void Physic::setUpConn(char kind,int& a,int& b)
{
	char s[20],t[20];

	cModule* parent = parentModule();

	//a is the gate on the physic module,
	//b is on the compound module
	d("setUpConn");

	//define the gate's name
	if(kind == 'I')
		strcpy(t,"Rx%d\0");
	else
		strcpy(t,"Tx%d\0");

	//create the gates to this module and its parent
	gatesNum++;

	sprintf(s,t,gatesNum);
	a = addNewGate(this,s,kind);

	b = addNewGate(parent,s,kind);
	d("added gate 'b' :"<<s);

	if( (a == -1)||(b == -1))
	{
		d("Error in the addGateVector function ");
		wait(2);
	}
	//connect the gates
	if(kind == 'O')
	{
	    this->gate(a)->connectTo(parent->gate(b));
	    //connect((cModule*)this,a,(cLinkType *)NULL,(cModule*)parent,b);
	    d("ph->comp connected!");
	}
	else
	{
	   //connect((cModule*)parent,b,(cLinkType *)NULL,(cModule*)this,a);
	   parent->gate(b)->connectTo(this->gate(a));
	   d("comp->ph connected!");
	}
}

int Physic::addNewGate(cModule *mod, char* gname, char type)
{
	cGate* g;
	int index = 0,
	    gateNum = 0;
	bool found = false;
	int i = 0;

	d("gates number before connection:"<<mod->gates());

	//look for a free entry
	while((i< mod->gatev.items()) && (!found))
	{
		g = (cGate*) mod->gate(i);
		d("gate: "<<i);
		if(g == NULL)
		{
			d("found a null gate vector place. Use it !");
			g = new cGate(gname,type);
			mod->gatev.addAt(i,g);
			g->setOwnerModule( mod, i);
			index = i;
			found = true;
		}
		else
		if(!g->isConnected())
		{
			d("found a not connected gate. recycling :"<<g->name());
			delete g;
			g = new cGate(gname,type);
			mod->gatev.addAt(i,g);
			g->setOwnerModule( mod, i);
			found = true;
			index = i;
		}
		else
			i++;
	}
	//all the entries are used, add a new gate
	if(!found)
	{
		d("free place not found, adding a new one ");
		mod->addGate(gname,type);
		gateNum = mod->gates();

		if(mod->hasGate(gname))
		{
			d("has gate ok !");
			index = mod->findGate(gname);
		}
		else
		{
			d("--- ERROR new gate not found!");
		}
	}

	//mod->setGateSize(gname,mod->gates());
	g = (cGate*) mod->gate(index);
	//g->setIndex(index,gateNum);

	//g->setIndex(index,mod->gates());
	g->setOwnerModule(mod,index);
	//cGate* newGate = new cGate(gname, type);

	return index;

}

bool Physic::disconnectFrom(int rPhysic, int rMob)
{
	int parent =(int) parentModule()->id();
	int me = id();

	d("-----disconnect------");

	//disconnect the link between the two nodes
	putDownConn(me,parent,rMob,rPhysic);


	d("Disconnection successfull!");
	return true;
}


//           Left -------> arrow -------> Right
bool Physic::putDownConn(int lph,int leftId, int rightId,int rph)
{
	cModule *left = (cModule*)simulation.module(leftId);
	cModule *right = (cModule*)simulation.module(rightId);

	cModule *lPhysic = (cModule*)simulation.module(lph);
	cModule *rPhysic = (cModule*)simulation.module(rph);

	//a->b-->c->d
	int a =-1,
	    b =-1,
	    c =-1,
	    d =-1;

	cGate *g;

	d("put down Connection between:"<<left->name()<<", "<<left->id()<<" - "<<right->name()<<", "<<right->id());

	//look for the connection gate to delete
	if(!getGateIndex(left,right,a,b,c,d)  )
	{
		d("PutDown: gateindex returned false! ******************************");
		return false;
	}

	d("deleting gates a: "<<a<<" b: "<<b<<" c: "<<c<<" d: "<<d);



	//tryal....
//	left->setDisplayString(left->displayString(),true);
//	right->setDisplayString(right->displayString(),true);
//	lPhysic->setDisplayString(lPhysic->displayString(),true);
//	rPhysic->setDisplayString(rPhysic->displayString(),true);

	g = (cGate*)lPhysic->gate(a);
	//g->setTo(NULL);	// Deprecated
	g->disconnect();	// Also removes other's gate

	lPhysic->gatev.remove(a);
	delete g;


	//get rid of the old gate
	g = (cGate*)left->gatev.get(b);
//	g->setTo(NULL);
	g->disconnect();
//	g->setFrom(NULL);		Already disconnected by previous disconnect
	left->gatev.remove(b);
	d(g->name());
	delete g;

	g = (cGate*)right->gatev.get(c);
	right->gatev.remove(c);
//	g->setTo(NULL);
	g->disconnect();
//	g->setFrom(NULL);

	d(g->name());
	delete g;

	g = (cGate*)rPhysic->gatev.get(d);
	rPhysic->gatev.remove(d);
	//g->setFrom(NULL);
	d(g->name());
	delete g;

	// this was suggested by Varga to avoid the errors that come out using TkEnv
	// but it doesn't seem to work...
	//left->setDisplayString(left->displayString());
	return true;
}



bool Physic::getGateIndex(cModule* left,cModule *right,int& a,int&b, int& c, int& d)
{
	int gateNum = left->gates();
	cGate *g,*farGate;
	bool found = false;
	int i = 0;

	d("In GetGateIndex from "<<left->name()<<" to "<<right->name());
	while( (i< left->gatev.items()) &&( !found ))
	{
		d(i);
		g = (cGate *)left->gatev.get(i);

		if ((g != NULL) &&(g->type()== 'O') && (g->isConnected()))
		{
			d("maybe.... "<<g->name());

			if(g->toGate()->ownerModule()->id() == right->id())
			{
				//it's the right gate
				farGate = (cGate*) g->toGate();
				d("gates to disconnect found");
				a = g->fromGate()->id();
				b = i;
				c = g->toGate()->id();
				d = farGate->toGate()->id();
				found = true;
			}
		}
		i++;
	}
	return found;
}


void Physic::finish()
{
	d("Physic  says bye");
	 FILE* fout = fopen("collectedData.dat","a");

	 d("Messages with errors..."<<msgWithErr);
	 fprintf(fout,"Messages with errors........ %d\n",msgWithErr);

	 fclose(fout);
}

bool Physic::isReachable(int from, int to)
{
	Physic *s = (Physic*) simulation.module(from);
	Physic *d = (Physic*) simulation.module(to);
	int x,y;
	double dist,a;
	bool r;

	d->getPos(x,y);

	//Pitagora @ work...
	dist = sqrt( ((x - s->posX )*
		      (x - s->posX )) +
		     ((y - s->posY )*
		      (y - s->posY )) );

	d("Distance from "<<from<<" to "<<to<<"= "<<dist);
/*	a = sqrt( ((d->posX - s->posX )*
		      (d->posX - s->posX )) +
		     ((d->posY - s->posY )*
		      (d->posY - s->posY )) );
*/
	r = ( dist <= range);
	return r;
}

void Physic::getVect(int i, int& ph, int& mh)
{
	cNeighbour * n = NULL;
	n = (cNeighbour*)hosts[i];

	ph = n->ph;
	mh = n->mobHost;
}

void Physic::setPos(int x, int y)
{
	posX = x;
	posY = y;
}

void Physic::getPos(int &x, int &y)
{
	x = posX;
	y = posY;
}






