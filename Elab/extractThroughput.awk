
BEGIN		{ printf"speed \t nHops \t throughput\n"; 
		  speed =0;
		} 

/Speed/		{ speed = $3;}
/throughput/	{ printf"%d \t %d \t %.0f \n",speed, $4, $5;}

