
set terminal postscript eps
set size 2,2
set output "Matrix.ps"
set title "Matrix e'tornata"
set xlabel "X area"
set ylabel "Y area "
set zlabel "traffic"

#set xtics (100,200,300,400)
set ytics (100,200,300,400)


set parametric

splot \
"matrix.dat" index 0 using 1:2:3 "%lf%lf%lf" title "Random Walk" \
with lines 1


show key
show xlabel
show ylabel
show output
