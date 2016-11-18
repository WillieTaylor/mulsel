#include "util/wt/incl/util.h"
#include "multal.h"

struct	slot	{
		int	id;	// for peptide
		struct slot	*more, *less;
		}
	;
typedef struct	slot	Slot;

struct	best	{ // for each sequence hold the hits in a binary tree
		int	score, m, l;
		struct best	*more, *less;
		}
	;
typedef struct	best	Best;

typedef struct	{
		unsigned int	id;	// sequence id
		int	count;	// N peptides (list length)
		int	*slot;	// list of (ordered) peptide ids
		}
	Pept;
Pept	*pept_list;
int	n_pept = 0, n_prot = 0;

struct node	{	// leaf node in the pair tree
		unsigned int	id;
		int	count;
		struct	node	*more, *less;
		Slot	*slot;
		}
	;
typedef struct	node	Node;
Node	*key;
int	n_pair = 0;

struct pair	{
		unsigned int	id;
		char	count;
		struct	pair	*more, *less;
		}
	;
typedef struct	pair	Pair;
Pair	*gate;
int	ngates,
	free_pairs = 0;

typedef struct	{
		unsigned int	id;	// code for protein pair
		unsigned char	count; // score for pair
		}
	Prot;
//Prot	*pair_list; now defined in mulsel.c and passed through count()
int	in_pair;

int	base[PEPLEN],
	n_alloc, p_alloc = 1000,
	jump, joff;

int	soft_level = 0, diag_up, min_pept, max_hold;

int	*nth;

int	get_nth();
Slot	*get_slot();
Best	*get_best();
Node	*get_node();
Pair	*get_pair();
Prot	*pair_out();
Prot	*count();
char	*decode();
char	soften();

int	pair_limit = 16000000;

init_slots (nprots,pep_len,soft)
int	nprots, pep_len, soft;
{	int	peplen = abs(pep_len),
		i, j, power = 1;
	printf("\nCalculating peptide table based on length %d\n", peplen);
	if (soft) {
		printf("peptide softened to level %d\n", soft);
		soft_level = soft;
		if (pep_len<0) {
			printf("(default softening ignored)\n");
			pep_len = -pep_len;
		}
	}
	if (pep_len<0) {
		soft_level = peplen-4;
		printf("peptide softened by default to level %d\n", soft_level);
	}
	for (i=peplen; i>0; i--) {
		base[i-1] = power;
		power = power*NACIDS;
	}
	nth = (int*)malloc(sizeof(int)*(nprots+1));	TEST(nth)
	key = (Node*)malloc(sizeof(Node)*(nprots+1));	TEST(key)
	for (i=1; i<=nprots; i++) {
	{	Node	*k = key+i;
		nth[i] = min_pept;
		k->id = i;
		k->slot = 0;
		k->count = 0;
		k->more = k->less = 0;
		}
	}
}

char	soften(res,soft)
char	res;
int	soft;
{
	res = UPPER(res);
	if (res=='B') return 'D';
	if (res=='Z') return 'E';
	if (soft==0) return res;
	if (res=='S') return 'T';
	if (res=='L') return 'I';
	if (soft==1) return res;
	if (res=='D') return 'E';
	if (res=='N') return 'Q';
	if (soft==2) return res;
	if (res=='V') return 'I';
	if (res=='R') return 'K';
	if (soft==3) return res;
	if (res=='Y') return 'F';
	if (res=='A') return 'G';
	return res;
}

find_slot (open,pep_len,pep_in,prot)
// used by copy_seq() in load_seq() to load-up the peptide tree for each prot
char	*pep_in;
int	open, pep_len, prot;
{	int	peplen = abs(pep_len), 
		coff = 'A'-1,
		hole, id = 0, i;
	Node	*p = key+prot;
	char	peptide[PEPLEN];
	for (i=0; i<peplen; i++) {
		peptide[i] = soften(pep_in[i],soft_level);
		id += (peptide[i]-coff)*base[i];
	}
	peptide[peplen] = '\0';
Ps(peptide) Pi(id) NL
	if (p->slot) get_slot(p,p->slot,id);
	else	p->slot = get_slot(p,0,id);
}

Slot *get_slot (node, slot, new_id)
// makes a leaf node in the peptide tree for hash code <new_id>
Node	*node;
Slot	*slot;
int	new_id;
{
	Slot 	*n = slot;
	if (slot) 
	{	int	slot_id = slot->id;
		if (new_id == slot_id) { // found the peptide slot
			return n;	// return the slot pointer
		}
		if (new_id < slot_id) {
			if (slot->less) get_slot(node,slot->less,new_id);
			else	slot->less = get_slot(node,0,new_id);
		} else {
			if (slot->more) get_slot(node,slot->more,new_id);
			else	slot->more = get_slot(node,0,new_id);
		}
	} else {	// create a new slot
		n = (Slot*)malloc(sizeof(Slot));
		TEST(n)
		n->id = new_id;
		n->less = n->more = 0;
		(node->count)++;
	}
	return n;
}

int	get_nth (slot,n)
// for protein with id <slot> (best[pid]), find the Nth (max_hold) hightst score
Best	*slot;
int	*n;
{ int	s = min_pept;
	if (!slot) return;
	if (slot->more) s = get_nth(slot->more,n);
	(*n)++;
	if (*n==max_hold) return slot->score;
	if (slot->less) s = get_nth(slot->less,n);
	return s;
}

free_best (slot)
Best	*slot;
{
	if (!slot) return;
	if (slot->less) free_best(slot->less);
	if (slot->more) free_best(slot->more);
	free (slot);
}

in_slot (id, jd)
// count matching peptides between sorted lists
int	id;
{
	int	i, j, n;
	Pept	*pid = pept_list+id,
		*pjd = pept_list+jd;
	i = j = n = 0;
	while (i < pid->count && j < pjd->count) { // move down the lists (length=count)
		if (pid->slot[i]  < pjd->slot[j]) i++;
		if (pid->slot[i]  > pjd->slot[j]) j++;
		if (pid->slot[i] == pjd->slot[j]) { i++; j++;
			n++;
Pi(pid->slot[i]) NL
		}
	}
	return n;
}

Best *get_best (slot, n, more, kept)
// load the score <n> into a tree for the protein <slot> (= best[pid])
Best	*slot;
int	n, more, *kept;
{
	Best	*s = slot;
	if (slot) { // if not first find the position in the tree
		if (n <= slot->score) {		// less than current
			more += slot->m + 1;
			if (slot->less) {	// look lower
				slot->less = get_best(slot->less,n,more,kept);
				slot->l = (slot->less)->l + (slot->less)->m + 1;
			} else {	// otherwise set n
				slot->less = get_best(0,n,more,kept);
				if (slot->less) slot->l = 1;
			}
		} else {			// more than current
			if (slot->more) {	// look higher
				slot->more = get_best(slot->more,n,more,kept);
				slot->m = (slot->more)->m + (slot->more)->l + 1;
			} else {	// otherwise set n
				slot->more = get_best(0,n,more,kept);
				if (slot->more) slot->m = 1;
			}
			if (slot->more && more+slot->m >= max_hold) {
				s = slot->more;
				if (slot->less) free_best(slot->less);
				free (slot);
			}
		}
	} else { // make new slot
		if (more >= max_hold) return 0;
		s = (Best*)malloc(sizeof(Best)); TEST(s)
		s->score = n;
		s->less = s->more = 0;
		s->l = s->m = 0;
		*kept = 1;
	}
	return s;
}

Prot* count (pass,nprots,aspan,bspan,minp,hold,diag,pair_list,inpairs)
int	pass, nprots, aspan, bspan, minp, hold, diag;
Prot	*pair_list;
int	*inpairs;
{
	int	n, i, j, k, left, deep, topi, topj, n_pairs,
		npairs = (nprots*nprots-nprots)/2, pept_max;
	Best	**best;
	best = (Best**)malloc(sizeof(Best*)*(nprots+1));
	for (i=1; i<=nprots; i++) best[i] = 0;
	min_pept = minp;
	max_hold = hold;
	if (diag < 0) diag_up = min_pept; else diag_up = diag;
	ngates = nprots;		// set up N gates into pair data (on first pass)
	if (!pass || free_pairs) gate = (Pair*)malloc(sizeof(Pair)*(ngates+1));
	jump = npairs/ngates;		// at equal spacing across the pair id range
	left = npairs-jump*ngates;	// with equal margins at ends
	j = joff = (jump+left+1)/2;
	for (i=0; i<ngates; i++) {
	{	Pair	*g = gate+i;
			g->id = j;
			g->count = 0;
			g->more = g->less = 0;
			j += jump;
		}
	}
	printf("\n\n ****** CYCLE %d ******\n", pass);
	printf("Minimum score held = %d\n", min_pept);
	printf("Top %d scores held per sequence\n", max_hold);
	printf("Sequences in the range %d<Dij<%d scored\n",aspan,bspan+1);
	printf("Adjacent sequences scored with bonus of %d\n", diag_up);
	n_pairs = 0;		// counter for pairs found on this pass
	in_pair = *(inpairs);	// counter for growing pairs in pep_list
	// load adjacent pairs (-ve score = flag for adjacent in print_pair())
	aspan++;
	for (i=aspan; i<bspan; i++)
	{ int	pid = i, pjd = i+1;
	  unsigned int	id = pack(pid,pjd,nprots),
		way = min(max(0,(id-joff+1)/jump),nprots-1);
		n = in_slot(pid,pjd);
		get_pair(gate+way,id,-n);
		n_pairs++;
	}
	// load all pairs (in range) but not adjacent entries
	for (i=aspan; i<bspan-1; i++)
	{ int	pid = i;
		for (j=i+2; j<=bspan; j++) 
		{ int	pjd = j, pdif = abs(i-j);
		  unsigned int	id = pack(pid,pjd,nprots),
			way = min(max(0,(id-joff+1)/jump),nprots-1);
			n = in_slot(pid,pjd);	// count common peptides for i and j
			if (in_pair+n_pairs == pair_limit) break;
			if (n<min_pept) continue;
			topi = topj = 0;	// flags to tell if value kept
			if (n>nth[pid]) { // if n is better than scores held, keep it
				if (!best[pid]) // save the max n for pid
					best[pid] = get_best(        0,n,0,&topi);
				   else best[pid] = get_best(best[pid],n,0,&topi);
				deep = 0; // if kept, update the new Nth value
				if (topi) nth[pid] = get_nth(best[pid],&deep);
			}
			if (n>nth[pjd]) { // if n is better than scores held, keep it
				if (!best[pjd]) // save the max n for pid
					best[pjd] = get_best(        0,n,0,&topj);
				   else best[pjd] = get_best(best[pjd],n,0,&topj);
				deep = 0; // if kept, update the new Nth value
				if (topj) nth[pjd] = get_nth(best[pjd],&deep);
			}
			if (topi || topj) { // 
				get_pair(gate+way,id,n);
				n_pairs++;
			}
			if (in_pair+n_pairs == pair_limit) break;
 		}
		free_best(best[pid]);
		if (in_pair+n_pairs == pair_limit) break;
	}
	printf("%d pairs found this pass, %d previously collected\n", n_pairs, in_pair);
	pair_list = pair_out(pair_list,n_pairs,n_pair); // extract protein pairs and add to current list
	*(inpairs) = in_pair;
	return pair_list;
}

Pair *get_pair (pair, id, score)
// enters the pair score in the tree of pairs
Pair	*pair;
unsigned int	id;
int	score;
{
	if (pair) // look for the protein pair <id> (pid#pjd)
	{	int	pair_id = pair->id;
		if (id == pair_id) { // found
			if (pair->count+score < BYTE) // hold scores up to 255
				pair->count += score;
			else	pair->count  = BYTE;
			return;
		}	 
		if (id < pair_id) {
			if (pair->less) get_pair(pair->less,id,score);
			else	pair->less = get_pair(0,id,score);
		} else {
			if (pair->more) get_pair(pair->more,id,score); 	
			else	pair->more = get_pair(0,id,score);
		}
	} 
	else //	create a new pair entry
	{	Pair	*n;
		n = (Pair*)malloc(sizeof(Pair)); TEST(n)
		n->id = id;	// pid#pjd number
		n->less = n->more = 0;
		n->count = score;
		n_pair++;
		return n;
	}
}

pscore (pid, pjd, nprots)
int	pid, pjd;
{	unsigned int	id = pack(pid,pjd,nprots),
		way = (id-joff+1)/jump;
	int	count;
	way = min(max(0,way),nprots-1);
	count = find_pair(gate+way,id);
	return count;
 }

find_pair (pair, id)
Pair	*pair;
unsigned int	id;
{	int	count;
	unsigned int	pair_id = pair->id;
	if (id == pair_id) return (int)pair->count;
	count = 0;
	if (id < pair_id) {
		if (pair->less) count = find_pair(pair->less,id);
	} else {
		if (pair->more) count = find_pair(pair->more,id); 	
	}
	return count;
}

Prot* pair_out (pair_list,new_pairs)
// read off the pairs (through print_pair()) into pair_list[]
Prot	*pair_list;
int	new_pairs;
{
int	new_size;
int	i, j;
	if (in_pair == 0) { // start the collection
		new_size = new_pairs;
	} else {	// or expand the collection
		new_size = in_pair+new_pairs;
	}
	if (new_size >= pair_limit) {
		printf("%d too many sequence pairs\n", pair_limit); 
		new_size = pair_limit;
	}
	if (in_pair == 0) { // start the collection
		pair_list = (Prot*)malloc(sizeof(Prot)*new_size); TEST(pair_list)
	} else {	// or expand the collection
		pair_list = (Prot*)realloc(pair_list,sizeof(Prot)*new_size); TEST(pair_list)
	}
	for (i=0; i<ngates; i++) print_pair(gate+i,pair_list);
	if (free_pairs) free(gate);
	return pair_list;
}

print_pair (pair,pair_list)
// traverse the <pair> tree and copy sequence pairs to pair_list[]
Pair	*pair;
Prot	*pair_list;
{	
	if (pair->less) print_pair(pair->less,pair_list);
	if ((abs(pair->count) >= min_pept) && (in_pair < pair_limit)) {
		pair_list[in_pair].id = pair->id;
		if (pair->count < 0) { // bump-up adjacent pairs
			pair_list[in_pair].count = (unsigned char)(diag_up - pair->count);
			pair->count = -(pair->count);
		} else {
			pair_list[in_pair].count = pair->count;
		}
		in_pair++;
	}
	if (pair->more) print_pair(pair->more,pair_list);
	if (free_pairs) free(pair);
	return;
}

pept_out(nprots,npeps)
// extracts the sorted peptide list from a tree into pept_list[] for each prot
int	nprots, *npeps;
{
int	i, j;
	pept_list = (Pept*)malloc(sizeof(Pept)*(nprots+1));
	TEST(pept_list)
	for (i=1; i<=nprots; i++)	// for each protein <key> 
	{	Node	*node = key+i;
		Pept	*pep = pept_list+i;
		pep->id = node->id;	// the number of the protein (1...nprots)
		pep->count = node->count;	// the number of peptides seen (list length)
		npeps[i] = pep->count;	// pass up as a rough measure of sequence complexity
		pep->slot = (int*)malloc(pep->count*sizeof(int)); // holds the id
		TEST(pep->slot)
		n_prot = 0;
		if (node->slot) print_slot(node->slot,pep); // copy over the peptides
		else printf("*NB* no slot for prot = %d\n",i);
	}
	free(key);
}

print_slot (slot,pep)
// traverse the tree and store peptide id in pep->slot[]
Slot	*slot;
Pept	*pep;
{
if (slot->less) Pi(slot->less->id)
Pi(slot->id)
if (slot->more) Pi(slot->more->id) 
NL
	if (slot->less) print_slot(slot->less,pep);
	if (slot->id) {
		pep->slot[n_prot] = slot->id;
		n_prot++;
	}
	if (slot->more) print_slot(slot->more,pep);
	free(slot);
	return;
}

char	*decode (n,peplen)
int	n, peplen;
{
	char 	pep[PEPLEN];
	int	i, c, rem = n;
	for (i=0; i<peplen; i++) {
		c = rem/base[i];
		pep[i]='A'+c-1;
		rem -= c*base[i];
	}
	pep[peplen] = '\0';
	return pep;
}	

/* needed back here for multas at some point
reorder (pairs, pair_list, npairs)
// sorts pairs on pair_list[].count value
int	*pairs; // hashed pair IDs
Prot	*pair_list; 
int	npairs;
{
	int	i;
	int	*score;
	int	*place;
	printf("reorder ");
	score = (int*)malloc(npairs*sizeof(int));	TEST(score)
	place = (int*)malloc(npairs*sizeof(int));	TEST(place)
	printf(" malloc OK");
	for (i=0; i<npairs; i++) score[i] = pair_list[i].count;
	sort(0, score, 0, place, npairs, 1);
	free(score);
	for (i=0; i<npairs; i++) pairs[i] = pair_list[place[i]].id;
	free(pair_list);
	free(place);
	printf(" done\n");
	return;
}

may need long-long

unsigned int packs (i,j,n)	int	i, j, n;
//	references top half (+diag) of a 2D array (N*N) as a vector
{	int	ip, jp;
	if(n <= 0) { printf("Bad dimension in IPACK\n"); return -1; }
	if(i <= 0) { printf("Bad I in IPACK: i=%d\n",i); return -2; }
	if(j <= 0) { printf("Bad J in IPACK: j=%d\n",j); return -3; }
	ip = max(i,j);
	jp = min(i,j)-1;
	return (unsigned int)(ip-jp+n*jp-(jp*jp-jp)/2);
}

unpacks (ip,jp,n,id)	int *ip, *jp, n; unsigned int id;
//	references top half (+diag) of a 2D array (N*N) from a vector
{	int	i, j;
	double 	b = n+0.5,
		a = b*b-2*id;
	if ( n>92682) printf("Error in UNPACK: n=%d is too big\n",n); // unsigned in limit
	//if(n>65535) printf("Error in UNPACK: n=%d is too big\n",n);
	if(a<=0.0) printf("Error in UNPACK: n=%d, id=%d\n", n,id);
	j = (int)(b-sqrt(a)-0.00001);
	i = j+id-n*j+(j*j-j)/2;
	j++;
	if (j<=0 || j>n ) printf("Bad J in UNPACK: j=%d (n=%d id=%d)\n",j,n,id);
	if (i<=0 || i>n ) printf("Bad I in UNPACK: i=%d (n=%d id=%d)\n",i,n,id);
	*ip = min(i,j);
	*jp = max(i,j);
}
*/
