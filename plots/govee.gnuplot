N=ARGC-1
set terminal epslatex size 10.0in,(3.5*(3*N))in color colortext standalone header \
"\\usepackage{graphicx}\n\\usepackage{amsmath}\n\\usepackage[version=3]{mhchem}\n\\usepackage{siunitx}\n\\DeclareSIUnit\\count{c}\n\
\\sisetup{inter-unit-product = \\ensuremath {{}\\cdot{}},\
}%\n"

load 'border.gnuplot'

set output ARG1

array ARGV[ N ]
do for [i=1 : N] { eval sprintf("ARGV[%d] = ARG%d", i, i+1) }

load 'days_of_month.gnuplot'
load 'function.gnuplot'

array location[N]
do for [i=1 : N] { call 'location.gnuplot' sprintf("%d", i)  word(ARGV[i],1) }

set datafile separator "\t"

set bmargin 4
set timefmt "%Y-%m-%d %H:%M:%S"

set multiplot layout N*3,1
set xzeroaxis
set yzeroaxis

set xdata time
set format x "%m/%d\n%H:%M"
set xtics rotate by 90 offset 0,-2.5 #format "%D\n%R" time

set grid
set style arrow 1 head size character 2,20 ls 4 lw 2 lc rgb "blue"
set style arrow 8 head size character 2,20 ls 4 lw 4 lc rgb "light-pink"

set xrange[tm_str_day_befor(2):tm_str_day_befor(-1)]

day_befor(x) = strftime("%Y-%m-%d %H:%M:%S", local_time - x * 86400)
set arrow 1 as 1 from day_befor(0), graph 0 to day_befor(0), graph 1 nohead
set arrow 2 as 1 from day_befor(1), graph 0 to day_befor(1), graph 1 nohead
set arrow 3 as 1 from day_befor(2), graph 0 to day_befor(2), graph 1 nohead

set ylabel "Temp. (\\si{\\celsius})"

plot 35 lc "coral" notitle \
     , 30 lc "coral" notitle \
     , 25 lc "coral" notitle \
     , for [i=1:N] '< cat ' . ARGV[ i ] using (timecolumn(1)+3600*9):2 with linespoints ps 0.5 lt i title sprintf("%s \\si{\\celsius}", location[i])

#, for [i=1:N] '< cat ' . ARGV[i] using (timecolumn(1)+3600*9):2 smooth bezier lw 5 lt i title sprintf("%s \\si{\\celsius}", location[i]) \

set ylabel "R.H. (\\si{\\percent})"

plot for [i=1:N] '< cat ' . ARGV[i] using (timecolumn(1)+3600*9):3 with linespoints ps 0.5 lt i + N title sprintf("%s R.H.\\si{\\percent}", location[i])

set ylabel "A.H. (\\si{\\gram\\per\\cubic\\meter})"

plot for [i=1:N] '< cat ' . ARGV[i] using (timecolumn(1)+3600*9):(AH($3,$2)) with linespoints ps 0.5 lt i + N title sprintf("%s A.H.", location[i])


set xrange[tm_str_week_befor(2):tm_str_month_befor(-1)]
tm_week0 = local_time - (2 * 86400)

do for [i=1:6] {
    set arrow i as 8 from tm_str_week_befor(2-i), graph 0 to tm_str_week_befor(2-i), graph 1 nohead
}

set format x "%m/%d"
set xtics rotate by 90 offset 1,-2.5 #format "%D\n%R" time
set xtics 86400
set ylabel "Temp. (\\si{\\celsius})"

plot 35 lc "coral" notitle \
     , 30 lc "coral" notitle \
     , 25 lc "coral" notitle \
     , for [i=1:N] '< cat ' . ARGV[i] using (timecolumn(1)+3600*9):2 smooth bezier lw 5 lt i title sprintf("%s \\si{\\celsius}", location[i])

set ylabel "R.H. (\\si{\\percent})"

plot for [i=1:N] '< cat ' . ARGV[i] using (timecolumn(1)+3600*9):3 smooth bezier lw 5 lt i + N title sprintf("%s R.H.\\si{\\percent}", location[i])

set ylabel "A.H. (\\si{\\gram\\per\\cubic\\meter})"

plot for [i=1:N] '< cat ' . ARGV[i] using (timecolumn(1)+3600*9):(AH($3,$2)) smooth bezier lw 5 lt i + N title sprintf("%s A.H.", location[i])
