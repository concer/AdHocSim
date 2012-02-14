
#Create a plotable file extracting the latency information form 
#the collectedData.dat file

#xtract rough data
awk -f extractHops.awk ../collectedData.dat>hops1
#collctedData.dat >hops1

#sort
sort -n +0 hops1 > hops2

#generate the latency file
awk -f genPlotHost.awk hops2 > hopsData.dat

#emove temporaney files
rm -f hops1 hops2 

#create the plot file and plot it!
gnuplot hopsData.gp && gv HopsData.ps

