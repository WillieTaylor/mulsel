cat $argv[1].fasta | awk '/^>/{n++; print "seq" n "\n" $1} /^[A-Z]/{print $1 "\n" "*"}' | sed 's/>//' | sed 's/^-//' | sed 's/seq/>seq/' > $argv[1].seq
