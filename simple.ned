

simple Mobility
    parameters:
    	minSpeed,       // vel in m/s
	maxSpeed,
	movKind,        //0 torus, 1 rebound
	XRange,	YRange,
	pauseTime,      //pause waited after reacing of a destination
	moveInterval,   //time to elaps between two steps
	distance, 	//used in random waypoint
	target,		//used in pursuit model
	speedVariator;
    gates:
    	out: out;
endsimple

simple Physic
    parameters:
    	txPower,
	rxThreshold,
	channelDelay,
	channelDatarate,
	channelError;
    gates:
	in: fromMobility;
	in: fromMac;
	out: toMac;
endsimple


simple Mac
    parameters:
	inBufferSize,
	promisqueMode : bool;
    gates:
        in: fromPh;
        in: fromRoute;
        out: toRoute;
        out: toPh;
endsimple


simple Application
    parameters:
    	rate,       //paket per secod
	pktSize,
	hostNum,
	active,
	burstInterval; // time(s) between two data bursts
    gates:
    	out: out;
endsimple

simple Routing
    gates:
        in: fromMac;
	in: fromApp;
	out: toMac;
endsimple

//IEEE-802.11a channel
//channel etere
//	delay 0.0001;
//	datarate 11.04858e+6 ; // 11Mbit/sec
//	error 0.000001;
//endchannel




