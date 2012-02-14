
BEGIN		{  
		  speed =0;
		  sent =0;
		} 

/Speed/		{ speed = $3; }
/Sent data/	{ sent =  $4; }
/Delivered/	{ printf"%d \t %d \t %d \n",speed, sent, $4;}

