// bits.c ... functions on bit-strings
// part of SIMC signature files
// Bit-strings are arbitrarily long byte arrays
// Least significant bits (LSB) are in array[0]
// Most significant bits (MSB) are in array[nbytes-1]

// Written by Sixiang He, 2020

#include <assert.h>
#include "defs.h"
#include "bits.h"
#include "page.h"

typedef struct _BitsRep {
	Count  nbits;		  // how many bits
	Count  nbytes;		  // how many bytes in array
	Byte   bitstring[1];  // array of bytes to hold bits
	                      // actual array size is nbytes
} BitsRep;

// create a new Bits object
Bits newBits(int nbits)
{
	Count nbytes = iceil(nbits,8);
	Bits new = malloc(2*sizeof(Count) + nbytes);
	new->nbits = nbits;
	new->nbytes = nbytes;
	memset(&(new->bitstring[0]), 0, nbytes);
	return new;
}

// release memory associated with a Bits object
void freeBits(Bits b) {
	free(b);
}

// check if the bit at position is 1
Bool bitIsSet(Bits b, int position) {
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	// 1 byte = 8 bits
	int byte_pos = position / 8;
	int bit_offest = position % 8;
	Byte * list_of_bytes = b->bitstring;
	Byte byte = list_of_bytes[byte_pos];
	Byte mask = 1 << bit_offest;
	return (byte & mask) == mask? TRUE : FALSE;
}

// check whether one Bits b1 is a subset of Bits b2
Bool isSubset(Bits b1, Bits b2) {
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	if (b1->nbits > b2->nbits) {
		return FALSE;
	}
	for (Count i = 0; i < b1->nbytes; i++) {
		if ( (b1->bitstring[i] & b2->bitstring[i]) != b1->bitstring[i]) {
			return FALSE;
		}
	}
	return TRUE; 
}

// set the bit at position to 1
void setBit(Bits b, int position) {
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	// 1 byte = 8 bits
	int byte_pos = position / 8;
	int bit_offest = position % 8;
	Byte * list_of_bytes = b->bitstring;
	list_of_bytes += byte_pos;
	Byte mask = 1 << bit_offest;
	*list_of_bytes = *list_of_bytes | mask;
}

// set all bits to 1
void setAllBits(Bits b) {
	assert(b != NULL);
	Byte mask = 255; // 0b11111111
	Count i;
	for (i = 0; i < b->nbytes-1; i++) {
		b->bitstring[i] |= mask;
	}
	int bit_offest = b->nbits % 8;
	if (bit_offest != 0) {
		mask = mask >> (8 - bit_offest);
	}
	b->bitstring[i] |= mask;
}

// set the bit at position to 0
void unsetBit(Bits b, int position) {
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	// 1 byte = 8 bits
	int byte_pos = position / 8;
	int bit_offest = position % 8;
	Byte * list_of_bytes = b->bitstring;
	list_of_bytes += byte_pos;
	Byte mask = ~(1 << bit_offest);
	*list_of_bytes = *list_of_bytes & mask;
}

// set all bits to 0
void unsetAllBits(Bits b) {
	assert(b != NULL);
	Byte mask = 0; 
	Count i;
	for (i = 0; i < b->nbytes-1; i++) {
		b->bitstring[i] &= mask;
	}
	int bit_offest = b->nbits % 8;
	if (bit_offest != 0) {
		mask = ~((~mask) >> (8 - bit_offest));
	}
	b->bitstring[i] &= mask;
}

// bitwise AND ... b1 = b1 & b2
void andBits(Bits b1, Bits b2) {
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	for (Count i = 0; i < b1->nbytes; i++) {
		b1->bitstring[i] &= b2->bitstring[i];
	}
}

// bitwise OR ... b1 = b1 | b2
void orBits(Bits b1, Bits b2) {
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	Count i;
	for (i = 0; i < b1->nbytes - 1; i++) {
		b1->bitstring[i] |= b2->bitstring[i];
	}
	Byte mask = 255;
	int bit_offest = b1->nbits % 8;
	if (bit_offest != 0) {
		mask = mask >> (8 - bit_offest);
	}
	b1->bitstring[i] = b1->bitstring[i] | (b2->bitstring[i] & mask);
}

// get a bit-string (of length b->nbytes)
// from specified position in Page buffer
// and place it in a BitsRep structure
void getBits(Page p, Offset pos, Bits b) {
	Byte * address = addrInPage(p, pos, b->nbytes);
	memcpy(b->bitstring, address, b->nbytes);
}

// copy the bit-string array in a BitsRep
// structure to specified position in Page buffer
void putBits(Page p, Offset pos, Bits b) {
	Byte * address = addrInPage(p, pos, b->nbytes);
	memcpy(address, b->bitstring, b->nbytes);
}

// show Bits on stdout
// display in order MSB to LSB
// do not append '\n'
void showBits(Bits b) {
	assert(b != NULL);
    //printf("(%d,%d)",b->nbits,b->nbytes);
	for (int i = b->nbytes-1; i >= 0; i--) {
		for (int j = 7; j >= 0; j--) {
			Byte mask = (1 << j);
			if (b->bitstring[i] & mask)
				putchar('1');
			else
				putchar('0');
		}
		//putchar(' ');
	}
}
