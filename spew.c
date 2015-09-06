#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

static char *alphanum="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static char *HELP_MSG="\
Spew generates massive sequential lists of passwords, for use as\
\ninput for cracking software. Best suited for single brute-force\
\nattacks. It is your responsibility to use in a safe and legal\
\nmanner.\
\n\nOptions:\
\n\n-c --charset <charset>\
\n\tCharset to use for cracking\
\n\n-n --numeric\
\n\tUse numeric characters only\
\n\n-a --alphanum\
\n\tUse alphanumeric characters only (default)\
\n\n-r --resume [from]\
\n\tStart from an arbitrary start point, given as a number.\
\n\tFor example, if interrupted after 1 million guesses, you\
\n\tcould type --resume 1000000\
\n\n-b --blocksize [number]\
\n\tSpew allows ghetto parallelization, meaning you can set\
\n\tup a cracking cluster limited or no bandwidth between\
\n\tmachines, by dividing the task up beforehand. Spew splits\
\n\tthe password space into \"blocks\", and only computes its\
\n\tshare of passwords (\"lines\") in each block. This option\
\n\tsets the total number of lines per block, across all\
\n\tmachines involved.\
\n\n-l --my-lines [number]\
\n\tHow many lines to compute per block.\
\n\n-o --offset --offset-lines [number]\
\n\tWhere in the block to start computing. Use this to give\
\n\tinstance/machine a different part of the block to work on.\
\n\tBe careful when setting your offsets; incorrect values\
\n\twill result in duplicated effort and/or missed guesses!\
\n\nExamples:\
\n\nSimple cracking on a single machine, for a numeric password\
\n\tspew --numeric\
\n\nDistributed cracking of an alphanumeric password, between 3\
\nmachines, each performing an equal share of the work.\
\n\thost1%% spew --blocksize 3 --my-lines 1\
\n\thost2%% spew --blocksize 3 --my-lines 1 --offset 1\
\n\thost3%% spew --blocksize 3 --my-lines 1 --offset 2\
\n\nDistributed cracking of an alphanumeric password, between 3\
\nmachines, where machine 2 is 4 times as fast as the others. In such\
\na scenario, make use of your cracking software's benchmarking\
\nfeature, if available, to determine this.\
\n\thost1%% spew --blocksize 7 --my-lines 1\
\n\thost2%% spew --blocksize 7 --my-lines 4 --offset 1\
\n\thost3%% spew --blocksize 7 --my-lines 1 --offset 6\
\n\n";
uint_fast32_t charsl=0;
int debug=0;
char *chars="";
char *passn;
size_t passl=1;
void addDigit(size_t n)
{
	passl+=n;
	if(realloc(passn, passl)){}
	for(size_t i=passl-1; i>0; i--) {
		passn[i]=passn[i-n];
	}
	for(size_t i=0; i<n; i++) {
		passn[i]=0;
	}
}
void add(uint_fast32_t n)
{
	size_t d=passl-1;
	while(n) {
		uint_fast32_t carry=(passn[d]+n)/charsl;
		passn[d]=(passn[d]+n)%charsl;
		n=carry;
		if(n && (d==0)) {
			addDigit(1);
			n--;
			d++;
		}
		d--;
	}
}
void printpass()
{
	if(debug) {
		for(size_t i=0; i<passl; i++) {
			printf("%2u ", passn[i]);
		}
		printf(" = ");
	}
	for(size_t i=0; i<passl; i++) {
		printf("%c", chars[passn[i]]);
	}
	puts("");
}
void initpass(uint_fast32_t l)
{
	passn=calloc(l,sizeof(char));
	passl=l;
}
void spew(uint_fast64_t n, uint_fast32_t l, uint_fast32_t slots, uint_fast32_t assigned, uint_fast32_t offset)
{
	if(slots && (n-offset) % slots >= assigned) {
		n=(n/slots+1)*slots-offset;
	}
	add(n);
	uint_fast32_t skip=slots-assigned;
	while(1) {
		if((n-offset) % slots < assigned) {
			if(debug) {
				printf("%u - ", n);
			}
			printpass();
			n++;
			add(1);
		}
		else {
			if(debug) {
				printf("%u - skipping %u\n", (unsigned int)n, (unsigned int)skip);
			}
			add(skip);
			n+=skip;
		}
	}
}
void setCharset(char* c)
{
	charsl=strlen(c);
	chars=calloc(charsl+1, sizeof(char));
	strncpy(chars, c, charsl+1);
	if(debug) {
		fprintf(stderr, "Charset set to %u chars: \"%s\"\n", charsl, chars);
	}
}
void main(int argc, char **argv) {
	bool numeric=0;
	uint_fast64_t start=0;
	uint_fast32_t slots=1;
	uint_fast32_t assigned=1;
	uint_fast32_t offset=0;
	uint_fast32_t length=1;
	numeric=0;
	setCharset(alphanum);
	while (1) {
		static struct option long_options[] = {
			{"charset",      required_argument, 0, 99  },
			{"numeric",      optional_argument, 0, 110 },
			{"alphanum",     no_argument,       0, 97  },
			{"resume",       required_argument, 0, 114 },
			{"blocksize",    required_argument, 0, 116 },
			{"my-lines",     required_argument, 0, 115 },
			{"offset-lines", required_argument, 0, 111 },
			{"length",       required_argument, 0, 108 },
			{"debug",        optional_argument, 0, 100 },
			{0,              0,                 0, 0   }
		};
		char c = getopt_long(argc, argv, "c:n::ar:t:s:o:d::l:", long_options, 0);
		if (c == -1) {
		   break;
		}
		switch (c) {
			case 'c':
				setCharset(optarg);
				numeric=0;
				break;
			case 'n':
				setCharset("0123456789");
				numeric=1;
				break;
			case 'a':
				setCharset(alphanum);
				numeric=0;
				break;
			case 'r':
				start=strtoumax(optarg,NULL,10);
				break;
			case 'l':
				length=strtoul(optarg,NULL,10);
				break;
			case 't':
				slots=strtoul(optarg,NULL,10);
				break;
			case 's':
				assigned=strtoul(optarg,NULL,10);
				break;
			case 'o':
				offset=strtoul(optarg,NULL,10);
				break;
			case 'd':
				debug=1;
				break;
			default:
				fprintf(stderr, HELP_MSG);
				exit(1);
		}
	}
	initpass(length);
	spew(start, length, slots, assigned, offset);
}
