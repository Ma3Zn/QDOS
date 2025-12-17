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
#include "../include/parser_support.h"

/*
 *  File per la definizone di tutte le funzioni di supporto di cui necessita
 *  il parser
 */

//---------------------------------------------------------------------------//

/*
 *  Intefacce per la gestione di una sequenza di numeri
 */

//---------------------------------------------------------------------------//

/*
 *  Alloca un nuovo valore per una sequenza di numeri
 */
sequenza_numeri *alloc_valore_sequenza_numeri(void)
{
    sequenza_numeri *elem = (sequenza_numeri*)malloc(sizeof(sequenza_numeri));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (elem == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un elemento per una "
                        "sequenza di numeri alla linea: %ld\n."
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n", lineno);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.u_val = 0u;
    elem->next = NULL;

    return elem;
}

/*
 *  Inserimento di un nuovo elemnto all'interno di una sequenza di numeri
 */
sequenza_numeri *inserisci_valore_sequenza_numeri(sequenza_numeri *testa,
                                                    sequenza_numeri *elem)
{
    elem->next = testa;
    return elem;
}

/*
 *  Dealloca una sequenza di numeri
 */
void free_sequenza_numeri(sequenza_numeri **testa)
{
    if (testa == NULL || *testa == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    sequenza_numeri *next = (*testa)->next;
    free(*testa);
    *testa = NULL;

/*
 *  Verifichiamo se sono ancora presenti elementi nella lista
 */
    if (next != NULL)
    {
        free_sequenza_numeri(&next);
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Intefacce per la gestione della tabella delle variabili
 */

//---------------------------------------------------------------------------//

/*
 *  Alloca una nuova variabile
 */
variabile *alloc_variabile(void)
{
    variabile *var = (variabile*)malloc(sizeof(variabile));

/*
 *  Controlliamo l'esito dell'allocazione
 */
    if (var == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una varibaile.\n"
                    "Memoria insufficiente."
                    "ABORTING...\n");
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    var->valore = 0.0;
    var->nome = NULL;
    var->next = NULL;
    
    return var;
}

/*
 *  Dealloca una sequenza di numeri
 */
void free_lista_variabili(variabile **testa)
{
    if (testa == NULL || *testa == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    variabile *next = (*testa)->next;
    free(*testa);
    *testa = NULL;

/*
 *  Verifichiamo se sono ancora presenti elementi nella lista
 */
    if (next != NULL)
    {
        free_lista_variabili(&next);
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

/*
 *  Inserimento di una nuova variabile all'interno della tabella delle
 *  variabili
 */
variabile *inserisci_variabile(variabile *testa, variabile *var)
{
    var->next = testa;
    return var;
}

/*
 *  Ricerca di una variabile all'interno della tabella delle variabili
 */
variabile *ricerca_variabile(variabile *testa, char *nome)
{
    variabile *var = testa;
/*
 *  Ricerca lineare del blocco all'interno della tabella
 */
    while (var != NULL)
    {
        if (var->nome == NULL)
        {
            var = NULL;
            break;
        }
        
        int ris = strcmp(nome, var->nome);
/*
 *      Verfichiamo l'esito del confronto
 */
        if (ris == 0)
        {
            break;
        }
        else
        {
            ;   /* Do Nothing */
        }
        
        var = var->next;
    }

/*
 *  NON verifichiamo l'esito della ricerca perchè usiamo questa funzione
 *  anche per controllare la possibile ridefinizione di variabili
 */

    return var;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione invocata dal parser per segnalare la presenza di diversi errori
 */
void yyerror(char * s)
{
    fprintf(stderr, "ERRORE nel parsing: %s\n", s);
    return;
}