
BEGIN		{ printf"speed \t nHops \t latency\n"; 
		  speed =0;
		} 

/Speed/		{ speed = $3; }
/latency/	{ printf"%d \t %d \t %.6f \n",speed, $4, $5;}

