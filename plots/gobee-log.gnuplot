N=ARGC-1
load 'heading.gnuplot'

set output ARG1

array ARGV[ N ]
do for [i=1 : N] { eval sprintf("ARGV[%d] = ARG%d", i, i+1) }

load 'border.gnuplot'

set datafile separator "\t"

set bmargin 8
set timefmt "%Y-%m-%d %H:%M:%S"

set multiplot layout 2,1
set xzeroaxis
set yzeroaxis

set xdata time
set format x "%m-%d %H:%M"
set xtics rotate by 90 offset 0,-4.5

set grid

array floors[N]
do for [ i = 1 : N ] { floors[i] = ( substr(ARGV[i],17,27) eq 'A4C138E4544' ) ? 2 : 1 }

plot for [i=1:N] ARGV[ i ] every 60 using (timecolumn(1)+3600*9):2 with points pt 3+(i*2) ps 1.5 lt i notitle "Temp.\\si{\\celsius}" \
     , for [i=1:N] ARGV[ i ] using (timecolumn(1)+3600*9):2 smooth bezier lw 5 lt i title sprintf("%dF \\si{\\celsius}", floors[i])

plot for [i=1:N] ARGV[i] using (timecolumn(1)+3600*9):3 with linespoints pt i lt i + N title sprintf("%dF R.H.\\si{\\percent}", floors[i])


