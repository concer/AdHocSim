#ifndef AODVCONST_H
#define AODVCONST_H

//these are costants for the msg->kind field
//then it is important to do not set them with the
//same values of the other costants like DATA,MOVE..
//in costants.h that go in the same message field
#define RREQ 4
#define RREP 5
#define RERR 6
#define RREP_ACK  8 //for the rrep msg!
#define HELLO 9

/* ----- self messages ---- */
#define SEND_HELLO 11
//make a data send trial to be aborted
#define FLUSH 10
//delete the entry of a route in the routes table
#define DELETE   12
//ntice the rrep ack has failed to arrive
#define ESP_ACK 13
//make a node to be taken out from the black list
#define BLK_LIST 7

/* ------ message priorities ------ */
#define  P_RREP 1
#define  P_RREQ 2
#define  P_RERR 5
#define  P_RREP_ACK 0
#define  P_HELLO 7
#define  P_SEND_HELLO 6
#define  P_FLUSH  11
#define  P_DELETE 8
#define  P_ESP_ACK 9
#define  P_BLK_LIST 10

/* ----- time costants:   1sec = 1 ----- */

///evry 3ms 3   it shold be ALLOWED_HELLO_LOSS * HELLO_INTERVAL.
#define ACTIVE_ROUTE_TIMEOUT 3

#define MY_ROUTE_TIMEOUT 2 * ACTIVE_ROUTE_TIMEOUT

#define ALLOWED_HELLO_LOSS 2
//============
#define HELLO_INTERVAL 100

//
#define DELETE_PERIOD 4

//almost how many hops to go from side to side.
#define NET_DIAMETER 24

#define NET_TRAVERSAL_TIME 	3*NODE_TRAVERSAL_TIME*NET_DIAMETER / 2

#define PATH_TRAVERSAL_TIME 	2*NET_DIAMETER*NODE_TRAVERSAL_TIME

//maximum waiting time for an ack message
#define NEXT_HOP_WAIT  NODE_TRAVERSAL_TIME + 2

/* The IEEE AODV standard draft v10 specify the reverse route time life as :
 * current time + PATH_TRAVERSAL_TIME - 2* HopCount * NODE_TRAVERSAL_TIME
 * but the costant NODE_TRAVERSAL_TIME has an undefined value of 40.
 * If this value is intended to be seconds,it is extremly high.
 * For this reason I decided to define the route time out like it has been done
 * in the MONARCH project, setting it to 6 seconds.
 */

#define REV_ROUTE_LIFE 6

/* ------ other costants -----*/

#define INFINITE 65536

#define RREQ_RETRIES 3
#define RREP_RETRIES 2

#define TTL_START 1
#define TTL_INCREMENT 2
#define TTL_THRESHOLD 7

#define BLACKLIST_TIMEOUT		RREQ_RETRIES*NET_TRAVERSAL_TIME

//18 byte * 8 = 144 bit
#define CTRL_PKT_SIZE  512



#endif
