# head -1 RNAfold.out | sed 's/./x&/g' | tr "x" "\n" | awk '{if($1=="("){print $1,n; n++} if($1==")"){n--; print $1,n;} if($1=="."){print $1}}' | more
head -1 RNAfold.out | sed 's/./x&/g' | tr "x" "\n" | awk '{if($1=="("){print $1,n; if(last!="("){n++}} if($1==")"){if(last!=")"){n--} print $1,n} if($1=="."){print $1} last=$1}' | more
