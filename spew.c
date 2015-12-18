#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <help.h>
#include <password.h>

static char *alphanum="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
void spew(uint_fast64_t n, uint_fast32_t l, uint_fast32_t slots, uint_fast32_t assigned, uint_fast32_t offset, char* prefix)
{
	if(slots && (n-offset) % slots >= assigned) { //if start value is not an "assigned line",
		n=(n/slots+1)*slots-offset;               //bump up to next "assigned line"
	}
	add(n);
	uint_fast32_t skip=slots-assigned;
	while(1) {
		if((n-offset) % slots < assigned) { //there's probably a way to make this cheaper. it's in a pretty speed-critical section.
			printpass();
			n++;
			add(1);
		}
		else {
			if(debug) { //how much are these debug checks costing? this one is only slightly less critical than the ones in password.c
				printf("%u - skipping %u\n", (unsigned int)n, (unsigned int)skip);
			}
			add(skip);
			n+=skip;
		}
	}
}
void main(int argc, char **argv) {
	bool numeric=0;
	uint_fast64_t start=0;
	uint_fast32_t slots=1;
	uint_fast32_t assigned=1;
	uint_fast32_t offset=0;
	uint_fast32_t length=1;
	char *prefix="";
	numeric=0;
	setCharset(alphanum);
	while (1) {
		static struct option long_options[] = {
			{"charset",      required_argument, 0, 99  }, //c
			{"numeric",      no_argument,       0, 110 }, //n
			{"alphanum",     no_argument,       0, 97  }, //a
			{"resume",       required_argument, 0, 114 }, //r
			{"blocksize",    required_argument, 0, 116 }, //b
			{"length",       required_argument, 0, 101 }, //e
			{"my-lines",     required_argument, 0, 108 }, //l
			{"offset-lines", required_argument, 0, 111 }, //o
			{"debug",        optional_argument, 0, 100 }, //d
			{"prefix",       required_argument, 0, 112 }, //p
			{0,              0,                 0, 0   }
		};
		char c = getopt_long(argc, argv, "c:nar:b:e:l:o:d::p:", long_options, 0);
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
			case 'b':
				slots=strtoul(optarg,NULL,10);
				break;
			case 'e':
				length=strtoul(optarg,NULL,10);
				break;
			case 'l':
				assigned=strtoul(optarg,NULL,10);
				break;
			case 'o':
				offset=strtoul(optarg,NULL,10);
				break;
			case 'd':
				debug=1;
				break;
			case 'p':
				setPrefix(optarg);
				break;
			default:
				fprintf(stderr, HELP_MSG);
				exit(1);
		}
	}
	initpass(length);
	spew(start, length, slots, assigned, offset, prefix);
}
