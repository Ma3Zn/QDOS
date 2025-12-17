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

#include "../../Common/include/global.h"
#include "../../Common/include/gate.h"
#include "../include/custom_logic.h"

/*
 *  Header file che conterrà le definizioni di tutte le funzioni di
 *  'custom logic' definite da utente e delle variabili globali
 *  necessarie alla gestione di queste.
 * 
 *  Ogni funzione di custom logic dovrà avre la seguente firma
 *
 *      struct blocco_gate* nome_custom_logic (lista_misure *ptr);
 * 
 *  In particolare si richiede l'inizializzazione delle variabili
 *  globale 'custom_logic_definitions[]' e 'N_custom_logic'.
 * 
 *  La prima sarà usata come indice dei puntatori a funzione delle
 *  custom logic definite. Dunque il dato da inserire per
 *  l'inizializzazione di un elemento sarà
 *  { <alias_funzione_usato_nel_codice>, <puntatore_alla_funzione> }
 * 
 *  La seconda contiene il numero di custom logic definite dall'utente ed
 *  inserite all'interno della prima. In altre parole è la dimensione
 *  della prima
 */

//---------------------------------------------------------------------------//

/*
 *  Custom logic function for the Embedded Quantum Error Correction
 *  scheme proposed in Section 3.2 of the Thesis.
 * 
 *  It simply check the las measurement outcome of the ancilla.
 *  Then if we get a down (0) it will schedule the 'blocco_gate'
 *  recovery_error. While if we get an up (1) it will schedule
 *  the recovery_no_error block.
 */
blocco_gate *EQEC_check_ancilla(lista_misure *tabella_misure)
{
/*
 *  Check input
 */
    if(tabella_misure == NULL || tabella_misure->dim_eff == 0)
    {
        fprintf(stderr, "ERROR:: Custom logic invoked before "
                        "any measurement.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    
    int *measurement_outcome = get_misura(tabella_misure, 0);
/*
 *  Error Check
 */
    if (measurement_outcome == NULL)
    {
        fprintf(stderr, "ERROR:: impossible to retrive the last measurement "
                        "outcome.\n"
                        "ABORTING...\n\n");
        return NULL;
    }

    blocco_gate *blk = NULL;

/*
 *  Check the measurement outcome
 */
    if (measurement_outcome[0] == 1)
    {
/*
 *      We have detected an error, hence the recovery_error block has to be
 *      executed
 */
        blk = cerca_blocco(tabella_blk, "recovery_error");
    }
    else
    {
/*
 *      We havn't detected any error, hence the recovery_no_error block has
 *      to be executed
 */
        blk = cerca_blocco(tabella_blk, "recovery_no_error");
    }

/*
 *  Output necessario allo script MATLAB per generare il grafico di E in
 *  funzione di t/T2.
 */
    FILE *mis_out = fopen("measurement_outcome.txt", "w");
    
    if (mis_out != NULL)
    {
        fprintf(mis_out, "%d", measurement_outcome[0]);
    }
    else
    {
        ;   /* Do Nothing */
    }

    return blk;
}

//---------------------------------------------------------------------------//

/*
 *  Array che conterrà la definizione delle custom logic definite da utente
 */
custom_logic custom_logic_definitions[N_CSL] = 
{
    {"check_ancilla", &EQEC_check_ancilla}
};

/*
 *  Numero di funzioni di custom logic definite. Sarà la dimensione
 *  dell'array 'custom_logic_definitions'
 */
size_t N_custom_logic = N_CSL;