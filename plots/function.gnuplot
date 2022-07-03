# https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/

AH(RH,T) = (6.112 * exp((17.67 * T)/(T+243.5)) * RH * 2.1674)/(273.15+T)
