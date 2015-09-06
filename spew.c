#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "help.h"
#include "password.h"

static char *alphanum="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
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
