# 1 = file(.fasta)

# copy seqs
#tcsh fasta2seq.csh $argv[1]
cp $argv[1].seq final.seq
grep '>' final.seq | wc -l
cat $argv[1].seq | sed 's/>SEQ/<REV/' >> final.seq

#reduce
foreach level ( 99  )
	~/mulsel/tests/revs/chey/mulsel > sel.log
	set sel = `tail -1 sel$level.log`
	sed -i 's/</>/' final.seq
end
grep -v '\-' final.seq | sed 's/*$//' > final.fasta
