/*
 * f_codifica.c
 *
 *  Created on: 6-gen-2012
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - funzione per la codifica canonica
 */

#include <stdlib.h>
#include <stdio.h>

#include "f_array.h"

#include "structs.h"
#include "config.h"

/*
 * Funzione per la codifica canonica di Huffman a partire da un array contenete i codici con
 * la sola lunghezza.
 */
void codifica_canonica(struct code *array_codici[])
{
	int i = 0;
	int l = 0;
	int size = (NUM_CHAR/8);

	unsigned char buffer_codice[size];
	unsigned char buffer_second[size];

	for (i=0; i<size; i++){
		buffer_codice[i] = 0;
		buffer_second[i] = 0;
	}

	for (i=0; i<NUM_CHAR; i++){

		if (array_codici[i]->lunghezza != 0){
			if (l != 0){
				incrementa(buffer_codice,size);
			}
			if (array_codici[i]->lunghezza != l){
				shift_left(buffer_codice,size,(array_codici[i]->lunghezza-l));
				l = array_codici[i]->lunghezza;
			}

			array_codici[i]->canonico = (unsigned char *)malloc(array_codici[i]->lunghezza/8+1);

			copy(buffer_codice,buffer_second,size);
			shift_left(buffer_second,size,(size*8-l));
			copy(buffer_second,array_codici[i]->canonico,array_codici[i]->lunghezza/8+1);
		}
	}
}
