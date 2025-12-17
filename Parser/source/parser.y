%{
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
#include "../include/parser.h"
#include "../include/lexer.h"

#include <string.h>
%}

%union {
    char        *string;
    size_t      int_val;
    double      float_val;
}

%token <string>         NAME GATE MEASUREMENT CUSTOM_LOGIC INDEX QUDIT
%token <int_val>        INTEGER
%token <float_val>      FLOAT
%%

list:       /* Nothing */
        |   list ';' { /* Nothing */ }
        |   list def ';' { /* Nothing */ }
        |   list error ';'
{ 
    fprintf(stderr, "ERRORE nel parsing della linea: %ld.\n"
                    "ABORTING...\n\n", lineno);
    free_globals();
    exit(1);
}
        ;

def:    QUDIT '[' INTEGER ']' '=' '{' integer_sequence '}'
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di set di Qudit.\n"
                    "Numero Qudit: %ld.\n", lineno, $3);
#endif
/*
 *  Inizializziamo un array con gli elemnti della sequenza di numeri letti.
 *  Da tenere a mente che la sequenza di numeri è ordinata in verso opposto
 *  a quello di lettura
 */
    dimensioni_qudit = 
        (size_t*)realloc(dimensioni_qudit, sizeof(size_t)*(numero_qudit + $3));

/*
 *  Verifichiamo l'esito della riallocazione.
 */
    if (dimensioni_qudit == NULL)
    {
        fprintf(stderr, "ERRORE nella riallocazione della lista delle "
                        "dimensioni dei qudit.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *tmp = int_seq;

    for (size_t i = 1; i <= $3; ++i)
    {
/*
 *  Verifichiamo che il numero di qudit inizializzati e la dimensione
 *  della sequenza combacino
 */
        if (tmp == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni delle dimensioni "
                            "dei qudit.\n"
                            "Qudit dichiarati: %ld\n"
                            "Qudit inizializzati: %ld\n"
                            "ABORTING...\n\n",
                            lineno,
                            $3 + numero_qudit,
                            i + numero_qudit);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        dimensioni_qudit[numero_qudit + $3 - i] = tmp->valore.u_val;
        tmp = tmp->next;
    }

    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    fprintf(stdout, "valori sequenza letti:\n");
    
    for (size_t i = 0; i < $3; ++i)
    {
        fprintf(stdout, "%ld-esimo valore: %ld\n",
                        i, dimensioni_qudit[numero_qudit + i]);
    }

    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Teniamo traccia del numero di qudit dichiarati.
 */
    numero_qudit += $3;
}
        |   GATE '[' INTEGER ']' '=' INDEX
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di un gate custom.\n"
                    "Nome Gate: %s.\n"
                    "Numero Qudit: %ld.\n"
                    "/path/to/file: %s\n"
                    "==========\n\n", lineno, $1, $3, $6);
#endif
/*
 *  Allochiamo un gate che conterrà la definizione attuale
 */
    gate *g = alloc_gate($3, 0);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: allocazione definizione gate non "
                        "riuscita.\n", lineno);
        free_globals();
        free_gate(g);
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Recuperiamo la stringa corretta
 */
    char *s = $6;
    ++s;
    size_t len = strlen(s);
    s[len - 1] = '\0';

    g->operatore.path_to_file = s;
    g->nome_gate = $1;
    g->type = CSTM;

    esito_inserimento ris = inserisci_definizione(lst_def, g);

/*
 *  Verifichiamo l'esito dell'inserimento
 */
    if (ris == RDEF)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: Ridefinizione del gate %s.\n"
                        "Possibile che il nome sia già riservato per un gate "
                        "builtin.\n"
                        "Consultare il manuale dell'applicazione per capire "
                        "come stampare una lista esaustiva di tutti i gate "
                        "builtin.\n"
                        "ABORTING...\n\n", lineno, $1);
        free_globals();
        free_gate(g);
        exit(1);
    }
    else if (ris == GERR)
    {
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
        |   NAME '=' '{' inst_sequence '}'
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di un blocco di gate.\n"
                    "Nome blocco: %s.\n"
                    "==========\n\n", lineno, $1);
#endif

/*
 *  Verifichiamo che il blocco non sia già stato definito
 */
    blocco_gate *blk = cerca_blocco(tabella_blk, $1);

    if (blk != NULL)
    {
        fprintf(stderr, "ln %ld.\n"
                        "ERROR:: Ridefinizione blocco '%s'.\n", lineno, $1);
        free_globals();
        exit(1);
    }
    else
    {
        ;
    }

/*
 *  Allochiamo un blocco che andrà poi inserito all'interno della tabella
 *  dei blocchi
 */
    blk = alloc_blocco_gate();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (blk == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: allocazione blocco gate non "
                        "riuscita.\n", lineno);
        free_blocco_gate(blk);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    blk->nome_blocco = $1;
    blk->lst = lst_gate_supporto;
/*
 *  Liberiamo la variabile globale per poter leggere
 *  (se, in futuro, necessario) una nuova lista di gate
 */
    lst_gate_supporto = NULL;

    tabella_blk = inserisci_blocco(tabella_blk, blk);
}
        |   NAME '=' INTEGER
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di una variabile.\n"
                    "Nome: %s.\n"
                    "Valore: %f\n"
                    "==========\n\n",
                    lineno, $1, (double)$3);
#endif
/*
 *  Verifichiamo che la variabile non sia già definita. In tal caso
 *  sovrascriviamone il valore
 */
    variabile *var = ricerca_variabile(tabella_var, $1);

    if (var == NULL)
    {
/*
 *      Allochiamo la variabile
 */ 
        variabile *var = alloc_variabile();
/*  
 *      Verifichiamo l'esito dell'allocazione
 */ 
        if (var == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: Allocazione variabile non riuscita.\n",
                            lineno);
            free_lista_variabili(&var);
            free_globals();
            exit(1);
        }
        else
        {
            ; /* Do Nothing */
        }
/*  
 *      Inizializziamo la variabile
 */ 
        var->valore = (double)$3;
        var->nome = $1;

        tabella_var = inserisci_variabile(tabella_var, var);
    }
    else
    {
        var->valore = $3;
    } 
}
        |   NAME '=' FLOAT
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di una variabile.\n"
                    "Nome: %s.\n"
                    "Valore: %f\n"
                    "==========\n\n",
                    lineno, $1, $3);
#endif
/*
 *  Verifichiamo che la variabile non sia già definita. In tal caso
 *  sovrascriviamone il valore
 */
    variabile *var = ricerca_variabile(tabella_var, $1);

    if (var == NULL)
    {
/*
 *      Allochiamo la variabile
 */ 
        variabile *var = alloc_variabile();
/*  
 *      Verifichiamo l'esito dell'allocazione
 */ 
        if (var == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: Allocazione variabile non riuscita.\n",
                            lineno);
            free_lista_variabili(&var);
            free_globals();
            exit(1);
        }
        else
        {
            ; /* Do Nothing */
        }
/*  
 *      Inizializziamo la variabile
 */ 
        var->valore = $3;
        var->nome = $1;

        tabella_var = inserisci_variabile(tabella_var, var);
    }
    else
    {
        var->valore = $3;
    }
}
        ;

inst:       GATE '[' integer_sequence ']' '('float_sequence')' ';'
{

#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di gate builtin.\n"
                    "Nome gate: %s\n",
                    lineno, $1);
#endif
/*
 *  Cerchiamo la definizione del gate
 */
    gate *g_def = cerca_definizione(lst_def, $1);
/*
 *  verifichiamo l'esito della ricerca
 */
    if (g_def == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: possibile che il gate %s NON sia mai "
                        "stato definito.\n", lineno, $1);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *tmp_float = float_seq;
    sequenza_numeri *tmp_int = int_seq;
/*
 *  Contiamo il numero di qudit su cui applicare il gate.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
#ifdef DEBUG_PARSER
        fprintf(stdout, "\nValori sequenza interi letti:");
#endif
    size_t n_qudit = 0;
    while(tmp_int != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, " %ld ", tmp_int->valore.u_val);
#endif
        ++n_qudit;
        tmp_int = tmp_int->next;
    }
#ifdef DEBUG_PARSER
        fprintf(stdout, "\n\n");
#endif

/*
 *  Contiamo il numero di argomenti forniti al gate.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
#ifdef DEBUG_PARSER
        fprintf(stdout, "Valori sequenza decimali letti:");
#endif
    size_t n_arg = 0;
    while(tmp_float != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, " %e ", tmp_float->valore.f_val);
#endif
        ++n_arg;
        tmp_float = tmp_float->next;
    }
#ifdef DEBUG_PARSER
        fprintf(stdout, "\n\n");
#endif

/*
 *  Reinizzializziamo tmp alla testa della sequenza di numeri
 */
    tmp_float = float_seq;
    tmp_int = int_seq;

/*
 *  Allochiamo i parametri della misura necessari all'interfaccia per
 *  l'allocazione di una struttura dati
 */
    parametri_gate *par = alloc_parametri_gate(n_qudit, n_arg);

/*
 *  Verifichiamo che i parametri passati corrispondano nella forma a quelli
 *  della definizione
 */
    if (g_def->parametri->n_qudit != par->n_qudit || 
        g_def->parametri->n_arg   != par->n_arg)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: i parametri forniti non rispettano la "
                        "definizione del gate.\n"
                        "Numero indici qudit richiesti: %d.\n"
                        "Numero indici qudit forniti:   %d.\n"
                        "Numero argomenti richiesti:    %ld.\n"
                        "Numero argomenti forniti:      %ld.\n"
                        "ABORTING...\n",
                        lineno,
                        g_def->parametri->n_qudit,
                        par->n_qudit,
                        g_def->parametri->n_arg,
                        par->n_arg);
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Riempiamo i parametri del gate
 *
 *  indici qudit
 */
    for (size_t i = 1; i <= n_qudit; ++i)
    {
        if (tmp_int == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_qudit da fornire: %ld\n"
                            "n_qudit forniti:    %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_qudit, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->idx_qudit[n_qudit - i] = tmp_int->valore.u_val;
        tmp_int = tmp_int->next;

/*
 *      Verifichiamo che l'indice del qudit passato sia legale, i.e., che il
 *      sistema sia composto da un numero sufficiente di qudit.
 */
        if (par->idx_qudit[n_qudit - i] > numero_qudit)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: indice qudit troppo grande.\n"
                            "Indice qudit fornito: %ld\n"
                            "Numero di qudit del sistema "
                            "(l'indicizzazione parte da 0): %ld\n"
                            "Definire prima i qudit dei blocchi di gate.\n"
                            "ABORTING...\n\n",
                            lineno,
                            par->idx_qudit[n_qudit - i],
                            numero_qudit);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
/*
 *  Argomenti
 */
    for (size_t i = 1; i <= n_arg; ++i)
    {
        if (tmp_float == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_arg da fornire: %ld\n"
                            "n_arg forniti:    %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_arg, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->arg[n_arg - i] = tmp_float->valore.f_val;
        tmp_float = tmp_float->next;
    }

    free_sequenza_numeri(&float_seq);
    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    fprintf(stdout, "Indici qudit su cui applicare il gate: ");
    
    for (size_t i = 0; i < n_qudit - 1; ++i)
    {
        fprintf(stdout, "%ld, ", par->idx_qudit[i]);
    }
    
    fprintf(stdout, "%ld\n", par->idx_qudit[n_qudit - 1]);
    fprintf(stdout, "Argomenti del gate: ");
    
    for (size_t i = 0; i < n_arg - 1; ++i)
    {
        fprintf(stdout, "%f, ", par->arg[i]);
    }
    
    fprintf(stdout, "%f\n", par->arg[n_arg - 1]);
    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, par);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
        |   GATE '[' integer_sequence ']' ';'
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di gate custom.\n"
                    "Nome gate: %s\n",
                    lineno, $1);
#endif
/*
 *  Cerchiamo la definizione del gate
 */
    gate *g_def = cerca_definizione(lst_def, $1);
/*
 *  verifichiamo l'esito della ricerca
 */
    if (g_def == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: possibile che il gate %s NON sia mai "
                        "stato definito.\n", lineno, $1);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *tmp = int_seq;
/*
 *  Contiamo il numero di qudit su cui applicare il gate.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
    size_t n_qudit = 0;
    while(tmp != NULL)
    {
        ++n_qudit;
        tmp = tmp->next;
    }

/*
 *  Reinizzializziamo tmp alla testa della sequenza di numeri
 */
    tmp = int_seq;

/*
 *  Allochiamo i parametri della misura necessari all'interfaccia per
 *  l'allocazione di una struttura dati
 */
    parametri_gate *par = alloc_parametri_gate(n_qudit, 0);

/*
 *  Verifichiamo che i parametri passati corrispondano nella forma a quelli
 *  della definizione
 */
    if (g_def->parametri->n_qudit != par->n_qudit || 
        g_def->parametri->n_arg   != par->n_arg)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: i parametri forniti non rispettano la "
                        "definizione del gate.\n"
                        "Numero indici qudit richiesti: %d.\n"
                        "Numero indici qudit forniti:   %d.\n"
                        "Numero argomenti richiesti:    %ld.\n"
                        "Numero argomenti forniti:      %ld.\n"
                        "ABORTING...\n",
                        lineno,
                        g_def->parametri->n_qudit,
                        par->n_qudit,
                        g_def->parametri->n_arg,
                        par->n_arg);
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Riempiamo i parametri del gate
 */
    for (size_t i = 1; i <= n_qudit; ++i)
    {
        if (tmp == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_qudit dichiarati: %ld\n"
                            "n_qudit inizializzati: %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_qudit, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->idx_qudit[n_qudit - i] = tmp->valore.u_val;
        tmp = tmp->next;

/*
 *      Verifichiamo che l'indice del qudit passato sia legale, i.e., che il
 *      sistema sia composto da un numero sufficiente di qudit.
 */
        if (par->idx_qudit[n_qudit - i] > numero_qudit)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: indice qudit troppo grande.\n"
                            "Indice qudit fornito: %ld\n"
                            "Numero di qudit del sistema "
                            "(l'indicizzazione parte da 0): %ld\n"
                            "Definire prima i qudit dei blocchi di gate.\n"
                            "ABORTING...\n\n",
                            lineno,
                            par->idx_qudit[n_qudit - i],
                            numero_qudit);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    fprintf(stdout, "Indici qudit su cui applicare il gate: ");
    
    for (size_t i = 0; i < n_qudit - 1; ++i)
    {
        fprintf(stdout, "%ld, ", par->idx_qudit[i]);
    }
    
    fprintf(stdout, "%ld\n", par->idx_qudit[n_qudit - 1]);
    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, par);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
        |   MEASUREMENT '[' integer_sequence ']' ';'
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di misura.\n"
                    "Indici qudit da misurare: ", lineno);
#endif
    sequenza_numeri *tmp = int_seq;
/*
 *  Contiamo il numero di qudit da misurare.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
    size_t n_qudit = 0;
    while(tmp != NULL)
    {
        ++n_qudit;
        tmp = tmp->next;
    }

/*
 *  Reinizzializziamo tmp alla testa della sequenza di numeri
 */
    tmp = int_seq;

/*
 *  Allochiamo i parametri della misura necessari all'interfaccia per
 *  l'allocazione di una struttura dati
 */
    parametri_gate *par = alloc_parametri_gate(n_qudit, 0);

/*
 *  Riempiamo i parametri del gate
 */
    for (size_t i = 1; i <= n_qudit; ++i)
    {
        if (tmp == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_qudit dichiarati: %ld\n"
                            "n_qudit inizializzati: %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_qudit, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->idx_qudit[n_qudit - i] = tmp->valore.u_val;
        tmp = tmp->next;

/*
 *      Verifichiamo che l'indice del qudit passato sia legale, i.e., che il
 *      sistema sia composto da un numero sufficiente di qudit.
 */
        if (par->idx_qudit[n_qudit - i] > numero_qudit)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: indice qudit troppo grande.\n"
                            "Indice qudit fornito: %ld\n"
                            "Numero di qudit del sistema "
                            "(l'indicizzazione parte da 0): %ld\n"
                            "Definire prima i qudit dei blocchi di gate.\n"
                            "ABORTING...\n\n",
                            lineno,
                            par->idx_qudit[n_qudit - i],
                            numero_qudit);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    for (size_t i = 0; i < n_qudit - 1; ++i)
    {
        fprintf(stdout, "%ld, ", par->idx_qudit[i]);
    }
    
    fprintf(stdout, "%ld\n", par->idx_qudit[n_qudit - 1]);
    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Allochiamo una definizione fittizia di gate per poter usare le interfacce
 *  di allocazione delle istruzioni
 */
    gate *g_def = alloc_gate(n_qudit, 0);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_def == NULL)
    {
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_def->nome_gate = "measure";
    g_def->type = MSNT;

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, par);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_parametri_gate(par);
        free_gate(g_def);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    free_gate(g_def);
}
        |   CUSTOM_LOGIC NAME ';'
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di custom_logic.\n"
                    "Nome custom_logic: %s.\n"
                    "==========\n\n",
                    lineno, $2);
#endif
/*
 *  Verifichiamo che l'istruzione precedente sia stata correttamente inserita
 */
    if (ist_supporto != NULL)
    {
        fprintf(stderr, "ERRORE istruzione precedente NON correttamente "
                        "inserita nella tabella.\n"
                        "ABORTING...\n\n");
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Recuperiamo il puntatore alla funzione di custom logic dalla tabella
 */
    ist_supporto = alloc_istruzione_custom_logic($2);

/*
 *  Verifichiamo che l'istruzione precedente sia stata correttamente inserita
 */
    if (ist_supporto == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
        |   NAME ';'
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di blocco.\n"
                    "Nome: %s.\n"
                    "==========\n\n",
                    lineno, $1);
#endif
/*
 *  Verifichiamo che l'istruzione precedente sia stata correttamente inserita
 */
    if (ist_supporto != NULL)
    {
        fprintf(stderr, "ERRORE istruzione precedente NON correttamente "
                        "inserita nella tabella.\n"
                        "ABORTING...\n\n");
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  recuperiamo il blocco dalla tabella
 */
    blocco_gate *blk = cerca_blocco(tabella_blk, $1);
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (blk == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: blocco %s NON definito.\n",
                        lineno, $1);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Creiamo una definizione fittizia di gate
 */
    gate *g_def = alloc_gate(0,0);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_def == NULL)
    {
        free_gate(g_def);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_def->operatore.blk_ptr = blk;
    g_def->nome_gate = $1;
    g_def->type = BLCK;

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, NULL);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_gate(g_def);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    free_gate(g_def);
}
        ;

inst_sequence:          inst
{
#ifdef DEBUG_PARSER
    fprintf(stdout, "Iniziato parsing sequenza di istruzioni");
#endif
/*
 *  Questo è il primo passo di parsing di una sequenza NON vuota di numeri
 *  interi.
 */
    if (lst_gate_supporto != NULL)
    {
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);

        free_lista_gate(&lst_gate_supporto);
    }
    else
    {
        ;   /* Do Nothing */
    }

    lst_gate_supporto = alloc_lista_gate();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (lst_gate_supporto == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la corretta all'ocazione di una nuova istruzione
 */
    if (ist_supporto == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: Inserimento di un'istruzione nulla "
                        "all'interno di una lista di gate.\n"
                        "ABORTING...\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    inserisci_istruzione(lst_gate_supporto, ist_supporto);
/*
 *  Liberiamo la variabile d'appoggio per una possibile futura lettura di
 *  un'altra istruzione di supporto
 */
    ist_supporto = NULL;
}
                    |   inst_sequence inst
{
/*
 *  Questo è il primo passo di parsing di una sequenza NON vuota di numeri
 *  interi.
 */
    if (lst_gate_supporto == NULL)
    {
        fprintf(stdout, "ln %ld "
                        "ERROR:: seqeunza di istruzioni nulla quando "
                        "dovrebbe essere disponibile almeno un'istruzione.\n"
                        "ABORTING...\n\n", lineno);
        free_lista_gate(&lst_gate_supporto);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la corretta all'ocazione di una nuova istruzione
 */
    if (ist_supporto == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: Inserimento di un'istruzione nulla "
                        "all'interno di una lista di gate.\n"
                        "ABORTING...\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    inserisci_istruzione(lst_gate_supporto, ist_supporto);
/*
 *  Liberiamo la variabile d'appoggio per una possibile futura lettura di
 *  un'altra istruzione di supporto
 */
    ist_supporto = NULL;
}
                    ;
integer_sequence:       /* Nothing */
{
#ifdef DEBUG_PARSER
    if (int_seq != NULL)
    {
        fprintf(stderr, "Riconosciuta sequenza di numeri vuota alla linea: %ld "
                        "ma la sequenza di numeri non è NULLA.\n"
                        "Seqeunza di numeri deallocata improprioamente.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        fprintf(stdout, "Riconosciuta sequenza di numeri vuota alla linea: "
                        "%ld.\n", lineno);
    }
#endif
}
                    |   INTEGER
{
/*
 *  Questo è il primo passo di parsing di una sequenza NON vuota di numeri
 *  interi.
 */
    if (int_seq != NULL)
    {
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&int_seq);
    }
    else
    {
        ;   /* Do Nothing */
    }

    int_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (int_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    int_seq->valore.u_val = $1;
}
                    |   integer_sequence ',' INTEGER 
{
/*
 *  Secondo step di parsing di una sequenza di numeri interi. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (int_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri interi "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_sequenza_numeri(&elem);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.u_val = $3;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    int_seq = inserisci_valore_sequenza_numeri(int_seq, elem);
}
                    ;
float_sequence:         /* Nothing (N c Q)*/
{
#ifdef DEBUG_PARSER
    if (float_seq != NULL)
    {
        fprintf(stderr, "Riconosciuta sequenza di numeri vuota alla linea: %ld "
                        "ma la sequenza di numeri non è NULLA.\n"
                        "Seqeunza di numeri deallocata improprioamente.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        fprintf(stdout, "Riconosciuta sequenza di numeri vuota alla linea: "
                        "%ld.\n", lineno);
    }
#endif
}
                    |   NAME
{
/*
 *  Questo è un possibile primo passo di parsing di una sequenza NON vuota di
 *  numeri razionali.
 */
    if (float_seq != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&float_seq);
#endif
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (float_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Cerchiamo la variabile in questione
 */
    variabile *var = ricerca_variabile(tabella_var, $1);
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (var == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERRORE:: variabile %s NON definita\n", lineno, $1);
        free_lista_variabili(&var);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq->valore.f_val = var->valore;
}
                    |   FLOAT
{
/*
 *  Questo è un possibile primo passo di parsing di una sequenza NON vuota di
 *  numeri razionali.
 */
    if (float_seq != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&float_seq);
#endif
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (float_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq->valore.f_val = $1;
}
                    |   INTEGER
{
/*
 *  Questo è un possibile primo passo di parsing di una sequenza NON vuota di
 *  numeri razionali.
 */
    if (float_seq != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&float_seq);
#endif
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (float_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq->valore.f_val = (double)$1;
}
                    |   float_sequence ',' NAME
{
/*
 *  Secondo step di parsing di una sequenza di numeri. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (float_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Cerchiamo la variabile in questione
 */
    variabile *var = ricerca_variabile(tabella_var, $3);
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (var == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERRORE:: variabile %s NON definita\n", lineno, $3);
        free_sequenza_numeri(&elem);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.f_val = var->valore;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    float_seq = inserisci_valore_sequenza_numeri(float_seq, elem);
}
                    |   float_sequence ',' FLOAT
{
/*
 *  Secondo step di parsing di una sequenza di numeri. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (float_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.f_val = $3;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    float_seq = inserisci_valore_sequenza_numeri(float_seq, elem);
}
                    |   float_sequence ',' INTEGER
{
/*
 *  Secondo step di parsing di una sequenza di numeri. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (float_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.f_val = (double)$3;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    float_seq = inserisci_valore_sequenza_numeri(float_seq, elem);
}
                    ;
%%