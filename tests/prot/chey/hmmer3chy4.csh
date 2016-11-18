cp skip90.run skip.run
foreach part ( A B C D )
	echo running $part
	cp hmmer3chy$part.seq final.seq
	time code/mulsel > run$part.log
	grep '>' final.seq | wc -l
	mv final.seq final$part.seq
end
cat final[A-D].seq > reduced.seq
tcsh align.csh reduced
