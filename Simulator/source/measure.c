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

#include "../include/measure.h"
#include "../../Common/include/global.h"

/*
 *  Header file contenente tutte le definizioni delle funzioni necessarie
 *  all'esecuzione di misure del quantum computer simulato
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione per la struttura dati
 *  'misura'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'allocazione di una misura
 */
misura *alloc_misura(size_t dim)
{
    if (dim == 0)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    misura *M = (misura*)malloc(sizeof(misura));
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (M == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una misura.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    M->value = (int*)calloc(dim, sizeof(int));
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (M->value == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una misura.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_misura(&M);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
 
/*
 *  Inizializzazione dei valori misurati
 */
    for (size_t i = 0; i < dim; ++i)
    {
        M->value[i] = QUDIT_NON_MISURATO;
    }

    M->next = NULL;
    M->prev = NULL;
    M->dim = dim;

    return M;
}

/*
 *  Interfaccia per la deallocazione di una misura
 */
void free_misura(misura **M)
{
    if (M == NULL || *M == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if ((*M)->value != NULL)
    {
        free((*M)->value);
        (*M)->value = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(*M);
    *M = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per allocazione, deallocazione e manipolazione della struttura
 *  dati 'lista_misura'
 */

//---------------------------------------------------------------------------//

/*
 *  Inetrfaccia per l'allocazione di una struttura dati 'lista_misure'
 */
lista_misure *alloc_lista_misure(size_t dim)
{
    if (dim == 0)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lista_misure *lst_mis = (lista_misure*)malloc(sizeof(lista_misure));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (lst_mis == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per una lista "
                        "di misure.\n"
                        "Memoria indufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    lst_mis->dim_max = dim;
    lst_mis->dim_eff = 0;

    lst_mis->testa = NULL;
    lst_mis->coda = NULL;

    return lst_mis;
}

/*
 *  Interfaccia per la deallocazione di una struttura dati 'lista_misure'
 */
void free_lista_misure(lista_misure **lst_mis)
{
    if (lst_mis == NULL || *lst_mis == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < (*lst_mis)->dim_eff; ++i)
    {
        if ((*lst_mis)->testa != NULL)
        {
            misura *tmp = (*lst_mis)->testa->next;
            free_misura(&((*lst_mis)->testa));
            (*lst_mis)->testa = tmp;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free(*lst_mis);
    *lst_mis = NULL;

    return;
}

/*
 *  Interfaccia per recuperare la misura i-esima data una lista di misure.
 *  L'ordinamento è decrescente in senso temporale, i.e., la misura 0
 *  corrisponde all'ultima misurazione effettuata
 */
int *get_misura(lista_misure *lst_mis, size_t idx)
{
    if (lst_mis == NULL)
    {
        fprintf(stderr, "ERRORE nell'accesso ad una lista di misure.\n"
                        "Lista nulla.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else if (idx >= lst_mis->dim_eff)
    {
        fprintf(stderr, "ERRORE nell'accesso ad una lista di misure.\n"
                        "Indice richiesto fuori dai limiti.\n"
                        "Indice massimo disponibile: %ld\n"
                        "Indice richiesto: %ld\n"
                        "ABORTING...\n\n",
                        lst_mis->dim_eff - 1, idx);
    }
    else
    {
        ;   /* Do Nothing */
    }

    misura *tmp = lst_mis->testa;

    for (size_t i = 1; i <= idx; ++i)
    {
        tmp = tmp->next;
    }

    return tmp->value;
}

/*
 *  Interfaccia per l'inserimento di una misura all'interno della lista.
 *
 *  Esegue l'inserimento in testa di una misura a valori inizializzati
 *  al valore QUDIT_NON_MISURATO. Ritorna il vettore delle misura inserita
 *  per facilitare l'inserimento dei valori misurati
 */
lista_misure *inserisci_misura(lista_misure *lst_mis, misura *mis)
{
    if (lst_mis == NULL || mis == NULL)
    {
        fprintf(stderr, "ERRORE nell'accesso ad una lista di misure.\n"
                        "Argomenti nulli.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (lst_mis->dim_eff == 0)
    {
        lst_mis->testa = mis;
        lst_mis->coda = mis;
        ++(lst_mis->dim_eff);
    }
    else if (lst_mis->dim_eff < lst_mis->dim_max)
    {
        mis->next = lst_mis->testa;
        lst_mis->testa->prev = mis;

        lst_mis->testa = mis;
        ++(lst_mis->dim_eff);
    }
    else if (lst_mis->dim_eff == lst_mis->dim_max)
    {
        lst_mis->coda = lst_mis->coda->prev;
        free_misura(&(lst_mis->coda->next));
    }
    else
    {
        ;   /* Do Nothing */
    }

    return lst_mis;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'proiettore'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione della struttura dati 'proiettore'
 */
extern
proiettore *alloc_proiettore(size_t dim, size_t dim_tot)
{
    if (dim == 0)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    proiettore *prj = (proiettore*)malloc(sizeof(proiettore));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (prj == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un proittore "
                        "per la misurazione di un qudit.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo memoria opportuna per i putatori ai proiettori effettivi
 */
    prj->P = (gsl_matrix_complex**)calloc(dim, sizeof(gsl_matrix_complex*));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (prj->P == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un proittore "
                        "per la misurazione di un qudit.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_proiettore(&prj);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo memoria opportuna per i proiettori effettivi
 */
    for (size_t i = 0; i < dim; ++i)
    {
        prj->P[i] = gsl_matrix_complex_calloc(dim_tot, dim_tot);

/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (prj->P[i] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione di un proittore "
                            "per la misurazione di un qudit.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_proiettore(&prj);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    prj->dim = dim;

    return prj;
}

/*
 *  Interfacce per la deallocazione della struttura dati 'proiettore'
 */
void free_proiettore(proiettore **prj)
{
    if (prj == NULL || *prj == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < (*prj)->dim; ++i)
    {
        gsl_matrix_complex_free((*prj)->P[i]);
        (*prj)->P[i] = NULL;
    }

    free((*prj)->P);
    (*prj)->P = NULL;

    free((*prj));
    *prj = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'conteggio'
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione della struttura dati 'conteggio'
 */
extern
conteggio *alloc_conteggio()
{
    conteggio *count = (conteggio*)malloc(sizeof(conteggio));

    if (count == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    count->stato  = NULL;
    count->valore = 0;

    return count;
}

/*
 *  Interfacce per la deallocazione della struttura dati 'conteggio'
 */
extern
void free_conteggio(conteggio **count)
{
    if (count == NULL || *count == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if ((*count)->stato != NULL)
    {
        free((*count)->stato);
        (*count)->stato = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(*count);
    *count = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una simulazione e l'indice di un qudit genera i
 *  proiettori opportuni per ogni valore osservabile da una misura del qudit
 */
proiettore *costruisci_proiettore(simulazione *sim, size_t idx_qudit)
{
    if (sim == NULL || idx_qudit > numero_qudit)
    {
        fprintf(stderr, "ERRORE nella generazione di un proittore.\n"
                        "Argoemnti non validi.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Dimensione del qudit da misurare
 */
    size_t dim = dimensioni_qudit[idx_qudit];
/*
 *  Dimensione totale dello spazio
 */
    size_t dim_tot = sim->sys->dim;

/*
 *  Allocazione del proiettore
 */
    proiettore *prj = alloc_proiettore(dim, dim_tot);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (prj == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do nothing */
    }

/*
 *  Variabile d'appoggio
 */
    gsl_complex one  = gsl_complex_rect(1.0, 0.0);

/*
 *  Indice che ci dirà quale autovalore stiamo al momento osservnado
 */
    size_t val = 0;
/*
 *  Indice che conta gli autovettori consecutivi per un dato autovalore
 */
    size_t freq_count = 0;
/*
 *  Calcoliamo la frequenza di cambio degli autovettori
 */
    size_t freq = 1;
    
    for (size_t i = idx_qudit + 1; i < numero_qudit; ++i)
    {
        freq *= dimensioni_qudit[i];
    }
/*
 *  Costruiamo la rappresentazione matriciale degli operatori di proiezione
 */
    for (size_t i = 0; i < dim_tot; ++i)
    {
/*
 *      Siamo nella base degli autovettori quindi la rappresentazione è banale
 */
        gsl_matrix_complex_set(prj->P[val], i, i, one);
        
        ++freq_count;
/*
 *      Verifichiamo se dobbiamo o meno cambiare autovalore
 */
        if (freq_count == freq)
        {
/*
 *          Ciclicità degli autovalori
 */
            if (val < (dim - 1))
            {
                ++val;
            }
            else
            {
                val = 0;
            }

            freq_count = 0;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    return prj;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data la matrice di densità rho del sistema ed un proiettore
 *  generalizzato del sistema calcola la probabilità di osservare l'autovalore
 *  associato al proiettore stesso.
 * 
 *  Essendo nella base degli autovettori dell'hamiltoniana i proiettori sono
 *  diagonali e dunque la traccia di rho*P corrisponde alla somma degli
 *  elementi della diagonale di rho in corrispondenza degli elementi sulla
 *  diagonale del proiettore non nulli
 */
double calcola_probabilita(gsl_matrix_complex *rho,
                            proiettore *prj,
                            size_t idx)
{
/*
 *  Dividiamo la probabilità di errore
 */
    double p_err = p_errore/((double)(prj->dim - 1));

    double prob = 0.0;

    for (size_t j = 0; j < prj->dim; ++j)
    {
        if (j != idx)
        {
            for (size_t i = 0; i < rho->size1; ++i)
            {
                gsl_complex tmp1 = gsl_matrix_complex_get(rho, i, i);
                gsl_complex tmp2 = gsl_matrix_complex_get(prj->P[j], i, i);

                if(fabs(tmp2.dat[0]) > 1e-12)
                {
                    prob += (p_err * tmp1.dat[0]);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < rho->size1; ++i)
            {
                gsl_complex tmp1 = gsl_matrix_complex_get(rho, i, i);
                gsl_complex tmp2 = gsl_matrix_complex_get(prj->P[j], i, i);

                if(fabs(tmp2.dat[0]) > 1e-12)
                {
                    prob += ((1 - p_errore) * tmp1.dat[0]);
                }
            }
        }
        
    }
    return prob;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una simulazione ed un proiettore esegue una scelta
 *  "randomica" dello stato in cui proiettare
 */
int scegli_esito_misura(simulazione *sim, proiettore *prj)
{
/*
 *  Dimensione del qudit in esame
 */
    size_t dim = prj->dim;
/*
 *  Vettore che cobnterrà le varie probabilità di misura per ogni valore
 *  osservabile del qudit
 */
    double prob[dim];
/*
 *  Calcoliamo le probabilità per ogni valore
 */
    for (size_t i = 0; i < dim; ++i)
    {
        prob[i] = calcola_probabilita(sim->rho, prj, i);
    }

/*
 *  Scegliamo l'esito pseudo-casualmente
 */
    double esito = gsl_rng_uniform(rng);
/*
 *  Recuperiamo l'esito corretto
 */
    double p = 0.0;
    int i = -1;
    
    do
    {
        ++i;
        p += prob[i];
    } while (p < esito);

    return i;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato l'esito di una misura determina pseudo-casulamente se
 *  avviene o meno un'errore nella misura stessa
 */
extern
size_t scegli_proiettore(misura *mis, size_t idx_qudit)
{
/*
 *  Valore misurato
 */
    int val = mis->value[idx_qudit];
/*
 *  Dimensione del qudit
 */
    size_t dim = dimensioni_qudit[idx_qudit];
/*
 *  Dividiamo la probabilità di errore
 */
    double p_err = p_errore/((double)(dim - 1));
/*
 *  Scegliamo l'esito pseudo-casualmente
 */
    double esito = gsl_rng_uniform(rng);
/*
 *  Recuperiamo l'esito corretto
 */
    double p = 0.0;
    int i = -1;
    
    do
    {
        ++i;
        if (i != val)
        {
            p += p_err;
        }
        else
        {
            p += (1 - p_errore);
        }
    } while (p < esito);

    return i;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data la matrice di denstità rho ed un proiettore Pj proietta
 *  lo stato rho sul sottospazio individuato da Pj
 */
extern
uint8_t proietta_stato(simulazione *sim, gsl_matrix_complex *prj)
{
/*
 *  Variabili d'appoggio
 */
    size_t dim = sim->sys->dim;
    gsl_complex one  = gsl_complex_rect(1.0, 0.0);
    gsl_complex zero = gsl_complex_rect(0.0, 0.0);
    gsl_matrix_complex *tmp1 = gsl_matrix_complex_calloc(dim, dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (tmp1 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, one, sim->rho, prj, zero, tmp1);
    gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, one, prj, tmp1, zero, sim->rho);

/*
 *  Calcoliamo il fattore di normalizzazione
 */
    gsl_complex normalizzazione = gsl_complex_rect(0.0, 0.0);
    gsl_complex tmp2            = gsl_complex_rect(0.0, 0.0);
    for (size_t i = 0; i < prj->size1; ++i)
    {
        tmp2 = gsl_matrix_complex_get(prj, i, i);

        if (fabs((tmp2.dat[0])) > 1e-12)
        {
            tmp2 = gsl_matrix_complex_get(sim->rho, i, i);
            normalizzazione.dat[0] += tmp2.dat[0];
        }
    }
/*
 *  Dobbiamo dividere la matrice di densità per il termine di normalizzazione
 */
    normalizzazione.dat[0] = 1.0/normalizzazione.dat[0];
/*
 *  Normaliziamo lo stato del sistema
 */
    gsl_matrix_complex_scale(sim->rho, normalizzazione);

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'esecuzione di una misura
 */
simulazione *esegui_misura(simulazione *sim,
                            size_t *idx_qudit,
                            size_t n_qudit,
                            uint8_t output)
{
    if (sim == NULL || idx_qudit == NULL || n_qudit == 0)
    {
        fprintf(stderr, "ERRORE nell'invocazione di una misura.\n"
                        "Forniti parametri illegali.\n"
                        "ABORTING...\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Stampiamo la matrice pre-misura sul file rho_finale.txt
 */
    if (output > 0)
    {
        controllo_stampa_soluzione_finale(sim->out, sim->rho);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo lo spazio per una misura
 */
    misura *mis = alloc_misura(numero_qudit);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (mis == NULL)
    {
        free_simulazione(sim);
        return NULL; 
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t proiettore_da_applicare;

    proiettore *prj = NULL;

    for (size_t i = 0; i < n_qudit; ++i)
    {
        prj = costruisci_proiettore(sim, idx_qudit[i]);
#ifdef DEBUG_PROIETTORE
        for (size_t j = 0; j < dimensioni_qudit[idx_qudit[i]]; ++j)
        {
            fprintf(stdout, "\nProiettore %ld-esimo:\n", j);
            stampa_matrice(stdout, prj->P[j]);
        }
#endif
/*
 *      Verifichiamo l'esito dell'operazione
 */
        if (prj == NULL)
        {
            free_simulazione(sim);
            free_proiettore(&prj);
            free_misura(&mis);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        mis->value[idx_qudit[i]] = scegli_esito_misura(sim, prj);
        proiettore_da_applicare = scegli_proiettore(mis, idx_qudit[i]);
        uint8_t ris = proietta_stato(sim, prj->P[proiettore_da_applicare]);
/*
 *      Verifichiamo l'esito della proiezione
 */
        if (ris > 0)
        {
            free_simulazione(sim);
            free_proiettore(&prj);
            free_misura(&mis);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
#ifdef DEBUG
    fprintf(stdout, "\n\n"
                    "Indice qudit: %ld\n"
                    "Esito misura: %d\n"
                    "Proiettore applicato: %ld\n",
                    idx_qudit[i],
                    mis->value[idx_qudit[i]],
                    proiettore_da_applicare);
#endif
        free_proiettore(&prj);
    }

    tabella_mis = inserisci_misura(tabella_mis, mis);
/*
 *  Verifichiamo l'esito dell'inserimento
 */
    if (tabella_mis == NULL)
    {
        free_simulazione(sim);
        free_proiettore(&prj);
        free_misura(&mis);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Verifichiamo se dobbiamo o meno stampare la matrice di densità aggiornata
 *
 *  TODO::  Verificare se l'output a tempo zero va bene o meno
 */
    if (output > 0)
    {
        controllo_stampa_output_simulazione(sim->out, 0.0, sim->rho);
    }
    else
    {
        ;   /* Do Nothing */
    }

#ifdef DEBUG
    fprintf(stdout, "\nrho:\n");
    stampa_matrice(stdout, sim->rho);
#endif
    return sim;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiorna il conteggio delle misurazioni ottenute per un
 *  determinato stato in base 
 */
extern
void aggiorna_conteggio(conteggio **count,
                        size_t dim,
                        int *esito_misura,
                        size_t n_qudit)
{
/*
 *  Recuperiamo l'indice del conteggio
 */
    size_t idx = 0;
    size_t dim_sp_dx = 1;

    for (size_t i = 0; i < n_qudit; ++i)
    {
/*
 *      Calcoliamo la dimensione dello spazio a destra
 */
        for (size_t j = i + 1; j < n_qudit; ++j)
        {
            dim_sp_dx *= dimensioni_qudit[j];
        }

        idx += esito_misura[i] * dim_sp_dx;
        dim_sp_dx = 1;
    }
#ifdef DEBUG
    size_t val = count[idx]->valore;
    val += 1;
    val -=1;
#endif
    ++(count[idx]->valore);

    if (count[idx]->stato == NULL)
    {
        count[idx]->stato = (char*)calloc(n_qudit + 3, sizeof(char));

        if (count[idx]->stato == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                            "ABORTING...\n\n");
        }
        else
        {
            ;   /* Do Nothing */
        }

        count[idx]->stato[0] = '|';
        for (size_t i = 1; i < n_qudit + 1; ++i)
        {
            count[idx]->stato[i] = esito_misura[i - 1] + '0';
        }
        count[idx]->stato[n_qudit + 1] = '>';
        count[idx]->stato[n_qudit + 2] = '\0';

    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che fornito lo stato attuale del sistema ed il numero di
 *  simulazioni da eseguire 'n_shots' eseguira 'n_shots' misure del sistema
 */
conteggio **esegui_misura_sistema(simulazione *sim, size_t N)
{
    if (sim == NULL || N == 0)
    {
        fprintf(stderr, "ERRORE nell'esecuzione della misurazione finale "
                        "del sistema.\n"
                        "Argomenti non validi.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t dim = sim->sys->dim;
    conteggio **count = (conteggio**)calloc(dim, sizeof(conteggio*));

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (count == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di un'array.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < dim; ++i)
    {
        count[i] = alloc_conteggio();
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (count[i] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                            "ABORTING...\n\n");
            
            for (size_t j = 0; j < i; ++j)
            {
                free_conteggio(&(count[j]));
            }

            free(count);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

/*
 *  Allochiamo una simulazione fittizia
 */
    simulazione *sim_tmp = alloc_simulazione();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (sim_tmp == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una simulazione.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo lo spazio per uno stato fittizio del sistema
 */
    sim_tmp->rho = gsl_matrix_complex_calloc(dim, dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (sim_tmp->rho == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Inizializzazione simulazione fittizia
 */
    sim_tmp->avviamento = 0;
    sim_tmp->out = sim->out;
    sim_tmp->sys = sim->sys;
    sim_tmp->sch = NULL;

/*
 *  Array contenente gli indici dei qudit da misurare.
 *  [Richiesto dalle interfacce]
 */
    size_t idxs_qudit[numero_qudit];
    for (size_t i = 0; i < numero_qudit; ++i)
    {
        idxs_qudit[i] = i;
    }

/*
 *  Aumentiamo la memoria per la tabella delle misure per poter contenere
 *  tutti gli esiti degli shots.
 */
    tabella_mis->dim_max = N;

/*
 *  Eseguiamo la misurazione del sistema N volte
 */
    for (size_t i = 0; i < N; ++i)
    {
/*
 *      Copiamo lo stato del sistema
 */
        gsl_matrix_complex_memcpy(sim_tmp->rho, sim->rho);
/*
 *      Eseguiamo una misura
 */
        sim_tmp = esegui_misura(sim_tmp, idxs_qudit, numero_qudit, 0);
/*
 *      Verifichiamo l'esito della misuarazione
 */
    if (sim_tmp == NULL)
    {
        free_simulazione(sim_tmp);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
/*
 *      Recuperiamo l'ultima misuarazione eseguita
 */
        int *esito_misura = get_misura(tabella_mis, 0);
/*
 *      Aggiorniamo il conteggio
 */
        aggiorna_conteggio(count, dim, esito_misura, numero_qudit);
    }

    return count;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che scrive standard output la distribuzione finale simulata
 */
void stampa_conteggio(struct conteggio **count, size_t dim)
{
    fprintf(stdout, "\nRisultati ottenuti:\n");
    for (size_t i = 0; i < dim; ++i)
    {
        if (count[i]->valore > 0)
        {
            fprintf(stdout, "%s:\t%ld\n",count[i]->stato, count[i]->valore);
        }
    }
    fprintf(stdout, "\n");

    return;
}
