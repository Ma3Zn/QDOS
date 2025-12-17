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

#ifndef MEASURE_H
#define MEASURE_H

#include "../../Common/include/common.h"
#include "simulation.h"

/*
 *  Header file contenente tutte le dichiarazioni dei tipi di dato e delle
 *  funzioni necessarie all'esecuzione di misure del quantum computer simulato
 */

/*
 *  Macro che definisce il numero di default di misure da tenere in memoria
 */
#define MEM_MISURE          10
/*
 *  Macro che definisce il valore di un qudit non misurato durante
 *  una misura
 */
#define QUDIT_NON_MISURATO -1

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che conterrà gli esiti di una misura
 */
typedef struct misura
{
/*
 *  Array che conterrà gli esiti della misura. Un valore negativo
 *  corrisponde ad un qudit NON misurato
 */
    int *value;
/*
 *  Numero di qudit del sistema e dimensione dell'array dei valori osservati
 */
    int dim;
/*
 *  Puntatore alla misura successiva in memoria
 */
    struct misura *next;
/*
 *  Puntatore alla misura precedente in memoria
 */
    struct misura *prev;
} misura;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione per la struttura dati
 *  'misura'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'allocazione di una misura
 */
extern
misura *alloc_misura(size_t dim);

/*
 *  Interfaccia per la deallocazione di una misura
 */
extern
void free_misura(misura **M);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che terrà traccia degli esiti delle ultime 'memoria_misure'
 *  misure
 */
typedef struct lista_misure
{
/*
 *  Puntatore alla testa della lista, questa sarà sempre la misura più
 *  recentemente eseguita
 */
    misura *testa;
/*
 *  Puntatore alla testa della lista, questa sarà sempre la misura meno
 *  recentemente eseguita
 */
    misura *coda;
/*
 *  Dimensione massima della lista
 */
    size_t dim_max;
/*
 *  Dimensione effettiva della lista
 */
    size_t dim_eff;
} lista_misure;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per allocazione, deallocazione e manipolazione della struttura
 *  dati 'lista_misura'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'allocazione di una struttura dati 'lista_misure'.
 *
 *  Imposta la dimensione massima della lista pari all'argomento passato
 */
extern
lista_misure *alloc_lista_misure(size_t dim);

/*
 *  Interfaccia per la deallocazione di una struttura dati 'lista_misure'
 */
extern
void free_lista_misure(lista_misure **lst_mis);

/*
 *  Interfaccia per recuperare la misura i-esima data una lista di misure.
 *  L'ordinamento è decrescente in senso temporale, i.e., la misura 0
 *  corrisponde all'ultima misurazione effettuata
 */
extern
int *get_misura(lista_misure *lst_mis, size_t idx);

/*
 *  Interfaccia per l'inserimento di una misura all'interno della lista.
 *
 *  Esegue l'inserimento in testa di una misura a valori inizializzati
 *  al valore QUDIT_NON_MISURATO. Ritorna il vettore delle misura inserita
 *  per facilitare l'inserimento dei valori misurati
 */
extern
lista_misure *inserisci_misura(lista_misure *lst_mis, misura *mis);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che conterrà, per un dato qudit tutti i possibili
 *  proiettori corretti
 */
typedef struct proiettore
{
/*
 *  Vettore contenente puntatori alla rappresentazione matriciale dei vari
 *  proiettori
 */
    gsl_matrix_complex **P;
/*
 *  Dimensione del qudit, combacia con il numero di poiettori
 */
    size_t dim;
} proiettore;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'proiettore'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione della struttura dati 'proiettore'
 */
extern
proiettore *alloc_proiettore(size_t dim, size_t dim_tot);

/*
 *  Interfacce per la deallocazione della struttura dati 'proiettore'
 */
extern
void free_proiettore(proiettore **prj);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che conterrà i conteggi delle misurazioni finali con gli
 *  stati prodotto rispettivi
 */
typedef struct conteggio
{
/*
 *  Valore del conteggio
 */
    size_t valore;
/*
 *  Stato relativo al conteggio
 */
    char *stato;
} conteggio;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'conteggio'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione della struttura dati 'conteggio'
 */
extern
conteggio *alloc_conteggio();

/*
 *  Interfacce per la deallocazione della struttura dati 'conteggio'
 */
extern
void free_conteggio(conteggio **count);

//---------------------------------------------------------------------------//
/*
 *  Funzione che data una simulazione e l'indice di un qudit genera i
 *  proiettori opportuni per ogni valore osservabile da una misura del qudit
 */
extern
proiettore *costruisci_proiettore(simulazione *sim, size_t idx_qudit);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data la matrice di densità rho del sistema ed un proiettore
 *  del sistema calcola la probabilità di osservare l'autovalore associato al
 *  proiettore stesso
 */
extern
double calcola_probabilita(gsl_matrix_complex *rho,
                            proiettore *prj,
                            size_t idx);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una simulazione ed un proiettore esegue una scelta
 *  "randomica" dello stato in cui proiettare
 */
extern
int scegli_esito_misura(simulazione *sim, proiettore *prj);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato l'esito di una misura determina pseudo-casulamente se
 *  avviene o meno un'errore nella misura stessa
 */
extern
size_t scegli_proiettore(misura *mis, size_t idx_qudit);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data la matrice di denstità rho ed un proiettore Pj proietta
 *  lo stato rho sul sottospazio individuato da Pj
 */
extern
uint8_t proietta_stato(simulazione *sim, gsl_matrix_complex *prj);

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'esecuzione di una misura
 */
extern
simulazione *esegui_misura(simulazione *sim,
                            size_t *idx_qudit,
                            size_t n_qudit,
                            uint8_t output);

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiorna il conteggio delle misurazioni ottenute per un
 *  determinato stato in base 
 */
extern
void aggiorna_conteggio(conteggio **count,
                        size_t dim,
                        int *esito_misura,
                        size_t n_qudit);

//---------------------------------------------------------------------------//

/*
 *  Funzione che fornito lo stato attuale del sistema ed il numero di
 *  simulazioni da eseguire 'n_shots' eseguira 'n_shots' misure del sistema
 */
extern
conteggio **esegui_misura_sistema(simulazione *sim, size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione che scrive standard output la distribuzione finale simulata
 */
extern
void stampa_conteggio(struct conteggio **count, size_t dim);

#endif