/*
cc all2all.c -o all2all util/wt/util.o -lm

*/
#include "util/wt/incl/util.h"
#define N 99999
#define L 200

main() {
int  i,j,k, n = 0;
char line[L];
char *seqs[N];
FILE	*txt;
	txt = fopen("shuffle.txt", "r");
	while (1) { int io = read_line(txt,line);
		if (io<1) break;
		seqs[n] = (char*)alloca(sizeof(char)*L);
		for (i=0; i<io; i++) seqs[n][i] = line[i];
		n++;
	}
	Pi(n) NL
	for (j=0; j<n-1; j++) { int hit = 0;
		for (i=j+1; i<n; i++) { int l = min(strlen(seqs[i]),strlen(seqs[j]));
			for (k=0; k<l; k++) {
				if (seqs[i][k]==seqs[j][k]) hit++;
			}
		}
	}
}
