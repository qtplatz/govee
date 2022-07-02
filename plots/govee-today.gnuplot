N=ARGC-1
set terminal epslatex size 10.5in,(4*N*2)in color colortext standalone header \
"\\usepackage{graphicx}\n\\usepackage{amsmath}\n\\usepackage[version=3]{mhchem}\n\\usepackage{siunitx}\n\\DeclareSIUnit\\count{c}"

set output ARG1

array ARGV[ N ]
do for [i=1 : N] { eval sprintf("ARGV[%d] = ARG%d", i, i+1) }

load 'days_of_month.gnuplot'

array location[N]
do for [i=1 : N] { call 'location.gnuplot' sprintf("%d", i)  word(ARGV[i],1) }

load 'border.gnuplot'

set datafile separator "\t"

set bmargin 8
set timefmt "%Y-%m-%d %H:%M:%S"

set multiplot layout 4,1
set xzeroaxis
set yzeroaxis

set xdata time
set format x "%m-%d\n%H:%M"
set xtics rotate by 90 offset 0,-2.5

set grid

tm = time(0)+(9*3600)
today    = strftime("%Y-%m-%d 00:00:00", tm )
tomorrow = strftime("%Y-%m-%d 00:00:00", tm+24*3600)

set xrange [today:tomorrow]

plot for [i=1:N] '< cat ' . ARGV[ i ] every 60 using (timecolumn(1)+3600*9):2 with points pt 3+(i*2) ps 1.5 lt i notitle "Temp.\\si{\\celsius}" \
     , for [i=1:N] '< cat ' . ARGV[ i ] using (timecolumn(1)+3600*9):2 smooth bezier lw 5 lt i title sprintf("%s \\si{\\celsius}", location[i])

plot for [i=1:N] '< cat '.ARGV[i] using (timecolumn(1)+3600*9):3 with linespoints pt i lt i + N title sprintf("%s R.H.\\si{\\percent}", location[i])

month0    = strftime("%Y-%m-01 00:00:00", tm )
month1    = strftime(sprintf("%%Y-%%m-%02d 00:00:00", days_of_this_month), tm )

set xrange [month0:month1]

plot for [i=1:N] '< cat '.ARGV[ i ] using (timecolumn(1)+3600*9):2 smooth bezier lw 5 lt i title sprintf("%s \\si{\\celsius}", location[i])

plot for [i=1:N] '< cat '.ARGV[i] using (timecolumn(1)+3600*9):3 smooth bezier lw 5 lt i + N title sprintf("%s R.H.\\si{\\percent}", location[i])
