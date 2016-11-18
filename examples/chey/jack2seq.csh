cat start.jack | sed 's/ /#= /' | tr "#" "\n" | awk '/^>/ {print $1}; /^=/ {print "Seq " $0}; /^[A-Z-]/ {print $0 "\n" "*"}' > rhod.gap.seq

