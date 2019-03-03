/*
 * MergeSort.c
 *
 *  Created on: 4-nov-2011
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - algoritmi di sorting
 */

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "config.h"

/*
 * Funzione ausiliaria per il merge sort delle lunghezze.
 */
void merge_code(struct code *a[], int left, int center, int right)
{
	int i, j, k;
	struct code *b[256];

	i = left;
	j = center+1;
	k = 0;

	while ((i<=center) && (j<=right)) {
		if (a[i]->lunghezza < a[j]->lunghezza) {
			b[k] = a[i];
			i++;
		} else if (a[i]->lunghezza == a[j]->lunghezza){
			if (a[i]->carattere < a[j]->carattere){
				b[k] = a[i];
				i++;
			} else {
				b[k] = a[j];
				j++;
			}
		} else {
			b[k] = a[j];
			j++;
		}

		k++;
	}

	while (i<=center) {
		b[k] = a[i];
		i++;
		k++;
	}

	while (j<=right) {
		b[k] = a[j];
		j++;
		k++;
	}

	for (k=left; k<=right; k++)
		a[k] = b[k-left];
}

/*
 * Funzione principale per il merge sort dedicato alle lunghezze
 */
void mergesort_code(struct code *a[], int left, int right)
{
	int center;

	if(left<right) {
		center = (left+right)/2;
		mergesort_code(a, left, center);
		mergesort_code(a, center+1, right);
		merge_code(a, left, center, right);
	}
}

//-----------------------------------------------------------------------------------------------------------------

/*
 * Funzione ausiliaria per il merge sort delle frequenze.
 */
void merge_freq(struct nodo *a[], int left, int center, int right)
{
	int i, j, k;
	struct nodo *b[256];

	i = left;
	j = center+1;
	k = 0;


	while ((i<=center) && (j<=right)) {
		if (a[i]->frequenza <= a[j]->frequenza) {
			b[k] = a[i];
			i++;
		} else {
			b[k] = a[j];
			j++;
		}


		k++;
	}

	while (i<=center) {
		b[k] = a[i];
		i++;
		k++;
	}

	while (j<=right) {
		b[k] = a[j];
		j++;
		k++;
	}

	for (k=left; k<=right; k++){
		a[k] = b[k-left];
	}
}

/*
 * Funzione principale per il merge sort dedicato alle frequenze
 */
void mergesort_freq(struct nodo *a[], int left, int right)
{
	int center;

	if(left<right) {
		center = (left+right)/2;
		mergesort_freq(a, left, center);
		mergesort_freq(a, center+1, right);
		merge_freq(a, left, center, right);
	}
}

//-----------------------------------------------------------------------------------------------------------

/*
 * Funzione ausiliaria per il merge sort delle lunghezze.
 */
void merge_car(struct code *a[], int left, int center, int right)
{
	int i, j, k;
	struct code *b[256];

	i = left;
	j = center+1;
	k = 0;

	while ((i<=center) && (j<=right)) {
		if (a[i]->carattere < a[j]->carattere) {
			b[k] = a[i];
			i++;
		} else {
			b[k] = a[j];
			j++;
		}

		k++;
	}

	while (i<=center) {
		b[k] = a[i];
		i++;
		k++;
	}

	while (j<=right) {
		b[k] = a[j];
		j++;
		k++;
	}

	for (k=left; k<=right; k++)
		a[k] = b[k-left];
}

/*
 * Funzione principale per il merge sort dedicato alle lunghezze
 */
void mergesort_car(struct code *a[], int left, int right)
{
	int center;

	if(left<right) {
		center = (left+right)/2;
		mergesort_car(a, left, center);
		mergesort_car(a, center+1, right);
		merge_car(a, left, center, right);
	}
}
