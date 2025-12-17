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

#ifndef PARSER_SUPPORT_H
#define PARSER_SUPPORT_H

#include "../../Common/include/common.h"

/*
 *  Header File per la dichiarazione di tutte le funzioni di supporto
 *  di cui necessita il parser
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati necessaria per la gestione di una sequenza di numeri
 */

typedef struct sequenza_numeri
{
    union {
        size_t u_val;
        double f_val;
    } valore;

    struct sequenza_numeri *next;
    
} sequenza_numeri;

//---------------------------------------------------------------------------//

/*
 *  Intefacce per la gestione di una sequenza di numeri
 */

//---------------------------------------------------------------------------//

/*
 *  Alloca un nuovo valore per una sequenza di numeri
 */
extern
sequenza_numeri *alloc_valore_sequenza_numeri(void);

/*
 *  Inserimento di un nuovo elemnto all'interno di una sequenza di numeri
 */
extern
sequenza_numeri *inserisci_valore_sequenza_numeri(sequenza_numeri *testa,
                                                    sequenza_numeri *elem);

/*
 *  Dealloca una sequenza di numeri
 */
extern
void free_sequenza_numeri(sequenza_numeri **testa);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati necessaria per la gestione della tabella delle variabili
 *  matematiche builtin o definite dall'utente
 */

typedef struct variabile
{
    char *nome;
    double valore;

    struct variabile *next;
} variabile;

//---------------------------------------------------------------------------//

/*
 *  Intefacce per la gestione della tabella delle variabili
 */

//---------------------------------------------------------------------------//

/*
 *  Alloca una nuova variabile
 */
extern
variabile *alloc_variabile(void);

/*
 *  Dealloca una sequenza di numeri
 */
extern
void free_lista_variabili(variabile **testa);

/*
 *  Inserimento di una nuova variabile all'interno della tabella delle
 *  variabili
 */
extern
variabile *inserisci_variabile(variabile *testa, variabile *var);

/*
 *  Ricerca di una variabile all'interno della tabella delle variabili
 */
extern
variabile *ricerca_variabile(variabile *testa, char *nome);

//---------------------------------------------------------------------------//

/*
 *  Funzione invocata dal parser per segnalare la presenza di diversi errori
 */
extern void yyerror(char * s);

//---------------------------------------------------------------------------//

#endif