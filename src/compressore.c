/*
 * compressore.c
 *
 *  Created on: 20-ott-2011
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - codifica
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/structs.h"
#include "lib/MergeSort.h"

#include "lib/f_array.h"
#include "lib/f_codifica.h"

#include "lib/config.h"

#define debug 0

// Variabili  ausiliarie per la funzione ricorsiva della "codifica"
unsigned int buffer_codice    	= 0;
unsigned int buffer_lunghezza 	= 0;
unsigned int buffer_codice_mask = 1;

//Dichiarazione di Funzioni

/*
 * Funzione per la crezione dell'array di frequenze. Il file viene percorso una volta per contare
 * l'apparizione di ogni singolo simbolo. Nel contempo viene contato anche il numero totale di
 * byte che compongono il file.
 */
unsigned int conteggia (
		char *argomento,						// nome del file
		struct nodo *arr_frequenze[NUM_CHAR]	// array di puntatori a nodi in cui verranno scritte le frequenze
)
{
	FILE *f = fopen(argomento,"rb");
	unsigned char buffer;
	unsigned int n = -1;

	while (!feof(f))
	{
		fread((unsigned char *)(&buffer),sizeof(unsigned char),1,f);
		arr_frequenze[(int)buffer]->frequenza ++;
		n++;
	}

	fclose(f);

	if (n == 0)
	{
		printf("Non e' stato trovato un file valido.");
		exit(1);
	}

	return n;
}

/*
 * Funzione per l'unione di due alberi. Questa funzione iterativa viene utilizzata per creare un
 * albero partendo dal basso, cioe' dalle foglie.
 */
struct nodo *unisci_alberi(
		struct nodo* a,							// primo nodo
		struct nodo* b							// secondo nodo
)
{
	struct nodo *temp;

	if (a->frequenza == 0 )
	{
		temp = b;
	}
	else if (a->frequenza != 0 && b->frequenza == 0)
	{
		temp = a;
	}
	else if (a->frequenza != 0 && b->frequenza != 0)
	{
		temp = (struct nodo *)malloc(sizeof(struct nodo));
		temp->dx = a;
		temp->sx = b;
		temp->frequenza = a->frequenza+b->frequenza;
		temp->carattere = 0;
	}
	else
	{
		temp = NULL;
	}
	return temp;
}

/*
 * Funzione ricorsiva per codificare secondo i dettami di messer Huffman. Utilizza delle variabili
 * globali per tenere traccia delle lunghezze rispetto alla radice dell'albero. Queste lunghezze
 * saranno utilizzate per la cofica canonica.
 */
void codifica(
		struct nodo *radice,					// albero delle frequenze
		struct code *codici[NUM_CHAR]			// array di puntatori a codici in cui saranno scritti i codici associati ad ogni simbolo
)
{
	if (radice == NULL)
	{
		printf("Radice dell'albero non valida.\n");
		exit(4);
	}
	if (radice->sx != NULL)
	{
		buffer_lunghezza++;
		buffer_codice_mask <<= 1;
		codifica(radice->sx,codici);
	}
	if (radice->sx == NULL && radice->dx == NULL)
	{
		codici[radice->carattere]->lunghezza = buffer_lunghezza;
	}
	if (radice->dx != NULL)
	{
		buffer_lunghezza++;
		buffer_codice |= buffer_codice_mask;
		buffer_codice_mask <<=1;
		codifica(radice->dx,codici);
	}
	buffer_lunghezza--;
	buffer_codice_mask >>= 1;
	buffer_codice &= ~buffer_codice_mask;
}

/*
 * Funzione per la compressioen di un file. Una volta che tutto e' pronto per la codifica, questa
 * funzione viene chiamata e procede alla rilettura del primo file e alla scrittura di un secondo
 * file compresso con i codici generati dalla codifica canonica.
 */
void comprimi (
		char *argomento1,						// nome del file da comprimere
		char* argomento2,						// nome del file compresso, divenderà argomento2.thuf
		struct code *codici[NUM_CHAR],			// array di codici
		unsigned int val						// lunghezza originaria del file
)
{
	FILE *n;									// file compresso
	FILE *f;									// file originale

	char filename[255];

	int i = 0;
	int x = 1;									// moltiplicatore per il buffer di scrittura 		--DEPRECATO--

	unsigned char lettura;						// buffer di lettura
	struct buffer scrittura;					// buffer di scrittura
	struct buffer scrittura_mask;				// maschera per la scrittura dei singoli bit nel buffer di scrittura

	scrittura.size      = NUM_CHAR;
	scrittura_mask.size = scrittura.size;

	scrittura.pointer      = 0;
	scrittura_mask.pointer = scrittura.pointer;

	scrittura.data       = (unsigned char *)calloc(scrittura.size,1);
	scrittura_mask.data  = (unsigned char *)calloc(scrittura_mask.size,1);

	sprintf(filename,"%s.thuf",argomento2);

	f = fopen(argomento1,"rb");									// apertura del file originale
	n = fopen(filename,"wb+");									// apertura del file compresso

	fwrite((unsigned int *)&val,sizeof(unsigned int),1,n);		// scrittura della lunghezza del file
	for (i=0;i<NUM_CHAR;i++)
	{															// scrittura della t abella dei codici
		fputc(codici[i]->lunghezza,n);							// sfruttando la codifica canonica vengono passate unicamente le lunghezze dei simboli
	}

	while (!feof(f))
	{
		fread((unsigned char *)(&lettura),sizeof(unsigned char),1,f);									// lettura del prossimo byte
		for (i=0; i<scrittura.size; i++)
		{
			scrittura_mask.data[i] = 0;																	// pulizia della maschera
		}
		copy(codici[(int)lettura]->canonico,scrittura_mask.data,codici[(int)lettura]->lunghezza/8+1);	// copia del codice relativo al carattere attuale nella maschera di scrittura
		shift_right(scrittura_mask.data,scrittura_mask.size,scrittura.pointer);							// allineamento a destra del codice
		orand(scrittura.data,scrittura_mask.data,scrittura.size,OR);									// unione del codice da scrivere con il buffer di scrittura
		scrittura.pointer += codici[(int)lettura]->lunghezza;											// incremento del numero di bit contenuti nel buffer
		while (scrittura.pointer/(8*x)> 0)																// se il buffer di scrittura e' pieno con più di 8 bit...
		{
			fwrite((unsigned char*)scrittura.data,sizeof(unsigned char),x,n);							// ...scrive un byte
			shift_left(scrittura.data,scrittura.size,(8*x));											// elimina il byte appena scritto dal buffer di lettura
			scrittura.pointer -= (8*x);																	// decrementa l'indicatore del numero di bit presenti nell'array
		}																								// se e' presente più di un byte l'operazioen veine ripetuta
	}

	if(scrittura.pointer > 0)																			// se il buffer di scrittura contiene ancora dei bit, questi devono essere scritti
	{
		scrittura_mask.data = (unsigned char*)malloc(sizeof(unsigned char)*(scrittura.pointer/8+1));	// creazione di un buffer con una lunhgezza corretta, riciclando la maschera di scrittura
		for (i=0; i<scrittura_mask.size/8+1;i++){
			scrittura_mask.data[i] = 0xFF;																// creazione di una maschera pinea di bit a '1'
		}
		shift_right(scrittura_mask.data,scrittura_mask.size,scrittura.pointer);							// allineamento a destra della maschera per salvare i bit già presenti
		orand(scrittura.data,scrittura_mask.data,scrittura.size,OR);									// unione della maschera con il buffer di scrittura
		fwrite(scrittura.data,sizeof(unsigned char),scrittura.pointer/8+1,n);							// scrittura del buffer di scrittura
	}

	fclose(n);
	fclose(f);
}

/*
 * Programma principale.
 */
int Canonical_Huffman_Code (
		char *filename1,						// nome del file da comprimere
		char *filename2							// nome del file compresso
)
{
	struct nodo *huffman = NULL;				// Radice dell'albero di Huffman

	int i = 0;									//Contatori interi
	int j = 0;
	unsigned int n = 0;

	struct nodo *frequenze[NUM_CHAR];									// array per le frequenze
	struct code *codici[NUM_CHAR];										// array per i codici utilizzati
	struct nodo *zero = (struct nodo *)malloc(sizeof(struct nodo));		// nodo vuoto


	if (debug)
		printf("Inizializzazione in corso.\n");

	for (i=0; i<NUM_CHAR; i++){											// Inizializzazione
		frequenze[i] = (struct nodo *)malloc(sizeof(struct nodo));
		frequenze[i]->sx = NULL;
		frequenze[i]->dx = NULL;
		frequenze[i]->frequenza = 0;
		frequenze[i]->carattere = i;

		codici[i] = (struct code *)malloc(sizeof(struct code));
		codici[i]->lunghezza = 0;
		codici[i]->carattere = i;
		codici[i]->canonico  = NULL;
	}

	zero->carattere = 0;
	zero->frequenza = 0;
	zero->sx = NULL;
	zero->dx = NULL;

	if (debug)
		printf("Inizializzazione completata.\n");

	// Inizio del processo di compressione
	n = conteggia(filename1,frequenze);									// conteggio delle frequenze

	printf(" %d byte...",n);
	fflush(stdout);

	mergesort_freq(frequenze,0,NUM_CHAR-1);								// ordinamento dell'array delle frequenze

	for (i=0;i<NUM_CHAR-1;i++)											// creazione dell'albero di Huffman
	{
		frequenze[i] = unisci_alberi(frequenze[i],frequenze[i+1]);		// unione delle foglie (o dei nodi) con la frequenza più bassa
		frequenze[i+1] = zero;											// eliminazione di una foglia (o un nodo) utilizzato
		mergesort_freq(frequenze,0,NUM_CHAR-1);							// ordinamento dell'array delel frequenze
	}
	huffman = frequenze[255];											// la radice dell'albero si trova nell'ultimo nodo

	codifica(huffman,codici);											// codifica di Huffman e creazione delle lunghezze
	mergesort_code(codici,0,NUM_CHAR-1);								// ordinamento dei codici
	codifica_canonica(codici);											// creazione dei codici mediante la codifica canonica

	if (debug)
	{
		for (i=0; i<NUM_CHAR;i++){
			if (codici[i]->lunghezza != 0){
				printf("[%3d] [%2d]",codici[i]->carattere,codici[i]->lunghezza);
				for(j=0; j<codici[i]->lunghezza/8+1;j++){
					printf("%02x",codici[i]->canonico[j]);
				}
				printf("\n");
			} else {
				printf("[%3d] [%2d] 0 \n",codici[i]->carattere,codici[i]->lunghezza);

			}
		}
		printf("\n");
	}

	mergesort_car(codici,0,NUM_CHAR-1);									// ordinamento dei codici secondo l'ordine dei simboli
	comprimi(filename1,filename2,codici,n);								// compressione

	return 0;
}
