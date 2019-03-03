
/*
 * ubwt.h
 *
 *  Created on: 15/gen/2012
 *      Author: nathan & dna
 */


#include <stdio.h>
#include <stdlib.h>

// Tipi di dato

typedef struct nodo Nodo;
struct nodo{
	unsigned char c;
	struct nodo *next;
};

// Funzioni

/*
 * Funzione che dato un carattere e una lista, ne scorre i nodi e ritorna la posizione occupata
 * dal carattere cercato. Viene usata nella codifica.
 */
unsigned char trovaPosizioneChar(
		Nodo* l,							// lista
		unsigned char carattere				// carattere cercato
)
{
	Nodo *presente = l;
	int i = 0;
	while(carattere != presente->c){
		presente = presente->next;
		i++;
	}
	return (unsigned char)i;
}

/*
 * Funzione che dato una posizione e una lista, ne scorre i nodi e ritorna il carattere occupato
 * dalla posizione data. Viene usata nella decodifica.
 */
unsigned char trovaIndiceChar(
		Nodo* l,							// lista
		unsigned char posizione				// carattere cercato
)
{
	Nodo *presente = l;
	int i = 0;

	while(i != posizione){
		presente = presente->next;
		i++;
	}
	return presente->c;
}

/*
 * Funzione per l'inserimento in coda di un nuovo carattere.
 */
Nodo *inserisciInCoda(
		Nodo *l,							// lista
		unsigned char i						// carattere da aggiungere
)
{
	Nodo *p;
	Nodo *new;

	new = (Nodo *)malloc(sizeof(Nodo));
	new->c = i;
	new->next = NULL;

	if (l == NULL)
		l = new;
	else
	{
		p = l;
		while(p->next != NULL)
			p = p->next;
		p->next = new;
	}
	return l;
}

/*
 * Funzione per la creazione completa di una lista di caratteri in ordine crescente.
 */
Nodo *creaLista()
{
	Nodo *l = NULL;
	int i   = 0;
	for(i = 0; i < 256; i++)
		l = inserisciInCoda(l, (unsigned char)i);
	return l;
}

/*
 * Funzione priincipale dell'algoritmo. Data una lista e un carattere, questa funzione sposta
 * il carattere in prima posizione in modo che le funzioni di ricerca funzionino nel modo
 * corretto.
 */
Nodo *spostaInTesta(
		Nodo *l,								// lista
		unsigned char carattere					// carattere da spostare
)
{
	Nodo *head   = l;
	Nodo *actual = l;
	Nodo *prec   = NULL;

	while (carattere != actual->c)
	{
		prec = actual;
		actual = actual->next;
	}

	if (actual->next == NULL)
		prec->next = NULL;
	else
		prec->next = actual->next;
	actual->next = head;
	head = actual;

	return head;
}

/*
 * Funzione principale di codifica mediante l'algoritmo Move to Front.
 * Dato un file di input e un file di output, questa funzione codifica l'input caricandolo
 * nell'output.
 */
void MoveToFrontCode(
		char *file_in,							// file in input
		char *file_out							// file in output
)
{
	FILE *f;
	FILE *n;

	unsigned char buffer_lettura;
	unsigned char buffer_scrittura;

	Nodo* lista = NULL;

	int file_size;

	lista = creaLista();

	f = fopen(file_in,"rb");
	n = fopen(file_out,"wb");

	if(f == NULL)
	{
		printf("File di lettura non disponibile.\n");
		exit(8);
	}
	if(n == NULL)
	{
		printf("File di scrittura non disponibile.\n");
		exit(6);
	}

	fseek(f,0,SEEK_END);
	file_size = ftell(f);	// ritorna la dimensione completa del file
	fseek(f,0,SEEK_SET);

	while(file_size > 0)
	{
		file_size--;
		fread((unsigned char*)&buffer_lettura,sizeof(unsigned char),1,f);
		buffer_scrittura = trovaPosizioneChar(lista,buffer_lettura);
		if (buffer_scrittura != 0)
			lista = spostaInTesta(lista,buffer_lettura);

		fwrite((unsigned char*)&buffer_scrittura,sizeof(unsigned char),1,n);
	}
	fclose(f);
	fclose(n);
}

/*
 * Funzione principale di decodifica mediante l'algoritmo Move to Front.
 * Dato un file di input e un file di output, questa funzione decodifica l'input caricandolo
 * nell'output.
 */
void MoveToFrontDecode(
		char *file_in,							// file in input
		char *file_out							// file in output
)
{
	FILE *f;
	FILE *n;

	unsigned char buffer_lettura;
	unsigned char buffer_scrittura;

	Nodo* lista = NULL;

	lista = creaLista();

	f = fopen(file_in,"rb");
	n = fopen(file_out,"wb");

	if(f == NULL)
	{
		printf("File di lettura non disponibile.\n");
		exit(6);
	}
	if(n == NULL)
	{
		printf("File di scrittura non disponibile.\n");
		exit(8);
	}

	fseek(f,0,SEEK_END);
	int file_size = ftell(f);	// ritorna la dimensione del file
	fseek(f,0,SEEK_SET);

	while(file_size > 0)
	{
		file_size--;
		fread((unsigned char*)&buffer_lettura,sizeof(unsigned char),1,f);
		buffer_scrittura = trovaIndiceChar(lista,buffer_lettura);
		if (buffer_lettura != 0)
			lista = spostaInTesta(lista,buffer_scrittura);
		fwrite((unsigned char*)&buffer_scrittura,sizeof(unsigned char),1,n);
	}
	fclose(f);
	fclose(n);
}
