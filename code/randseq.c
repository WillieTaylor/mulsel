/*
cc -O randseq.c -o randseq util/wt/util.o util/wt/geom.o util/wt/sort.o -lm

*/
#include "util/wt/incl/util.h"

main(argc,argv) int argc; char *argv[]; {
int	i,j,k, len = 100, nseq = 10000, tail = len/10;
char	seq[9999], set, base = argv[1][0];
float	gap = 0.1, mutate = 0.01;
long    rseed = time(0)+(int)(base-'A');
        srand48(rseed);
	sscanf(argv[2],"%d", &nseq);
	for (j=0; j<len; j++) {
		set = base;
		if (drand48() < gap) set = '-';
		if (j > len-tail) set = '-';
		if (j < tail) set = '-';
		seq[j] = set;
	}
	for (i=1; i<nseq; i++) { int n = 0;
		for (j=0; j<len; j++) {
			if (seq[j] == base) n++;
		}
		if (n > len/3) {
			printf(">seq%c%d\n", base,i);
			printf("%c family seq %d  %f %d %c\n", base,i,drand48(),n,base);
			printf("%s*\n", seq);
			set = 'A'+(int)(drand48()*26.0);
		} else {
			set = argv[1][0];
		}
		if (drand48() < gap) set = '-';
		for (j=0; j<len; j++) {
			if (drand48() > mutate*(float)i/(float)nseq) continue;
			seq[j] = set;
		}
	}
}
