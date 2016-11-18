# 1 = file(.fasta)

# copy seqs
tcsh fasta2seq.csh $argv[1]
cp $argv[1].seq final.seq
grep '>' final.seq | wc -l

#reduce
foreach level ( 90 80 70 60 50 )
	cp skip$level.run skip.run
	set tim = `time code/mulsel > sel$level.log`
	set sel = `tail -1 sel$level.log`
	set fam = `cat final.seq | grep family | awk '{if($8!=last){print $8}; last = $8}' | wc -l`
	echo $level $tim[1] $sel $fam
end

# align
code/multas > mul.log
cat final.tre

# predict and colour
tcsh main/psipred.csh final
eog finalpred.png
