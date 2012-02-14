/*compute the delivery rate h <speed, sent, delivered> point*/

NR == 1		{
			h = $2;
			sp = $1
			samples = 1;
		}
NR >1 		{
			if( sp != $1)  
			{
				printf"%d \t %.2f\n", sp, h /samples;
			
				/* reinitialize the variables  */
				h = $2;
				sp = $1;
				samples = 1;
			}
			else
			{
				h += $2;
				samples ++;
			}
		}

END		{
			/* print the last row that otherwise wouldn't be printed */
			
			printf"%d \t %.2f\n", sp, h /samples;
		}
		



