// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{
	Bits b1 = newBits(16);
	Bits b2 = newBits(14);
	setBit(b1, 15);
	setBit(b1, 2);
	setBit(b1, 8);
	printf("t=0: "); showBits(b1); printf("\n");
	printf("t=0: "); showBits(b2); printf("\n");
	setAllBits(b1);
	setAllBits(b2);
	printf("t=1: "); showBits(b1); printf("\n");
	printf("t=1: "); showBits(b2); printf("\n");
	orBits(b2, b1);
	printf("t=2: "); showBits(b1); printf("\n");
	printf("t=2: "); showBits(b2); printf("\n");
	unsetAllBits(b1);
	unsetBit(b2, 3);
	unsetBit(b2, 11);
	unsetBit(b2, 6);
	printf("t=3: "); showBits(b1); printf("\n");
	printf("t=3: "); showBits(b2); printf("\n");
	orBits(b1, b2);
	printf("t=4: "); showBits(b1); printf("\n");
	printf("t=4: "); showBits(b2); printf("\n");

	/*
	Bits b = newBits(60);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");
	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	printf("t=2: "); showBits(b); printf("\n");
	if (bitIsSet(b,5)) printf("Bit 5 is set\n");
	if (bitIsSet(b,10)) printf("Bit 10 is set\n");
	setAllBits(b);
	printf("t=3: "); showBits(b); printf("\n");
	unsetBit(b, 40);
	printf("t=4: "); showBits(b); printf("\n");
	if (bitIsSet(b,20)) printf("Bit 20 is set\n");
	if (bitIsSet(b,40)) printf("Bit 40 is set\n");
	if (bitIsSet(b,50)) printf("Bit 50 is set\n");
	unsetBit(b, 59);
	printf("t=5: "); showBits(b); printf("\n");
	*/
	return 0;
}
