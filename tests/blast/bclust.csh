# 1 = sequences (shuffle9)

if ( ! -e $argv[1].fasta ) then
	cat ../code/$argv[1].seq | grep -v fam | grep -v '\*' > $argv[1].fasta
endif
echo Running BlastClust on `grep '>' $argv[1].fasta | wc -l` sequences in $argv[1].fasta
time ./bclust -i $argv[1].fasta > bclust.log
rm clust.out
foreach line ( `grep seq bclust.log | tr -d "[a-z] [0-9]"` )
	echo $line >> clust.out
end
wc clust.out
