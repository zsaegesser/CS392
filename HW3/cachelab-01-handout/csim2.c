#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

#include "cachelab.h"

#define OPTS "hvsEbt:"
#define OPTS_HELP \
" Options:\n"\
"  -h          Optional help flag that prints usage info\n"\
"  -v          Optional verbose flag that displays trace info\n"\
"  -s <s>      Number of set index bits\n"\
"  -E <E>      Associativity\n"\
"  -b <b>      Number of block bits\n"\
"  -t <tracefile>   Name of the valgrind trace to replay\n"

//static int g_help = 0;
static int g_verbose = 0;
static int g_set = 0;
static int g_ass = 0;
static int g_block = 0;
static char *g_infilename = NULL;

static void usage(const char *bin_name){
	printf("Usage: %s  [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n%s", bin_name, OPTS_HELP);
}

/* Parses command-line arguments and returns 0 on success or -1 on error */
static int parse_arguments(int argc, char **argv)
{
	int c;

	while ((c = getopt(argc, argv, OPTS)) != -1) {
		switch (c) {
		case 'v':
			g_verbose = 1;
			break;
		case 's':
			g_set = atoi(argv[optind]);
			break;
    case 'E':
      g_ass = atoi(argv[optind]);
      break;
    case 'b':
      g_block = atoi(argv[optind]);
      break;
    case 't':
  	 	g_infilename = optarg;
  	 	break;
    case 'h':
      usage(argv[0]);
      exit(0);
      break;
		case '?':
		default:
			usage(argv[0]);
			return -1;
		}
	}
  if (optind >= argc) {
    usage(argv[0]);
    return -1;
  }

   return 0;
 }

static int simulator(int lilS, int E, int lilB, FILE *in){

	//set up everything
//	int bigB = (1 << lilB);
	int bigS = (1 << lilS);
	//get the size of the cache simulator
//	int size_of_cache = bigS*(E*(bigB+8));
	//create cache simulator and set all bytes to 0
//	char *cache = (char *)calloc(size_of_cache, sizeof(char));
	//file scrolling variables
	size_t llen = 0;
	size_t flen = 0;
	char *line = NULL;
	char action = ' ';
	char address[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
	char grabsize = ' ';
	int t = 64-(lilS+lilB);
	int addressint = 0;
	int smask = 1;
	int tmask = 1;
	int bmask = 1;
	int miss = 0;
	int hit = 0;
	int evict = 0;
	int valids[bigS][E];
	int tags[bigS][E];


	for(int i = 0; i <bigS; i++){
		for(int j = 0; j<E; j++){
			valids[i][j] = 0;
			tags[i][j] = 0;
		}
	}


	//char *current;
	//printf("mem in this cache = %d \n", (int) *(cache+(sizeof(char)*98)));
	//begining of the simulation
//	for(int i = 0; i < 96; i++){
//		*(cache+(sizeof(char)*i)) = 10;
//	}
//	for(int i = 0; i < 97; i++){
//		printf("mem in this cache = %d \n", (int) *(cache+(sizeof(char)*i)));
//	}
	while((flen = getline(&line, &llen, in)) != -1){
		int count = 0;
		addressint = 0;
//		address = {'0', '0', '0', '0', '0', '0', '0', '0'};
		for(int i = 0; i <8; i++){
			address[i] = '0';
		}
		if(*(line+1) == 76){
			action = 'L';
		}
		else if(*(line+1) == 77){
			action = 'M';
		}
		else if(*(line+1) == 83){
			action = 'S';
		}
		else {
			action = 'I';
		}
		while(*(line+3+count) != 44){
			count++;
		}

	//	printf("COUNT = %d \n", count);
		//count to end of address then move back from there and store into char address
		for(int i = 0; i<count;  i++){
			address[8-count+i] = *(line+3+i);
		}
		//get the size to 'return'
		grabsize = *(line+1+count);
		for(int i =0; i<8;i++){
			addressint += (address[i] - '0') * pow(16,(7-i));
		}

		for(int i = 0; i < lilB; i++){
			bmask = (bmask << 1) + 1;
		}
		for(int i = 0; i < lilS; i ++){
			smask = (smask << 1) + 1;
		}
		smask = smask << lilB;
		for(int i = 0; i < t; i++){
			tmask = (tmask << 1) + 1;
		}
		tmask = tmask << (lilS+lilB);
		int emptyspot = -1;

	//	printf("ACTION: %d \n", action);
		if(action != 77){
			if(((addressint & (smask)) >> lilB) < bigS){
				for(int i = 0; i < bigS; i++){
					for(int j = 0; j < E; j++){
						if(((addressint & (smask)) >> lilB) == i && valids[i][j] == 1 && tags[i][j] == ((addressint & (tmask)) >> (lilS + lilB))){
							printf("HIT Found address: %d \n", addressint);
							hit++;
							goto LEAVE;
						}
					}
				}
				for(int i = E-1; i >= 0; i--){
					if(valids[((addressint & (smask)) >> lilB)][i] == 0){
						emptyspot = i;
					}
				}
				if(emptyspot != -1){
					printf("MISS Did not find address: %d \n Added Address %d to Set: %d Row: %d \n", addressint, addressint, ((addressint & (smask)) >> lilB), emptyspot);
					miss++;
					valids[((addressint & (smask)) >> lilB)][emptyspot] = 1;
					tags[((addressint & (smask)) >> lilB)][emptyspot] = ((addressint & (tmask)) >> (lilS + lilB));
					goto LEAVE;
				}
				else{
					printf("MISS Did not find address: %d \n Added Address %d to Set: %d Row: %d \n EVICTION \n", addressint, addressint, ((addressint & (smask)) >> lilB), 0);
					miss++;
					evict++;
					valids[((addressint & (smask)) >> lilB)][0] = 1;
					tags[((addressint & (smask)) >> lilB)][0] = ((addressint & (tmask)) >> (lilS + lilB));
					goto LEAVE;
				}

			}
		}
		else{
			AGAIN:
	//		printf("M CASE: SET = %d \n", ((addressint & (smask)) >> lilB));
			if(((addressint & (smask)) >> lilB) < bigS){
				for(int i = 0; i < bigS; i++){
					for(int j = 0; j < E; j++){
						if(((addressint & (smask)) >> lilB) == i && valids[i][j] == 1 && tags[i][j] == ((addressint & (tmask)) >> (lilS + lilB))){
							printf("HIT Found address: %d \n", addressint);
							hit++;
							goto LEAVE;
						}
					}
				}
				for(int i = E-1; i >= 0; i--){
					if(valids[((addressint & (smask)) >> lilB)][i] == 0){
						emptyspot = i;
					}
				}
				if(emptyspot != -1){
					printf("MISS Did not find address: %d \n Added Address %d to Set: %d Row: %d \n", addressint, addressint, ((addressint & (smask)) >> lilB), emptyspot);
					miss++;
					valids[((addressint & (smask)) >> lilB)][emptyspot] = 1;
					tags[((addressint & (smask)) >> lilB)][emptyspot] = ((addressint & (tmask)) >> (lilS + lilB));
					goto AGAIN;
				}
				else{
					printf("MISS Did not find address: %d \n Added Address %d to Set: %d Row: %d \n", addressint, addressint, ((addressint & (smask)) >> lilB), 0);
					miss++;
					evict++;
					valids[((addressint & (smask)) >> lilB)][0] = 1;
					tags[((addressint & (smask)) >> lilB)][0] = ((addressint & (tmask)) >> (lilS + lilB));
					goto AGAIN;
				}
			}
		}

		LEAVE:;



		// for(current = line; *current; current++){
		// 	printf("%s\n", current);
		// }
	}

// for(int i = size_of_cache; i >0; i--){
// 	free(cache+(sizeof(char)*i));
// }
	printSummary(hit,miss, evict);
	printf("IGNORE THIS: %d %d %d\n", action, t, grabsize);
	printf("IGRNORE THIS TOO: %s ", address);
	return 0;
}




int main(int argc, char **argv)
{
  FILE *fin = stdin;

  if(parse_arguments(argc, argv) != 0);{
//  printf("s= %d \n", g_set);
//  printf("E= %d \n", g_ass);
//  printf("b= %d \n", g_block);
	//printf("here is bad \n");
  //return -1;
  }
  if (g_infilename) {
		if ((fin = fopen(g_infilename, "r")) == NULL) {
			perror("cannot open input file");
			return -1;
		}
	}
  if(simulator(g_set, g_ass, g_block, fin)){
		perror("It seems something went wrong... try again later");
	}
	//printSummary(0,0,0);
  return 0;
}
