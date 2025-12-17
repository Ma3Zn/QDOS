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

#ifndef OUTPUT_H
#define OUTPUT_H

#include "common.h"
#include "rotation.h"
#include "physical_system.h"
#include "pulse.h"

/*
 *  File contenente le dichiarazioni di tutte le funzioni e strutture dati per
 *  la gestione dell'output su file necessario al nostro programma
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati contenente tutte le informazioni per l'output del simulatore
 */
typedef struct
{
/*
 *  Stream su file della soluzione FINALE approssimata dal simulatore.
 *  Fondamentale per poter concatenare l'esecuzione del programma.
 */
    FILE *f_out_rho_finale;
/*
 *  Stream su file della soluzione approssimata all'equazione differenziale
 *  ad ogni istante temporale specificato tramite il parametro N
 */
    FILE *f_out_rho;
/*
 *  Stream su file degli istanti temporali a cui viene stampata la matrice di
 *  densita'
 */
    FILE *f_out_t;
/*
 *  Stream su file delle schedule parallelizzate che verranno simulate
 */
    FILE *f_out_sch;
/*
 *  Stream su file della sequenza di rotazioni che verrà eseguita
 */
    FILE *f_out_rot;
/*
 *  Flag che ci dice se eseguire o meno la simulazione.
 *  0 --> simulazione disabilitata
 *  1 --> simulazione abilitata
 */
    uint8_t simulazione_abilitata;
/*
 *  Flag che ci dice se stampare o meno in un file separato la soluzione
 *  finale approssimata dal simulatore
 *  0 --> NON abilitato
 *  1 --> abilitato in formato binario
 *  2 --> abilitato in formato decimale
 */
    uint8_t output_soluzione_finale_abilitato;
/*
 *  Flag che ci dice se eseguire o meno lo stream di output della soluzione
 *  e dei tempi
 *  0 --> NON abilitato
 *  1 --> abilitato in formato binario
 *  2 --> abilitato in formato decimale
 */
    uint8_t output_simulazione_abilitato;
/*
 *  Numero di istanti in cui stampare la soluzione durante la simulazione di
 *  un impulso
 */
    size_t N;
/*
 *  Flag che ci dice se stampare o meno lo stato della simulazione
 *  0 --> NON abilitato
 *  1 --> stampa solo avanzamento dei gate
 *  2 --> stampa anche avanzamento degli impulsi
 */
    uint8_t avanzamento_simulazione;
/*
 *  Flag che ci dice se stampare o meno la schedule eseguita dal simulatore.
 *  0 --> NON abilitato
 *  1 --> abilitato in formato binario
 *  2 --> abilitato in formato decimale
 */
    uint8_t output_schedule_abilitato;
/*
 *  Flag che ci dice se stampare o meno la sequenza di rotazioni eseguita dal 
 *  simulatore.
 *  0 --> NON abilitato
 *  1 --> abilitato in formato binario
 *  2 --> abilitato in formato decimale
 */
    uint8_t output_rotazioni_abilitato;
}output;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  di output
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione della struttura di output per il simulatore
 */
extern
output *alloc_output();

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione della struttura di output per il simulatore
 */
extern
void free_output(output *out);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la matrice [M]
 */
extern
void stampa_matrice(FILE *F, const gsl_matrix_complex *M);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la matrice [M] in formato binario
 */
extern
void stampa_matrice_bin(FILE *F, const gsl_matrix_complex *M);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in quale formato stampare l'evoluzione della
 *  simulazione
 */
extern
void controllo_stampa_output_simulazione(output *out,
                                            const double t,
                                            const gsl_matrix_complex *M);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in quale formato stampare la soluzione finale
 *  approssimata dal simulatore
 */
extern
void controllo_stampa_soluzione_finale(output *out,
                                        const gsl_matrix_complex *M);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la rotazione Z [rot]
 */
extern
void stampa_rotazione_Z(FILE *F, const rotazione_Z *rot);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la rotazione piana [rot]
 */
extern
void stampa_rotazione_piana(FILE *F, const rotazione_piana *rot);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la seqeunza di rotazioni [seq]
 */
extern
void stampa_sequenza_rotazioni(FILE *F, const sequenza_rotazioni *seq);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la seqeunza di rotazioni [seq] in
 *  formato binario direttamente leggibile dal simulatore
 */
extern
uint8_t stampa_sequenza_rotazioni_bin(FILE *F,
                                        const sequenza_rotazioni *seq,
                                        const size_t indici[2]);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in che formato stampare la seqeunza di
 *  rotazioni [seq]
 */
extern
void controllo_output_rotazioni(output *out,
                                const sequenza_rotazioni *seq,
                                const size_t indici[2]);

//---------------------------------------------------------------------------//

/*
 *  Funzione che legge dal file [F] una seqeunza di rotazioni, la lettura
 *  degli indici di connessione spetta al chiamamte che così ha modo di
 *  sapere quante schedule differenti sono presenti sul file
 */
extern
sequenza_rotazioni *leggi_sequenza_rotazioni(FILE *F);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] l'impulso [imp]
 */
extern
void stampa_impulso(FILE *F, const impulso *imp);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] l'impulso parallelo [imp]
 */
extern
void stampa_impulsi_paralleli(FILE *F, const impulsi_paralleli *imp);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la sequenza di impulsi [seq]
 */
extern
void stampa_sequenza_impulsi(FILE *F, const sequenza_impulsi *seq);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la schedule [sch]
 */
extern
void stampa_schedule(FILE *F, const schedule *sch);

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la schedule [sch] in formato binario
 */
extern
uint8_t stampa_schedule_bin(FILE *F, const schedule *sch);

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in che formato stampare la schedule [sch].
 */
extern
void controllo_output_schedule(output *out,
                                const schedule *sch);

//---------------------------------------------------------------------------//

/*
 *  Funzione che legge dal file [F] una schedule, la lettura degli indici
 *  di connessione spetta al chiamamte che così ha modo di sapere quante
 *  schedule differenti sono presenti sul file
 */
extern
schedule *leggi_schedule(FILE *F, gsl_matrix_complex *M, gsl_vector *aut);

//---------------------------------------------------------------------------//

/*
 *  Funzione che inizializza la variabile di output passata con i valori
 *  impostati dal parser delle opzioni
 */
extern
uint8_t init_output(output *out);

#endif