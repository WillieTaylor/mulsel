cat $argv[1].fasta | awk '/^>/{n++; print "*" "\n" "seq" n "\n" $1} /^[A-Z]/{print $1}' | sed 's/>//' | sed 's/^-//' | sed 's/seq/>SEQ/' > $argv[1].seq
