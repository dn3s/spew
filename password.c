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
static char *pass="";
static size_t passl=1;
void addDigit(size_t n)
{
	size_t i;
	passl+=n;
	if(!realloc(passn, passl)){
		fprintf(stderr, "Realloc failed!\n");
		exit(1);
	}
	if(!realloc(pass, passl+1)){ //don't forget the trailing null!
		fprintf(stderr, "Realloc failed!\n");
		exit(1);
	}
	pass[passl]=0;
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
			printf("passl: %02u, passn[]:", passl);
		for(i=0; i<passl; i++) {
			printf(" %02u", passn[i]);
		}
		printf(", pass: ");
	}
	printf(prefix);
	for(i=0; i<passl; i++) {
		pass[i]=chars[passn[i]];
	}
	puts(pass);
}
void initpass(uint_fast32_t l)
{
	passn=calloc(l,sizeof(char));
	pass=calloc(l+1,sizeof(char));
	pass[l]=0;
	passl=l;
}
void setCharset(char* c)
{
	charsl=strlen(c);
	chars=c;
	if(debug) {
		fprintf(stderr, "Charset set to %u chars: \"%s\"\n", charsl, chars);
	}
}
void setPrefix(char* p)
{
	//puts("setPrefix");
	prefix=p;
	if(debug) {
		fprintf(stderr, "Prefix set to \"%s\"\n", prefix);
	}
}
