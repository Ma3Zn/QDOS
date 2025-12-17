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

#ifndef GATE_BUILT_IN_h
#define GATE_BUILT_IN_h

#include "../../Common/include/common.h"
#include "../../Common/include/gate.h"

//---------------------------------------------------------------------------//

/*
 *  Header file conetnete tutte le funzioni necessarie alla creazione dei
 *  gate built-in forniti in QDOS
 */

//---------------------------------------------------------------------------//

/*
 *  Strutture dati necessaria per l'installazione delle definizioni dei
 *  gate built in
 */
typedef struct gate_BLTN
{
/*
 *  Nome funzione (usata come chiave di ricerca)
 */
    char *nome;

/*
 *  Puntatore alla funzione relativa al nome
 */
    gate_operator* (*func_ptr) (parametri_gate *parametri);

/*
 *  Numero qudit su cui opera il gate
 */
    size_t n_qudit;

/*
 *  Numero parametri richiesti dal gate
 */
    size_t n_arg;
}gate_BLTN;


//---------------------------------------------------------------------------//

/*
 *  Funzione che data una matrice unitaria la rende unitaria speciale
 */
extern
gsl_matrix_complex *normalizza(gsl_matrix_complex *u);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di una specifica rotazione Z
 */
extern
gate_operator *imp_nullo(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di una specifica rotazione Z
 */
extern
gate_operator *Z_rot(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di una specifica rotazione piana
 */
extern
gate_operator *P_rot(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate X generalizzato
 */
extern
gate_operator *X_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate Z generalizzato
 */
extern
gate_operator *Z_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate H generalizzato
 */
extern
gate_operator *H_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate S generalizzato
 */
extern
gate_operator *S_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CX generalizzato, i.e., un
 *  C_inc
 */
extern
gate_operator *CX_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CZ generalizzato
 */
extern
gate_operator *CZ_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CR generalizzato, gate fondamentale
 *  per l'implementazione della QFT
 */
extern
gate_operator *CR_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CX generalizzato modificato, i.e.,
 *  un C_exc
 */
extern
gate_operator *CEX_generalizzato(parametri_gate *param);

//---------------------------------------------------------------------------//
/*
 *  Macro per una definizione consistente della dimensione dell'array dei
 *  gate built in e del numero di gate built in
 */
#define N_BLTN   11

/*
 *  Array che conterrà la definizione dei gate builtin
 */
extern gate_BLTN gate_builtin_definitions[];

/*
 *  Numero di gate builtin disponibili. Sarà la dimensione
 *  dell'array 'gate_builtin_definitions'
 */
extern size_t N_gate_builtin;

//---------------------------------------------------------------------------//

/*
 *  Funzione che installa i gate built in all'interno della lista delle
 *  definizioni
 */
extern
uint8_t install_BLTN(lista_definizioni_gate *lista_def);

#endif