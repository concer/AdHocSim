
set terminal postscript eps
set size 2,2
set output "Throughput.ps"
set title "<Hops performed x Throughput> mon a 11e6 bit/sec channel"

set xlabel "X number of hops "
set ylabel "bit/s"

#set terminal postscript eps color

set parametric

set yrange [0:10e6]

plot \
"thData.dat" index 0 using 1:2 "%lf%lf" title "speed = 15" \
with lines 1
#,\
"#thData.dat" index 1 using 1:2 "%lf%lf" title "speed = 2" \
#with lines 5,\
#"thData.dat" index 5 using 1:2 "%lf%lf" title "speed = 5" \
#with lines 3

show key
show xlabel
show ylabel
show output
