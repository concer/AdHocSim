/*compute the latency mean of each <speed, nHops, latency> point*/

NR == 2		{
			speed = $1;
			hops = $2;
			lat = $3;
			samples = 1;
		}
NR > 2		{
			if( (speed != $1) || (hops != $2) )
			{
				/* print the computed <speed, nHops, latency> point */
				
				printf"%d \t %f\n", hops, lat/samples;
			
				/* create a new graph index for gnuplot compatibility */
				if(speed != $1)
					printf"\n\n";

				/* reinitialize the variables  */
				samples = 1;
				speed = $1;
				hops = $2;
				lat = $3;
			}
			else
			{
				samples ++;
				lat += $3;
			}
		}

END		{
			/* print the last row that otherwise wouldn't be printed */
			
			printf"%d \t %f\n", hops, lat/samples;
							
		}
				
		



