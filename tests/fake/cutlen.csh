# user_file,&inseq,&mins,&maxs,&pep_len,&table,&pep_span,&soft
# final.seq  92680 20 2000  4 1 8 0

echo pep.len = $argv[1]  soft = $argv[2]
cp save/skip*.run .
eval "sed -i 's/2000  X/2000  $argv[1]/' skip*.run"
eval "sed -i 's/ Y / $argv[2] /' skip*.run"
cp shuffle.seq final.seq
grep '>' shuffle.seq | wc -l
foreach level ( 90 80 70 60 50 )
	cp skip$level.run skip.run
	set tim = `time ./mulsel > sel$level.log`
	set sel = `tail -1 sel$level.log`
	set fam = `cat final.seq | grep family | awk '{if($8!=last){print $8}; last = $8}' | wc -l`
	echo $level $tim[1] $sel $fam
end
./multas > mul.log
cat final.tre | grep -v '^$' | awk '{if($11!=last){print $11}; last = $11}' | wc -l
