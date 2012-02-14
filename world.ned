import "mobileHost";

module World
    parameters:
        dim ,
	width,
	height;

    submodules:
    
        mobileHost: MobileHost[dim];
	   parameters:
		numHost = dim,
		Xbound = width,
		Ybound = height,

		//x = width /2,
	        //y = height /2;
		x = intuniform(5, width -5),
		y = intuniform(5, height -5);
		//x = 60 + (index % 5 ) * 120,
                //y = 30 + (index - index %  5 ) * 30 ;

	display: "b=20,20,oval";


     display: "p=10,10;b=$width,$height";
endmodule 


network world : World
    parameters:
    	dim = input,
	width = input,
	height = input;

endnetwork



