/*
 * Ubwt.c
 *
 *  Created on: 15/gen/2012
 *      Author: nathan
 */


#include <stdio.h>
#include <stdlib.h>

#include "config.h"

void estrai(
		unsigned char *L,		// e'la stringa letta da file, priva del'intidicatore di posizione
		unsigned int posizione,	// contiene la posizione della stringa originale nella matrice ordinata
		unsigned int dimensione,// la dimensione della stringa letta da file
		FILE *fileOut
)
{
	unsigned int T[dimensione];			// l'array contenente le posizioni delle corrispondenze fra L e F
	unsigned int flags[dimensione];		// array di controllo per la creazione corretta di T
	unsigned char F[dimensione];		// conterra'la stringa T ordinata
	unsigned char risultato[dimensione];// conterra'la stringa decodificata (quella originale)
	unsigned int posT = posizione;		// variabile necessaria per la decodifica
	unsigned char temp;					// carattere usato per l'ordinamento di F
	unsigned int i = 0;					// i e j, utilizzate nei cicli for
	unsigned int j = 0;

	int n = dimensione -1;


	// questo ciclo riempie l'array F con i caratteri di L e inizializa a 0 tutti le posizioni di flags[]
	for (i = 0; i < dimensione; i++)
	{
		F[i]=L[i];
		flags[i]=0;
	}

	i = 0;
	j = 0;

	// questo ciclo odina F
	for(i=0; i < dimensione; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(F[j] > F[j+1])
			{
				temp = F[j];
				F[j] = F[j+1];
				F[j+1]=temp;
			}
		}
	}

	i = 0;
	j = 0;


	// questo ciclo riempie l'array di corrisondenze fra L e F
	for(int i = 0; i < dimensione; i++)
	{
		j = 0;

		// la corrispondenza viene salvata in T a patto che L[i] e F[j] siano uguali
		// e che F[j] non sia ancora stato utilizzato (flags[j] != 0)
		while(L[i] != F[j] || (L[i] == F[j] && flags[j]))
			j++;
		T[i] = j;
		flags[j]= 1;
	}

	// l'ultimo carattere della stringa originale  quello alla posizione "posizione" della stringa codificata
	risultato[dimensione-1]=*(L+posizione);

	i = 0;

	// tramite l'array T viene aggiornato il valore di posT.
	// L'array L contiene nella posizione posT il valore da scrivere in risultato, partendo dal fondo.
	for(int i = 1; i < dimensione; i++){
		posT = T[posT];
		risultato[dimensione-i-1]=*(L+posT);
	}

	fwrite(risultato, dimensione, sizeof(unsigned char), fileOut);
}





void Burrow_Wheeler_Transform_Decode(
		char fileIn[255],
		char fileOut[255]
)
{
	FILE * daLeggere;
	FILE * daScrivere;
	int dimensione = buffer_size;					// variabile che determina la dimensione del buffer di lettura
	unsigned char *buffer;							// buffer in cui verrï¿½ caricato il file
	size_t risultato;								// numero di byte effettivamente letti tramite fread

	unsigned int posizione = 0;

	// il file da leggere viene aperto in lettura,
	// se il file non esiste viene segnaato l'errore e il programma esce con status 1
	daLeggere = fopen (fileIn,"rb" );
	if (daLeggere==NULL)
	{
		fputs ("Errore di lettura del file",stderr);
		exit (1);
	}

	daScrivere = fopen(fileOut,"wb");
	fclose(daScrivere);
	daScrivere = fopen(fileOut,"ab");


	/*
	// ottengo la dimensione in byte del file da leggere
	fseek (daLeggere , 0 , SEEK_END);
	dimensione = ftell (daLeggere);
	rewind (daLeggere);
	 */

	// viene allocata la memoria per il buffer dove verrï¿½ caricato l'intero file,
	// se l'operazione non va a buon fine viene segnalato l'errore e il programma esce con status 2
	buffer = (unsigned char*) malloc (sizeof(unsigned char)*dimensione);
	if (buffer == NULL)
	{
		fputs ("Impossibile allocare la memoria",stderr);
		exit (2);
	}


	// viene letto e caricato il file nel buffer,
	// se l'operazione non va a buon fine (risultato != dimensione) viene segnalato l'errore e il programma
	// esce con status 2
	do
	{
		risultato = fread (buffer,1,dimensione,daLeggere);
		if((int)risultato == dimensione)
		{
			fread(&posizione, 1, (sizeof(unsigned int)), daLeggere);
		}
		else
		{
			risultato=risultato-4;
			fseek(daLeggere, -4, SEEK_END);
			fread(&posizione, 1, (sizeof(unsigned int)), daLeggere);
		}
		estrai(buffer, posizione, risultato, daScrivere);
		// chiamata alla funzione che si occupa di estrarre la stringa originale conoscendo
		// quella codificata e "posizione"

	}
	while (risultato == dimensione);

	fclose (daLeggere);
	fclose (daScrivere);
	free (buffer);
}


