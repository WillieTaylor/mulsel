# 1 = seqs

./cd-hit -i shuffle$argv[1].fasta -o shuffle$argv[1].90.out -c 0.9
./cd-hit -i shuffle$argv[1].fasta -o shuffle$argv[1].80.out -c 0.8
./cd-hit -i shuffle$argv[1].fasta -o shuffle$argv[1].70.out -c 0.7
./cd-hit -i shuffle$argv[1].fasta -o shuffle$argv[1].60.out -c 0.6 -n 4
./cd-hit -i shuffle$argv[1].fasta -o shuffle$argv[1].50.out -c 0.5 -n 3
./cd-hit -i shuffle$argv[1].fasta -o shuffle$argv[1].40.out -c 0.4 -n 2

