 2089  cp shuffle30.seq shuffle.seq
 2090  tcsh softlens.csh
 2091  vim soft30.dat
 2092  more soft30.dat
 2093  grep '^90 ' soft30.dat | awk '{print $2}' | tr -d "u" > soft30.time
 2094  grep '^50 ' soft30.dat | awk '{print $6}' > soft30.last
 2095  paste soft30.last soft30.time > soft30.txt
