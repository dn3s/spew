#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
#include <password.h>

static uint_fast32_t charsl=0;
static char *chars="";
static char *prefix="";
static char *passn;
static size_t passl=1;
void addDigit(size_t n)
{
	size_t i;
	passl+=n;
	if(!realloc(passn, passl)){
		fprintf(stderr, "Realloc failed!\n");
		exit(1);
	}
	for(i=passl-1; i>0; i--) {
		passn[i]=passn[i-n];
	}
	for(i=0; i<n; i++) {
		passn[i]=0;
	}
}
void add(uint_fast32_t n) //Advance the string by n permutations
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
	size_t i;
	if(debug) {
		for(i=0; i<passl; i++) {
			printf("passl: %2u passn: %2u ", passl, passn[i]);
		}
		printf(" = ");
	}
	printf(prefix);
	for(i=0; i<passl; i++) {
		printf("%c", chars[passn[i]]); //TODO: test whether this is faster or building a string and printing it in one go is faster.
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
void setPrefix(char* p)
{
	prefix=p;
	if(debug) {
		fprintf(stderr, "Prefix set to \"%s\"\n", charsl, chars);
	}
}
