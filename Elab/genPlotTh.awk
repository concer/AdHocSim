/*compute the throughput mean of each <speed, nHops, throughput> point*/

NR == 2		{
			speed = $1;
			hops = $2;
			th = $3;
			samples = 1;
		}
NR > 2		{
			if( (speed != $1) || (hops != $2) )
			{
				/* print the computed <speed, nHops, th> point */
				
				printf"%d \t %f\n",hops, th/samples;
			
				/* print an empty row for gnuplot compatibility */
				if(speed != $1)
					printf"\n\n";

				/* reinitialize the variables  */
				samples = 1;
				speed = $1;
				hops = $2;
				th = $3;
			}
			else
			{
				samples ++;
				th += $3;
			}
		}

END		{
			/* print the last row that otherwise wouldn't be printed */
			
			printf"%d \t %f\n", hops, th/samples;
							
		}
				
		



