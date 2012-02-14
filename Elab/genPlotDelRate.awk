/*compute the delivery rate h <speed, sent, delivered> point*/

NR == 2		{
			speed = $1;
			sent  = $2;
			del = $3;
		}
NR > 2		{
			if( speed != $1)  
			{
				
				
				tmp = 0;
				
				printf"%d \t %.2f\n", speed, del / sent * 100;
			
				/* reinitialize the variables  */
				speed = $1;
				sent = $2;
				del = $3;
			}
			else
			{
				sent += $2;
				del += $3
			}
		}

END		{
			/* print the last row that otherwise wouldn't be printed */
			
				printf"%d \t %.2f\n", speed, del / sent * 100;
		}
		



