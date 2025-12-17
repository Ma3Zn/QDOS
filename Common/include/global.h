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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "../../Parser/include/parser_support.h"
#include "../../Simulator/include/measure.h"
#include "gate.h"

/*
 *  Definizioni deel valore del flag globale che ci dice se il meccanismo di
 *  connessione tra qudit è abilitato o meno
 */
#define CONNESSIONE_DISABILITATA    0u
#define CONNESSIONE_ABILITATA       1u

/*
 *  Header file contenente le dichiarazioni di tutte le variabili globali
 *  necessarie al parsing e al set-up del simulatore
 */

//---------------------------------------------------------------------------//

/*
 *  Dichiarazione del flag che ci dice se il meccanismo di connessione tra
 *  qudit è abilitato o meno
 */
extern uint8_t flag_connessione;

/*
 *  Dichiarazione di una sequenza di numeri per per una 'integer_sequence'
 *  Abbiamo bisogno di due sequenze distinte in quanto ci ritroveremo a fare
 *  il parsing di più sequenze contemporaneamente prima di poterle installare
 *  nelle tabelle
 */
extern sequenza_numeri *int_seq;

/*
 *  Dichiarazione di una sequenza di numeri per una 'float_sequence'
 */
extern sequenza_numeri *float_seq;

/*
 *  Dichiarazione della lista delle definizioni
 */
extern lista_definizioni_gate *lst_def;

/*
 *  Dichiarazione di un'istruzione di supporto che conterrà l'istruzione 
 *  osservata ma non ancora inserita nella lista di gate che forma il corpo
 *  di un blocco di gate
 */
extern istruzione *ist_supporto;

/*
 *  Lista di gate d'appoggio che verràutilizzata durante la riduzione di una
 *  sequenza di istuzioni, e poi nella riduzione NAME = { inst_sequence };
 *  verrà copiata nellàopportuno blocco
 */
extern lista_gate *lst_gate_supporto;

/*
 *  Dichiarazione della tabella dei blocchi
 */
extern blocco_gate *tabella_blk;

/*
 *  Dichiarazione della tabella delle variabili
 */
extern variabile *tabella_var;

/*
 *  Numero della linea attuale di parsing
 */
extern size_t lineno;

/*
 *  Variabile che conterrà il numero di qudit di cui è composto il sistema
 */
extern size_t numero_qudit;

/*
 *  Array che conterrà le dimensioni dei qudit di cui è composto il sistema
 */
extern size_t *dimensioni_qudit;

/*
 *  Variabile che ci dice se le simulazioni comprendono errore o meno
 */
 extern uint8_t errore_abilitato;

/*
 *  Probabilità di errore in una misura
 */
extern double p_errore;

/*
 *  Numero di misure da tenere in memoria
 */
extern size_t memoria_misure;

/*
 *  Lista delle misure eseguite
 */
extern lista_misure *tabella_mis;

/*
 *  Array che conterrà la definizione delle custom logic definite da utente
 */
extern custom_logic custom_logic_definitions[];

/*
 *  Numero di funzioni di custom logic definite. Sarà la dimensione dell'array
 */
extern size_t N_custom_logic;

/*
 *  RNG del programma necessario per l'esecuzione delle misure
 */
extern gsl_rng *rng;

/*
 *  Numero di misurazioni finali del sistema da eseguire. In questo modo
 *  simuliamo l'esecuzione del circuito 'n_shots' volte
 */
extern size_t n_shots;

/*
 *  Frequenza output
 */
extern size_t freq_output;

/*
 *  Directory file di input
 */
extern char *in_dir;

/*
 *  Directory file di output
 */
extern char *out_dir;

/*
 *  Output finale della matrice di densità
 */
extern size_t out_rho_final;

/*
 *  Output della matrice di densità durante la simualzione
 */
extern size_t out_rho_simulation;

/*
 *  Output della sequenza di rotazioni eseguita
 */
extern size_t out_rotation;

/*
 *  Output della schedule eseguita
 */
extern size_t out_schedule;

/*
 *  Simulazione da eseguire
 */
extern size_t simulation_enabled;

/*
 *  Output avanzamento simulazione
 */
extern size_t simulation_progress;

/*
 *  Versione programma
 */
extern char *versione_programma;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione delle variabili globali
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di tutte le variabilli globali che lo richiedono
 */
extern
uint8_t alloc_globals(void);

/*
 *  Funzione per la deallocazione di tutte le variabilli globali che lo
 *  richiedono
 */
extern
void free_globals(void);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la concatenazine di due stringhe. Necessaria nella corretta
 *  inizializzazione delle interfacce di input e di output
 */
extern
char *concatena(char *s_root, char *s_postfix);

#endif