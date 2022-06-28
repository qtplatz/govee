N=ARGC-1
load 'heading.gnuplot'

set output ARG1

array ARGV[ N ]

do for [i=1 : N] { eval sprintf("ARGV[%d] = ARG%d", i, i+1) }

load 'border.gnuplot'
#load 'margin.gnuplot'
#set lmargin 15
set bmargin 8

set datafile separator "\t"
set timefmt "%Y-%m-%d %H:%M:%S"

set multiplot layout N,2
set xzeroaxis
set yzeroaxis

set xdata time
set format x "%m-%d %H:%M"
set xtics rotate by 90 offset 0,-4.5

do for [ i = 1 : N ] {

floor = ( substr(ARGV[i],17,27) eq 'A4C138E4544' ) ? 2 : 1

set title sprintf("%dF %s", floor, substr(ARGV[i],17,27) )

plot ARGV[i] using (timecolumn(1)+3600*9):2 with linespoints pt 7 title "Temp.\\si{\\celsius}"

plot ARGV[i] using (timecolumn(1)+3600*9):3 with linespoints pt 5 lc "forest-green" title "R.H.\\si{\\percent}"

}
