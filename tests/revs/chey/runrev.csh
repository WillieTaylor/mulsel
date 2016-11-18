# 1 = file(.fasta)

# copy seqs
#tcsh fasta2seq.csh $argv[1]
cp $argv[1].seq final.seq
grep '>' final.seq | wc -l
cat $argv[1].seq | sed 's/>SEQ/<REV/' >> final.seq

#cat $argv[1].seq | sed 's/>SEQ/<REV/' > final.seq
#cat $argv[1].seq >> final.seq

#reduce
foreach level ( 90 80 70 60 50 40 38 36 36 )
	cp skip$level.run skip.run
	set tim = `time ./mulsel > sel$level.log`
	grep 'SEQ and REV together' sel$level.log
	if ( $? == 0 ) then
	# revert to last set of sequences
		cp final.hold final.seq
		break
	endif
	set sel = `tail -1 sel$level.log`
	set fam = `cat final.seq | grep family | awk '{if($8!=last){print $8}; last = $8}' | wc -l`
	set seq = `grep '^.SEQ' final.seq | wc -l`
	set rev = `grep '^.REV' final.seq | wc -l`
	echo $level $tim[1] $sel $fam $seq $rev
	#sed -i 's/</>/' final.seq
	if ( $seq == 1 ) break
	if ( $rev == 1 ) break
	@ all = $seq + $rev
	if ( $all < 100 ) break
	cp final.seq final.hold
end

# align
code/multas > mul.log
cat final.tre
grep aligned mul.log
echo last cut = `grep '^[1-9][0-9][0-9]$' mul.log | tail -1`
echo

exit

# predict and colour
if ( -e psipred ) rm -r psipred
tcsh main/psipred.csh final > psipred.log
eog finalpred.png
