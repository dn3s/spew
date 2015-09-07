#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

static uint_fast32_t charsl=0;
static char *chars="";
static char *passn;
static size_t passl=1;
static bool debug=0;
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
void setCharset(char* c)
{
	charsl=strlen(c);
	chars=calloc(charsl+1, sizeof(char));
	strncpy(chars, c, charsl+1);
	if(debug) {
		fprintf(stderr, "Charset set to %u chars: \"%s\"\n", charsl, chars);
	}
}
