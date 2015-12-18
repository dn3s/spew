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
	if(!realloc(pass, passl+1)){ //add another char for the null terminator!
		fprintf(stderr, "Realloc failed!\n");
		exit(1);
	}
	pass[passl]=0;
	for(i=passl-1; i>0; i--) { //bump over current digits to the right by n places
		passn[i]=passn[i-n];
		pass[i]=pass[i-n];
	}
	for(i=0; i<n; i++) { //initialize the new digits to zero.
		passn[i]=0;
		pass[i]=chars[0];
	}
}
void add(uint_fast32_t n) //Advance the string by n permutations
{
	size_t d=passl; //d is the digit to start with: the last digit. Note the last digit is actually passl-1, but we'll let the decrement operation in the while loop deal with that FOR OPTIMIZATION
	while(n) {
		d--; //next digit. This is at the start FOR OPTIMIZATION.
		uint_fast32_t carry=(passn[d]+n)/charsl; //the amount to carry over to the next digit
		passn[d]=(passn[d]+n)%charsl; //add n to the current digit, mod charsl
		pass[d]=chars[passn[d]];
		n=carry; //and now discard the remainder "used up" in this digit by just setting it to carry
		if((d==0) && n) { //if n is non-zero (there's still more to add) and we've run out of digits, add another digit, and bump indexes accordingly. The order is chosen since d==0 is less frequently true than n is (short-circuit eval gains)
			addDigit(1); //we could do this in bursts instead of every time, to save on malloc overhead, but adding digits doesn't happen frequently (at least for normal use cases), and the complexities introduced into every cycle to deal with that may even outweigh the benefits once addigin digits (inevitably) becomes more rare further into runtime.
			n--;
			d++;
		}
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
	puts(pass);
}
void initpass(uint_fast32_t l)
{
	//allocate memory
	passn=calloc(l,sizeof(char));
	pass=calloc(l+1,sizeof(char));
	//initialize values
	pass[l]=0;
	pass[0]=chars[0];
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
