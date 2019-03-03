/*
 * BWT.c
 *
 *  Created on: 04/gen/2012
 *      Author: Nathan Koefer
 */


#include <stdio.h>
#include <stdlib.h>

#include "config.h"

int debug = 0;

char *file1;
char *file2;

void scrivi(
		unsigned char *buffer,
		unsigned char *puntatori[],
		int dimensione
)
{
	FILE *perHuff = fopen(file2, "ab");
	unsigned int flag=0;


	// questo ciclo si occupa di ciclare l'array che costituisce la matrice virtuale di stringhe
	// e scrive su file il carattere precedente a quello puntato, ricostruendo in pratica l'ultima
	// colonna della matrice ordinata
	for(unsigned int i = 0; i < dimensione; i++)
	{
		// questo controllo e' necessario perchŽ nel caso che il carattere puntato sia il primo della stringa
		// originale, tornando indietro non si otterrebbe il carattere previsto bensi' un errore.
		// Inoltre, e'anche l'opportunita'per ottenere la posizione della stringa originale all'interno della matrice ordinata.

		if(puntatori[i] == buffer) 		// confronto il carattere puntuato attuale con quello dell'inizio del buffero originale
		{
			fwrite((buffer+dimensione-1),sizeof(unsigned char),1,perHuff);
			flag = i;
		}
		else
		{
			fwrite((puntatori[i]-1),sizeof(unsigned char),1,perHuff);
		}
	}

	// l'ultimo carattere scritto su file (per ogni blocco di caratteri processato) e'la
	// posizione della stringa originale all'interno della matrice ordinata.
	fwrite(&flag, sizeof(int), 1, perHuff);
	fclose(perHuff);
}

/*
 * comparaStringhe
 * funzione che ricevendo i puntatori a due caratteri del buffer
 * e' in grado di ricostruire le intere stringhe e compararle fra loro,
 * restituendo un numero positivo in caso la prima sia > della seconda.
 */
int comparaStringhe(
		unsigned char *buffer,
		unsigned char *a,
		unsigned char *b,
		unsigned int dimensione
)
{

	// per determinare la posizione del offset sottraggo la posizione di memoria iniziale
	// del buffer originale a quella dei puntatori all'inizio delle stringhe da comparare
	unsigned int offsetA = a-buffer;
	unsigned int offsetB = b-buffer;
	int res = 0;

	for(unsigned int i = 0; i < dimensione; i++)
	{
		// i due controlli qui sotto sono necessari a causa della struttura utilizzata per generare
		// la matrice virtuale di stringhe shiftate. In caso gli offset raggiungano la dimensone
		// della stringa originale e'necessario riportarli a 0 in modo che riprendano dall'inizio
		// del buffer, andando "a capo"
		if (offsetA == (dimensione))
			offsetA=0;
		if (offsetB == (dimensione))
			offsetB=0;
		if(*(buffer+offsetA) != *(buffer+offsetB))
		{
			// il valore ritornato in caso le stringhe presentino una differenza,
			// e' la sottrazione fra i caratteri disuguali. un valore positivo indica che
			// la stringa maggiore e'la prima, uno negativo indica che lo e'la seconda.
			return((int)(*(buffer+offsetA))-(*(buffer+offsetB)));
		}
		offsetA++;
		offsetB++;
	}
	// se non sono state trovate differenze fra le stringhe, viene ritornato il valore 0.
	return res;
}


/*
 * ordina
 * questa funzione si occupa di ordinare un array di puntatori
 * agli elementi del buffer tramite il bubble sort.
 * Sfrutta la funzione comparaStringhe.
 */
void ordina(
		unsigned char *buffer,
		unsigned char *ptr[],
		unsigned int dimensione
)
{
	unsigned char *temp;
	// e'necessario utilizzare una variabile n che viene decrementata alla fine di ogni ciclo interno.
	// Ad ogni ciclo infatti abbiamo la certezza che l'ultima stringa sia posizionata correttamente.
	unsigned int n = dimensione-1;
	for (unsigned int i = 0; i < dimensione; i++)
	{
		for (long int j = 0; j < n; j++)
		{
			if(comparaStringhe(buffer, ptr[j], ptr[j+1], dimensione) >= 0)
			{
				temp = ptr[j];
				ptr[j] = ptr[j+1];
				ptr[j+1]=temp;
			}
		}
		n--;
	}
}

/*
 * estraiUltimo
 * questa funzione richiama il sorting ed estrae l'ultimo carattere
 * per ogni riga della tabella virtuale, scrivendo poi i caratteri su file
 */
void estraiUltimo(
		unsigned char *buffer,
		int dimensione
)
{
	unsigned char * puntatori[dimensione]; 				// creo l'array di puntatori e lo
	for(unsigned int i = 0; i < (dimensione); i++)		// inizializzo con gli elementi dell'array
	{
		puntatori[i]=(buffer+i);
	}
	ordina(buffer, puntatori, dimensione);
	scrivi(buffer, puntatori, dimensione);
}

/*
 * bwt
 * Funzione che si occupa di leggere tutto il file e caricarlo in un buffer.
 * Passa in seguito il buffer alle altre funzioni che procendono a ordinarlo
 * ed estrarre la stringa "ordinata".
 */
void Burrow_Wheeler_Transform_Code(
		char fileIn[255],
		char fileOut[255]
)
{
	file1 = fileIn;
	file2 = fileOut;

	FILE *daLeggere;
	FILE *daScrivere;

	int dimensione = buffer_size;					// variabile che determina la dimensione del buffer di lettura
	unsigned char *buffer;							// buffer in cui verra' caricato il file
	size_t risultato;								// numero di byte effettivamente letti tramite fread

	// il file da leggere viene aperto in lettura,
	// se il file non esiste viene segnaato l'errore e il programma esce con status 1
	daLeggere = fopen(file1,"rb");
	if (daLeggere==NULL)
	{
		fputs("Errore di lettura del file",stderr);
		exit(1);
	}

	daScrivere=fopen(file2, "wb");
	fclose(daScrivere);

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
		fputs("Impossibile allocare la memoria",stderr);
		exit(2);
	}

	// viene letto e caricato il file nel buffer,
	// se l'operazione non va a buon fine (risultato != dimensione) viene segnalato l'errore e il programma
	// esce con status 2


	do
	{
		risultato = fread (buffer,1,dimensione,daLeggere);
		// chiamata alla funzione che si occupa di estrarre la stringa "ordinata"
		estraiUltimo(buffer, risultato);
	}
	while (risultato == dimensione);

	fclose (daLeggere);
	free (buffer);
}
