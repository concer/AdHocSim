
set terminal postscript eps
set size 2,2
set output "DeliveryRatio.ps"
set title "<Speed avarage x Delivery ratio> Graph"

set xlabel "X speed (m/s) "
set ylabel "Delivery Ratio"

set terminal postscript eps color

set yrange[0:10]
set xrange[0:20]

set parametric

plot \
"delRatData.dat" index 0 using 1:2 "%lf%lf" title "Ratio" \
with lines 1

show key
show xlabel
show ylabel
show output
