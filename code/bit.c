
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
