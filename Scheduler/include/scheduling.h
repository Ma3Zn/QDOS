/*
 *  QDOS -- simulator of generic QuDits OperationS.
 *  Copyright (C) 2022  Matteo Mezzadri
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 *  You can find a verbatim copy of the GPL v3 inside the file 'GPL v3'.
 */

#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "../../Common/include/common.h"
#include "../../Common/include/lib.h"

/*
 *  Header file che contiene la dichiarazione di tutte le funzioni necessarie
 *  per effettuare lo scheduling degli impulsi necessari alla simulazione di
 *  una sequenza di rotazioni.
 * 
 *  HP::    Fondamentale tenere a mente che per eseguire una selezione delle
 *          fasi per gli impulsi in modo corretto noi stiamo supponendo che
 *          tutti i livelli energetici del sistema corrispondano con
 *          l'ordinamento degli autovalori di Sz
 * 
 *  TODO::  Rimuovere l'HP sopra
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che verrà utilizzata all'interno dell'algoritmo di
 *  traduzione di una rotazione generica in una sequenza di impulsi.
 *  Questa struttura conterrà il cammino da compiere tra i livelli del
 *  sistema quantistico per poter effettivamente eseguire la rotazione
 *  tra due livelli altrimenti inraggiungibili.
 */
typedef struct 
{
/*
 *  Vettore che conterrà i livelli che collegano il livello di partenza a
 *  quello finale
 */
    size_t *livelli;
/*
 *  Vettore di supporto che ci dice quali indici fanno già parte del cammino.
 *  Viene utilizzato per evitare di perdersi in cicli.
 */
    uint8_t *idx_usati;
/*
 *  Numero di livelli nel cammino, la distanza tra i due livelli è data da
 *  dim - 1. In generale dim <= size(*livelli)
 */
    size_t dim_eff;
/*
 *  Dimensione del vettore livelli
 */
    size_t dim;
} cammino;


//---------------------------------------------------------------------------//

/*
 *  Enumerazione che verra' utilizzata nell'algoritmo di parallelizzazione
 *  per tenere traccia di quali sequenze di impulsi poter parallelizzare
 */
typedef enum
{
    LIBERO,
    BLOCCATO
} stato_liv_par;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di un cammino
 */

//---------------------------------------------------------------------------//

/*
 *  Allocazione di un cammino
 */
extern
cammino *alloc_cammino(size_t N);

//---------------------------------------------------------------------------//

/*
 *  Deallocazione di un cammino
 */
extern
void free_cammino(cammino *path);

//---------------------------------------------------------------------------//

/*
 *  Funzione ricorsiva che ad ogni chiamata aggiunge un livello al camino.
 *  La direzione di ricerca parte dall'indice attuale e scorre verso destra
 *  presupponendo che livelli[0] < livelli[1]. L'algoritmo NON ha pretese di
 *  ottimalità ma bensì di terminazione, i.e., se c'è trova sempre una
 *  soluzione.
 */
extern
uint8_t cerca_cammino_ricorsivo(cammino *path,
                                sistema_composto *sys,
                                size_t livelli[2]);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un sistema composto e due livelli del sistema stesso
 *  cerca un cammino che connette i due suddetti livelli attraverso la
 *  matrice di connessione fornita.
 */
extern
cammino *cerca_cammino(sistema_composto *sys, size_t livelli[2]);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inizializzazione di un array di "stato_liv_par"
 */
extern
void init_arr_stato_liv(stato_liv_par *arr, size_t dim);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'aggiornamento dello stato dei livelli del sistema
 */
extern
void aggiorna_stato_liv(sequenza_impulsi *seq,
                        stato_liv_par *arr,
                        stato_liv_par stato);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se sono attualmente ancora disponibili livelli
 *  per l'esecuzione di impulsi paralleli
 */
extern
uint8_t controlla_disp_liv(stato_liv_par *arr, size_t dim);

//---------------------------------------------------------------------------//

/*
 *  Funzione che utilizzeremo per la traduzione di una rotazione Z in una
 *  sequenza di impulsi. Prende in input la rotazione da tradurre ed il sistema
 *  fisico studiato e restituisce l'opportuna sequenza di impulsi.
 * 
 *  HP: La matrice di connettività è "a scala"
 * 
 *  TODO::  Rimuovere l'HP sopra richiedendo smplicemente in INPUT una "path"
 *          che collega tutti i livelli, dovrebbe essere semplice
 *          implementare tutte le traduzioni una volta che si ha a disposizione
 *          questa path [IMPORTANTE!!!!]
 */
extern
sequenza_impulsi *traduci_rotazione_Z(rotazione_Z *rot, sistema_composto *sys);

//---------------------------------------------------------------------------//

/*
 *  Funzione che utilizzeremo per la traduzione di una rotazione piana in una
 *  sequenza di impulsi. Prende in input la rotazione da tradurre ed il sistema
 *  fisico studiato e restituisce l'opportuna sequenza di impulsi.
 * 
 *  HP: La matrice di connettività è "a scala"
 * 
 *  TODO::  Rimuovere l'HP sopra richiedendo smplicemente in INPUT una "path"
 *          che collega tutti i livelli, dovrebbe essere semplice
 *          implementare tutte le traduzioni una volta che si ha a disposizione
 *          questa path [IMPORTANTE!!!!]
 *
 *  TODO::  Ci devono essere sicuramente modi più puliti, coincisi e semplici
 *          per l'implementazione di queste due funzioni. 400 righe di codice
 *          che continua a ripetersi non sono la soluzione. Soprattutto
 *          quando poi decideremo di rimuovere l'HP della matrice di
 *          connettivita' "a scala". In quell'occasione suggerisco di
 *          rivisitare a fondo la struttura e la logica di quetse due
 *          funzioni
 */
extern
sequenza_impulsi *traduci_rotazione_piana(rotazione_piana *rot, 
                                            sistema_composto *sys);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla, dato lo stato dei livelli del sistema, se la
 *  sequenza passata è parallelizzabile
 */
extern
uint8_t controlla_se_par(sequenza_impulsi *seq,
                            stato_liv_par *arr_stato);

//---------------------------------------------------------------------------//

/*
 *  Funzione che parallelizza le due sequenze passate. Sostanzialmente fa
 *  un merge degli impulsi paralleli corrispondenti. Ritorna il puntatore
 *  alla nuova sequenza. E libera la memoria occupata dalle sequenze passate
 */
extern
sequenza_impulsi *unione_sequenze_impulsi(sequenza_impulsi **seq, size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione che fornita una sequenza di rotazioni e la matrice di
 *  connettività, che ricordo stiamo IPOTIZZANDO "a scala", genera una
 *  schedule di impulsi necessari per la simulazione di tutte le rotazioni
 *  presenti. La sequenza di rotazioni è sempre supposta avere questo ordine:
 *      1.) Rotazioni Z
 *      2.) Rotazioni piane nell'ordine inverso da quello in cui compaiono
 *          nella sequenza
 *  
 *  Questa convenzione è adottata in quanto è proprio ciò che si verifica
 *  per le rotazioni ottenute tramite l'algoritmo di decomposizione di una
 *  unitaria
 */
extern
schedule *crea_schedule(sequenza_rotazioni *seq_rot, sistema_composto *sys);

//---------------------------------------------------------------------------//

/*
 *  Funzione che fa un append della schedule sch_app alla schedule sch_base.
 *
 *  La schedule sch_app viene distrutta mentre viene restituito un puntatore
 *  alla schedule finale.
 */
extern 
schedule *unione_schedule(schedule *sch_base, schedule *sch_app);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una schedule sch tenta di parallelizzare l'esecuzione
 *  delle sequenze di impulsi che compongono la schedule stessa.
 * 
 *  La funzione e' distruttiva, nel senso che il puntatore passato verra'
 *  "deallocato" e ne verra' restituito uno nuvo alla schedule parallela
 * 
 *  'dim_sys' è la dimensione del sistema fisico composto
 */
extern
schedule *parallelizza_schedule(schedule *sch, size_t dim_sys);

#endif