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

#include "../include/global.h"
#include "../include/physical_system.h"

/*
 *  File contenente le definizioni di tutte le variabili globali
 *  necessarie al parsing e al set-up del simulatore
 */

//---------------------------------------------------------------------------//

/*
 *  Dichiarazione del flag che ci dice se il meccanismo di connessione tra
 *  qudit è abilitato o meno
 * 
 *  Di default la connessione è abilitata
 */
uint8_t flag_connessione = CONNESSIONE_ABILITATA;

/*
 *  Dichiarazione di una sequenza di numeri per per una 'integer_sequence'
 *  Abbiamo bisogno di due sequenze distinte in quanto ci ritroveremo a fare
 *  il parsing di più sequenze contemporaneamente prima di poterle installare
 *  nelle tabelle
 */
sequenza_numeri *int_seq = NULL;

/*
 *  Dichiarazione di una sequenza di numeri per una 'float_sequence'
 */
sequenza_numeri *float_seq = NULL;

/*
 *  Inizializzazione della lista delle definizioni
 */
lista_definizioni_gate *lst_def = NULL;

/*
 *  Dichiarazione di un'istruzione di supporto che conterrà l'istruzione 
 *  osservata ma non ancora inserita nella lista di gate che forma il corpo
 *  di un blocco di gate
 */
istruzione *ist_supporto = NULL;

/*
 *  Lista di gate d'appoggio che verràutilizzata durante la riduzione di una
 *  sequenza di istuzioni, e poi nella riduzione NAME = { inst_sequence };
 *  verrà copiata nellàopportuno blocco
 */
lista_gate *lst_gate_supporto = NULL;

/*
 *  Inizializzazione della tabella dei blocchi
 */
blocco_gate *tabella_blk = NULL;

/*
 *  Dichiarazione della tabella delle variabili
 */
variabile *tabella_var = NULL;

/*
 *  Numero della linea attuale di parsing
 */
size_t lineno = 1u;

/*
 *  Variabile che conterrà il numero di qudit di cui è composto il sistema
 */
size_t numero_qudit = 0u;

/*
 *  Array che conterrà le dimensioni dei qudit di cui è composto il sistema
 */
size_t *dimensioni_qudit = NULL;

/*
 *  Variabile che ci dice se le simulazioni comprendono errore o meno
 */
uint8_t errore_abilitato = 0;

/*
 *  Probabilità di errore in una misura
 */
double p_errore = 0.0;

/*
 *  Numero di misure da tenere in memoria
 */
size_t memoria_misure = MEM_MISURE;

/*
 *  Lista delle misure eseguite
 */
lista_misure *tabella_mis = NULL;

/*
 *  Sistema astratto
 */
sistema_astratto *abs_sys = NULL;

/*
 *  Sistema composto
 */
sistema_composto *comp_sys = NULL;

/*
 *  RNG del programma necessario per l'esecuzione delle misure
 */
gsl_rng *rng = NULL;

/*
 *  Numero di misurazioni finali del sistema da eseguire. In questo modo
 *  simuliamo l'esecuzione del circuito 'n_shots' volte
 */
size_t n_shots = 0;

/*
 *  Frequenza output
 */
size_t freq_output = 32;

/*
 *  Directory file di input
 */
char *in_dir = "./input/";

/*
 *  Directory file di output
 */
char *out_dir = "./output/";

/*
 *  Output finale della matrice di densità
 */
size_t out_rho_final = 2;

/*
 *  Output della matrice di densità durante la simualzione
 */
size_t out_rho_simulation = 2;

/*
 *  Output della sequenza di rotazioni eseguita
 */
size_t out_rotation = 2;

/*
 *  Output della schedule eseguita
 */
size_t out_schedule = 2;

/*
 *  Simulazione da eseguire o meno
 */
size_t simulation_enabled = 1;

/*
 *  Output avanzamento simulazione
 */
size_t simulation_progress = 2;

/*
 *  Versione programma
 */
char *versione_programma = "QDOS 1.0.0";

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione delle variabili globali
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di tutte le variabilli globali che lo richiedono
 */
uint8_t alloc_globals(void)
{
    rng = gsl_rng_alloc(gsl_rng_ranlxs2);
    tabella_var = alloc_variabile();
    tabella_blk = alloc_blocco_gate();
    lst_def = alloc_lista_definizioni_gate();
    tabella_mis = alloc_lista_misure(memoria_misure);

    if (    rng == NULL     || tabella_var == NULL || tabella_blk == NULL 
        ||  lst_def == NULL || tabella_mis == NULL)
    {
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return 0;
}

/*
 *  Funzione per la deallocazione di tutte le variabilli globali che lo
 *  richiedono
 */
void free_globals(void)
{
    free_sequenza_numeri(&int_seq);
    free_sequenza_numeri(&float_seq);

    free_lista_definizioni_gate(lst_def);

    free_istruzione(&ist_supporto);
    free_lista_gate(&lst_gate_supporto);

    blocco_gate *tmp = NULL;
    do {
        tmp = tabella_blk->next;
        free_blocco_gate(tabella_blk);
        tabella_blk = tmp;
    } while(tabella_blk != NULL);

    free_lista_variabili(&tabella_var);

    free(dimensioni_qudit);

    free_sistema_composto(comp_sys);
    free_sistema_astratto(abs_sys);

    free_lista_misure(&tabella_mis);

    gsl_rng_free(rng);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la concatenazine di due stringhe. Necessaria nella corretta
 *  inizializzazione delle interfacce di input e di output
 */
char *concatena(char *s_root, char *s_postfix)
{
    char *result = malloc(strlen(s_root) + strlen(s_postfix) + 1); 
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (result == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    strcpy(result, s_root);
    strcat(result, s_postfix);
    return result;
}