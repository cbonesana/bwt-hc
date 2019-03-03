/*
 * f_debug.c
 *
 *  Created on: 15-gen-2012
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - funzioni per il debugging
 */

#include <stdlib.h>
#include <stdio.h>

/*
 * DEBUGGING: stampa di un array di char.
 */
void stampa(
		unsigned char* data,						// array da stampare
		int size, 									// dimensione dell'array
		char *name									// etichetta da stampare
		)
{
	int i = 0;
	printf("%s ",name);
	for (i=0; i<size;i++)
		printf("%02x ",data[i]);
	printf("\n");
}
