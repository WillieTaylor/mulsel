foreach part ( A1 A2 B1 B2 C1 C2 D1 D2 )
	echo running $part
	cp hmmer3chy$part.seq final.seq
	cp skip90.run skip.run
	time code/mulsel > run90$part.log
	cp skip80.run skip.run
	time code/mulsel > run80$part.log
	cp skip70.run skip.run
	time code/mulsel > run70$part.log
	grep '>' final.seq | wc -l
	mv final.seq final$part.seq
end
cat final[A-D][12].seq > reduced.seq
tcsh align.csh reduced
