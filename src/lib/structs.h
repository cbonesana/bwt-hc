/*
 * structs.h
 *
 *  Created on: 5-nov-2011
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - dichiarazione delle strutture
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

struct nodo {					// Nodo di un albero
	struct nodo *sx;				// nodo sinistro
	struct nodo *dx;				// nodo destro
	unsigned int carattere;			// simbolo
	unsigned int frequenza;			// frequenza di comparizione
};

struct code {					// Struttura per i codici e la loro codifica
	unsigned char carattere;		// simbolo originale
	unsigned char lunghezza;		// lunghezza in bit del codice
	unsigned char *canonico;		// valore del codice
};

struct buffer {					// Struttura per i buffer
	unsigned char *data;			// dati contenuti (usare un malloc o calloc)
	unsigned int size;				// dimensione (in byte)
	unsigned int pointer;			// indicatore per quanto è pieno il buffer (in bit)
};

#endif /* STRUCTS_H_ */
