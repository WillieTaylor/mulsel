wc -l PF00072_$argv[1].txt
cat PF00072_$argv[1].txt | sed 's/ [A-Z\.a-z]/#&/' | awk -v r=$argv[1] '{n++; print ">" r "=" n "#" $0 "*"}' | tr "#" "\n" | tr -d "." > dash.seq
cat dash.seq | sed 's/-[0-9]/+&/' | tr -d "-" | tr "+" "-" | tr "[a-z]" "[A-Z]" | sed 's/^ //' > $argv[1].seq
grep '^>' $argv[1].seq | wc -l
