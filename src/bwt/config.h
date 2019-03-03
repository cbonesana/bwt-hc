/*
 * config.h
 *
 *  Created on: 18-gen-2012
 *      Author: dna
 */

#ifndef CONFIG_H_
#define CONFIG_H_


// definizione della dimensione del buffer di lettura e su cui verrˆ effettuato l'algoritmo.
// Al crescere della dimensione BWT tende a restituire risultati migliori (in media piu'caratteri
// identici in sequenza) ma la lentezza di esecuzione cresce con ragione n*log(n).
// E'necessario trovare la migliore alchimia fra velocitˆ e prestazioni.

#define buffer_size 1000


#endif /* CONFIG_H_ */
