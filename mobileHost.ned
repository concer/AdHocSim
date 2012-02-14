import  
    "simple"; 
              
module MobileHost 
    parameters: 
        numHost : numeric const, 
        x : numeric, // actual position
        y : numeric, //    "
        Xbound : numeric, 
        Ybound : numeric, 
	macAlgorithm  : string,
        routeAlgorithm : string, 
        mobilityModel : string; //randomWP or Pursuit or NormalWalk
                                
    submodules: 
                
        mobility: mobilityModel like Mobility; 
            parameters: 
                maxSpeed = input(0,"max speed"),
		minSpeed = input(0,"min speed"),
                movKind = input(0,"0 toroidal, 1 rebound"), 	
                XRange = Xbound, 
                YRange = Ybound, 
                moveInterval = input(0.5s ,"movement period"), 
                distance = input(100 ,"distance to cover"), 
                target = input(-1,"target node id"), 	//in this way I can choose a random target each time
                pauseTime = input(0,"pause between two moves"),
		speedVariator = input(1,"speed choice variance");
            display: "p=252,128;b=24,26";

	physic: Physic; 
            parameters: 
                txPower = input, 
                rxThreshold = input,
	        channelDelay = input,
	        channelDatarate = input,
	        channelError = input;
		
            display: "p=148,62;b=104,10";
	    
        mac: macAlgorithm like Mac; 
            parameters: 
                inBufferSize = input, 
                promisqueMode = input; 
            display: "p=148,116;b=104,10";
	    
        route: routeAlgorithm like Routing; 
            parameters: 
            display: "p=148,171;b=104,10";
	    
        app: Application; 
            parameters: 
                rate = input, 
                pktSize = input, 
                burstInterval = input, 
                hostNum = numHost, 
                active = input; 
            display: "p=148,216;b=104,10";
    connections: 
                 
        app.out --> route.fromApp display "m=m,24,16,24,16"; 
                                                             
        route.toMac --> mac.fromRoute display "m=m,24,0,24,4"; 
        mac.toRoute --> route.fromMac display "m=m,80,0,80,0"; 
                                                               
        mac.toPh --> physic.fromMac display "m=m,24,12,24,12"; 
        physic.toMac --> mac.fromPh display "m=m,80,0,80,8"; 
                                                             
        mobility.out --> physic.fromMobility display "m=m,64,8,88,44"; 
                                                                       
    display: "p=10,10;b=287,250";
endmodule 



