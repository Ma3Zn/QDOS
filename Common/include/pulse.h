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

#ifndef PULSE_H
#define PULSE_H

#include "common.h"
#include "physical_system.h"
#include "rotation.h"

/*
 *  Header file che conterrà tutte le strutture dati che rappresentano un
 *  impulso e che permetteranno l'effettiva simulazione delle rotazioni in
 *  cui l'unitaria speciale da simulare è stata decomposta
 */

/*
 *  Semi-larghezza della gaussiana considerata per la moduluzaione di un
 *  impulso
 */
#define SEMI_AMPIEZZA   3.0
#define AMPIEZZA        (2.0 * SEMI_AMPIEZZA)

//---------------------------------------------------------------------------//

/*
 *  Struttura dati rappresentante un impulso. Ogni impulso ha una parte comune
 *  a tutti gli impulsi, ovvero la sua "parte operatoriale", i.e., la matrice
 *  di connettività. Dunque un puntatore a questa viene omesso dalla struttura
 *  dati.
 */
typedef struct
{
    uint64_t livelli[2];    /* Stati tra cui effettuare la transizione */
    double theta;           /* Angolo di rotazione dell'impulso */
    double gamma;           /* Fase dell'impulso */
    double omega;           /* Frequenza risonante dell'impulso */
    double T;               /* Durata dell'impulso */
/*
 *  Funzione che ci permette di calcolare l'ampiezza dell'impulso all'istante
 *  temporale t passato come parametro
 */
    gsl_complex (*modulazione) (const double t, void *params);
} impulso;

//---------------------------------------------------------------------------//

/*
 *  Enumerazione che ci permette di definire lo stato in cui si trova un
 *  particolare impulso parllelo.
 * 
 *  TODO::  Mi pare altamente inutile --> da rimuovere in versioni successive
 */
typedef enum 
{
    NON_SIMULATO,
    SIMULAZIONE_IN_CORSO,
    SIMULATO
} stato_sim_impulso;

/*
 *  Struttura dati che ci permette di raggruppare in modo semplice impulsi da
 *  eseguire in parallelo. Utilizzata principalmente nella decomposizione
 *  in impulsi delle rotazioni piane e rotazioni Z.
 */
typedef struct
{
    stato_sim_impulso stato;    /* Stato simulazione impulso parallelo */
    impulso **arr_impulsi;      /* Sequenza di puntatori a impulsi */
    size_t dim;               /* Dimensione della sequenza */
} impulsi_paralleli;

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che contiene una sequenza di impulsi da eseguire.
 *  Gli impulsi paralleli contenuti all'interno di una stessa sequenza potranno
 *  essere a loro volta parallelizzati dal simulatore. Mentre due sequenze
 *  NON potranno essere parallelizzate dal simulatore.
 */
typedef struct 
{
/*
 *  Sequenza di puntatori a impulsi_paralleli
 */
    impulsi_paralleli **arr_impulsi_paralleli;
/*
 *  Dimensione della sequenza
 */
    size_t dim;
} sequenza_impulsi;

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che contiene ciò che il simulatore dovrà simulare.
 */
typedef struct schedule
{
/*
 *  Sequenza di puntatori a sequenze_impulsi
 */
    sequenza_impulsi **arr_sequenze;
/*
 *  Dimensione della sequenza
 */
    size_t dim;   
} schedule;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di impulsi
 */

//---------------------------------------------------------------------------//

/*
 *  Allocazione di un impulso
 */
extern
impulso *alloc_impulso();

/*
 *  Deallocazione di un impulso
 */
extern
void free_impulso(impulso *imp);

//---------------------------------------------------------------------------//

/*
 *  Allocazione di impulsi paralleli
 */
extern
impulsi_paralleli *alloc_impulsi_paralleli(size_t dim);

/*
 *  Deallocazione di impulsi paralleli
 */
extern
void free_impulsi_paralleli(impulsi_paralleli *imp_par);

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una sequenza di impulsi
 */
extern
sequenza_impulsi *alloc_sequenza_impulsi(size_t dim);

/*
 *  Deallocazione di una sequenza di impulsi
 */
extern
void free_sequenza_impulsi(sequenza_impulsi *seq);

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una schedule
 */
extern
schedule *alloc_schedule(size_t dim);

/*
 *  Riallocazione di una schedule
 */
extern
uint8_t realloc_schedule(schedule *sch, size_t dim);

/*
 *  Deallocazione schedule
 */
extern
void free_schedule(schedule *sch);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato l'angolo di rotazione di un impulso, la sua funzione
 *  di modulazione e la matrice di connettività, ne calcola la durata.
 * 
 *  Ritona 0 se tutto è andato a buon fine, un numero positivo se vi sono
 *  stati degli errori.
 */
extern
uint8_t imposta_durata_impulso(gsl_matrix_complex *M, impulso *V);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un impulso V ne calcola la modulazione all'istante t
 *  data da
 *
 *                              A = cos(omega * t)
 * 
 *  ATTENZIONE::    A differenza di quando abbiamo testato gli algoritmi di
 *                  simulazione il campo magnetico esterno o qualsiasi cosa
 *                  ne faccia le veci deve essere già compreso all'interno
 *                  della matrice di connettività fornita
 */
extern
gsl_complex modulazione_impulso_hs(const double t, void *params);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un impulso V ne calcola la modulazione all'istante t
 *  data da
 *
 *                  A = cos(omega * t) * exp(-{(t-T)/sigma}^2)
 *  
 *  con
 *                          T := SEMI_AMPIEZZA * sigma 
 *
 *  dove sigma è la deviazione standard della gaussiana.
 * 
 *  ATTENZIONE::    A differenza di quando abbiamo testato gli algoritmi di
 *                  simulazione il campo magnetico esterno o qualsiasi cosa
 *                  ne faccia le veci deve essere già compreso all'interno
 *                  della matrice di connettività fornita
 */
extern
gsl_complex modulazione_impulso_gauss(const double t, void *params);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la realizzazione di un impulso nullo necessario
 *  all'implementazione del gate built-in $$IDLE
 */
extern
gsl_complex modulazione_impulso_zero(const double t, void *params);

//---------------------------------------------------------------------------//

/*
 *  Funzione che inizializza un impulso tramite i parametri passati. 
 */
extern
impulso *init_impulso(sistema_composto *sys,
                        size_t lv_0, size_t lv_1,
                        double theta, double gamma,
                        gsl_complex (*modulazione) (const double t,
                                                    void *params));

//---------------------------------------------------------------------------//

/*
 *  Funzione che alloca un opportuno impulso di collegamento una volta dati
 *  i due livelli tra cui eseguire l'impulso. Questa funzione NON verifica
 *  la collegabilità tra i due livelli, il chiamante deve occuparsene.
 *  In caso di errore un puntatore nullo verrà restituito.
 * 
 *  ATTENZIONE::    Gli impulsi di collegamento onde evitare degli sfasamenti
 *                  relativi indesiderati venno eseguiti in un determinato
 *                  "ordine". In particolare:
 *                      -   Ogni impulso "a scendere" deve essere un "3*pi",
 *                          i.e., un pi sfasato di pi. Così facendo lo
 *                          sfasamento è scaricato sullo stato ad indice
 *                          minore (dopo lo scambio) e non viene applicato
 *                          allo stato su cui poi applicheremo la rotazione
 *                          desiderata
 *                      -   Ogni impulso "a salire" deve essere un "pi". In tal
 *                          modo lo sfasamento viene scaricato sullo stato ad
 *                          maggiore (dopo lo scambio)
 * 
 *  lv_0 --> lo stato che ci interessa spostare
 *  lv_1 --> lo stato IN CUI vogliamo spostare lo stato in lv_0
 * 
 *  ATTENZIONE::    L'ordine tra lv_0 ed lv_1 è fondamentale!!!
 *
 *  HP::    Stiamo supponendo una matrice di connessione del tipo Sy
 */
extern
impulso *crea_impulso_collegamento(sistema_composto *sys,
                                    uint64_t *livelli,
                                    size_t lv_0,
                                    size_t lv_1,
                                    gsl_complex (*modulazione)
                                    (const double t, void *params),
                                    uint8_t *trasposizione);

//---------------------------------------------------------------------------//

/*
 *  Funzione che determina il corretto angolo dell'impulso che applica una
 *  rotazione Z
 *
 *  HP::    Stiamo supponendo una matrice di connessione del tipo Sy
 */
extern
double determina_angolo_rotazione_Z(sistema_composto *sys,
                                    rotazione_Z *rot,
                                    size_t lv_0,
                                    size_t lv_1,
                                    uint8_t inversione);

//---------------------------------------------------------------------------//

/*
 *  Funzione che determina la coretta fase dell'impulso che applica
 *  l'effettiva rotazione piana
 * 
 *  HP::    Stiamo supponendo una matrice di connessione del tipo Sy
 */
extern
double determina_fase_impulso(sistema_composto *sys,
                                rotazione_piana *rot,
                                size_t lv_0,
                                size_t lv_1,
                                uint8_t trasposizione);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla la possibile risonanza di due impulsi. Ritorna 0
 *  se i due impulsi non sono in stretta risonanza, 1 altrimenti. Non protegge
 *  in alcun modo da piccoli leakage
 */
extern
uint8_t controlla_risonanza_impulsi(impulso *imp_1, impulso *imp_2);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una schedule ed un sistema composto corregge la
 *  durata degli impulsi in base ai parametri del sistema stesso
 */
extern
uint8_t correggi_impulsi(sistema_composto *sys, schedule *sch);

//---------------------------------------------------------------------------//

/*
 *  Funzione che conta il numero di impulsi di una schedule
 */
extern
size_t conta_impulsi_schedule(schedule *sch);

#endif