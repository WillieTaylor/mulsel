cat $argv[1].seq | awk '/seq/{printf(" = %s",$0)} /^[A-Z][A-Z]/{print "\n" $0}' | sed 's/ = //' > $argv[1].fasta
