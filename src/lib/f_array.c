/*
 * f_array.c
 *
 *  Created on: 3-gen-2012
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - funzioni sugli array
 */

#include <stdlib.h>
#include <stdio.h>

#include "config.h"

/*
 * Funzione per compiere un OR o un AND bit a bit su un array.
 */
void orand(
		unsigned char *first,					// array di destinazione
		unsigned char *second,					// array di provenienza
		int size,								// quantità di byte
		int operation							// AND o OR
)
{
	int j;

	switch (operation){
	case 1 :
		for (j=0; j<size;j++)
			first[j] |= second[j];
		break;
	case 2 :
		for (j=0; j<size;j++)
			first[j] &= second[j];
		break;
	}
}

/*
 * Funzione per la copia di un intero array di char.
 */
void copy(
		unsigned char *from,					// array di partenza
		unsigned char *to,						// array di destinazione
		int size								// grandezza dell'array di destinazione
)
{
	int j;
	if (size>0)
		for(j=0; j<size; j++)
			to[j] = from[j];
}

/*
 * Funzione per eseguire lo shift left in un array di char.
 */
void shift_left(
		unsigned char *array,					// array da shiftare
		int size,								// dimensione dell'array
		int amount								// numero di bit
)
{
	register unsigned int k     = amount >> 3;   	//  /8;
	register unsigned int j     = amount & 0x07;	// % 8;
	register unsigned char *p   = array;
	register unsigned int i     = 0;
	register unsigned int limit = size-k;;
	register unsigned int shift;

	if (k != 0)
	{
		for (i=0; i<limit; i++)
			*(p+i) = *(p+i+k);
		for (;i<size;i++)
			*(p+i) = 0;
	}
	if (j != 0)
	{
		limit--;
		i = 0;
		shift = 8-j;

		do
		{
			*(p+i) <<= j;
			if(i < limit)
				*(p+i) |= *(p+i+1) >> shift;
			else
				return;
			i++;
		}
		while(1);
	}
	/*int i = 0;
	unsigned char temp;
	unsigned char mask;
	unsigned char k = amount / 8;
	unsigned char j = amount % 8;
	if (k != 0){
		for (i=0; i<size-k; i++)
			array[i] = array[i+k];
		for (;i<size;i++)
			array[i] = 0;
	}
	if (j != 0){
		for (i=0; i<size-k;i++){
			array[i] <<= j;
			if((i+1) >= size){
				temp = 0;
			} else {
				temp = array[i+1];
			}
			mask = 0xFF << (8-j);
			array[i] |= (mask & temp) >> (8-j);
		}
	}*/
}

/*
 * Funzione per eseguire lo shift right in un array di char.
 */
void shift_right(
		unsigned char *array,					// array da shiftare
		int size,								// dimensione dell'array
		int amount								// numero di bit
)
{
	register unsigned int k  	= amount >> 3;   	// / 8;
	register unsigned int j  	= amount & 0x07; 	// % 8;
	register unsigned char *p 	= array;
	register unsigned int i 	= 0;
	register unsigned int limit = size-k;
	register unsigned int shift;

	if (k != 0)
	{
		for (i=size-1; i>=k; i--)
			*(p+i) = *(p+i-k);
		for (i=0;i<k;i++)
			*(p+i) = 0;
	}
	if (j != 0)
	{
		limit--;
		i = size-1;
		shift = 8-j;
		do
		{
			*(p+i) >>= j;
			if(i > 0)
				*(p+i) |= *(p+i-1) << shift;
			else
				return;
			i--;
		}
		while(1);
	}
	/*int i = 0;
	unsigned char temp;
	unsigned char mask;
	unsigned char k = amount / 8;
	unsigned char j = amount % 8;
	if (k != 0){
		for (i=size-1; i>=k; i--)
			array[i] = array[i-k];
		for (; i>=0; i--)
			array[i] = 0;
	}
	if (j != 0){
		for (i=size-1; i>=0; i--){
			array[i] >>= j;
			if (i == 0){
				temp = 0;
			} else {
				temp = array[i-1];
			}
			mask = 0xFF >> (8-j);
			array[i] |= (mask & temp) << (8-j);
		}
	}*/
}

/*
 * Funzione per la comparazione di due array per length caratteri. Se uguali ritorna 1, se diversi 0;
 */
int compare(
		unsigned char *data1,					// primo array
		unsigned char *data2,					// secondo array
		int length								// lunghezza di comparazione
)
{
	int i=0;
	int flag=1;
	for (i=0; i<length; i++){
		if (data1[i] != data2[i]){
			flag = 0;
			break;
		}
	}
	return flag;
}

/*
 * Funzione per l'incremento di un array.
 */
void incrementa(
		unsigned char *data,
		int size
)
{
	int i=0;
	for (i=size-1; i>0; i--){
		data[i]++;
		if (data[i] != 0){
			break;
		}
	}
}
