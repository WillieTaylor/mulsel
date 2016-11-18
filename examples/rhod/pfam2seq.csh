cat PF00072_seed.txt | tr -d "\."  | awk '{n++; print ">pfam" n "\n" $1 "\n" $2 "\n" "*"}' > seedPF.seq
cat PF00072_seed.txt | tr "\." "x" | awk '{n++; print ">pfam" n "\n" $1 "\n" $2 "\n" "*"}' > gapsPF.seq
