/* ------ constants used in the simulator  ------ */
#ifndef COSTANTSZ_H
#define COSTANTSZ_H

#define MOVE 2
#define DATA 3

/* ------ message priorities ------ */
#define P_MOVE 4
#define P_DATA  3

/* ------ time costants ------ */


/*should include queueing delays, interrupt processing times and transfer  times*/
#define NODE_TRAVERSAL_TIME 30e-03



/* ------ other costants ------ */

#define BROADCAST -1
//#define DEBUG a

#ifdef DEBUG
	#define d(x) ev<<((parentModule()->id()-2)/5)<<","<<name()<<":"<<x<<"\n"
#endif

#ifndef DEBUG
	#define d(x) /* x .*/
#endif

#define dd(x) ev<<((parentModule()->id()-2)/5)<<","<<name()<<":"<<x<<"\n"


#endif


