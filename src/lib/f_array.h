/*
 * f_array.h
 *
 *  Created on: 3-gen-2012
 *      Author: Claudio "Dna" Bonesana
 *      Title:  Algoritmo di Huffman - funzioni sugli array
 */

#ifndef FUNCTIONS_ARRAY_H_
#define FUNCTIONS_ARRAY_H_


void orand(unsigned char *first,unsigned char *second,int size,int operation);

void copy(unsigned char *from, unsigned char *to,int size);

void shift_left(unsigned char *array,int size,int amount);
void shift_right(unsigned char *array,int size,int amount);

int compare(unsigned char *data1,unsigned char *data2,int length);

void incrementa(unsigned char *data,int size);

#endif /* FUNCTIONS_ARRAY_H_ */
