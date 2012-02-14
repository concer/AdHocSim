
#Create a plotable file extracting the latency information form 
#the collectedData.dat file

#xtract rough data
awk -f extractLatency.awk ../collectedData.dat >latDat1

#ort it 
sort latDat1 -n +0 +1 >latDat2

#generate the latency file
awk -f genPlotLat.awk latDat2 > latData.dat

#emove temporaney files
rm -f latDat1 latDat2 

#create the plot file and plot it!
gnuplot latency.gp && gv Latency.ps

