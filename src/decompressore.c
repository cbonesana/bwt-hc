/*
 * decompressore.c
 *
 *  Created on: 6-nov-2011
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - decodifica
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/structs.h"
#include "lib/MergeSort.h"

#include "lib/f_array.h"
#include "lib/f_codifica.h"

#include "lib/config.h"

#include "lib/f_debug.h"

#define debug 0

/*
 * Funzione per la crezione dell'array di frequenze a partire dalla tabella con le lunghezze dei
 * codici. Questa funzione legge i primi 4 byte (lunghezza del file) e i successivi 256
 * (lunghezze dei codici) ricreando la struttura base per la codifica canonica.
 */
unsigned int ricrea_lunghezze (
		FILE *f, 								// file in input
		struct code *arr_lunghezze[NUM_CHAR]	// array dei puntatiri ai codici
)
{
	int i = 0;
	unsigned int n = 0;
	unsigned char buffer[256];
	fseek(f,0,SEEK_SET);
	fread((unsigned int *)&n,sizeof(unsigned int),1,f);
	fread((unsigned char *)&buffer,sizeof(unsigned char),256,f);

	for(i=0;i<NUM_CHAR;i++)
	{
		arr_lunghezze[i]->lunghezza = buffer[i];
	}
	return n;
}

/*
 * Programma principale.
 */
int Canonical_Huffman_Decode (
		char *filename1,						// nome del file da decomprimere
		char *filename2							// nome del file decompresso
)
{
	struct code *codici[NUM_CHAR];				// array con i codici

	unsigned int val = 0;
	int i,j;									// contatori interi
	FILE *f;
	FILE *n;

	struct buffer primario;						// buffer di scrittura lettura primario
	struct buffer secondario;
	struct buffer primario_mask;
	struct buffer temp1;
	struct buffer temp2;

	for (i=0; i<NUM_CHAR;i++)
	{
		codici[i] = (struct code *) malloc(sizeof(struct code));
		codici[i]->lunghezza = 0;
		codici[i]->carattere = (unsigned char)i;
		codici[i]->canonico  = NULL;
	}

	f = fopen(filename1,"rb");

	if(debug)
		printf("Il file %s e' stato aperto\n",filename1);
	if (f == NULL)
	{
		printf("Non e' stato possibile aprire un file in lettura.\n");
		exit(2);
	}

	val = ricrea_lunghezze(f,codici);
	printf(" %d byte ",val);
	fflush(stdout);
	mergesort_code(codici,0,NUM_CHAR-1);
	codifica_canonica(codici);

	if (debug)
		printf("Codifica canonica di Huffman completata\n");

	n=fopen(filename2,"wb");

	if(debug)
		printf("Il file %s e' stato creato\n",filename2);

	if (n == NULL)
	{
		printf("Non e' stato possibile aprire un file in scrittura\n");
		exit(3);
	}

	primario.size      = codici[NUM_CHAR-1]->lunghezza/8+1;		// buffer di lettura primario (sempre pieno)
	secondario.size    = primario.size*4;						// buffer di lettura secondario (a contenuto variabile)
	primario_mask.size = primario.size;							// maschera per operazioni sui codici
	temp1.size         = primario.size;							// buffer per operazioni sul buffer primario
	temp2.size         = secondario.size;						// buffer per operazioni sul secondo buffer


	primario.pointer      = 0;
	secondario.pointer    = 0;
	primario_mask.pointer = 0;
	temp1.pointer         = 0;
	temp2.pointer         = 0;

	primario.data      = (unsigned char *) calloc(sizeof(unsigned char)*primario.size,1);
	secondario.data    = (unsigned char *) calloc(sizeof(unsigned char)*secondario.size,1);
	primario_mask.data = (unsigned char *) calloc(sizeof(unsigned char)*primario_mask.size,1);
	temp1.data         = (unsigned char *) calloc(sizeof(unsigned char)*temp1.size,1);
	temp2.data         = (unsigned char *) calloc(sizeof(unsigned char)*temp2.size,1);

	if (debug)
		printf("Inizio decompressione\n");

	fread((unsigned char *)(primario.data),sizeof(unsigned char),primario.size,f);			// caricamento del buffer primario
	fread((unsigned char *)(secondario.data),sizeof(unsigned char),secondario.size,f);		// caricamento del buffer secondario

	while (val > 0)																			// la lettura continua fin quando non vengono decompressi tutti i byte
	{
		if(debug)
		{
			printf("Buffer primario:   ");
			for (j=0; j<primario.size; j++)
			{
				printf("%02x ",primario.data[j]);
			}
			printf("\nBuffer secondario: ");
			for (j=0; j<secondario.size; j++)
			{
				printf("%02x ",secondario.data[j]);
			}
			printf("\n");
		}

		for (i=0; i<NUM_CHAR; i++)																		// ricerca della sequenza corretta partendo dalla più lunga
		{
			if(codici[i]->lunghezza != 0)
			{
				for (j=0; j<temp1.size;j++)																// creazione delle maschere che verranno utilizzate
				{
					temp1.data[j] = 0xFF;
					primario_mask.data[j] = 0;
				}

				shift_left(temp1.data,temp1.size,temp1.size*8 - codici[i]->lunghezza);							// preparo la maschera eliminando i bit da non cancellare
				orand(temp1.data,primario.data,temp1.size,AND);													// copio il contenuto del buffer primario nel temporaneo, sfruttando la maschera
				copy(codici[i]->canonico,primario_mask.data,codici[i]->lunghezza/8+1);							// memorizzo solamente i codici che mi interessano

				shift_right(primario_mask.data,primario_mask.size,primario_mask.size*8-codici[i]->lunghezza);	// elimino i bit in eccesso
				shift_left(primario_mask.data,primario_mask.size,primario_mask.size*8-codici[i]->lunghezza);	// riallineo

				if (compare(primario_mask.data,temp1.data,codici[i]->lunghezza/8+1) == 1)						// comparo la maschera letta con i codici in memoria e se trovo una corrispondenza...
				{
					val--;
					fwrite((unsigned char *)&(codici[i]->carattere),sizeof(unsigned char),1,n);			// ...scrivo il caratterre decompresso sul nuovo file
					fflush(n);

					shift_left(primario.data,primario.size,codici[i]->lunghezza);						// elimino i bit decompressi
					copy(secondario.data,temp2.data,temp2.size);										// preparo il lavoro sul buffer secondario
					shift_left(temp2.data,temp2.size,secondario.pointer);								// sistemo eliminando i bit già utilizzati indicati dal valore di .pointer
					copy(temp2.data,temp1.data,temp1.size);												// copio parte del buffer secondario nel primario temporaneo
					shift_right(temp1.data,temp1.size,temp1.size*8-codici[i]->lunghezza);				// allineo i bit per la copia nel buffer primario

					if (debug){
						stampa(primario.data,primario.size,"primario:  ");
						stampa(temp1.data,temp1.size,"temp1:     ");
						stampa(secondario.data,secondario.size,"secondario:");
						stampa(temp2.data,temp2.size,"temp2:     ");
					}

					orand(primario.data,temp1.data,primario.size,OR);									// inserisco i nuovi bit nel buffer primario, completandolo
					secondario.pointer += codici[i]->lunghezza;											// incremento l'indicatore dei bit utilizzati dal buffer secondario

					if (secondario.pointer/8 > 0)														// se sono stati liberati un multiplo di 8 bit, ricarico il buffer secondario di tale valore
					{
						if (debug)
							printf("sto leggendo\n");

						for (j=0; j<temp2.size;j++)														// pulizia del buffer secondario temporaneo
							temp2.data[j] = 0;

						fread(temp2.data,sizeof(unsigned char),secondario.pointer/8,f);					// pointer/8 sicuramente > 0

						shift_left(secondario.data,secondario.size,(secondario.pointer/8)*8);			// preparazione del buffer secondario, /8*8 per avere un valore corretto di byte interi
						shift_right(temp2.data,temp2.size,(temp2.size-secondario.pointer/8)*8);			// allineamento dei nuovi bit
						orand(secondario.data,temp2.data,secondario.size,OR);							// unione dei nuovi bit

						secondario.pointer -= (secondario.pointer/8)*8;									// decremento dell'indicatore dei bit utilizzati

					}
					break;																				// esco dal ciclo e passo a trovare la prossima sequenza per i prossimi bit
				}
			}
		}
	}

	fclose(f);
	fclose(n);

	return 0;
}
