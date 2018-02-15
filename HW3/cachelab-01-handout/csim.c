/*
 * Zach Saegesser
 * I pledge my honor I have abided by the Stevens honor system
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <getopt.h>
#include "cachelab.h"

#define OPTS_HELP \
" Options:\n"\
"  -h          Optional help flag that prints usage info\n"\
"  -v          Optional verbose flag that displays trace info\n"\
"  -s <s>      Number of set index bits\n"\
"  -E <E>      Associativity\n"\
"  -b <b>      Number of block bits\n"\
"  -t <tracefile>   Name of the valgrind trace to replay\n"

int g_verbose = 0;

unsigned g_lils = 0;
unsigned g_E = 0;
unsigned g_lilb = 0;
typedef struct{
    unsigned valid;
    unsigned long tag;
    unsigned oldest;
}line;

typedef struct{
    unsigned long t;
    unsigned long s;
    unsigned long b;
}address;

static void usage(const char *bin_name){
	printf("Usage: %s  [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n%s", bin_name, OPTS_HELP);
}

/*
 * Goes through all the input arguments and sets variables (global and references)
 */
int parse_arguments(int argc, char** argv, char* path){
//  for(int i=0; i < argc; i++){
//    printf("%c , ", *argv[i]);
//  }
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));
//    printf("%c \n", getopt(argc, argv, "h:s:v:E:b:t:"));

    opterr = 0;
    int opt=0;
    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1){
    //  printf("OPT = %c\n", opt);
        switch (opt) {
            case 'h':
                printf("%s", OPTS_HELP);
                exit(0);
            case 's':
                g_lils = atoi(optarg);
              //  printf("lils = %d\n", g_lils);
                break;
            case 'v':
              //  printf("IN V = %c\n", opt);
                g_verbose = 1;
                break;
            case 'E':
                g_E = atoi(optarg);
                break;
            case 'b':
                g_lilb = atoi(optarg);
                break;
            case 't':
                strcpy(path, optarg);
                break;
            default:
                usage(argv[0]);
                exit(0);
                break;
        }
    }
    return 0;
}

/*
 * Uses the structs to keep reply correctly to an address request
 * 1-HIT
 * 2-MISS
 * 3-MISS EVICTION
 */
unsigned simulator(line **cache, address tempaddr, unsigned S,unsigned E){
    unsigned reply = 0;
    unsigned long i = tempaddr.s;
    int hit = 0;
    int emptyLine = -1;
    int maxoldest = 0;
    int temp2 = -1;
    line *currentSetInCache = cache[i];
    for (int j=0; j<E; j++) {
        line currentLineInCache = currentSetInCache[j];
        if (currentLineInCache.valid == 0) {
            emptyLine = j;
            continue;
        }
        //HIT
        if (currentLineInCache.valid == 1 && currentLineInCache.tag == tempaddr.t) {
            hit = 1;
            reply = 1;
            cache[i][j].oldest = 0;
            break;
        }

        //New Oldest
        if (currentLineInCache.valid == 1 && currentLineInCache.tag != tempaddr.t) {
            cache[i][j].oldest += 1;

        }
    }

    //MISS
    if (emptyLine != -1 && hit == 0) {
        line addLine = {1,tempaddr.t,0};
        cache[i][emptyLine] = addLine;
        reply = 2;
    }

    //MISS EVICTION
    if (emptyLine == -1 && hit == 0) {
        for (int k=0; k<E; k++) {
            if (cache[i][k].oldest>=maxoldest &&cache[i][k].valid != 0) {
                maxoldest= cache[i][k].oldest;
                temp2 = k;
            }
        }
        line tempLine = {1,tempaddr.t,0};
        cache[i][temp2] = tempLine;
        reply = 3;
    }

    return reply;
}

/*
 * Takes an address and turns it into the address struct by
 * bit masking and shifting
 */
address addressToStruct(long a1){
    address answer = {0,0,0};
    answer.b = a1& (0x7fffffffffffffff>>(63-g_lilb));
    a1 = a1>>g_lilb;
    answer.s = a1& (0x7fffffffffffffff>>(63-g_lils));
    a1 = a1>>g_lils;
    answer.t = a1;
    return answer;
}

int main(int argc, char ** argv)
{
  char path[20] = "";

  parse_arguments(argc, argv, path);
  //  printf("%d", g_lils);
    unsigned S = 1<<g_lils;
    unsigned hits = 0;
    unsigned misses = 0;
    unsigned evictions = 0;

    line **cache;

    //Ask for cache space.
    //Cache is pointing to S pointers.
    //Each of them (line pointer) is pointing to E lines.
    cache = (line**) malloc(S*sizeof(line *));
    for (int i=0;i<S;i++){
        cache[i] = (line*)malloc(g_E*sizeof(line));
    }

    //Initialization.
    for (int i=0; i<S; i++) {
        for (int j=0; j<g_E; j++) {
            cache[i][j].oldest = 0;
            cache[i][j].tag = 0;
            cache[i][j].valid = 0;
        }
    }

    char ID;
    unsigned long addr;
    int size;
    FILE *pFile;
    pFile = fopen(path, "r");

    //Read address from file. Access the cache by this address.
    while (fscanf(pFile, " %c %lx,%d",&ID, &addr, &size)!=EOF) {
      //if verbose is set, print the line you just scanned
        if(g_verbose){
          char temp = (char) ID;
          int temp2 = (int) addr;
          printf("%c %x %d ", temp, temp2, size);
        }
        if (ID == 'I') {
            continue;
        }
        //if M is set then you are guarenteed to hit on the second try
        if (ID == 'M') {
            hits += 1;
        }
        address tempaddr = addressToStruct(addr);
        unsigned simResponse = simulator(cache, tempaddr, S, g_E);
        switch (simResponse) {
            case 1:
                if(g_verbose){
                    printf("hit\n");
                }

                hits += 1;
                break;
            case 2:
              if (ID == 'M') {
                if(g_verbose){
                  printf("miss hit\n");
                }
              }
              else{
                if(g_verbose){
                  printf("miss\n");
                }
              }
                misses += 1;
                break;
            case 3:
                if (ID == 'M') {
                  if(g_verbose){
                    printf("miss eviction hit\n");
                  }
                }
                else{
                  if(g_verbose){
                    printf("miss eviction\n");
                  }
                }
                evictions += 1;
                misses +=1;
                break;
            default:
                printf("I hate C so much");
                break;
        }
    }
    fclose(pFile);
    for (int i=0; i<S; i++) {
        free(cache[i]);
    }
    free(cache);
    printSummary(hits, misses, evictions);
    return 0;

}
