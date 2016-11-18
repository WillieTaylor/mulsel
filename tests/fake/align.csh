#cc -c -ggdb seqsort.c
#cc -ggdb mulsel.c -o mulsel seqsort.o util/wt/util.o util/wt/sort.o -lm
#cc -c -O seqsort.c
#cc -O mulsel.c -o mulsel seqsort.o util/wt/util.o util/wt/sort.o -lm

cp shuffle.seq final.seq
grep '>' shuffle.seq | wc -l
#foreach level ( 90 80 70 60 50 )
foreach level ( 90 80 70 )
	cp skip$level.run skip.run
	set tim = `time ./mulsel > sel$level.log`
	set sel = `tail -1 sel$level.log`
	set fam = `cat final.seq | grep family | awk '{if($8!=last){print $8}; last = $8}' | wc -l`
	echo $level $tim[1] $sel $fam
end
./multas > mul.log
cat final.tre | grep -v '^$' | awk '{if($11!=last){print $11}; last = $11}' | wc -l
