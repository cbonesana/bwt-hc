/*
 * main.c
 *
 *  Created on: 6-gen-2012
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman
 */

/*
 * 1) File da comprimere non valido
 * 2) Parametro non corretto
 * 3) Impossibile aprire un file inscrittura per la decodifica
 * 4) Creazione dell'albero di Huffman fallita
 *
 * 6) Apertura del file di lettura per MtF fallita
 * 8) Apertura del file di scrittura per MtF fallita
 */

#include <stdlib.h>
#include <stdio.h>

#include "lib/config.h"

#include "compressore.h"
#include "decompressore.h"

#include "mtf/mtf.h"

#include "bwt/bwt_code.h"
#include "bwt/bwt_decode.h"

int main (int argc, char *argv[])
{
	char *input;
	char *output;
	char *file_mtf = "temp.mtf";
	char *file_bwt = "temp.bwt";

	if (argc < 3)
	{
		if (argc == 2 && argv[1][1] == 'm')
		{
			printf("Manuale di utilizzo\n\n");
			printf("\t./compressore -parametro input output\n\n");
			printf("parametro:  -c per comprimere\n");
			printf("            -d per decomprimere\n");
			printf("            -m per mostrare questo manuale\n");
			printf("input:      nome del file da utilizzare\n");
			printf("output:     nome del file dopo l'utilizzo\n\n");
			printf("grandezza massima: 4GB (4 294 967 296 byte)\n");
		}
		else
		{
			printf("Parametro non corretto\n");
			exit(2);
		}
	}
	else
	{
		if (argc < 4)
		{
			printf("compressore -parametro input output\n");
			exit (1);
		}

		input = argv[2];
		output = argv[3];

		switch (argv[1][1])
		{
		case 'c' :												// Compressione
			printf("Compressione in corso...");
			fflush(stdout);
			Burrow_Wheeler_Transform_Code(input,file_bwt); 		// Burrow Wheeler Transform
			printf ("... "); fflush(stdout);
			MoveToFrontCode(file_bwt,file_mtf);					// Move To Front
			printf ("... "); fflush(stdout);
			Canonical_Huffman_Code(file_mtf,output);			// Huffman Canonico
			printf(" fatto!\n");
			break;
		case 'd' :												// Decompressione
			printf ("Decompressione in corso...");
			fflush(stdout);
			Canonical_Huffman_Decode(input,file_mtf);			// Huffman Canonico
			printf ("... "); fflush(stdout);
			MoveToFrontDecode(file_mtf,file_bwt);				// Move To Front
			printf ("... "); fflush(stdout);
			Burrow_Wheeler_Transform_Decode(file_bwt,output); 	// Burrow Wheeler Transform
			printf(" fatto!\n");
			break;
		default :
			printf("Parametro non corretto\n");
			exit(2);
		}
		// pulizia dei file temporanei
		remove(file_mtf);
		remove(file_bwt);
	}
	return 0;
}
