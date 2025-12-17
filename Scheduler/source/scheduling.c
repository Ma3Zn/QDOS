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

#include "../include/scheduling.h"

/*
 *  File che contiene la definizione di tutte le funzioni necessarie per
 *  effettuare lo scheduling degli impulsi necessari alla simulazione di una
 *  sequenza di rotazioni.
 */

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di un cammino
 */

//---------------------------------------------------------------------------//

/*
 *  Allocazione di un cammino
 */
cammino *alloc_cammino(size_t N)
{
    cammino *path = (cammino*)malloc(sizeof(cammino));

    if (path == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di memoria per un cammino.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    path->livelli = (size_t *)calloc(sizeof(size_t), N);

    if (path->livelli == NULL)
    {
        fprintf(stderr, "ERROE nell'allocazione di memoria per un cammino.\n"
                        "ABORTING...\n\n");
        free_cammino(path);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    path->idx_usati = (uint8_t *)calloc(sizeof(uint8_t), N);

    if (path->idx_usati == NULL)
    {
        fprintf(stderr, "ERROE nell'allocazione di memoria per un cammino.\n"
                        "ABORTING...\n\n");
        free_cammino(path);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    path->dim_eff = 0;
    path->dim = N;

    return path;
}

//---------------------------------------------------------------------------//

/*
 *  Deallocazione di un cammino
 */
void free_cammino(cammino *path)
{
    if (path->livelli != NULL)
    {
        free(path->livelli);
        path->livelli = NULL;
    }
    else
    {
        ;
    }

    if (path->idx_usati != NULL)
    {
        free(path->idx_usati);
        path->idx_usati = NULL;
    }

    free(path);

/*
 *  path = NULL non ha senso farlo in quanto il puntatore alla funzione è
 *  passato per valore e non riferimento, quindi la modifica di path avrebbe
 *  effetto solo qui all'interno della funzione e non all'interno del chiamante      
 */
    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione ricorsiva che ad ogni chiamata aggiunge un livello al camino.
 *  La direzione di ricerca parte dall'indice attuale e scorre verso destra
 *  presupponendo che livelli[0] < livelli[1]. L'algoritmo NON ha pretese di
 *  ottimalità ma bensì di terminazione, i.e., se c'è trova sempre una
 *  soluzione.
 */
uint8_t cerca_cammino_ricorsivo(cammino *path,
                                sistema_composto *sys,
                                size_t livelli[2])
{
/*
 *  Recuperiamo l'indice del livello attuale
 */
    size_t lv_attuale = path->livelli[path->dim_eff - 1];

#ifdef DEBUG
    size_t lv_0 = livelli[0];
    size_t lv_1 = livelli[1];

    lv_0 += lv_1;
    lv_0 -= lv_1;
#endif

/*
 *  Cerchiamo prima fra tutti i livelli successivi a quello attuale
 */
    for (size_t i = lv_attuale + 1; i < path->dim; ++i)
    {
/*
 *      Se abbiamo già usato questo indice NON lo riutilizziamo
 */
        if (path->idx_usati[i])
        {
            continue;
        }
        else
        {
            ;   /* DO Nothing */
        }



        gsl_complex M_mn = gsl_matrix_complex_get(sys->M, lv_attuale, i);
        double abs_M_mn = gsl_complex_abs(M_mn);
/*
 *  Se l'elemento è NON nullo proviamo ad esplorare il cammino che lo contiene
 */
        if (abs_M_mn > 1e-12)
        {
/*
 *          L'ordine di queste operazioni è importante
 */
            path->livelli[path->dim_eff] = i;
            path->idx_usati[i] = 1;

            path->dim_eff += 1;

/*
 *          Il cammino è completo
 */
            if (i == livelli[1])
            {
                return 0;
            }
            else
            {
                ;   /* Do Nothing */
            }
/*
 *          Verifichiamo se i livelli sono direttamente collegabili
 */
            gsl_complex tmp = gsl_matrix_complex_get(sys->M, i, livelli[1]);

            if (gsl_complex_abs(tmp) < 1e-6)
            {
                size_t ris = cerca_cammino_ricorsivo(path, sys, livelli);
/*
 *              Controlliamo se abbiamo un cammino completo
 */ 
                if (ris == 0)
                {
                    return 0;
                }
                else
                {
                    ;   /* Do Nothing */
                }
            }
            else
            {
                path->livelli[path->dim_eff] = livelli[1];
                ++(path->dim_eff);
/*
 *              Abbiamo un cammino completo
 */
                return 0;;   /* Abbiamo un cammino completo */
            }

            path->dim_eff -= 1;
            path->idx_usati[i] = 0;
        }
    }

/*
 *  Se il livello attuale è 0 allora non abbiamo alcun livello precedente
 */
    if (lv_attuale == 0)
    {
        // path->dim_eff -= 1;
        // path->idx_usati[path->livelli[path->dim_eff]] = 0;

        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Cerchiamo ora fra tutti i livelli precedenti a quello attuale
 */
    for (size_t i = lv_attuale; i > 0; --i)
    {
/*
 *      Se abbiamo già usato questo indice NON lo riutilizziamo
 */
        if (path->idx_usati[i - 1])
        {
            continue;
        }
        else
        {
            ;   /* DO Nothing */
        }

        gsl_complex M_mn = gsl_matrix_complex_get(sys->M, lv_attuale, i - 1);
        double abs_M_mn = gsl_complex_abs(M_mn);
/*
 *  Se l'elemento è NON nullo proviamo ad esplorare il cammino che lo contiene
 */
        if (abs_M_mn > 1e-12)
        {
/*
 *          L'ordine di queste operazioni è importante
 */
            path->livelli[path->dim_eff] = i - 1;
            path->idx_usati[path->livelli[path->dim_eff]] = 1;

            path->dim_eff += 1;
/*
 *          Il cammino è completo. In questo modo copriamo anche il caso in cui
 *          livelli[1] < livelli[0]
 */
            if (i - 1 == livelli[1])
            {
                return 0;
            }

/*
 *          Verifichiamo se i livelli sono direttamente collegabili
 */
            gsl_complex tmp = gsl_matrix_complex_get(sys->M, i - 1, livelli[1]);

            if (gsl_complex_abs(tmp) < 1e-6)
            {
                size_t ris = cerca_cammino_ricorsivo(path, sys, livelli);
/*
 *              Controlliamo se abbiamo un cammino completo
 */ 
                if (ris == 0)
                {
                    return 0;
                }
                else
                {
                    ;   /* Do Nothing */
                }
            }
            else
            {
                path->livelli[path->dim_eff] = livelli[1];
                ++(path->dim_eff);
/*
 *              Abbiamo un cammino completo
 */
                return 0;
            }

            path->dim_eff -= 1;
            path->idx_usati[path->livelli[path->dim_eff]] = 0;
        }
    }

/*
 *  Non abbiamo più alternative. L'ordine è importante
 */
    // path->dim_eff -= 1;
    // path->idx_usati[path->livelli[path->dim_eff]] = 0;

    return 1;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un sistema composto e due livelli del sistema stesso
 *  cerca un cammino che connette i due suddetti livelli attraverso la
 *  matrice di connessione fornita.
 */
cammino *cerca_cammino(sistema_composto *sys, size_t livelli[2])
{
/*
 *  Allocazione del cammino
 */
    cammino *path = alloc_cammino(sys->M->size1);
/*
 *  Controlliamo l'esito dell'allocazione
 */
    if (path == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Inizializziamo il cammino
 */
    path->idx_usati[livelli[0]] = 1;
    path->livelli[0] = livelli[0];
    path->dim_eff = 1;

/*
 *  Verifichiamo se i livelli sono direttamente collegabili
 */
    gsl_complex tmp = gsl_matrix_complex_get(sys->M, livelli[0], livelli[1]);

    if (gsl_complex_abs(tmp) < 1e-6)
    {
/*
 *      Invochiamo la ricerca ricorsiva del cammino
 */
        size_t ris = cerca_cammino_ricorsivo(path, sys, livelli);

/*
 *      Controlliamo l'esito della ricerca
 */
        if (ris)
        {
            fprintf(stderr, "ERRORE nella rierca del cammino collegante i "
                            "livelli %ld e %ld.\n"
                            "ABORTING...\n\n", livelli[0], livelli[1]);
            free(path);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
        path->livelli[1] = livelli[1];
        ++(path->dim_eff);
    }

    return path;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inizializzazione di un array di "stato_liv_par"
 */
void init_arr_stato_liv(stato_liv_par *arr, size_t dim)
{
    for (size_t i = 0; i < dim; ++i)
    {
        arr[i] = LIBERO;
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'aggiornamento dello stato dei livelli del sistema
 */
void aggiorna_stato_liv(sequenza_impulsi *seq,
                        stato_liv_par *arr,
                        stato_liv_par stato)
{
    for (size_t i = 0; i < seq->dim ; ++i)
    {
        for (size_t j = 0; j < seq->arr_impulsi_paralleli[i]->dim; ++j)
        {
            impulso *tmp = seq->arr_impulsi_paralleli[i]->arr_impulsi[j];
            arr[tmp->livelli[0]] = stato;
            arr[tmp->livelli[1]] = stato;
        }
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se sono attualmente ancora disponibili livelli
 *  per l'esecuzione di impulsi paralleli
 * 
 *  Lentezza allo stato puro
 */
uint8_t controlla_disp_liv(stato_liv_par *arr, size_t dim)
{
    for (size_t i = 0; i < dim; ++i)
    {
        if (arr[i])
        {
            ;   /* Do Nothing */
        }
        else
        {
            return (uint8_t)1U;
        }
    }

    return (uint8_t)0U;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che utilizzeremo per la traduzione di una rotazione Z in una
 *  sequenza di impulsi. Prende in input la rotazione da tradurre ed il sistema
 *  fisico studiato e restituisce l'opportuna sequenza di impulsi.
 *
 *  TODO::  Dare modo all'utente di selezionare la forma dell'impulso da
 *          utilizzare. Al mometo staimo usando di default impulsi gaussiani.
 * 
 *  ATTENZIONE::    Gli impulsi di collegamento onde evitare degli sfasamenti
 *                  relativi indesiderati venno eseguiti in un determinato
 *                  "ordine". In particolare:
 *                      -   Ogni impulso "a scendere" deve essere un "3*pi",
 *                          i.e., un pi sfasato di pi. Così facendo lo
 *                          sfasamento è scaricato sullo stato ad indice
 *                          minore (dopo lo scambio) e non viene applicato
 *                          allo stato su cui poi applicheremo la rotazione
 *                          desiderata
 *                      -   Ogni impulso "a salire" deve essere un "pi". In tal
 *                          modo lo sfasamento viene scaricato sullo stato ad
 *                          maggiore (dopo lo scambio)
 */
sequenza_impulsi *traduci_rotazione_Z(rotazione_Z *rot, sistema_composto *sys)
{
/*
 *  Controlliamo l'ordine dei livelli di rotazione.
 *
 *  Deve essere:    livello[0] < livello[1]
 */
    controlla_ordine_livelli_rotazione_Z(rot);
    controlla_ottimalita_parametri_rot_Z(rot);

/*
 *  Cerchiamo un cammino che colleghi i due livelli interessati dalla rotazione
 */
    cammino *path = cerca_cammino(sys, rot->livello);
/*
 *  Controlliamo l'esito della ricerca
 */
    if (path == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Lunghezza del cammino. Il cammino contiene sia il livello iniziale che quello
 *  finale
 */
    size_t lunghezza = path->dim_eff - 1;

/*
 *  Verifichiamo la correttezza del cammino trovato
 */
    if (    path->livelli[0]         != rot->livello[0]
        ||  path->livelli[lunghezza] != rot->livello[1])
    {
        fprintf(stderr, "ERRORE nella forma del cammino che dovrebbe "
                        "collegare i due livelli soggetti alla rotazione.\n"
                        "Gli estremi non combaciano.\n"
                        "ABORTING...\n\n");
        free_cammino(path);

        return NULL;
    }

/*
 *  Controlliamo quanto distano i livelli interessati dalla rotazione. Questo
 *  dato basta per conoscere l'esatta dimensione della sequenza di impulsi.
 *  Non avendo la matrice di connessione a scala la distanza tra due livelli
 *  è semplicemente la lunghezza del cammino
 */
    size_t distanza_livelli = lunghezza;

    if (distanza_livelli == 0)
    {
        fprintf(stderr, "ERRORE in una rotazione Z. Livelli coincidenti.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_impulsi *seq = NULL;

/*
 *  Flag che ci dirà se, nel caso in cui la matrice di connessione NON sia a
 *  scala, è necessario eseguire una inversione della rotazione piana da
 *  eseguire. Questo si rende necessario quando per esempio lo stato "1"
 *  viene spostato su un indice più piccolo di quello che popola lo stato "0".
 * 
 *  Ovvero:     Supponiamo di dover compiere una rotazione Z tra [1,2] ed
 *              il cammino a nostra dispozione è {1,0,2}. Per come funziona
 *              il nostro algoritmo di scheduling il primo passo che eseguiremo
 *              sarà quello di spostare 2 in 0. Dopodichè dovremo eseguire
 *              l'impulso che esegue la rotazione Z tra 0 ed 1. Ci troviamo
 *              con gli stati invertiti di ordine. Per risolvere il problema
 *              ci basterebbe invertire la rotazione Z (data la sua
 *              simmetria rispetto all'antidiagonale). Per fare ciò ci basta
 *              invertire il segno dell'angolo di rotazione.
 */
    uint8_t inversione = 0;

/*
 *  Array che tiene traccia della posizione del livello "0" e del livello "1".
 *  Necessario per individuare quando un'inversione dell'operatore risulta
 *  necessaria.
 */
    uint64_t livello[2] = {rot->livello[0], rot->livello[1]};

/*
 *  Flag che indicherà se la distanza tra i livelli è pari o dispari.
 *  Ci tornerà utile in quanto in caso di distanza pari abbiamo necessita'
 *  di eseguire un impulso in più rispetto al caso dispari
 */
    uint8_t pari = 0; 

/*
 *  Abbiamo bisogno di
 *
 *      [se dispari]
 *      + distanza_livelli/2 per portare vicino i due livelli
 *      + 2 impulso che effettui la rotazione effettiva (ci servono 2
 *        impulsi per implementare un $Rz)
 *      + distanza_livelli/2 per riportare i livelli al loro posto
 *
 *      [se pari]
 *      + (distanza_livelli-1)/2 + 1 per portare vicino i due livelli
 *      + 2 impulso che effettui la rotazione effettiva (ci servono 2
 *        impulsi per implementare un $Rz)
 *      + (distanza_livelli-1)/2 + 1 per riportare i livelli al loro posto
 * 
 *      ATTENZIONE::    Le divisioni vanno fatte in aritmetica intera!
 */
    if (distanza_livelli % 2 == 1)
    {
        size_t tmp = distanza_livelli / 2;
        seq = alloc_sequenza_impulsi(2 * tmp + 2);
    }
    else
    {
        size_t tmp = (distanza_livelli - 1) / 2;
        seq = alloc_sequenza_impulsi(2 * tmp + 4);
        pari = 1;
    }

/*
 *  Numero di impulsi aggiunti alla sequenza
 */
    size_t dim_eff = 0;

/*
 *  Teniamo traccia dei livelli degli impulsi
 */
    size_t idx_lv_0 = 0;
    size_t idx_lv_1 = lunghezza;

/*
 *  Variabili d'appoggio
 */
    impulso *imp_1 = NULL;
    impulso *imp_2 = NULL;

/*
 *  Se la distanza è pari aggiungiamo un impulso sfasato per poter
 */
    if (pari == 1)
    {
        seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione Z.\nABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 - 1],
                                            modulazione_impulso_gauss,
                                            &inversione);
                                            
        if (imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una"
                            " rotazione Z.\nABORTING...\n\n",
                            dim_eff + 1);

            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_2;
        
/*
 *      Teniamo traccia della posizione del livello "1"
 */
        livello[1] = path->livelli[idx_lv_1 - 1];

        ++dim_eff;
        --idx_lv_1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Implementiamo i primi impulsi di shift. Parte comune al caso pari e a
 *  quello dispari
 */
    for (; idx_lv_1 - idx_lv_0 > 2; ++idx_lv_0, --idx_lv_1)
    {
/*
 *      Allochiamo un impulso parallelo formato da due impulsi
 */
        seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(2);

        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione Z.\nABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Allochiamo i due impulsi che formano l'impulso parallelo
 */
        imp_1 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_0], 
                                            path->livelli[idx_lv_0 + 1],
                                            modulazione_impulso_gauss,
                                            &inversione);
        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 - 1],
                                            modulazione_impulso_gauss,
                                            &inversione);
        
        if (imp_1 == NULL || imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una rotazione"
                            " Z.\nABORTING...\n\n",
                            dim_eff + 1);
            
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Controlliamo che gi impulsi non siano risonanti tra di loro. In tal
 *      caso solo uno sarà aggiunto alla schedule
 */
        if (!controlla_risonanza_impulsi(imp_1, imp_2))
        {
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        }
        else
        {
            fprintf(stderr, "ATTENZIONE impulsi paralleli con frequenza simile"
                            ".\n"
                            "Rimozione automatica di uno dei due.\n"
                            "Se si è utilizzato l'algoritmo di decomposizione "
                            "di un'unitaria in rotazioni piane e rotazioni Z "
                            "la probabilità di errori nei risultati è molto "
                            "elevata.\n"
                            "Si consiglia di rivedere il sistema fisico, "
                            "oppure di fornire direttamente una sequenza di "
                            "impulsi che decompone l'unitaria desiderata.\n\n"
                            );

            free_impulsi_paralleli(seq->arr_impulsi_paralleli[dim_eff]);

            seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;

            free_impulso(imp_2);
        }

/*
 *      Teniamo traccia della posizione dei livelli "0" ed "1" 
 */
        livello[0] = path->livelli[idx_lv_0 + 1];
        livello[1] = path->livelli[idx_lv_1 - 1];

        ++dim_eff;
    }

    if (idx_lv_1 - idx_lv_0 != 1)
    {
        fprintf(stderr, "ERRORE nell'algoritmo di traduzione di una rotazione"
                        " Z.\n"
                        "I livelli tra cui si intende applicare l'$Rz"
                        " NON sono alla giusta distanza.\n"
                        "ABORTING...\n\n");
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Implementiamo la rotazione Z effettiva.
 *  Questa consiste in un "3pi" seguito da un impulso di "pi"
 *  sfasato dell'angolo theta di rotazione
 *
 *  Implementiamo l'impulso di "3pi" come un impulso di "pi" sfasato di "pi"
 */
    
    seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
    if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                        " l'impulso parallelo %ld nella traduzione"
                        " di una rotazione Z.\nABORTING...\n\n",
                        dim_eff + 1);
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    imp_1 = init_impulso(sys,
                            path->livelli[idx_lv_0],
                            path->livelli[idx_lv_1],
                            M_PI, M_PI, modulazione_impulso_gauss);
    
    if (imp_1 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                        " l'impulso %ld nella traduzione di una"
                        " rotazione Z.\nABORTING...\n\n",
                        dim_eff + 1);

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
    ++dim_eff;

/*
 *  Implementiamo il secondo impulso che termina la rotazione Z
 */
    seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
    if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                        " l'impulso parallelo %ld nella traduzione"
                        " di una rotazione Z.\nABORTING...\n\n",
                        dim_eff + 1);
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo corretto ordinamento livelli
 *
 *  TODO::  Verificare che non ci siano conflitti con il controllo
 *          della necessità di inversione durante gli impulsi
 *          di collegamento
 */
    if (path->livelli[idx_lv_0] > path->livelli[idx_lv_1])
    {
        if (inversione == 0u || inversione == 3u)
        {
            inversione = 1;
        }
        else
        {
            inversione = 0;
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    double phi =  determina_angolo_rotazione_Z(sys, rot,
                                                path->livelli[idx_lv_0],
                                                path->livelli[idx_lv_1],
                                                inversione);

    imp_1 = init_impulso(sys,
                            path->livelli[idx_lv_0],
                            path->livelli[idx_lv_1],
                            M_PI, phi, modulazione_impulso_gauss);
    
    if (imp_1 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                        " l'impulso %ld nella traduzione di una"
                        " rotazione Z.\nABORTING...\n\n",
                        dim_eff + 1);

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
    ++dim_eff;

/*
 *  Aggiungiamo gli impulsi di "discesa" che ripristinano il corretto
 *  ordine degli stati
 */
    for (   ; 
            (pari == 1  ?   idx_lv_1 - idx_lv_0 < distanza_livelli - 1 
                        :   idx_lv_1 - idx_lv_0 < distanza_livelli);
            --idx_lv_0, ++idx_lv_1
        )
    {
/*
 *      Allochiamo un impulso parallelo formato da due impulsi
 */
        seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(2);

        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione Z.\nABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Allochiamo i due impulsi che formano l'impulso parallelo
 */
        imp_1 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_0], 
                                            path->livelli[idx_lv_0 - 1],
                                            modulazione_impulso_gauss,
                                            &inversione);
        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 + 1],
                                            modulazione_impulso_gauss,
                                            &inversione);
        
        if (imp_1 == NULL || imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una rotazione"
                            " Z.\nABORTING...\n\n",
                            dim_eff + 1);
            
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Controlliamo che gi impulsi non siano risonanti tra di loro. In tal
 *      caso solo uno sarà aggiunto alla schedule
 */
        if (!controlla_risonanza_impulsi(imp_1, imp_2))
        {
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        }
        else
        {
            fprintf(stderr, "ATTENZIONE impulsi paralleli con frequenza simile"
                            ".\n"
                            "Rimozione automatica di uno dei due.\n"
                            "Se si è utilizzato l'algoritmo di decomposizione "
                            "di un'unitaria in rotazioni piane e rotazioni Z "
                            "la probabilità di errori nei risultati è molto "
                            "elevata.\n"
                            "Si consiglia di rivedere il sistema fisico, "
                            "oppure di fornire direttamente una sequenza di "
                            "impulsi che decompone l'unitaria desiderata.\n\n"
                            );

            free_impulsi_paralleli(seq->arr_impulsi_paralleli[dim_eff]);

            seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;

            free_impulso(imp_2);
        }

/*
 *      Teniamo traccia della posizione dei livelli "0" ed "1" 
 */
        livello[0] = path->livelli[idx_lv_0 - 1];
        livello[1] = path->livelli[idx_lv_1 + 1];

        ++dim_eff;
    }

/*
 *  Se pari aggiungere l'ultimo impulso di discesa
 */
    if (pari == 1)
    {
    seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione Z.\nABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 + 1],
                                            modulazione_impulso_gauss,
                                            &inversione);
        
        if (imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una"
                            " rotazione Z.\nABORTING...\n\n",
                            dim_eff + 1);

            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_2;

/*
 *      Teniamo traccia della posizione del livello "1" 
 */
        livello[1] = path->livelli[idx_lv_1 + 1];

        ++dim_eff;
        ++idx_lv_1;
    }

/*
 *  Sanity Check
 */
    if (    path->livelli[idx_lv_0] != rot->livello[0]
        ||  path->livelli[idx_lv_1] != rot->livello[1])
    {
        fprintf(stderr, "ERRORE nella traduzione di una rotazione Z.\n"
                        "I livelli finali non combaciano con quelli iniziali."
                        "\n"
#ifdef DEBUG
                        "Livelli iniziali: \t %ld \t %ld \n"
                        "Livelli finali: \t %ld \t %ld \n"
#endif
                        "ABORTING...\n\n"
#ifdef DEBUG
                        , rot->livello[0], rot->livello[1],
                        path->livelli[idx_lv_0], path->livelli[idx_lv_1]
#endif
                );
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return seq;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che utilizzeremo per la traduzione di una rotazione piana in una
 *  sequenza di impulsi. Prende in input la rotazione da tradurre ed il sistema
 *  fisico studiato e restituisce l'opportuna sequenza di impulsi.
 * 
 *  Praticamente identica a "traduci_rotazione_Z"
 * 
 *  TODO::  Ci devono essere sicuramente modi più puliti, coincisi e semplici
 *          per l'implementazione di queste due funzioni. 400 righe di codice
 *          che continua a ripetersi non sono la soluzione. Soprattutto
 *          quando poi decideremo di rimuovere l'HP della matrice di
 *          connettivita' "a scala". In quell'occasione suggerisco di
 *          rivisitare a fondo la struttura e la logica di quetse due
 *          funzioni
 * 
 *  TODO::  Dare modo all'utente di selezionare la forma dell'impulso da
 *          utilizzare. Al mometo staimo usando di default impulsi gaussiani.
 * 
 *  ATTENZIONE::    Gli impulsi di collegamento onde evitare degli sfasamenti
 *                  relativi indesiderati venno eseguiti in un determinato
 *                  "ordine". In particolare:
 *                      -   Ogni impulso "a scendere" deve essere un "3*pi",
 *                          i.e., un pi sfasato di pi. Così facendo lo
 *                          sfasamento è scaricato sullo stato ad indice
 *                          minore (dopo lo scambio) e non viene applicato
 *                          allo stato su cui poi applicheremo la rotazione
 *                          desiderata
 *                      -   Ogni impulso "a salire" deve essere un "pi". In tal
 *                          modo lo sfasamento viene scaricato sullo stato ad
 *                          maggiore (dopo lo scambio)
 */
sequenza_impulsi *traduci_rotazione_piana(rotazione_piana *rot,
                                            sistema_composto *sys)
{
/*
 *  Controlliamo l'ordine dei livelli di rotazione.
 *
 *  Deve essere:    livello[0] < livello[1]
 */
    controlla_ordine_livelli_rotazione_piana(rot);
    controlla_ottimalita_parametri_rot_piana(rot);

/*
 *  Cerchiamo un cammino che colleghi i due livelli interessati dalla rotazione
 */
    cammino *path = cerca_cammino(sys, rot->livello);
/*
 *  Controlliamo l'esito della ricerca
 */
    if (path == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Lunghezza del cammino. Il cammino contiene sia il livello iniziale che quello
 *  finale
 */
    size_t lunghezza = path->dim_eff - 1;

/*
 *  Verifichiamo la correttezza del cammino trovato
 */
    if (    path->livelli[0]         != rot->livello[0]
        ||  path->livelli[lunghezza] != rot->livello[1])
    {
        fprintf(stderr, "ERRORE nella forma del cammino che dovrebbe "
                        "collegare i due livelli soggetti alla rotazione.\n"
                        "Gli estremi non combaciano.\n"
                        "ABORTING...\n\n");
        free_cammino(path);

        return NULL;
    }

/*
 *  Controlliamo quanto distano i livelli interessati dalla rotazione. Questo
 *  dato basta per conoscere l'esatta dimensione della sequenza di impulsi.
 *  Non avendo la matrice di connessione a scala la distanza tra due livelli
 *  è semplicemente la lunghezza del cammino
 */
    size_t distanza_livelli = lunghezza;

    if (distanza_livelli == 0)
    {
        fprintf(stderr, "ERRORE in una rotazione piana. Livelli coincidenti.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_impulsi *seq = NULL;

/*
 *  Flag che ci dirà se, nel caso in cui la matrice di connessione NON sia a
 *  scala, è necessario eseguire una trasposizione della rotazione piana da
 *  eseguire. Questo si rende necessario quando per esempio lo stato "1"
 *  viene spostato su un indice più piccolo di quello che popola lo stato "0".
 * 
 *  Ovvero:     Supponiamo di dover compiere una rotazione piana tra [1,2] ed
 *              il cammino a nostra dispozione è {1,0,2}. Per come funziona
 *              il nostro algoritmo di scheduling il primo passo che eseguiremo
 *              sarà quello di spostare 2 in 0. Dopodichè dovremo eseguire
 *              l'impulso che esegue la rotazione piana tra 0 ed 1. Ci troviamo
 *              con gli stati invertiti di ordine. Per risolvere il problema
 *              ci basterebbe trasporre la rotazione piana (data la sua
 *              simmetria rispetto all'antidiagonale). Per fare ciò ci basta
 *              aggiungere pi al parametro beta e poi cambiare il segno (sempre
 *              di beta)
 */
    uint8_t trasposizione = 0u;

/*
 *  Array che tiene traccia della posizione del livello "0" e del livello "1".
 *  Necessario per individuare quando una trasposizione dell'operatore risulta
 *  necessaria.
 */
    uint64_t livello[2] = {rot->livello[0], rot->livello[1]};

/*
 *  Flag che indicherà se la distanza tra i livelli è pari o dispari.
 *  Ci tornerà utile in quanto in caso di distanza pari abbiamo necessita'
 *  di eseguire un impulso in più rispetto al caso dispari
 */
    uint8_t pari = 0; 

/*
 *  Abbiamo bisogno di
 *
 *      [se dispari]
 *      + distanza_livelli/2 per portare vicino i due livelli
 *      + 1 impulso che effettui la rotazione effettiva (ci servono 2
 *        impulsi per implementare un $Rz)
 *      + distanza_livelli/2 per riportare i livelli al loro posto
 *
 *      [se pari]
 *      + (distanza_livelli-1)/2 + 1 per portare vicino i due livelli
 *      + 1 impulso che effettui la rotazione effettiva (ci servono 2
 *        impulsi per implementare un $Rz)
 *      + (distanza_livelli-1)/2 + 1 per riportare i livelli al loro posto
 *
 *      ATTENZIONE::    Le divisioni vanno fatte in aritmetica intera!
 */
    if (distanza_livelli % 2 == 1)
    {
        size_t tmp = distanza_livelli / 2;
        seq = alloc_sequenza_impulsi(2 * tmp + 1);
    }
    else
    {
        size_t tmp = (distanza_livelli - 1) / 2;
        seq = alloc_sequenza_impulsi(2 * tmp + 3);
        pari = 1;
    }

/*
 *  Numero di impulsi aggiunti alla sequenza
 */
    size_t dim_eff = 0;

/*
 *  Teniamo traccia dei livelli degli impulsi
 */
    size_t idx_lv_0 = 0;
    size_t idx_lv_1 = lunghezza;

/*
 *  Variabili d'appoggio
 */
    impulso *imp_1 = NULL;
    impulso *imp_2 = NULL;

/*
 *  Se la distanza è pari aggiungiamo un impulso sfasato
 */
    if (pari == 1)
    {
        seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione piana.\nABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 - 1],
                                            modulazione_impulso_gauss,
                                            &trasposizione);
        
        if (imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una"
                            " rotazione piana.\nABORTING...\n\n",
                            dim_eff + 1);

            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Teniamo traccia della posizione del livello "1"
 */
        livello[1] = path->livelli[idx_lv_1 - 1];

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_2;
        
        ++dim_eff;
        --idx_lv_1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Implementiamo i primi impulsi di shift. Parte comune al caso pari e a
 *  quello dispari
 */
    for (; idx_lv_1 - idx_lv_0 > 2; ++idx_lv_0, --idx_lv_1)
    {
/*
 *      Allochiamo un impulso parallelo formato da due impulsi
 */
        seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(2);

        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione piana.\nABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Allochiamo i due impulsi che formano l'impulso parallelo
 */
        imp_1 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_0], 
                                            path->livelli[idx_lv_0 + 1],
                                            modulazione_impulso_gauss,
                                            &trasposizione);
        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 - 1],
                                            modulazione_impulso_gauss,
                                            &trasposizione);
        
        if (imp_1 == NULL || imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una rotazione"
                            " piana.\nABORTING...\n\n",
                            dim_eff + 1);
            
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Controlliamo che gi impulsi non siano risonanti tra di loro. In tal
 *      caso solo uno sarà aggiunto alla schedule
 */
        if (!controlla_risonanza_impulsi(imp_1, imp_2))
        {
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        }
        else
        {
            fprintf(stderr, "ATTENZIONE impulsi paralleli con frequenza simile"
                            ".\n"
                            "Rimozione automatica di uno dei due.\n"
                            "Se si è utilizzato l'algoritmo di decomposizione "
                            "di un'unitaria in rotazioni piane e rotazioni Z "
                            "la probabilità di errori nei risultati è molto "
                            "elevata.\n"
                            "Si consiglia di rivedere il sistema fisico, "
                            "oppure di fornire direttamente una sequenza di "
                            "impulsi che decompone l'unitaria desiderata.\n\n"
                            );

            free_impulsi_paralleli(seq->arr_impulsi_paralleli[dim_eff]);

            seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;

            free_impulso(imp_2);
        }

/*
 *      Teniamo traccia della posizione dei livelli "0" ed "1"
 */
        livello[0] = path->livelli[idx_lv_0 + 1];
        livello[1] = path->livelli[idx_lv_1 - 1];

        ++dim_eff;
    }

    if (idx_lv_1 - idx_lv_0 != 1)
    {
        fprintf(stderr, "ERRORE nell'algoritmo di traduzione di una rotazione"
                        " piana.\n"
                        "I livelli tra cui si intende applicare la rotazione"
                        " piana NON sono alla giusta distanza.\n"
                        "ABORTING...\n\n");
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Implementiamo effettivamente la rotazione piana.
 */
    seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
    if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                        " l'impulso parallelo %ld nella traduzione"
                        " di una rotazione piana.\nABORTING...\n\n",
                        dim_eff + 1);
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Calcoliamo i corretti parametri per l'impulso.
 *
 *  Da tenere a mente che qui beta NON può essere arrivato negativo
 */
    if (rot->beta < 0)
    {
        fprintf(stderr, "Fase negativa, evento inaspettato.\n"
                        "ABORTING...\n\n");
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo corretto ordinamento livelli
 *
 *  TODO::  Verificare che non ci siano conflitti con il controllo
 *          della necessità di trasposizione durante gli impulsi
 *          di collegamento
 */
    if (path->livelli[idx_lv_0] > path->livelli[idx_lv_1])
    {
        if (trasposizione == 0u || trasposizione == 3u)
        {
            trasposizione = 1u;
        }
        else
        {
            trasposizione = 0u;
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    double angolo = rot->theta;
    double fase   = determina_fase_impulso(sys, rot,
                                            path->livelli[idx_lv_0],
                                            path->livelli[idx_lv_1],
                                            trasposizione);

    imp_1 = init_impulso(sys,
                            path->livelli[idx_lv_0],
                            path->livelli[idx_lv_1],
                            angolo, fase, modulazione_impulso_gauss);
    
    if (imp_1 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                        " l'impulso %ld nella traduzione di una"
                        " rotazione piana.\n""ABORTING...\n\n",
                        dim_eff + 1);

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
    ++dim_eff;

/*
 *  Aggiungiamo gli impulsi di "discesa" che ripristinano il corretto
 *  ordine degli stati
 */
    for (   ; 
            (pari == 1  ?   idx_lv_1 - idx_lv_0 < distanza_livelli - 1 
                        :   idx_lv_1 - idx_lv_0 < distanza_livelli);
            --idx_lv_0, ++idx_lv_1
        )
    {
/*
 *      Allochiamo un impulso parallelo formato da due impulsi
 */
        seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(2);

        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione piane.\n"
                            "ABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Allochiamo i due impulsi che formano l'impulso parallelo
 */
        imp_1 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_0], 
                                            path->livelli[idx_lv_0 - 1],
                                            modulazione_impulso_gauss,
                                            &trasposizione);
        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 + 1],
                                            modulazione_impulso_gauss,
                                            &trasposizione);
        
        if (imp_1 == NULL || imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una rotazione"
                            " piana.\n"
                            "ABORTING...\n\n",
                            dim_eff + 1);
            
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Controlliamo che gi impulsi non siano risonanti tra di loro. In tal
 *      caso solo uno sarà aggiunto alla schedule
 */
        if (!controlla_risonanza_impulsi(imp_1, imp_2))
        {
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
        }
        else
        {
            fprintf(stderr, "ATTENZIONE impulsi paralleli con frequenza simile"
                            ".\n"
                            "Rimozione automatica di uno dei due.\n"
                            "Se si è utilizzato l'algoritmo di decomposizione "
                            "di un'unitaria in rotazioni piane e rotazioni Z "
                            "la probabilità di errori nei risultati è molto "
                            "elevata.\n"
                            "Si consiglia di rivedere il sistema fisico, "
                            "oppure di fornire direttamente una sequenza di "
                            "impulsi che decompone l'unitaria desiderata.\n\n"
                            );

            free_impulsi_paralleli(seq->arr_impulsi_paralleli[dim_eff]);

            seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_1;

            free_impulso(imp_2);
        }

/*
 *      Teniamo traccia della posizione dei livelli "0" ed "1"
 */
        livello[0] = path->livelli[idx_lv_0 - 1];
        livello[1] = path->livelli[idx_lv_1 + 1];

        ++dim_eff;
    }

/*
 *  Se pari aggiungere l'ultimo impulso di discesa
 */
    if (pari == 1)
    {
    seq->arr_impulsi_paralleli[dim_eff] = alloc_impulsi_paralleli(1);
        
        if (seq->arr_impulsi_paralleli[dim_eff] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso parallelo %ld nella traduzione"
                            " di una rotazione piana.\n"
                            "ABORTING...\n\n",
                            dim_eff + 1);
            
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        imp_2 = crea_impulso_collegamento(sys, livello,
                                            path->livelli[idx_lv_1], 
                                            path->livelli[idx_lv_1 + 1],
                                            modulazione_impulso_gauss,
                                            &trasposizione);
        
        if (imp_2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per"
                            " l'impulso %ld nella traduzione di una"
                            " rotazione piana.\n"
                            "ABORTING...\n\n",
                            dim_eff + 1);

            seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[1] = imp_2;
            free_sequenza_impulsi(seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        seq->arr_impulsi_paralleli[dim_eff]->arr_impulsi[0] = imp_2;
        
/*
 *      Teniamo traccia della posizione del livello "1"
 */
        livello[1] = path->livelli[idx_lv_1 + 1];

        ++dim_eff;
        ++idx_lv_1;
    }

/*
 *  Sanity Check
 */
    if (    path->livelli[idx_lv_0] != rot->livello[0]
        ||  path->livelli[idx_lv_1] != rot->livello[1])
    {
        fprintf(stderr, "ERRORE nella traduzione di una rotazione piana.\n"
                        "I livelli finali non combaciano con quelli iniziali."
                        "\n"
#ifdef DEBUG
                        "Livelli iniziali: \t %ld \t %ld \n"
                        "Livelli finali: \t %ld \t %ld \n"
#endif
                        "ABORTING...\n\n"
#ifdef DEBUG
                        , rot->livello[0], rot->livello[1],
                        path->livelli[idx_lv_0], path->livelli[idx_lv_1]
#endif
                );
        
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return seq; 
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla, dato lo stato dei livelli del sistema, se la
 *  sequenza passata è parallelizzabile
 */
uint8_t controlla_se_par(sequenza_impulsi *seq,
                            stato_liv_par *arr_stato)
{
    /*
     *  Valore di ritorno della funzione
     */
    uint8_t flag = 1;

    impulso *tmp = NULL;

    for (size_t i = 0; (i < seq->dim) && flag; ++i)
    {
        for (size_t j = 0; 
            (j < seq->arr_impulsi_paralleli[i]->dim) && flag; 
            ++j)
        {
            tmp = seq->arr_impulsi_paralleli[i]->arr_impulsi[j];

/*
 *          Vero solo se uno dei livelli necessari all'impulso NON è
 *          disponibile
 */
            if (arr_stato[tmp->livelli[0]] || arr_stato[tmp->livelli[1]])
            {
                flag = 0;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
    }

    return flag;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che parallelizza le sequenze passate. Sostanzialmente fa
 *  un merge degli impulsi paralleli corrispondenti. Ritorna il puntatore
 *  alla nuova sequenza, e libera la memoria occupata dalle sequenze passate
 */
sequenza_impulsi *unione_sequenze_impulsi(sequenza_impulsi **seq, size_t N)
{
/*
 *  Calcoliamo la dimensione della sequenza di impulsi da allocare. Questa è
 *  semplicemente il massimo tra le dimensioni delle seqeunze passate
 */
    size_t dim = 0;

    for (size_t i = 0; i < N; ++i)
    {
        if (seq[i]->dim > dim)
        {
            dim  = seq[i]->dim;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

/*
 *  Allochiamo la sequenza di impulsi che conterrà l'unione di tutte quelle
 *  passate.
 */
    sequenza_impulsi *seq_new = alloc_sequenza_impulsi(dim);

    if(seq_new == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per "
                        "una nuova sequenza di impulsi al momento dell'"
                        "unione di sequenze di impulsi durante la "
                        "parallelizzazione di una schedule.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Variabile d'appoggio che ad ogni iterazione contiene il numero di impulsi
 *  che costituiranno l'impulso paralllelo desiderato
 */
    size_t n_impulsi = 0;

/*
 *  Uniamo gli impulsi paralleli delle varie sequenze in un unico impulso
 *  parallelo
 */
    for (size_t i = 0; i < dim; ++i)
    {
/*
 *      Inizializzazione variabili d'appoggio
 */
        n_impulsi = 0;

/*
 *      Recuperiamo il numero totale di impulsi paralleli da unire
 */
        for (size_t j = 0; j < N; ++j)
        {
            if (seq[j]->dim > i)
            {
                n_impulsi += seq[j]->arr_impulsi_paralleli[i]->dim;
            }
        }

/*
 *      Allochiamo un impulso parallelo delle giuste dimensioni
 */
        impulsi_paralleli *imp_par = alloc_impulsi_paralleli(n_impulsi);

        if (imp_par == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per "
                            "un impulso parallelo contenente l'unione "
                            "degli impulsi paralleli di piu' sequenze.\n"
                            "Aborting...\n");
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      "Riempiamo" l'i impulso parallelo della sequenza
 *
 *      Resettiamo n_impulsi cosicchè possa funzionare da indice
 */

        n_impulsi = 0;

        for (size_t j = 0; j < N; ++j)
        {
            if (seq[j]->dim > i)
            {
                for (size_t k = 0; k < seq[j]->arr_impulsi_paralleli[i]->dim; ++k)
                {
                    imp_par->arr_impulsi[n_impulsi] = 
                        seq[j]->arr_impulsi_paralleli[i]->arr_impulsi[k];
/*  
 *                  Per evitare che il successivo free della sequenza vada ad
 *                  eliminare anche l'impulso
 */ 
                    seq[j]->arr_impulsi_paralleli[i]->arr_impulsi[k] = NULL;
                    ++n_impulsi;
                }
            }
        }
        seq_new->arr_impulsi_paralleli[i] = imp_par;
    }

/*
 *  Liberiamo la memoria allocata per le sequenze di impulsi
 */
    for (size_t i = 0; i < N; ++i)
    {
        free_sequenza_impulsi(seq[i]);
    }

    return seq_new;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che fornita una sequenza di rotazioni e la matrice di
 *  connettività, che ricordo stiamo IPOTIZZANDO "a scala", genera una
 *  schedule di impulsi necessari per la simulazione di tutte le rotazioni
 *  presenti. La sequenza di rotazioni è sempre supposta avere questo ordine:
 *      1.) Rotazioni Z
 *      2.) Rotazioni piane
 *  
 *  Questa convenzione è adottata in quanto è proprio ciò che si verifica
 *  per le rotazioni ottenute tramite l'algoritmo di decomposizione di una
 *  unitaria
 */
schedule *crea_schedule(sequenza_rotazioni *seq_rot, sistema_composto *sys)
{
    if (seq_rot == NULL || sys == NULL)
    {
        fprintf(stderr, "ERRORE nella generazione di una schedule.\n"
                        "Rotazioni o sistema nulli.\n"
                        "ABORTING...\n\n");
    }
    else
    {
        ;   /* TBD */
    }

    size_t dim = seq_rot->dim;
    schedule *sch = alloc_schedule(dim);

    if (sch == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per una"
                        " schedule probabile memoria esaurita.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Traduciamo le rotazioni in sequenze di impulsi
 */
    for (size_t i = 0; i < dim; ++i)
    {
        switch (seq_rot->rot[i]->type)
        {
        case ID_ROT_PIANA:
            sch->arr_sequenze[i] =
                traduci_rotazione_piana(seq_rot->rot[i]->ptr.piana, sys);
            break;
        
        case ID_ROT_Z:
            sch->arr_sequenze[i] =
                traduci_rotazione_Z(seq_rot->rot[i]->ptr.Z, sys);
            break;

        default:
            break;
        }
        
/*
 *      Verifichiamo l'esito della traduzione
 */
        if (sch->arr_sequenze[i] == NULL)
        {
            free_schedule(sch);
            return NULL; /* Errore già segnalato */
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    
    return sch;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che fa un append della schedule sch_app alla schedule sch_base.
 *
 *  La schedule sch_app viene distrutta mentre viene restituito un puntatore
 *  alla schedule finale.
 */
schedule *unione_schedule(schedule *sch_base, schedule *sch_app)
{
    if (sch_base == NULL && sch_app == NULL)
    {
        return NULL;
    }
    else if (sch_base == NULL)
    {
        return sch_app;
    }
    else if (sch_app == NULL)
    {
        return sch_base;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Calcoliamo la dimensione della schedule finale
 */
    size_t dim = sch_base->dim;

/*
 *  Riallochiamo la schedule base
 */
    uint8_t ris = realloc_schedule(sch_base, dim + sch_app->dim);

    if (ris > 0)
    {
        fprintf(stderr, "ERRORE nella riallocazione di una schedule.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_schedule(sch_base);
        free_schedule(sch_app);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo l'esito della riallocazione
 */
    if (sch_base == NULL)
    {
        fprintf(stderr, "ERRORE nella riallocazione di una schedule.\n"
                        "Memoria Insufficiente.\n"
                        "ABORTING...\n\n");
        free_schedule(sch_app);
        return NULL;
    }
    else
    {
        ;   /* DO Nothing */
    }

/*
 *  Copiamo gli impulsi paralleli di sch_app in sch_base
 */
    for (size_t i = dim; i < sch_base->dim; ++i)
    {
        sch_base->arr_sequenze[i] = sch_app->arr_sequenze[i - dim];
        sch_app->arr_sequenze[i - dim] = NULL;
    }

    free_schedule(sch_app);

    return sch_base;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una schedule sch tenta di parallelizzare l'esecuzione
 *  delle sequenze di impulsi che compongono la schedule stessa.
 * 
 *  La funzione e' distruttiva, nel senso che il puntatore passato verra'
 *  "deallocato" e ne verra' restituito unp nuvo alla schedule parallela
 * 
 *  'dim_sys' è la dimensione del sistema fisico composto
 */
schedule *parallelizza_schedule(schedule *sch, size_t dim_sys)
{
    size_t dim = sch->dim;  /* Dimensione della schedule passata */
    size_t dim_eff = 0;     /* Dimensione effettiva della nuova schedule */

/*
 *  Allochiamo una nuova schedule. Inizialmente la allochiamo della 
 *  stessa dimensione di quella passata, dopodichè la ridurremo di
 *  dimensioni se necessario
 */
    schedule *sch_new = alloc_schedule(dim);

    if (sch_new == NULL)
    {
        fprintf(stderr, "ERRORE nella paralellizzazione della schedule.\n"
                        "Memoria per l'allocazione di una nuova schedule"
                        "insufficiente.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Vettore di appoggio che ci dira' quali sequenze parallele possono
 *  essere parallelizzate
 */
    stato_liv_par stato_livelli[dim_sys];

/*
 *  Array d'appoggio per la parallelizzazione delle sequenze. Ad ogni
 *  iterazione conterrà i puntatori alle sequenze da parallelizzare
 */
    sequenza_impulsi *(seq_da_par[dim]);

/*
 *  Variabile d'appoggio che conterrà il numero delle sequenze da
 *  parallelizzare ad ogni iterazione
 */
    size_t seq_parallelizzate = dim;

/*
 *  Ciclo per includere tutte le sequenze di impulsi all'interno della nuova
 *  schedule
 */
    for (size_t i = 0; i < dim; ++i)
    {
/*
 *      Inizializzazione variabili d'appoggio
 */
        init_arr_stato_liv(stato_livelli, dim_sys);

        for (size_t j = 0; j < seq_parallelizzate; ++j)
        {
            seq_da_par[j] = NULL;
        }

        seq_parallelizzate = 0;

/*
 *      Sequenza ancora da includere.
 */
        if (sch->arr_sequenze[i] != NULL)
        {
/*
 *          Aggiungiamo la seqeunza alle sequenze da parallelizzare
 */
            seq_da_par[seq_parallelizzate] = sch->arr_sequenze[i];

/*
 *          Aggiorniamo lo stato dei livelli del sistema
 */
            aggiorna_stato_liv(sch->arr_sequenze[i],
                                stato_livelli,
                                BLOCCATO);

/*
 *          Rimuoviamo la sequenza aggiunta dalla schedule
 */
            sch->arr_sequenze[i] = NULL;
            ++seq_parallelizzate;


/*          Individuiamo le sequenze che possono essere parallelizzate assieme
 *          alla i-esima. Queste saranno necessariamente successive ad essa
 *          data la struttura sequenziale del nostro algoritmo.
 *          (stupido ma visto il tempo a disposizione l'unica cosa fattibile)
 */
            for (size_t j = i + 1; j < dim; ++j)
            {
/*
 *              Non ci sono più livelli disponibili per la parallelizzazione di
 *              altre sequenze
 */
                if (!(controlla_disp_liv(stato_livelli, dim_sys)))
                {
                    break;
                }
                else
                {
                    ;   /* Do Nothing */
                }
                
/*
 *              Se la sequenza è parallelizzabile e non è già inclusa,
 *              includiamola tra le sequenze da parallelizzare e rimuoviamola
 *              dalla schedule attuale
 */
                if (sch->arr_sequenze[j] != NULL)
                {
                    if (controlla_se_par(sch->arr_sequenze[j],
                                            stato_livelli))
                    {
                        seq_da_par[seq_parallelizzate] = sch->arr_sequenze[j];
    
                        aggiorna_stato_liv(sch->arr_sequenze[j],
                                        stato_livelli,
                                        BLOCCATO);
    
                        sch->arr_sequenze[j] = NULL;
                        ++seq_parallelizzate;
                    }
                    else
                    {
                        aggiorna_stato_liv(sch->arr_sequenze[j],
                                        stato_livelli,
                                        BLOCCATO);
                    }
                }
                else
                {
                    ;   /* Do Nothing */
                } 
            }

/*
 *          Parallelizziamo le sequenze ed inseriamo il risultato all'interno
 *          della nuova schedule
 */
            sch_new->arr_sequenze[dim_eff]=
                unione_sequenze_impulsi(seq_da_par, seq_parallelizzate);

/*
 *          Teniamo traccia delle dimensioni effettive della nuova schedule
 */
            ++dim_eff;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    if(realloc_schedule(sch_new, dim_eff))
    {
        fprintf(stderr, "ERRORE nella riallocazione della schedule dopo "
                        "la sua parallelizzazione.\n"
                        "ABORTING...\n\n");
        free_schedule(sch);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free_schedule(sch);

    return sch_new;
}
