# 1 = nseq per base (always a bit less)

# tcsh randseq.csh 6360
# makes ~90K (92681 = pair ID limit)

cc -O randseq.c -o randseq util/wt/util.o util/wt/geom.o util/wt/sort.o -lm

# random.seq  = single line + gaps in order made
# shuffle.aln = random order + gaps
# shuffle.seq = random order - gaps
# shuffle.txt = random order - gaps - code - title

rm random.seq
foreach base ( A C D E F G H I K L M N P Q R S T V W Y )
	./randseq $base $argv[1] | tr "BJOUXZ" "-" >> random.seq
end
grep -v '[0-9]' random.seq | sed 's/.$//' > random.txt
cat random.seq | tr "\n" "#" | tr "*" "\n" | grep -v '^>' | grep family  | sort -n -k5 | tr "#" "\n" | sed 's/^$/*/' > shuffle.aln
sed -i '1 d' shuffle.aln
echo "*" >> shuffle.aln
cat shuffle.aln | tr -d "\-" > shuffle.seq
cat shuffle.seq | grep -v '[0-9]' | grep -v '*' > shuffle.txt
cat shuffle.txt | wc -l
