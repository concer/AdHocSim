
#Create a plotable file extracting the latency information form 
#the collectedData.dat file

#xtract rough data
awk -f extractDalRate.awk ../collectedData.dat >ratDat1

#sort
sort -n +0 ratDat1 > ratDat2

#generate the latency file
awk -f genPlotDelRate.awk ratDat2 > delRatData.dat

#emove temporaney files
rm -f ratDat1 ratDat2 

#create the plot file and plot it!
gnuplot delivertyRatio.gp && gv DeliveryRatio.ps

