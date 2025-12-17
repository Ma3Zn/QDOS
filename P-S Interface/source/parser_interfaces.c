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

#include "../include/parser_interfaces.h"
#include "../../Common/include/global.h"

/*
 *  File contenente tutte le definizioni delle funzioni necessarie alla 
 *  creazione di sequenze di operazioni simulabili dal simulatore
 *  (questo includeil set-up degli ambienti di simulazione)
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione dei parametri di
 *  un'operazione
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione dei parametri di un'operazione
 */
parametri_operazione *alloc_parametri_operazione(size_t N)
{
    parametri_operazione *param = 
        (parametri_operazione*)malloc(sizeof(parametri_operazione));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione degli indici di connessione "
                        "per un'operazione.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    param->indici = (size_t*)calloc(N, sizeof(size_t));

    if (param->indici == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione degli indici di connessione "
                        "per un'operazione.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n");
        free_parametri_operazione(&param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    param->numero_connessioni = N;
    return param;
}

/*
 *  Funzione per la deallocazione dei parametri di un'operazione
 */
void free_parametri_operazione(parametri_operazione **param)
{
    if (param == NULL || *param == NULL)
    {
        return;
    }
    else
    {
        ;
    }

    free((*param)->indici);
    (*param)->indici = NULL;
    (*param)->numero_connessioni = 0;

    free(*param);
    *param = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di un'operazione
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un'operazione
 */
operazione *alloc_operazione(size_t N)
{
    operazione *op = (operazione*)malloc(sizeof(operazione));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (op == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un'operazione.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (N > 0)
    {

        parametri_operazione *param = alloc_parametri_operazione(N);

/*  
 *      Verifichiamo l'esito dell'allocazione
 */ 
        if (param == NULL)
        {
/*  
 *          Errore già segnalato
 */ 
            free_operazione(&op);
            return NULL;
        }
        else
        {
            ;
        }
        op->parametri = param;
    }
    else
    {
        op->parametri = NULL;
    }

    return op;
}

/*
 *  Funzione per la deallocazione di un'operazione
 */
void free_operazione(operazione **op)
{
    if (op == NULL || *op == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free_parametri_operazione(&((*op)->parametri));

    if ((*op)->tipo == SCH)
    {
        if ((*op)->val.sch != NULL)
        {
            free_schedule((*op)->val.sch);
            (*op)->val.sch = NULL;
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

    free(*op);
    *op = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'lista_operazioni'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di una 'lista_operazioni'
 */
lista_operazioni *alloc_lista_operazioni(void)
{
    lista_operazioni *lst = (lista_operazioni*)malloc(sizeof(lista_operazioni));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (lst == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di operazioni.\n"
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
 *  Funzione per la deallocazione di una 'lista_operazioni'
 */
void free_lista_operazioni(lista_operazioni **lst)
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
        (*lst)->testa = (*lst)->testa->next;
        free_operazione(&((*lst)->testa));
    }
        
    free(*lst);
    *lst = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'blocco_operazioni'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un blocco di gate
 */
blocco_operazioni *alloc_blocco_operazioni(void)
{
    blocco_operazioni *blk =
        (blocco_operazioni*)malloc(sizeof(blocco_operazioni));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (blk == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un blocco di operazioni.\n"
                        "ABORTING...\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lista_operazioni *lst = alloc_lista_operazioni();

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (lst == NULL)
    {
        free_blocco_operazioni(&blk);
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
void free_blocco_operazioni(blocco_operazioni **blk)
{
    if (blk == NULL || *blk == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do nothing */
    }

    free_lista_operazioni(&((*blk)->lst));
    (*blk)->lst = NULL;

    free(*blk);
    *blk = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un'istruzione all'interno di una
 *  'lista_operazioni'
 */
void inserisci_operazione(lista_operazioni *lst, operazione *op)
{
    if (lst == NULL || op == NULL)
    {
        fprintf(stderr, "ERRORE nell'inserimento di un'instruzione "
                        "all'interno di un blocco.\n"
                        "ABORTING...\n\n");
                        free_lista_operazioni(&lst);
                        free_operazione(&op);
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
        lst->coda->next = op;
        lst->coda = op;
    }
    else
    {
        lst->testa = op;
        lst->coda = op;
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un blocco di operazioni all'interno della
 *  tabella dei blocchi
 */
blocco_operazioni *inserisci_blocco_operazioni(blocco_operazioni *tabella,
                                                blocco_operazioni *blk)
{
    if (blk == NULL || tabella == NULL)
    {
        fprintf(stderr, "ERRORE nell'inserimento di un blocco di istruzioni "
                        "nella tabella oopertuna.\n"
                        "ABORTING...\n");
        free_blocco_operazioni(&tabella);
        free_blocco_operazioni(&blk);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inserimento in coda del nuovo blocco. Così facendo il main sarà in testa
 *  alla tabella in quanto il main, per poter includere gli altri blocchi,
 *  deve essere definito dopo di questi (nel programmma) e la generazione della
 *  tabella dei blocchi di gate viene fatta con inserimento in testa di fatto
 *  ribaltando l'ordine dei blocchi stessi mettendo il main in testa alla
 *  tabella
 */
    blocco_operazioni *tmp = tabella;

    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }

    tmp->next = blk;
    return tabella;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per verificare la presenza di un blocco di operazioni all'interno
 *  della tabella dei blocchi. Ritorna un puntatore nullo se il blocco
 *  NON è presente
 */
blocco_operazioni *cerca_blocco_operazioni(blocco_operazioni *tabella,
                                            char *nome_blocco)
{
    blocco_operazioni *tmp = tabella;
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
 *  Verifichiamo l'esito della ricerca
 */
    if (tmp == NULL)
    {
        fprintf(stderr, "Blocco %s NON definito.\n"
                        "ABORTING...\n\n", nome_blocco);
    }
    else
    {
        ;   /* Do Nothing */
    }

    return tmp;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che, forniti gli indici dei qudit coinvolti dal gate, va a 
 *  fare il set-up del sistema composto opportuno per lo scheduling
 * 
 *  TODO::  liberare la memoria punatta precedentemente dai campi che ora
 *          andiamo a riutilizzare
 */
sistema_composto *init_sistema_composto_sch(parametri_gate *param)
{
/*
 *  Liberiamo tutta la memoria che andrebbe deferenziata e dunque persa
 */
    reset_sistema_composto(comp_sys);

/*
 *  Calcoliamo la dimensione del sistema attuale
 */
    size_t dim = 1;
    for (size_t i = 0; i < param->n_qudit; ++i)
    {
        dim *= abs_sys->n_livelli[param->idx_qudit[i]];
    }

/*
 *  Variabile d'appoggio per verificare i valori di ritorno delle varie
 *  invocazioni compiute
 */
    uint8_t ris = 0;

/*
 *  Costruiamo la matrice di connettività
 */
    gsl_matrix_complex *M = gsl_matrix_complex_calloc(dim, dim);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (M == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    {
        ;   /* Do nothing */
    }

    ris = costruisci_matrice_connettivita(M, abs_sys,
                                          param->idx_qudit,
                                          param->n_qudit);
/*
 *  Verifichiamo l'esito della costruzione dlla matrice di connettività
 */
    if (ris)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if(comp_sys->M != NULL)
    {
        gsl_matrix_complex_free(comp_sys->M);
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    comp_sys->M = M;

#ifdef DEBUG
    fprintf(stdout, "\nMATRICE DI CONNESSIONE:\n");
    stampa_matrice(stdout, comp_sys->M);
#endif

/*
 *  Costruiamo l'hamiltoniana del sistema
 */
    gsl_matrix_complex *hamiltoniana = gsl_matrix_complex_calloc(dim, dim);

    for (size_t i = 0; i < param->n_qudit; ++i)
    {
        ris = aggiungi_termine(hamiltoniana,
                                abs_sys,
                                param->idx_qudit,
                                i,
                                param->n_qudit);
        if (ris > 0)
        {
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
#ifdef DEBUG
    fprintf(stdout, "\nHAMILTONIANA:\n");
    stampa_matrice(stdout, hamiltoniana);
#endif
/*
 *  Verfichiamo se il meccanismo di switch on/off delle connessioni è abilitato
 *  e in tal caso aggiungiamo tutti i termini necessari all'hamiltoniana
 */
    if (flag_connessione == CONNESSIONE_ABILITATA)
    {
/*
 *      Variabile d'appoggio che terrà traccia di quali connessioni sono già
 *      state accese
 */ 
        uint8_t flag_idx_conn[numero_qudit][numero_qudit];
/*  
 *      Recuperiamo tutte le connessioni da abilitare 
 */ 
        for (size_t i = 0; i < param->n_qudit; ++i)
        {
/* 
 *          Primo indice del qudit che ci fornisce la riga della matrice
 *          di flag
 */    
            size_t riga = param->idx_qudit[i];
            for (size_t j = i + 1; j < param->n_qudit; ++j)
            {
                size_t colonna = param->idx_qudit[j];
                if (riga <= colonna)
                {
                    flag_idx_conn[riga][colonna] = 1u;
                }
                else
                {
                    flag_idx_conn[colonna][riga] = 1u;
                }
            }
        }

/*  
 *      Aggiungiamo tutti i termini di connessioni dovuti ricordando che
 *                                  J_ij = J_ji
 */ 
        for (size_t i = 0; i < abs_sys->n_qudit; ++i)
        {
            for (size_t j = i + 1; j < abs_sys->n_qudit; ++j)
            {
/*  
 *              Verifichiamo se aggiungere o meno il termine di connessione tra
 *              il qudit i-esimo ed il qudit j-esimo
 */ 
                if (flag_idx_conn[i][j] == 1u)
                {
                    size_t idx[2] = {i,j};
                    ris =
                        aggiungi_termine_connesso(hamiltoniana,
                                                    abs_sys,
                                                    idx,
                                                    param->idx_qudit,
                                                    param->n_qudit);
/*  
 *                  Verifichiamo l'esito dell'operazione
 */ 
                    if(ris)
                    {
                        gsl_matrix_complex_free(hamiltoniana);
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
            }
        }
    }
    else
    {
        ;   /* Do Nothing */
    }
#ifdef DEBUG
    fprintf(stdout, "\nHAMILTONIANA:\n");
    stampa_matrice(stdout, hamiltoniana);
#endif
/*
 *  Allochiamo la memoria per gli autovalori e gli autovettori della
 *  Hamiltoniana
 */
    gsl_matrix_complex *autovettori = gsl_matrix_complex_calloc(dim, dim);
    gsl_vector *autovalori = gsl_vector_calloc(dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (autovettori == NULL || autovalori == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazine della memoria per gli "
                        "autovalori o gli autovettori del Hamiltoniana.\n"
                        "ABORTING...\n\n");
        gsl_matrix_complex_free(hamiltoniana);
        gsl_matrix_complex_free(autovettori);
        gsl_vector_free(autovalori);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Diagonalizzazione del Hamiltoniana
 */
    ris = cerca_autovalori(hamiltoniana, autovettori, autovalori);
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (ris > 0)
    {
        gsl_matrix_complex_free(hamiltoniana);
        gsl_matrix_complex_free(autovettori);
        gsl_vector_free(autovalori);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

#ifdef DEBUG
    fprintf(stdout, "\nAUTOVETTORI:\n");
    stampa_matrice(stdout, autovettori);
#endif

/*
 *  Copiamo gli autovalori
 */
    if (comp_sys->autovalori != NULL)
    {
        gsl_vector_free(comp_sys->autovalori);
    }
    else
    {
        ;   /* Do Nothin */
    }
    comp_sys->autovalori = autovalori;
/*
 *  Copiamo la matrice di connessione e la ruotiamo sulla base degli
 *  autovettori
 */
    cambio_base(comp_sys->M, autovettori);

#ifdef DEBUG
    cambio_base(hamiltoniana, autovettori);
    fprintf(stdout, "\nHAMILTONIANA:\n");
    stampa_matrice(stdout, hamiltoniana);

    fprintf(stdout, "\nMATRICE DI CONNESSIONE:\n");
    stampa_matrice(stdout, comp_sys->M);
#endif

/*
 *  Liberiamo la memoria allocata
 */
    gsl_matrix_complex_free(hamiltoniana);
    gsl_matrix_complex_free(autovettori);
    
    return comp_sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che esegue uno shift dei livelli delle rotazioni in base ai qudit
 *  a cui devono essere applicate le rotazioni
 */
uint8_t shift_livelli_schedule(schedule *sch, parametri_gate *param)
{
    size_t n_qudit_gate = param->n_qudit;

    size_t idx_qudit_ordinato[n_qudit_gate];

    uint8_t flag[numero_qudit];
    for (size_t i = 0; i < numero_qudit; ++i)
    {
        flag[i] = 0;
    }

    for (size_t i = 0; i < n_qudit_gate; ++i)
    {
        flag[param->idx_qudit[i]] = 1;
    }

    size_t n = 0;
    for (size_t i = 0; i < numero_qudit; ++i)
    {
        if (flag[i] == 1)
        {
            idx_qudit_ordinato[n] = i;
            ++n;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

/*
 *  Calcoliamo il massimo livello raggiungibile con i qudit
 *  in uso
 */
    size_t lv_max = 1;

    for (size_t i = 0; i < n_qudit_gate; ++i)
    {
        lv_max *= dimensioni_qudit[param->idx_qudit[i]];
    }

/*
 *  Calcoliamo tutti gli shift dei livelli
 */
    size_t shift_livelli[lv_max];
    shift_livelli[0] = 0;

/*
 *  Range livelli per iterazione
 */
    size_t d = 0;
    size_t D = 1;

    for (int i = n_qudit_gate - 1; i >= 0; --i)
    {
/*
 *      Set-up range livelli shift
 */
        d = D;
        D *= dimensioni_qudit[idx_qudit_ordinato[i]];
/*
 *      Indice del qudit che stiamo cinsiderando ora
 */
        size_t idx_qudit_attuale = idx_qudit_ordinato[i];
/*
 *      Dimensione dello spazio 'a destra' del qudit attuale.
 *      Se tale spazio è vuoto settiamo comunque la dimensione ad 1.
 *      In tal modo potremo usare sempre lo stesso algoritmo per il calcolo
 *      dello shift del livello
 */
        size_t dim_sp_dx = 0;
/*
 *      Calcoliamo il valore di 'dim_sp_dx'
 */
        for (size_t j = idx_qudit_attuale + 1; j < numero_qudit; ++j)
        {
            if (j == idx_qudit_attuale + 1)
            {
                dim_sp_dx = 1;
            }
            else
            {
                ;   /* Do Nothing */
            }

            dim_sp_dx *= dimensioni_qudit[j];
        }

/*
 *      Calcoliamo lo shift dei livelli relativi al qudit attuale
 */
        size_t base = 0;
        size_t base_val = 1;
        for (size_t j = d; j < D; ++j)
        {
            shift_livelli[j]  = dim_sp_dx == 0 ? base_val : dim_sp_dx * base_val;
            shift_livelli[j] += shift_livelli[base];

            ++base;

            if (base >= d)
            {
                base = 0;
                ++base_val;
            }
        }
    }

    for (size_t i = 0; i < sch->dim; ++i)
    {
        sequenza_impulsi *seq_imp = sch->arr_sequenze[i];
        
        for (size_t j = 0; j < seq_imp->dim; ++j)
        {
            impulsi_paralleli *imp_par = seq_imp->arr_impulsi_paralleli[j];

            for (size_t k = 0; k < imp_par->dim; ++k)
            {
                impulso *imp = imp_par->arr_impulsi[k];

                imp->livelli[0] = shift_livelli[imp->livelli[0]];
                imp->livelli[1] = shift_livelli[imp->livelli[1]];
            }
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un operatore derivante dall'invocazione di un gate
 *  lo traduce in una schedule
 */
schedule *traduci_operatore_gate(gate_operator *operatore)
{
    sequenza_rotazioni *seq_rot = NULL;
    schedule *sch = NULL;

/*
 *  Verifichiamo il tipo dell'operatore
 */
    switch (operatore->type)
    {
    case COD_UNT:
/*
 *      Decomponiamo l'unitaria ottenuta in una sequenza di
 *      rotazioni
 */
        seq_rot = decomponi(operatore->val.unt);
/*      
 *      Verifichiamo l'esito della decomposizione
 */
        if (seq_rot == NULL)
        {
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

    case COD_SQR:
/*
 *      Controlliamo se arriviamo qui come esito di un fall-trhough
 *      dal caso precedente o se è questo il primo caso da eseguire
 */
        if (seq_rot == NULL)
        {
/*
 *          Inizializziamo la sequenza di rotazioni
 */
            seq_rot = operatore->val.seq;         
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Traduciamo la sequenza di rotazioni in una schedule da
 *      poter far eseguire al simulatore
 */
        sch = crea_schedule(seq_rot, comp_sys);
/*
 *      Verifichiamo l'esito della traduzione
 */
        if (sch == NULL)
        {
            free_seq_rot(&seq_rot);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    case COD_SCH:
/*
 *      Controlliamo se arriviamo qui come esito di un fall-trhough
 *      dal caso precedente o se è questo il primo caso da eseguire
 */
        if (sch == NULL)
        {
/*
 *          Inizializziamo la schedule
 */
            sch = operatore->val.sch;
        }
        else
        {
            ;   /* Do Nothing */
        }
    default:
        break;
    }

    free_seq_rot(&seq_rot);

/*
 *  Abbiamo eseguito un free della sequenza contenuta nell'operatore.
 *  Dobbiamo propagare questa informazione per evitare un doppio free.
 */
    if (operatore->type == COD_SQR)
    {
        operatore->val.seq = NULL;
    }
    else if (operatore->type == COD_SCH)
    {
        operatore->val.sch = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return sch;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un gate g lo traduce in schedule
 */
schedule *traduci_gate(gate *g)
{
    gate_operator *operatore = NULL;
    FILE *f_g = NULL;

/*
 *  Inizializziamo il sistema composto
 */
    comp_sys = init_sistema_composto_sch(g->parametri);

/*
 *  Calcoliamo le dimensione dell'unitaria da eseguire
 */
    size_t dim = 1;
    for (size_t i = 0; i < g->parametri->n_qudit; ++i)
    {
        dim *= dimensioni_qudit[g->parametri->idx_qudit[i]];
    }

/*
 *  Verifichiamo l'esito dell'inizializzazione
 */
    if (comp_sys == NULL)
    {
        fprintf(stderr, "ERRORE nell'inizializzazione del sistema composto "
                        " per la traduzione dei gate.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo che tipo di gate abbiamo
 *
 *  Qui accettiamo solo BLTN e CSTM gate, qualsiasi altra cosa è da intendersi
 *  come un errore
 */
    switch(g->type)
    {
        case BLTN:
/*
 *          Eseguiamo la funzione associata al gate e recuperiamo l'operatore
 *          corrispondente
 */
            operatore = g->operatore.func_ptr(g->parametri);
/*
 *          Verifichiamo l'esito dell'operazione
 */
            if (operatore == NULL)
            {
                fprintf(stderr, "ERRORE nell'esecuzione della funzione relativa "
                                "al gate built-in: %s\n"
                                "ABORTING...\n\n", g->nome_gate);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
            break;

        case CSTM:
/*
 *          Apriamo il file contenente le info relative all'operatore associato
 *          al gate
 */
            f_g = fopen(g->operatore.path_to_file, "r");
/*
 *          Verifichiamo l'apertura del file
 */
            if (f_g == NULL)
            {
                fprintf(stderr, "ERRORE nell'apertura del file.\n"
                                "%s no such file or directory.\n"
                                "ABORTING...\n\n",
                                g->operatore.path_to_file);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
/*
 *          Allochiamo un'operatore da riempire con le info contenute nel file
 */
            operatore = alloc_gate_operator();
/*
 *          Verifichiamo l'esito dell'allocazione
 */
            if (operatore == NULL)
            {
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
/*
 *          Recuperiamo il correto tipo dell'operatore
 */
            size_t codice_gate;
            size_t ris = fread(&codice_gate, sizeof(size_t), 1, f_g);
/*
 *          Inizializziamo in modo appropriato l'operatore
 */
            switch(codice_gate)
            {
                case COD_UNT:
                    operatore->type = COD_UNT;
/*
 *                  Allochiamo l'unitaria da eseguire
 */
                    operatore->val.unt = gsl_matrix_complex_calloc(dim, dim);
/*
 *                  Verifichiamo l'esito dell'allocazione
 */
                    if (operatore->val.unt == NULL)
                    {
                        fprintf(stderr, "ERRORE nell'allocazione "
                                        "dell'unitaria relativa al custom "
                                        "gate: %s\n"
                                        "ABORTING...\n\n", g->nome_gate);
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing */
                    }
/*
 *                  Leggiamo l'unitaria da simulare
 */
                    ris = gsl_matrix_complex_fread(f_g, operatore->val.unt);
/*
 *                  Verifichiamo l'esito della lettura
 */
                    if (ris == GSL_EFAILED)
                    {
                        fprintf(stderr, "ERRORE nella lettura dell'unitaria "
                                        "associata al custom gate: %s.\n"
                                        "ABORTING...\n", g->nome_gate);
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing */
                    }
                    break;

                case COD_SQR:
                    operatore->type = COD_SQR;
/*
 *                  Leggiamo la sequenza di rotazioni da eseguire
 */
                    operatore->val.seq = leggi_sequenza_rotazioni(f_g);
/*
 *                  Verfichiamo l'esito della lettura
 */
                    if (operatore->val.seq == NULL)
                    {
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing */
                    }
                    break;

                case COD_SCH:
                    operatore->type = COD_SCH;
/*
 *                  Leggiamo la schedule
 */            
                    operatore->val.sch = leggi_schedule(f_g,
                                            comp_sys->M,
                                            comp_sys->autovalori);
/*             
 *                  Verififchiamo l'esito della lettura della schedule
 */            
                    if (operatore->val.sch == NULL)
                    {
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing */
                    }
                    break;

                default:
                    fprintf(stderr, "ERRORE nel codice identificativo del "
                                    "custom gate: %s\n"
                                    "codice letto: %ld\n"
                                    "codici ammissibili:\n"
                                    "\t\tUnitaria --> %d\n"
                                    "\t\tSequenza Rotazioni --> %d\n"
                                    "\t\tSchedule --> %d\n"
                                    "ABORTING...\n\n",
                                    g->nome_gate, codice_gate,
                                    CODICE_UNITARIA,
                                    CODICE_SEQUENZA_ROTAZIONI,
                                    CODICE_SCHEDULE);
                    return NULL;
                    break;
            }
            break;
            
        default:
#ifdef DEBUG
            fprintf(stderr, "ERRORE nel tipo di gate da tradurre.\n"
                            "ABORTING...\n\n");
#endif
            return NULL;
            break;
    }
/*
 *  Traduziamo l'operatore ricavato dall'esecuzione del gate in una schedule
 */
    schedule *sch = traduci_operatore_gate(operatore);

#ifdef DEBUG
    stampa_schedule(stdout, sch);
#endif

/*
 *  Parallelizziamo la schedule
 */
    // sch = parallelizza_schedule(sch, dim);
/*
 *  Verifichiamo l'esito della parallelizzazione
 */
    if (sch == NULL)
    {
        free_gate_operator(&operatore);
        free_schedule(sch);
        return NULL;
    }
    else
    {
        ; /* Do Nothing */
    }

/*
 *  Eseguiamo uno shift dei livelli delle rotazioni in
 *  quanto dobbiamo traslarle sul sitema completo
 * 
 *  Devi tenere conto dell'ordinamento fisico dei 
 *  livelli
 */
    uint8_t ris = shift_livelli_schedule(sch, g->parametri);

    if (ris > 0)
    {
        free_gate_operator(&operatore);
        free_schedule(sch);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

#ifdef DEBUG
    stampa_schedule(stdout, sch);
#endif

    free_gate_operator(&operatore);
    return sch;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data un'istruzione la traduce in un'operazione
 */
operazione *traduci_istruzione(istruzione **ist)
{
    if (ist == NULL || *ist == NULL)
    {
        return NULL;
    }
    else
    {
        ;
    }

    operazione *op = NULL;

    if ((*ist)->tipo == GATE_INST)
    {
        gate *g = (*ist)->val.g;

/* 
 *      Recuperiamo gli indici dei qudit da utilizzare per
 *      l'inizializzazione del sistema composto
 */
        size_t n_qudit = 0;
        if (g->type != BLCK)
        {
            n_qudit = g->parametri->n_qudit;

            if (n_qudit == 0u)
            {
                fprintf(stderr, "ERRORE nell'utilizzo del gate %s.\n"
                                "Non è stato specificato nessun qudit "
                                "su cui applicarlo.\n"
                                "ABORTING...\n\n", g->nome_gate);
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

        op = alloc_operazione(n_qudit);
/*  
 *      Verifichiamo l'esito dell'allocazione
 */ 
        if (op == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione di un'operazione.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_operazione(&op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
        op->next = NULL;

        switch (g->type)
        {
            case BLTN:
/*
 *              fall-through to CSTM
 */
            case CSTM:
/*
 *              Inseriamo gli indici dei qudit nei parametri
 *              dell'operazione
 */
                for (size_t i = 0; i < n_qudit; ++i)
                {
                    size_t idx = g->parametri->idx_qudit[i];

                    if (idx > numero_qudit - 1)
                    {
                        fprintf(stderr, "ERRORE negli indici dei qudit del "
                                        "gate: %s\n"
                                        "Indice %ld-esimo fornito: %ld\n"
                                        "Indice massimo consentito: %ld\n"
                                        "ABORTING...\n\n",
                                        g->nome_gate,
                                        i, idx,
                                        numero_qudit - 1);
                        free_operazione(&op);
                        return NULL;
                    }

                    op->parametri->indici[i] = idx;
                }
/*
 *              Creiamo la schedule relativa al gate attuale
 */
                schedule *sch = traduci_gate(g);
/*
 *              Verifichiamo l'esito della traduzione
 */
                if (sch == NULL)
                {
                    free_operazione(&op);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }

                op->val.sch = sch;
                op->tipo = SCH;
                break;

            case MSNT:
/*
 *              Inizializziamo gli indici dei qudit da misurare
 */
                for (size_t i = 0; i < op->parametri->numero_connessioni; ++i)
                {
                    op->parametri->indici[i] = g->parametri->idx_qudit[i];
                }

                op->val.sch = NULL;
                op->tipo = MSN;
                break;

            case BLCK:
                free_parametri_operazione(&(op->parametri));
                op->val.nome_blocco = g->operatore.blk_ptr->nome_blocco;
                op->parametri = NULL;
                op->tipo = BLK;
                break;

            default:
                fprintf(stderr, "ERRORE nella traduzione di un'istruzione.\n"
                                "Tipo gate associato NON valido.\n"
                                "ABORTING...\n\n");
                free_operazione(&op);
                return NULL;
                break;
        }
    }
    else if ((*ist)->tipo == CUSTOM_LOGIC_INST)
    {
        op = alloc_operazione(0);

        op->val.csl = (*ist)->val.custom_logic_function;
        op->next = NULL;
        op->tipo = CSL;
    }
    else
    {
        fprintf(stderr, "ERRORE nella traduzione di un'istruzione.\n"
                        "Tipo istruzione sconosciuto.\n"    
                        "ABORTING...\n\n");
        return NULL;
    }

    return op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un blocco di gate lo traduce in un blocco di
 *  operazioni
 */
blocco_operazioni *traduci_blocco_gate(blocco_gate *gate_blk)
{
    if (gate_blk == NULL)
    {
        fprintf(stderr, "ERRORE nella traduzione di un blocco di gate.\n"
                        "Blocco fornito NULLO.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo il blocco di operazioni da eseguire
 */
    blocco_operazioni *op_blk = alloc_blocco_operazioni();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (op_blk == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un blocco di "
                        "operazioni.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Copiamo il nome del blocco di gate da tradurre
 */
    op_blk->nome_blocco = gate_blk->nome_blocco;

    istruzione *ist_attuale = gate_blk->lst->testa;

/*
 *  Traduciamo una ad una le varie istruzioni della lista che compone il
 *  blocco
 */
    while (ist_attuale != NULL)
    {
        operazione *op = traduci_istruzione(&ist_attuale);

/*
 *      Verifichiamo l'esito della traduzione
 */
        if (op == NULL)
        {
            free_blocco_operazioni(&op_blk);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Inseriamo nel blocco l'operazione appena ricavata
 */
        inserisci_operazione(op_blk->lst, op);

/*
 *      Dobbiamo controllare che ist_attuale non sia NULLA in quanto anche
 *      traduci_istruzione() può far avanzare il puntatore
 */
        if (ist_attuale != NULL)
        {
            ist_attuale = ist_attuale->next;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    return op_blk;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una tabella di blocchi di gate li traduce e li installa
 *  in una tabella di blocchi di operazioni
 */
blocco_operazioni *traduci_tabella_blocchi(blocco_gate *tabella_gate)
{
    if (tabella_gate == NULL)
    {
        fprintf(stderr, "ERRORE nella traduzione della tabella di gate.\n"
                        "Tabella fornita NULLA.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    blocco_operazioni *tabella_operazioni = alloc_blocco_operazioni();

/*
 *  NON vogliamo modificare il puntatore passato in input
 */
    blocco_gate *ptr_copy = tabella_gate;

    while (ptr_copy->next != NULL)
    {
        blocco_operazioni *blk_op = traduci_blocco_gate(ptr_copy);

/*
 *      Verifichiamo l'esito della traduzione
 */
        if (blk_op == NULL)
        {
            free_blocco_operazioni(&tabella_operazioni);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Inseriamo il nuovo blocco di operazioni all'interno della tabella
 *      delle operazioni
 */
        inserisci_blocco_operazioni(tabella_operazioni, blk_op);

        ptr_copy = ptr_copy->next;
    }

    return tabella_operazioni;
}
