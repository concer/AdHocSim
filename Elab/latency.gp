
set terminal postscript eps
set size 2,2
set output "Latency.ps"
set title "< Hops performed x Latency measured> Graph"
#set xlabel "X speed avarage"
#set ylabel "Y umber of hops "
#set zlabel "latency"

set xlabel "X number of hops "
set ylabel "latency"

#set xtics (100,200,300,400)
#set ytics (100,200,300,400)

#set xrange[1:7]

set terminal postscript eps color

set parametric

plot \
"latData.dat" index 0 using 1:2 "%lf%lf" title "speed = 0  m/s" \
with lines 1
#,\
#"latData.dat" index 1 using 1:2 "%lf%lf" title "speed = 1  m/s" \
#with lines 3,\
#"latData.dat" index 5 using 1:2 "%lf%lf" title "speed = 5  m/s" \
#with lines 5

show key
show xlabel
show ylabel
show output
