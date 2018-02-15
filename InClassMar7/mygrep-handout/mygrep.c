#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>


/* Zach Saegesser */
/* I pledge my honor that I have abided by the Stevens honor System */

#define OPTS "ibnhcf:"
#define OPTS_HELP \
" Options:\n"\
"  -f <file>   Read data from file instead of standard input\n"\
"  -c          Only count how many lines matched.\n"\
"  -h          Print this help message\n"\
"  -n          Each output line is preceded by its relative line number in \n              the file, starting at line 1.\n"\
"  -b          The offset in bytes of a matched pattern is displayed in \n              front of the respective matched line.\n"\
"  -i          Perform case insensitive matching. By default, mygrep is \n              case sensitive. \n"

/* Globals for command line options */
static int g_count_only = 0; /* Count lines only */
static char *g_search_string = NULL; /* String i'm searching for */
/* Read input from this file, instead of stdin */
static char *g_infilename = NULL;
static int g_line_number = 0;
static int g_offset = 0;
static int g_case_insensitive = 0;


/* Print usage information */
static void usage(const char *bin_name)
{
	printf("Usage: %s [OPTIONS] <search string>\n%s", bin_name, OPTS_HELP);
}

/* Parses command-line arguments and returns 0 on success or -1 on error */
static int parse_arguments(int argc, char **argv)
{
	int c;

	while ((c = getopt(argc, argv, OPTS)) != -1) {
		switch (c) {
		case 'i':
			g_case_insensitive = 1;
			break;
		case 'b':
			g_offset = 1;
			break;
		case 'n':
			g_line_number = 1;
			break;
		case 'c':
			g_count_only = 1;
			break;
		case 'f':
			g_infilename = optarg;
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
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

	g_search_string = argv[optind];
	return 0;
}

/* Return 0 if line matches or -1 otherwise */
// static int line_matches(const char *line, const char *sstr)
// {
// 	return -1;
// }

static int compare(char x, char y){
	if(tolower(x) == tolower(y)){
		return 1;
	}
	else{
		return 0;
	}
}
/* Search for string sstr in input read from in */
static int search_string(FILE *in, const char *sstr)
{
	size_t llen = 0;
	size_t flen = 0;
	char *line = NULL;
	char *current;
	const char *original =sstr;
	int count = 0;
	int line_counter = -1;
	int offset_loop = 0;


	while((flen = getline(&line, &llen, in)) != -1){
		line_counter++;
		offset_loop = 0;
	//	printf("%i\n", llen);
	//	printf("%i\n", flen);
		for(current = line; *current; current++){
		//	printf("%c", *current);

			if(!*sstr && !g_count_only && !g_line_number && !g_offset){
				printf("%s", line );
			}
			else if(!*sstr && g_count_only){
				count++;
			}
			else if(!*sstr && g_line_number && !g_offset){
				printf("%i", line_counter);
				printf("%s", " " );
				printf("%s", line );
			}
			else if(!*sstr && g_offset && !g_line_number){
				printf("%i", offset_loop);
				printf("%s", " " );
				printf("%s", line );
				offset_loop = 0;
			}
			else if(!*sstr && g_offset && g_line_number){
				printf("%i", offset_loop);
				printf("%s", " " );
				printf("%i", line_counter);
				printf("%s", " " );
				printf("%s", line );
				offset_loop = 0;
			}
			if(!g_case_insensitive){
				if(*current == *sstr){
					sstr++;
				} else {
					sstr = original;
					offset_loop++;
				}
			}
			else {
				if(compare(*sstr, *current)){
					sstr++;
				} else {
					sstr = original;
					offset_loop++;
				}
			}

		}

	}
	/*fgets(line,sizeof(line),in)!= NULL*/
	/*char line [1000]; */
	// size_t llen = 0;
	// int r = 0;
	// char *line = NULL;
	// unsigned long sstr_matches = 0;
	// while(r = getline(&line, &llen, in) > 0) /* read a line from a file */ {
	// 	int temp1 = 0;
	// 	int temp2 = 0;
	// 		while(temp1 != sizeof(line) && temp2 != sizeof(sstr)){
	// 			if(line[temp1] == sstr[temp2]){
	// 				temp1++;
	// 				temp2++;
	// 				if(temp2 == sizeof(sstr)){
	// 					sstr_matches++;
	// 				}
	// 			}
	// 			else{
	// 				temp1++;
	// 				temp2 =0;
	// 			}
	// 		}
  //  }
	free(line);
//	free(original);

	if (g_count_only) {
		printf("%i\n", count);
	}
	return 0;
}

int main(int argc, char **argv)
{
	FILE *fin = stdin;
	int rval  = 0;

	if (parse_arguments(argc, argv) != 0)
		return -1;

	if (g_infilename) {
		if ((fin = fopen(g_infilename, "r")) == NULL) {
			perror("cannot open input file");
			return -1;
		}
	}

	rval = search_string(fin, g_search_string);

	fclose(fin);
	return rval;
}
