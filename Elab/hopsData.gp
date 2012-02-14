
set terminal postscript eps
set size 2,2
set output "HopsData.ps"
set title "<Hosts speed average  x message hops average>"

set xlabel "X speed (m/s) "
set ylabel "Hops average"

set terminal postscript eps color

;set xrange[0:14]
set yrange[0:4]

set parametric

plot \
"hopsData.dat" index 0 using 1:2 "%lf%lf"  title " hops average" \
with lines 1

show key
show xlabel
show ylabel
show output
