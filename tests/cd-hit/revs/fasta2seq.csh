cat $argv[1].out | awk '/seq/{n++; print ">>seq" n "\n" $0} /^[A-Z]/{print $1 "*"}' | sed 's/>//' > $argv[1].seq
