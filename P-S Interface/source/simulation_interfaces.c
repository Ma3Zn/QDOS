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

#include "../include/simulation_interfaces.h"
#include "../../Simulator/include/measure.h"
#include "../../Common/include/output.h"

/*
 *  File che contiene le dichiarazioni delle funzioni necessarie per la
 *  gestione dei dati di input del programma e il set-up degli
 *  ambienti di simulazione. Si occuperà anche del parsing delle
 *  opzioni passate dall'utente al programma, ed in base a queste
 *  effettuerà la corretta gestione delle simulazioni richieste.
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione che, forniti gli indici dei qudit coinvolti dal gate, va a 
 *  fare il set-up del sistema composto opportuno per la simulazione
 * 
 *  TODO::  liberare la memoria punatta precedentemente dai campi che ora
 *          andiamo a riutilizzare
 */
sistema_composto *init_sistema_composto_sim(parametri_operazione *param)
{
/*
 *  Liberiamo tutta la memoria che andrebbe deferenziata e dunque persa
 */
    reset_sistema_composto(comp_sys);

    size_t dim = abs_sys->dim;
/*
 *  Variabile d'appoggio per verificare i valori di ritorno delle varie
 *  invocazioni compiute
 */
    uint8_t ris = 0;
    size_t N = param->numero_connessioni;

/*
 *  Vettore d'appoggio necessario per la funzione di connessione dei qudit
 */
    size_t indici_qudit[abs_sys->n_qudit];
    for (size_t i = 0; i < abs_sys->n_qudit; ++i)
    {
        indici_qudit[i] = i;
    }
/*
 *  Vettore d'appoggio
 */
    size_t idxs[abs_sys->n_qudit];
    for (size_t i = 0; i < abs_sys->n_qudit; ++i)
    {
        idxs[i] = i;
    }

/*
 *  Costruiamo l'hamiltoniana del sistema
 */
    gsl_matrix_complex *hamiltoniana = gsl_matrix_complex_calloc(dim, dim);

    for (size_t i = 0; i < abs_sys->n_qudit; ++i)
    {
        ris = aggiungi_termine(hamiltoniana,
                                abs_sys,
                                idxs,
                                i,
                                abs_sys->n_qudit);

        if (ris)
        {
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

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
        for (size_t i = 0; i < N; ++i)
        {
/* 
 *          Primo indice del qudit che ci fornisce la riga della matrice
 *          di flag
 */    
            size_t riga = param->indici[i];
            for (size_t j = i + 1; j < N; ++j)
            {
                size_t colonna = param->indici[j];
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
                                                    indici_qudit,
                                                    abs_sys->n_qudit);
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
    if (comp_sys->M != NULL)
    {
        gsl_matrix_complex_free(comp_sys->M);
    }
    comp_sys->M = gsl_matrix_complex_calloc(abs_sys->dim, abs_sys->dim);

    if (comp_sys->M == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /*  Do Nothing */
    }

    gsl_matrix_complex_memcpy(comp_sys->M, abs_sys->M_tot);
    cambio_base(comp_sys->M, autovettori);

#ifdef DEBUG
    cambio_base(hamiltoniana, autovettori);
    fprintf(stdout, "\nHAMILTONIANA:\n");
    stampa_matrice(stdout, hamiltoniana);

    fprintf(stdout, "\nMATRICE DI CONNESSIONE:\n");
    stampa_matrice(stdout, comp_sys->M);
#endif

/*
 *  Verifichiamo se è presente o meno l'errore
 */
    if (abs_sys->operatore_errore != NULL)
    {
        for (size_t i = 0; i < abs_sys->n_qudit; ++i)
        {
/*
 *          Copiamo gli operatori di errore e li ruotiamo sulla base degli
 *          autovettori
 */
            gsl_matrix_complex_memcpy(comp_sys->operatore_errore[i],
                                       abs_sys->operatore_errore[i]);
            cambio_base(comp_sys->operatore_errore[i], autovettori);
/*
 *          Copiamo i ratei di dephasing degli operatori d'errore
 */
            comp_sys->rateo[i] = abs_sys->rateo[i];
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Liberiamo la memoria allocata
 */
    gsl_matrix_complex_free(hamiltoniana);
    gsl_matrix_complex_free(autovettori);

    return comp_sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che conta il numero di operazioni all'interno di un blocco
 */
size_t conta_numero_operazioni_blocco(blocco_operazioni *blk)
{
    operazione *copy_ptr = blk->lst->testa;
    size_t n_op = 0;

    while (copy_ptr != NULL)
    {
        ++n_op;
        copy_ptr = copy_ptr->next;
    }
    
    return n_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un blocco di operazioni ed un ambiente di simulazione
 *  esegue la simulazione del blocco NON stesso utilizzando lo switch on/off
 *  delle connessioni dei qudit
 */
simulazione *simula_blocco_operazioni(blocco_operazioni *blk,
                                        simulazione *sim,
                                        blocco_operazioni *tabella_op)
{
    size_t n_op_attuale = 1;
    size_t n_op_totale = 0;
/*
 *  Contiamo il numero di operazioni presenti nel blocco per l'output di
 *  avanzamento
 */
    if (sim->out->avanzamento_simulazione > 0)
    {
        n_op_totale = conta_numero_operazioni_blocco(blk);
        fprintf(stdout, "\nINIZIO simulazione blocco di gate: %s\n",
                        blk->nome_blocco);
    }
    else
    {
        ;
    }

    blocco_operazioni *next_blk = NULL;

/*
 *  Variabile d'appoggio che conterrà l'esito delle varie operazioni
 *  compiute in seguito
 */
    uint8_t ris = 0;
/*
 *  Recuperiamo la prima operazione da simulare
 */
    operazione *op = blk->lst->testa;

/*
 *  Cicliamo sulle operazioni da eseguire
 */
    while (op != NULL)
    {
/*
 *  Output stato di avanzamento
 */
    if (sim->out->avanzamento_simulazione > 1)
    {
        fprintf(stdout, "Simulazione operazione %ld/%ld",
                        n_op_attuale,
                        n_op_totale);
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Individuiamo il tipo dell'operazione
 */
        switch (op->tipo)
        {
            case SCH:
/*
 *              Output stato di avanzamento
 */
                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "\t(gate)\n");
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Inizializziamo in modo corretto il sistema composto per la
 *              simulazione
 */
                comp_sys = init_sistema_composto_sim(op->parametri);
/*
 *              Verifichiamo l'esito dell'inizializzazione
 */
                if (comp_sys == NULL)
                {
                    fprintf(stderr, "ERRORE nell'inizializzazione del sistema "
                                    "fisico per la simulazione di una "
                                    "schedule.\n"
                                    "ABORTING...\n\n");
                    free_simulazione(sim);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Correggiamo le durate degli impulsi della schedule
 */
                ris = correggi_impulsi(comp_sys, op->val.sch);
#ifdef DEBUG
                stampa_schedule(stdout, op->val.sch);
#endif
/*
 *              Verifichiamo l'esito della correzione
 */
                if (ris > 0)
                {
                    free_simulazione(sim);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              set-up della simulazione da eseguire
 */
                sim->sch = op->val.sch;
                sim->sys = comp_sys;
/*
 *              Eseguiamo la simulazione della schedule
 */
                ris = simula(sim);
/*
 *              Verifichiamo l'esito della simulazione
 */
                if (ris == 1)
                {
                    free_simulazione(sim);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Verifichiamo se dobbiamo o meno eseguire l'output della
 *              schedule eseguita
 */
                controllo_output_schedule(sim->out, sim->sch);

                break;

            case BLK:
/*
 *              Output stato di avanzamento
 */
                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "\t(blocco)\n");
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Recuperiamo il blocco da simulare
 */
                next_blk =
                    cerca_blocco_operazioni(tabella_op, op->val.nome_blocco);
/*
 *              Verifichiamo l'esito della ricerca
 */
                if (next_blk == NULL)
                {
                    free_simulazione(sim);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Lanciamo la simulazione del nuovo blocco
 */
                sim = simula_blocco_operazioni(next_blk, sim, tabella_op);
/*
 *              Verifichiamo l'esito dell'operazione
 */
                if (sim == NULL)
                {
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }

/*
 *              Output stato di avanzamento
 */
                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "\n");
                }
                else
                {
                    ;   /* Do Nothing */
                }

                break;

            case MSN:
/*
 *              Output stato di avanzamento
 */
                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "\t(misura)\n");
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Creazione di parametri fittizi per l'operazione
 */
                parametri_operazione *tmp_param =
                    alloc_parametri_operazione(1);
/*
 *              Verifichiamo l'esito dell'allocazione
 */
                if (tmp_param == NULL)
                {                    
                    free_simulazione(sim);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Settiamo un indice fittizio
 */
                tmp_param->indici[0] = 0;
/*
 *              Inizializziamo in modo corretto il sistema composto per la
 *              simulazione
 */
                comp_sys = init_sistema_composto_sim(tmp_param);
/*
 *              Verifichiamo l'esito dell'inizializzazione
 */
                if (comp_sys == NULL)
                {
                    fprintf(stderr, "ERRORE nell'inizializzazione del sistema "
                                    "fisico per la simulazione di una "
                                    "schedule.\n"
                                    "ABORTING...\n\n");
                    free_simulazione(sim);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              set-up della simulazione da eseguire
 */
                sim->sch = NULL;
                sim->sys = comp_sys;
/*
 *              Eseguiamo la misura
 */
                sim = esegui_misura(sim, 
                                    op->parametri->indici,
                                    op->parametri->numero_connessioni,
                                    1);
                break;

            case CSL:
                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "\t(custom logic)\n");
                }
                else
                {
                    ;   /* Do Nothing */
                }
/*
 *              Eseguiamo la funzione di custom logic richiesta
 */
                blocco_gate *gate_blk = op->val.csl(tabella_mis);
/*
 *              Verifichiamo l'esito dell'invocazione
 */
                if (gate_blk == NULL)
                {
                    fprintf(stderr, "ERRORE nell'esecuzione della custom logic"
                                    " richiesta.\n"
                                    "Blocco gate restituito NULLO.\n"
                                    "ABORTING...\n\n");
                    free_simulazione(sim);
                    return NULL;
                }
                else if (gate_blk->lst == NULL)
                {
                    fprintf(stderr, "ATTENZIONE il blocco fornito dalla "
                                    "custom logic e' vuoto.\n");
                }
                else
                {
                    blocco_operazioni *op_blk = traduci_blocco_gate(gate_blk);
/*
 *                  Verifichiamo l'esito della traduzione
 */
                    if (op_blk == NULL)
                    {
                        fprintf(stderr, "ERRORE nella traduzione del blocco "
                                        "di gate generato dalla custom "
                                        "logic.\n"
                                        "ABORTING...\n\n");
                        free_simulazione(sim);
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing*/
                    }
/*
 *                  Simuliamo il blocco di operazioni
 */
                    sim = simula_blocco_operazioni(op_blk, sim, tabella_op);
/*
 *                  Verifichiamo l'esito della simulazione
 */
                    if (op_blk == NULL)
                    {
                        fprintf(stderr, "ERRORE nella simulazione del blocco "
                                        "di gate generato dalla custom "
                                        "logic.\n"
                                        "ABORTING...\n\n");
                        free_simulazione(sim);
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing*/
                    }

/*
 *                  Output stato di avanzamento
 */
                    if (sim->out->avanzamento_simulazione > 1)
                    {
                        fprintf(stdout, "\n");
                    }
                    else
                    {
                        ;   /* Do Nothing */
                    }
                }

                break;

            default:
                fprintf(stderr, "ERRORE nella simulazione di un'operazione.\n"
                                "Tipo operazione sconosciuto.\n"
                                "ABORTING...\n\n");
                free_simulazione(sim);
                return NULL;
                break;
        }
/*
 *      Avanziamo nella lista delle operazioni
 */
        op = op->next;
        ++n_op_attuale;
    }

    if (sim->out->avanzamento_simulazione > 0)
    {
        fprintf(stdout, "FINE simulazione blocco di gate: %s\n",
                        blk->nome_blocco);
    }
    else
    {
        ;
    }

    return sim;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che nel caso di simulazioni con switch on/off delle connessioni
 *  tra qudit abilitate (-J1) gestirà la corretta simulazione del circuito
 *  fornito come input.
 *  Ritorna un puntatore nullo se si è verificato un errore
 */
simulazione *simula_circuito(input *in, output *out)
{
/*
 *  Eseguiamo il parsing del programma fornito in input.
 *
 *  Il corretto stream su file deve essere impostato tramite la variabile
 *  yyin dal parser delle opzioni
 * 
 *  Il controllo di possibili errori durante il parsing è già implementato
 *  direttamente nel parser
 */
    yyparse();

    if (out->avanzamento_simulazione > 0)
    {
        fprintf(stdout, "Parsing del programma QDOS fornito in input "
                        "terminato senza errori.\n");
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Set-up del sistema_astratto
 */
    abs_sys = init_sistema_astratto(in);
/*
 *  Verifichiamo l'esito dell'allocazione del sistema astratto
 */
    if (abs_sys == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allocazione del sistema composto
 */
    comp_sys = NULL;
/*
 *  Verifichiamo se la simulazione comprede o meno l'errore
 */
    if (errore_abilitato > 0)
    {
        comp_sys =
            alloc_sistema_composto_ERR(abs_sys->n_qudit, abs_sys->dim);
    }
    else
    {
        comp_sys =
            alloc_sistema_composto_NO_ERR(abs_sys->n_qudit, abs_sys->dim);
    }
/*
 *  Verifichiamo l'esito dell'allocaione del sistema composto
 */
    if (comp_sys == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo la struttura dati necessaria per eseguire una simulazione
 */
    simulazione *sim = alloc_simulazione();
/*
 *  Verifichiamo l'allocazione della simulazione
 */
    if (sim == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della simulazione.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inizializzazione della simulazione
 */
    sim->out = out;
    sim->sch = NULL;        
    sim->sys = NULL;
    sim->rho = gsl_matrix_complex_calloc(abs_sys->dim, abs_sys->dim);
/*
 *  Verifichiamo l'allocazione delle condizioni iniziali
 */
    if (sim->rho == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione delle condizioni iniziali"
                        "ABORTING...\n\n");
        free_simulazione(sim);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Lettura del parametro di avviamento
 */
    uint8_t ris = fread(&(sim->avviamento), sizeof(double), 1, in->f_in_P);
/*
 *  Verifichiamo la lettura del parametro di avviamento
 */
    if (ris != 1)
    {
        fprintf(stderr, "ERRORE nella lettura del parametro di avviamento."
                        "ABORTING...\n\n");
        free_simulazione(sim);
        
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Lettura delle condizioni iniziali
 */
    ris = gsl_matrix_complex_fread(in->f_in_P, sim->rho);
/*
 *  Verifichiamo l'esito della lettura delle condizioni iniziali
 */
    if (ris == GSL_EFAILED)
    {
        fprintf(stderr, "ERRORE nella lettura delle condizioni iniziali.\n"
                        "ABORTING...\n\n");
        free_simulazione(sim);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo che tra i blocchi definiti vi sia anche il 'main'
 */
    blocco_gate *tmp = cerca_blocco(tabella_blk, "main");
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (tmp == NULL)
    {
        fprintf(stderr, "ERRORE nella composizione del circuito di input.\n"
                        "Blocco 'main' NON definito.\n"
                        "ATTENZIONE lo spelling è case-sensitive.\n"
                        "ABORTING...\n\n");
        free_simulazione(sim);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Traduzione della tabella di gate in una tabella di operazioni
 */
    blocco_operazioni *tabella_operazioni = 
        traduci_tabella_blocchi(tabella_blk);
/*
 *  Verifichiamo l'esito della traduzione
 */
    if (tabella_operazioni == NULL)
    {
        fprintf(stderr, "ERRORE nella traduzione della tabella dei blocchi "
                        "di gate nella tabella dei blocchi di operazioni.\n"
                        "ABORTING...\n\n");
        free_simulazione(sim);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Ricerca del blocco di operazioni 'main'
 */
    blocco_operazioni *blocco_operazioni_main = 
        cerca_blocco_operazioni(tabella_operazioni, "main");
/*
 *  Per sicurezza verifichiamo l'esito della ricerca per individuare bachi
 *  nella procedura di traduzione, perchè, i è certi che il blocco di gate main,
 *  in questo punto del programma, sia presente (abbiamo già verificato)
 */
    if (blocco_operazioni_main == NULL)
    {
        fprintf(stderr, "ERRORE nella traduzione in operazioni simulabili del "
                        "blocco di gate main.\n"
                        "Errore inatteso.\n"
                        "ABORTING...\n\n");
        free_blocco_operazioni(&tabella_operazioni);
        free_simulazione(sim);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *      Verifichiamo se dobbiamo o meno eseguire la simulazione
 */
    if (out->simulazione_abilitata)
    {
/*
 *      Simuliamo il blocco di operazioni corrispondente al main
 */
        sim = simula_blocco_operazioni(blocco_operazioni_main,
                                        sim, tabella_operazioni);
/*
 *      Verifica esito simulazione
 */
        if (sim == NULL)
        {
            return NULL;
        }
        else
        {
/*
 *          Verifichiamo se dobbiamo o meno stampare su file la matrice di
 *          densità finale
 */
            controllo_stampa_soluzione_finale(out, sim->rho);
/*
 *          Verifichiamo se la misura finale del sistema è stata richiesta
 */ 
            if (n_shots > 0)
            {
/*  
 *              Eseguiamo la misurazione del sistema finale 'n_shots' volte
 */ 
                conteggio **count = esegui_misura_sistema(sim, n_shots);
                stampa_conteggio(count, sim->sys->dim);
/*  
 *              Deallocazione della memoria 
 */ 
                for (size_t i = 0; i < sim->sys->dim; ++i)
                {
                    free_conteggio(&(count[i]));
                }
                free(count);
            }
            else
            {
                ;   /* Do Nothing */
            }

            if (out->avanzamento_simulazione > 0)
            {
                fprintf(stdout, "Simulazione del programma QDOS fornito "
                                "in input terminata senza errori.\n");
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
        
    }
    else
    {
        ;   /* Do Nothing */
    }

    return sim;
}

//---------------------------------------------------------------------------//