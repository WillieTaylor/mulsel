# 1 = gapped sequences, 2 = fractional gap cutoff

#paste -d '#' codes.dat cols.dat | tr " " "~" > seqs.tmp
set len = `cat seqs.tmp | head -2 | tail -1 | wc -c`
echo Query length is $len
@ m = 1
@ max = $len / $argv[2]
echo Removing sequences with over $max gaps
foreach seq (`cat seqs.tmp`)
	set n = `echo "$seq" | tr -d "-" | wc -c`
	@ gaps = $len - $n
	if ( $gaps > $max && $m > 1 ) continue
	echo "$seq" | tr "#" "\n" | tr "~" " " >> $argv[1].cut.faln
	@ m++
end
echo $m left
