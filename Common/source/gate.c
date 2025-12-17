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

#include "../../Parser/include/parser_support.h"
#include "../include/rotation.h"
#include "../include/common.h"
#include "../include/global.h"
#include "../include/pulse.h"
#include "../include/gate.h"

/*
 *  File contenente le definizione delle funzioni necessarie alla gestione
 *  delle tabelle utilizzate dal parser
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'parametri_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione dei parametri del gate
 */
parametri_gate *alloc_parametri_gate(uint8_t n_qudit, size_t n_arg)
{
    parametri_gate *parametri = 
        (parametri_gate *)malloc(sizeof(parametri_gate));

/*
 *  Controlliamo l'esito dell'allocazione
 */
    if (parametri == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione dei parametri del gate.\nn"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t *idx_qudit = 
        (size_t*)calloc(sizeof(size_t), n_qudit);

    double *arg =
        (double*)calloc(sizeof(double), n_arg);

/*
 *  Controlliamo l'esito dell'allocazione
 */
    if (idx_qudit == NULL || arg == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione dei parametri del gate.\nn"
                        "ABORTING...\n\n");
        free(idx_qudit);
        free(arg);

        free_parametri_gate(parametri);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    parametri->n_arg = n_arg;
    parametri->n_qudit = n_qudit;

    parametri->idx_qudit = idx_qudit;
    parametri->arg = arg;

    return parametri;
}

/*
 *  Funzione per la deallocazione dei parametri del gate
 */
void free_parametri_gate(parametri_gate *parametri)
{
    if (parametri == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (parametri->idx_qudit != NULL)
    {
        free(parametri->idx_qudit);
        parametri->idx_qudit = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (parametri->arg != NULL)
    {
        free(parametri->arg);
        parametri->arg = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(parametri);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'BLTN_return_type'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un gate_operator
 */
gate_operator *alloc_gate_operator()
{
    gate_operator *ptr = (gate_operator*)malloc(sizeof(gate_operator*));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ptr == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per "
                        "il valore di ritorno dell'invocazione di "
                        "un gate.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return ptr;
}

/*
 *  Funzione per la deallocazione di un gate_operator
 */
void free_gate_operator(gate_operator **ptr)
{
    if (ptr == NULL || *ptr == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    switch ((*ptr)->type)
    {
        case COD_UNT:
            if ((*ptr)->val.unt != NULL)
            {
                gsl_matrix_complex_free((*ptr)->val.unt);
            }
            break;

        case COD_SQR:
            if ((*ptr)->val.seq != NULL)
            {
                free_seq_rot(&((*ptr)->val.seq));
            }
            break;

        case COD_SCH:
            if ((*ptr)->val.sch != NULL)
            {
                free_schedule((*ptr)->val.sch);
            }
            break;

        default:
            break;
    }

    free(*ptr);
    *ptr = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un gate
 */
gate *alloc_gate(uint8_t n_qudit, size_t n_arg)
{
    gate *g = (gate *)malloc(sizeof(gate));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un gate.\n"
                        "ABORTING\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    parametri_gate *parametri = alloc_parametri_gate(n_qudit, n_arg);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (parametri == NULL)
    {
/*
 *      Errore già segnalato
 */
        free_gate(g);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g->operatore.blk_ptr = NULL;
    g->parametri = parametri;
    g->nome_gate = NULL;
    g->type = UNDF;

    return g;
}

/*
 *  Funzione per la deallocazione di un gate
 */
void free_gate(gate *g)
{
    if (g == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (g->parametri != NULL)
    {
        free_parametri_gate(g->parametri);
        g->parametri = NULL;
    }
    else
    {
        ; /* Do Nothing */
    }

    free(g);
    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'lista_definizioni_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di una lista di definizioni di gate
 */
lista_definizioni_gate *alloc_lista_definizioni_gate(void)
{
    lista_definizioni_gate *lst = (lista_definizioni_gate *)malloc(sizeof(lista_definizioni_gate));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (lst == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della lista delle definizoni "
                        "dei gate.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gate **indici_lista = (gate**)calloc(sizeof(gate*), STEP_DIM_LISTA);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (indici_lista == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della lista delle definizoni "
                        "dei gate.\n"
                        "ABORTING...\n\n");
        free_lista_definizioni_gate(lst);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lst->indici_lista = indici_lista;
    lst->dim_max = STEP_DIM_LISTA;
    lst->dim_eff = 0u;

    return lst;
}

/*
 *  Funzione per la riallocazione di una lista di definizioni di gate
 */
lista_definizioni_gate
    *realloc_lista_definizioni_gate(lista_definizioni_gate *lst, size_t step)
{
    gate **tmp = (gate**)realloc(lst->indici_lista,
                    (lst->dim_max + step) * sizeof(gate *));
    
/*
 *  Verifichiamo l'esito della riallocazione
 */
    if (tmp == NULL)
    {
        fprintf(stderr, "ERRORE nella riallocazione della lista delle definizioni "
                        "dei gate.\n Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_lista_definizioni_gate(lst);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lst->dim_max += step;
    lst->indici_lista = tmp;

    return lst;
}

/*
 *  Funzione per la deallocazione di una lista di definizioni di gate
 */
void free_lista_definizioni_gate(lista_definizioni_gate *lst)
{
    if (lst == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < lst->dim_max; ++i)
    {
        if (lst->indici_lista[i] != NULL)
        {
            free_gate(lst->indici_lista[i]);
            lst->indici_lista[i] = NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free(lst->indici_lista);
    lst->indici_lista = NULL;

    free(lst);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per la gestione delle custom_logic definite da utente
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per la ricerca di una custom logic all'interno della tabella
 */
custom_logic_ptr cerca_custom_logic(custom_logic *tabella_custom_logic,
                                    size_t dim,
                                    char *f_name)
{
    if (tabella_custom_logic == NULL && dim > 0)
    {
        fprintf(stderr, "ERRORE nell'inserimento della tabella delle "
                        "custom_logic.\n"
                        "Tabella nulla ma non vuota.\n"
                        "Verificare la dimesnione inserita della tabella.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < dim; ++i)
    {
        if (strcmp(tabella_custom_logic[i].nome, f_name) == 0)
        {
            return tabella_custom_logic[i].f_ptr;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    fprintf(stderr, "ERRORE custom_logic: %s NON trovata.\n"
                    "Verificare di averla correttamente definita nel file "
                    "'custom_logic.c' e di aver inizializzato correttamente "
                    "le due variabili 'custom_logic_definitions' e "
                    "'N_custom_logic'.\n"
                    "ABORTING...\n\n", f_name);
    return NULL;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'blocco_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un gate da eseguire. Prende
 *  come argomenti la definizione del gate da implementare ed i parametri
 *  del gate da eseguire
 */
istruzione *alloc_istruzione_gate(gate *def_gate, parametri_gate *param)
{
/*
 *  Verifichiamo che l'istruzione NON sia un blocco perchè in quel caso viene
 *  passato un puntatore nullo in param
 */
    if (def_gate->type != BLCK)
    {
/*
 *      Verifichiamo che i parametri passati corrispondano nella forma a quelli
 *      della definizione
 */
        if (def_gate->parametri->n_qudit != param->n_qudit || 
            def_gate->parametri->n_arg   != param->n_arg)
        {
            fprintf(stderr, "ERRORE nell'inizializzazione di un'istruzione.\n"
                            "I parametri forniti non rispettano la definizione.\n"
                            "ABORTING...\n");
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    istruzione *ist = (istruzione*)malloc(sizeof(istruzione));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un'istruzione.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo il gate da inserire nell'istruzione
 */
    gate *tmp = (gate *)malloc(sizeof(gate));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (tmp == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di gate.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_istruzione(&ist);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inizializzaimo il gate tramite la sua definizione
 */
    tmp->nome_gate = def_gate->nome_gate;
    tmp->type = def_gate->type;

    tmp->parametri = param;
/*
 *  In base al tipo di gate che abbiamo copiamo il corretto operatore
 */
    switch (tmp->type)
    {
        case BLTN:
            tmp->operatore.func_ptr = def_gate->operatore.func_ptr;
            break;

        case CSTM:
            tmp->operatore.path_to_file = def_gate->operatore.path_to_file;
            break;
        
        case MSNT:
/*
 *          Per le misure il gate non ha un operatore e dunque settiamo NULLO
 *          uno qualsiasi dei puntatori possibili
 */
            tmp->operatore.blk_ptr = NULL;
            break;

        case BLCK:
            tmp->operatore.blk_ptr = def_gate->operatore.blk_ptr;
            break;

        default:
            fprintf(stderr, "ERRORE nel tipo del gate da inserire come "
                            "istruzione.\n"
                            "ABORTING...\n\n");
            free_istruzione(&ist);
            free_gate(tmp);
            break;
    }

    ist->tipo = GATE_INST;
    ist->val.g = tmp;
    ist->next = NULL;

    return ist;
}

/*
 *  Funzione per l'allocazione di una custom logic da eseguire. Prende in
 *  input il puntatore alla funzione da eseguire.
 */
istruzione *alloc_istruzione_custom_logic(char *f_name)
{
    istruzione *ist = (istruzione*)malloc(sizeof(istruzione));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un'istruzione.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    ist->val.custom_logic_function = 
        cerca_custom_logic(custom_logic_definitions, N_custom_logic, f_name);

/*
 *  Verifichiamo l'esito della ricerca
 */
    if (ist->val.custom_logic_function == NULL)
    {
        fprintf(stderr, "ERRORE nella ricerca della custom logic: %s.\n"
                        "Funzione non trovata.\n"
                        "ABORTING...\n\n", f_name);
        free_istruzione(&ist);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    ist->tipo = CUSTOM_LOGIC_INST;
    ist->next = NULL;

    return ist;
}

/*
 *  Funzione per la deallocazione di un'istruzione
 */
void free_istruzione(istruzione **ist)
{
    if (ist == NULL || (*ist) == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if ((*ist)->tipo == GATE_INST)
    {
        if ((*ist)->val.g != NULL)
        {
            free_gate((*ist)->val.g);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(*ist);
    *ist = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'lista_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di una 'lista_gate'
 */
lista_gate *alloc_lista_gate(void)
{
    lista_gate *lst = (lista_gate*)malloc(sizeof(lista_gate));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (lst == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di gate.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lst->testa = NULL;
    lst->coda  = NULL;

    return lst;
}

/*
 *  Funzione per la deallocazione di una 'lista_gate'
 */
void free_lista_gate(lista_gate **lst)
{
    if (lst == NULL || *lst == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    while ((*lst)->testa != NULL)
    {
        istruzione *tmp = (*lst)->testa->next;
        free_istruzione(&((*lst)->testa));
        (*lst)->testa = tmp;
    }

    free(*lst);
    *lst = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'blocco_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un blocco di gate
 */
blocco_gate *alloc_blocco_gate(void)
{
    blocco_gate *blk = (blocco_gate*)malloc(sizeof(blocco_gate));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (blk == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un blocco di gate.\n"
                        "ABORTING...\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lista_gate *lst = alloc_lista_gate();

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (lst == NULL)
    {
        free_blocco_gate(blk);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    blk->nome_blocco = NULL;
    blk->next = NULL;
    blk->lst = lst;

    return blk;
}

/*
 *  Funzione per la deallocazione di una lista di definizioni di gate
 */
void free_blocco_gate(blocco_gate *blk)
{
    if (blk == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do nothing */
    }

    free_lista_gate(&(blk->lst));

    free(blk);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la gestione della dimensione del vettore degli indici
 *  della lista delle definizioni
 */
uint8_t aumenta_dimensione_lista(lista_definizioni_gate *lst)
{
    lst = realloc_lista_definizioni_gate(lst, STEP_DIM_LISTA);

/*
 *  Verifichiamo l'esito della riallocazione
 */
    if (lst == NULL)
    {
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che esegue la ricerca binaria del gate di nome [nome]
 *  nella lista [lst_def]. Il valore di [*idx] al termine della funzione,
 *  se il gate è stato trovato, ne conterrà l'indice all'interno della lista.
 *  Il valore di ritorno della funzione ci dice se il gate è stato trovato [1]
 *  o meno [0].
 */
uint8_t ricerca_definizione_gate(lista_definizioni_gate *lst,
                                    char *nome,
                                    size_t *idx)
{
#ifdef DEBUG
fprintf(stderr, "Lista def gate:\n");
for(size_t i = 0; i < lst->dim_eff; ++i)
{
    fprintf(stderr, " %s ", lst->indici_lista[i]->nome_gate);
}
fprintf(stderr, "\n");
#endif

    if (lst == NULL || nome == NULL || idx == NULL)
    {
        fprintf(stderr, "ERRORE nella chiamata alla funzione di ricerca "
                        "binaria per una definizione di un gate.\n"
                        "Almeno un argomento è NULLO.\n");
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Controlliamo se la lista è vuota
 */
    if (lst->dim_eff == 0)
    {
        return 0;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t fine = lst->dim_eff - 1u;
    size_t inizio = 0u;
    size_t m = 0;
    int ris = 0;

/*
 *  Verifichiamo gli estremi
 */
    ris = strcmp(nome, lst->indici_lista[inizio]->nome_gate);
/*
 *  Verifichiamo se va inserito al primo posto della lista
 */
    if (ris < 0)
    {
        *idx = inizio;
        return 0;
    }
    else if (ris == 0)
    {
        *idx = inizio;
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    ris = strcmp(nome, lst->indici_lista[fine]->nome_gate);
/*
 *  Verifichiamo se va inserito all'ultimo posto della lista
 */
    if (ris > 0)
    {
        *idx = fine + 1;
        return 0;
    }
    else if (ris == 0)
    {
        *idx = fine;
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Se non va posto agli estremi controlliamo in che indice va inserito
 */
    while (fine > inizio + 1u)
    {
        m = (inizio + fine) / 2u;
        
        ris = strcmp(nome, lst->indici_lista[m]->nome_gate);
/*
 *  Verifichiamo l'ordine relativo tra il punto medio e nome
 */
        if (ris < 0)
        {
            fine = m;
        }
        else if (ris > 0)
        {
            inizio = m;
        }
        else
        {
            *idx = m;
            return 1;
        }
    }

#ifdef DEBUG
/*
 *  Sanity check
 */
    if (fine - inizio > 1)
    {
        fprintf(stderr, "ERRORE nell'algoritmo di ricerca binaria.\n\n");
    }
    else
    {
        ;   /* Do Nothing */
    }
#endif

/*
 *  Siccome lst->indici_lista[inizio] < nome l'indice corretto sarà quello
 *  di fine, il quale, al termine della ricerca sarà legato ad inizio dalla
 *  relazione
 *                              fine = inizio + 1
 */
    *idx = fine;

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un gate all'interno della lista
 *  delle definizioni. Ritorna un valore positivo se si sono verificati
 *  degli errori
 */
esito_inserimento inserisci_definizione(lista_definizioni_gate *lst, gate *g)
{
    esito_inserimento ris;
/*
 *  Controlliamo la legalità degli argomenti passati
 */
    if (lst == NULL || g == NULL)
    {
        fprintf(stderr, "ERRORE nell'inserimento di un gate nella "
                        "lista delle definizioni.\n"
                        "Argomento NULLO.\n"
                        "ABORTING...\n\n");
        free_lista_definizioni_gate(lst);
        free_gate(g);

        ris = GERR;

        return ris;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
/*
 *  Verifichiamo che la lista abbia spazio a sufficienza per una nuova
 *  definizone.
 */
    if (lst->dim_max - lst->dim_eff == 0u)
    {
/*
 *      Verifichiamo l'esito della riallocazione della lista
 */
        if (aumenta_dimensione_lista(lst) > 0)
        {
            ris = GERR;
            return ris;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Cerchiamo l'indice in cui inserire il nuovo gate
 */
    size_t idx = 0u;
    uint8_t duplicato = ricerca_definizione_gate(lst, g->nome_gate, &idx);

/*
 *  Verfichiamo se il gate è già presente o meno nella lista delle definizioni.
 *
 *  Come rara eccezione in questo caso, poichè qui non abbiamo info sullo
 *  stato del parsing, lasciamo la segnalazione dell'errore al chiamante.
 */
    if (duplicato == 1)
    {
/*
 *      NON permettiamo la ridefinizione di gate
 */
        ris = RDEF;
        return ris;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inseriamo il gate nella lista.
 *
 *  Controlliamo se inserire direttamente il gate in coda o se dobbiamo
 *  eseguire uno shift di indici
 */
    if (idx < lst->dim_eff)
    {
        gate *tmp1;
        gate *tmp2;

        tmp1 = lst->indici_lista[idx];
        lst->indici_lista[idx] = g;

        for (size_t i = idx + 1; i <= lst->dim_eff + 1; ++i)
        {
            tmp2 = lst->indici_lista[i];
            lst->indici_lista[i] = tmp1;
            tmp1 = tmp2;
        }
    }
    else
    {
        lst->indici_lista[lst->dim_eff] = g;
    }

    lst->dim_eff += 1;
    ris = OK;

    return ris;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la ricerca di un gate all'interno della lista
 *  delle definizioni
 */
gate *cerca_definizione(lista_definizioni_gate *lst, char *nome_gate)
{
/*
 *  Controlliamo la legalità degli argomenti passati
 */
    if (lst == NULL || nome_gate == NULL)
    {
        fprintf(stderr, "ERRORE nella ricerca di un gate nella "
                        "lista delle definizioni.\n"
                        "Argomento NULLO.\n"
                        "ABORTING...\n\n");
        free_lista_definizioni_gate(lst);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t  idx = 0;
    uint8_t ris = ricerca_definizione_gate(lst, nome_gate, &idx);

/*
 *  Verifichiamo l'esito della ricerca
 */
    if (ris == 0)
    {
        fprintf(stderr, "ERRORE gate %s NON definito.\n"
                        "ABORTING...\n", nome_gate);
        free_lista_definizioni_gate(lst);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return lst->indici_lista[idx];
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un'istruzione all'interno di una 'lista_gate'
 */
void inserisci_istruzione(lista_gate *lst, istruzione *ist)
{
    if (lst == NULL)
    {
        fprintf(stderr, "ln %ld: ERRORE nell'inserimento di un'instruzione "
                        "all'interno di un blocco.\n"
                        "Lista delle definizioni nulla.\n"
                        "ABORTING...\n\n", lineno);

                        free_istruzione(&ist);

                        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (ist == NULL)
    {
        fprintf(stderr, "ln %ld: ERRORE nell'inserimento di un'instruzione "
                        "all'interno di un blocco.\n"
                        "Istruzione nulla.\n"
                        "ABORTING...\n\n", lineno);

                        free_lista_gate(&lst);

                        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo che la sequenza di istruzioni non sia vuota.
 *  Nel caso va inizializzata.
 */
    if (lst->testa != NULL)
    {
        lst->coda->next = ist;
        lst->coda = ist;
    }
    else
    {
        lst->testa = ist;
        lst->coda = ist;
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un blocco di gate all'interno della
 *  tabella dei blocchi
 */
blocco_gate *inserisci_blocco(blocco_gate *tabella, blocco_gate *blk)
{
    if (blk == NULL || tabella == NULL)
    {
        fprintf(stderr, "ERRORE nell'inserimento di un blocco di gate "
                        "nella tabella oopertuna.\n"
                        "ABORTING...\n");
        free_blocco_gate(tabella);
        free_blocco_gate(blk);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inserimento in testa del nuovo blocco
 */
    blk->next = tabella;
    return blk;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per verificare la presenza di un blocco di gate all'interno della
 *  tabella dei blocchi. Ritorna un puntatore nullo se il blocco NON è presente
 */
blocco_gate *cerca_blocco(blocco_gate *tabella, char *nome_blocco)
{
    blocco_gate *tmp = tabella;
/*
 *  Ricerca lineare del blocco all'interno della tabella
 */
    while (tmp != NULL)
    {
/*
 *  Verifichiamo che la tabella non sia vuota
 */
        if (tmp->nome_blocco != NULL)
        {
            int ris = strcmp(nome_blocco, tmp->nome_blocco);
/*
 *          Verfichiamo l'esito del confronto
 */
            if (ris == 0)
            {
                break;
            }
            else
            {
                ;
            }
        }
        else
        {
            ;   /* Do Nothing */
        }
        
        tmp = tmp->next;
    }

/*
 *  NON verifichiamo l'esio della ricerca in qunato usiamo questa funzione
 *  anche per verificare che non si tenti di ridefinire un blocco di gate
 */

    return tmp;
}

//---------------------------------------------------------------------------//