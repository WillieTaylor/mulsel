cp simple.run skip.run
cp skip.run hold.run
grep '^>' $argv[1].seq | wc -l
@ n = 0
while ( 1 )
	@ n++
	echo $n pass
	cp $argv[1].seq final.seq
	time ./mulsel > mulsel.log
	tail -1 mulsel.log
	cat final.seq | grep family | awk '{s=$8; if(s!=last){print s}; last = s}' | wc -l
	tail -1 hold.run >> skip.run
	echo
end
