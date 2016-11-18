
/*
cc -O test.c -o test util/wt/util.o util/wt/geom.o util/wt/sort.o -lm

*/
#include "util/wt/incl/util.h"

main(argc,argv) int argc; char *argv[]; {
int	i,j,n;
unsigned int id, top = 4294967295;
	sscanf(argv[1],"%d", &n);
	Pi(n) NL
	id = pack(n,n-1,n);
	printf("id = %u\n",id);
	printf("%u\n",top);
	unpack(&i,&j,n,id);
	Pi(i) Pi(j) NL
}
