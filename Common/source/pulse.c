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

#include "../include/pulse.h"

/*
 *  File che contiene la definizione di tutte le interfacce necessarie alla
 *  gestione di impulsi
 */

//---------------------------------------------------------------------------//

/*
 *  Allocazione di un impulso
 */
impulso *alloc_impulso()
{
    return (impulso*)malloc(sizeof(impulso));
}

/*
 *  Deallocazione di un impulso
 */
void free_impulso(impulso *imp)
{
    free(imp);
    imp = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Allocazione di impulsi paralleli
 */
impulsi_paralleli *alloc_impulsi_paralleli(size_t dim)
{
    impulsi_paralleli *imp_par = 
        (impulsi_paralleli*)malloc(sizeof(impulsi_paralleli));

    if (imp_par == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    imp_par->arr_impulsi = (impulso**)calloc(sizeof(impulso*), dim);

    if (imp_par->arr_impulsi == NULL)
    {
        free_impulsi_paralleli(imp_par);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    imp_par->stato = NON_SIMULATO;  /* Stato di Default */

    imp_par->dim = dim;
    return imp_par;
}

/*
 *  Deallocazione di impulsi paralleli
 */
void free_impulsi_paralleli(impulsi_paralleli *imp_par)
{
    for (size_t i = 0; i < imp_par->dim; ++i)
    {
        if (imp_par->arr_impulsi[i] != NULL)
        {
            free_impulso(imp_par->arr_impulsi[i]);
            imp_par->arr_impulsi[i] = NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free(imp_par->arr_impulsi);
    imp_par->arr_impulsi = NULL;

    free(imp_par);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una sequenza di impulsi
 */
sequenza_impulsi *alloc_sequenza_impulsi(size_t dim)
{
    sequenza_impulsi *seq =
        (sequenza_impulsi*)malloc(sizeof(sequenza_impulsi));

    if (seq == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->arr_impulsi_paralleli =
        (impulsi_paralleli**)calloc(sizeof(impulsi_paralleli*), dim);
    
    if (seq->arr_impulsi_paralleli == NULL)
    {
        free_sequenza_impulsi(seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->dim = dim;
    return seq;
}

/*
 *  Deallocazione di una sequenza di impulsi
 */
void free_sequenza_impulsi(sequenza_impulsi *seq)
{
    for (size_t i = 0; i < seq->dim; ++i)
    {
        if (seq->arr_impulsi_paralleli[i] != NULL)
        {
            free_impulsi_paralleli(seq->arr_impulsi_paralleli[i]);
            seq->arr_impulsi_paralleli[i] = NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free(seq->arr_impulsi_paralleli);
    seq->arr_impulsi_paralleli = NULL;

    free(seq);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una schedule
 */
schedule *alloc_schedule(size_t dim)
{
    schedule *sch =
        (schedule*)malloc(sizeof(schedule));

    if (sch == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sch->arr_sequenze =
        (sequenza_impulsi**)calloc(sizeof(sequenza_impulsi*), dim);
    
    if (sch->arr_sequenze == NULL)
    {
        free_schedule(sch);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sch->dim = dim;
    return sch;
}

/*
 *  Riallocazione di una schedule
 */
uint8_t realloc_schedule(schedule *sch, size_t dim)
{
    sequenza_impulsi **ptr =
        (sequenza_impulsi**)realloc(sch->arr_sequenze, sizeof(sequenza_impulsi*) * dim);
    
    if (ptr == sch->arr_sequenze)
    {
        sch->dim = dim;
    }
    else if (ptr != NULL)
    {
        sch->arr_sequenze = ptr;
        sch->dim = dim;
    }
    else if (dim == 0)
    {
        sch->dim = 0;
    }
    else
    {
        return 1;
    }

    return 0;
}

/*
 *  Deallocazione schedule
 */
void free_schedule(schedule *sch)
{
    if (sch == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sch->arr_sequenze != NULL)
    {
        for (size_t i = 0; i < sch->dim; ++i)
        {
            if (sch->arr_sequenze[i] != NULL)
            {
                free_sequenza_impulsi(sch->arr_sequenze[i]);
                sch->arr_sequenze[i] = NULL;
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

    free(sch->arr_sequenze);
    sch->arr_sequenze = NULL;

    free(sch);
    
    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato l'angolo di rotazione di un impulso, la sua funzione
 *  di modulazione e la matrice di connettività, ne calcola la durata.
 * 
 *  Ritona 0 se tutto è andato a buon fine, un numero positivo se vi sono
 *  stati degli errori.
 */
uint8_t imposta_durata_impulso(gsl_matrix_complex *M, impulso *V)
{
    gsl_complex tmp = gsl_matrix_complex_get(M, V->livelli[0], V->livelli[1]);

/*
 *  Controlliamo di star eseguendo una traslazione legale
 */
    if (gsl_complex_abs(tmp) < 1e-12)
    {
        fprintf(stderr, "ERRORE nella formazione di un impulso.\n"
                        "Livelli non collegabili.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;    /* Do Nothing */
    }

/*
 *  Individuiamo in base alla funzione di modulazione il corretto valore di T
 */
    if (V->modulazione == modulazione_impulso_gauss)
    {
/*
 *      Impulso Gaussiano: l'impulso dura AMPIEZZA * sigma
 */
        V->T = AMPIEZZA * V->theta / (gsl_complex_abs(tmp) * sqrt(2.0 * M_PI));
    }
    else if (V->modulazione == modulazione_impulso_hs)
    {
/*
 *      Impulso rettangolare
 */
        V->T = V->theta / gsl_complex_abs(tmp);
    }
    else if (V->modulazione == modulazione_impulso_zero)
    {
        ;   /* Do Nothing */
    }
    else
    {
        fprintf(stderr, "ERRORE nell'assegnazione della funzione di "
                        "modulazione dell'impulso\n");
        return 1;
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un impulso V ne calcola la modulazione all'istante t
 *  data da
 *
 *                              A = cos(omega * t)
 * 
 *  ATTENZIONE::    A differenza di quando abbiamo testato gli algoritmi di
 *                  simulazione il campo magnetico esterno o qualsiasi cosa
 *                  ne faccia le veci deve essere già compreso all'interno
 *                  della matrice di connettività fornita
 */
gsl_complex modulazione_impulso_hs(const double t, void *params)
{
    impulso *V = (impulso*)params;

    gsl_complex ampiezza;

/*
 *  Verifichiamo che l'impulso sia ancora da eseguire
 *
 *  Check necessario in quanto potremmo avere più impulsi in parallelo che
 *  hanno durate differenti.
 */
    if (t > V->T)
    {
        ampiezza = gsl_complex_rect(0.0, 0.0);
    }
    else
    {
        double arg_cos = t * V->omega + V->gamma;
        double tmp = arg_cos / (2.0 * M_PI);
        int64_t quoziente = (int64_t)(tmp);

        arg_cos = 2.0 * M_PI * (tmp - (double)quoziente);

        tmp = cos(arg_cos);

        ampiezza = gsl_complex_rect(tmp, 0.0);
    }

    return ampiezza;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un impulso V ne calcola la modulazione all'istante t
 *  data da
 *
 *                  A = cos(omega * t) * exp(-{(t-T)/sigma}^2)
 *  
 *  con
 *                          T := SEMI_AMPIEZZA * sigma 
 *
 *  dove sigma è la deviazione standard della gaussiana.
 * 
 *  ATTENZIONE::    A differenza di quando abbiamo testato gli algoritmi di
 *                  simulazione il campo magnetico esterno o qualsiasi cosa
 *                  ne faccia le veci deve essere già compreso all'interno
 *                  della matrice di connettività fornita
 */
gsl_complex modulazione_impulso_gauss(const double t, void *params)
{
    impulso *V = (impulso*)params;

    gsl_complex ampiezza;

/*
 *  Deviazione standard della gaussiana che modula l'impulso
 */
    double sigma = V->T / AMPIEZZA;

/*
 *  Verifichiamo che l'impulso sia ancora da eseguire
 *
 *  Check necessario in quanto potremmo avere più impulsi in parallelo che
 *  hanno durate differenti. Non so se sia meglio metterlo qui o all'interno
 *  della valutazione temporale della sequenza di impulsi paralleli.
 *  Per il momento lo lascio qui privilegiando la sicurezza.
 */

    if (t > AMPIEZZA * sigma)
    {
        ampiezza = gsl_complex_rect(0.0, 0.0);
    }
    else
    {
        double arg_cos = t * V->omega + V->gamma;
        double tmp = arg_cos / (2.0 * M_PI);
        int64_t quoziente = (int64_t)(tmp);

        arg_cos = 2.0 * M_PI * (tmp - (double)quoziente);


        tmp = cos(arg_cos);

        double tmp2 = -1.0 
                    * (t - SEMI_AMPIEZZA * sigma) * (t - SEMI_AMPIEZZA * sigma) 
                    / (2.0 * sigma * sigma);
        tmp2  = exp(tmp2);

        tmp = tmp * tmp2;
        ampiezza = gsl_complex_rect(tmp, 0.0);
    }

    return ampiezza;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la realizzazione di un impulso nullo necessario
 *  all'implementazione del gate built-in $$IDLE
 */
gsl_complex modulazione_impulso_zero(const double t, void *params)
{
/*
 *  Necessario per ammutolire gcc e -Werror=unused-parameter visto che ritengo
 *  che abilitare il flag -Wno-unused-parameter per tutto il file sia un male
 *  peggiore
 */
    impulso *V = (impulso*)params;

    V->theta = 0.0 * (V->theta * t);
/*
 *  Ritorniamo ampiezza sempre nulla
 */
    gsl_complex ampiezza = gsl_complex_rect(0.0, 0.0);
    return ampiezza;
}

//---------------------------------------------------------------------------//

/*
 *  funzione che inizializza un impulso tramite i parametri passati. 
 */
impulso *init_impulso(sistema_composto *sys,
                        size_t lv_0, size_t lv_1,
                        double theta, double gamma,
                        gsl_complex (*modulazione) (const double t,
                                                    void *params))
{
    impulso *imp = alloc_impulso();

    if (imp == NULL )
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    imp->livelli[0] = lv_0;
    imp->livelli[1] = lv_1;
    imp->theta = theta;
    imp->gamma = gamma;

    double tmp1 = gsl_vector_get(sys->autovalori, lv_0);
    double tmp2 = gsl_vector_get(sys->autovalori, lv_1);
    imp->omega = fabs(tmp1 - tmp2);

    imp->modulazione = modulazione;

    uint8_t ris = imposta_durata_impulso(sys->M, imp);

    if(ris != 0)
    {
        free_impulso(imp);
        return NULL;
    }

    return imp;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che alloca un opportuno impulso di collegamento una volta dati
 *  i due livelli tra cui eseguire l'impulso. Questa funzione NON verifica
 *  la collegabilità tra i due livelli, il chiamante deve occuparsene.
 *  In caso di errore un puntatore nullo verrà restituito.
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
 *                          indice maggiore (dopo lo scambio)
 * 
 *  lv_0 --> lo stato che ci interessa spostare
 *  lv_1 --> lo stato IN CUI vogliamo spostare lo stato in lv_0
 * 
 *  ATTENZIONE::    L'ordine tra lv_0 ed lv_1 è fondamentale!!!
 *
 *  HP::    Stiamo supponendo una matrice di connessione del tipo Sy
 */
impulso *crea_impulso_collegamento(sistema_composto *sys,
                                    uint64_t *livello,
                                    size_t lv_0,
                                    size_t lv_1,
                                    gsl_complex (*modulazione)
                                    (const double t, void *params),
                                    uint8_t *trasposizione)
{
    
    double fase = 0.0;

/*
 *  Verifichiamo se l'impulso di collegamento è "a scendere" o "a salire"
 */
    if (lv_0 < lv_1)
    {
/*
 *      Se l'elemento fdella matrice di connessione sopra diagonale
 *      NON è positivo vuol dire che i livelli energetici non sono
 *      ordinati come gli autovalori di Sz e dunque ci balla una
 *      fase relativa di "pi".
 */
        if (test_ordine_livelli(sys->M, lv_0, lv_1))
        {
            fase = M_PI;
        }
        else
        {
            fase = 0.0;
        }

/*
 *      Controlliamo se questo impulso inverte l'ordine degli stati soggetti
 *      alla rotazione.
 */
        if (lv_1 < *(livello + 0))
        {
/*
 *          Determiniamo il corretto valore per il flag.
 *              0 --> trasposizione NON necessaria
 *              1 --> trasposizione necessaria avvenuta in discesa
 *              2 --> trasposizione necessaria avvenuta in salita
 *              3 --> trasposizione NON necessaria
 *                    (flip avvenuto in salita e discesa)
 */
            if (*trasposizione == 1u)
            {
                *trasposizione = 0u;
            }
            else if (*trasposizione == 2u)
            {
                *trasposizione = 3u;
            }
            else if (*trasposizione == 3u) 
            {
                *trasposizione = 2u;
            }
            else if (*trasposizione == 0u)
            {
                *trasposizione = 1u;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
        else if ((*trasposizione == 1u || *trasposizione == 3u) 
                && lv_1 < *(livello + 1))
        {
            (*trasposizione) -= 1u;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
/*
 *      Se l'elemento della matrice di connessione sopra diagonale
 *      NON è positivo vuol dire che i livelli energetici non sono
 *      ordinati come gli autovalori di Sz e dunque ci balla una
 *      fase relativa di "pi".
 */
        if (test_ordine_livelli(sys->M, lv_0, lv_1))
        {
            fase = 0.0;
        }
        else
        {
            fase = M_PI;
        }

/*
 *      Controlliamo se questo impulso inverte l'ordine degli stati soggetti
 *      alla rotazione.
 */
        if (lv_1 < *(livello + 0))
        {
/*
 *          Determiniamo il corretto valore per il flag.
 *              0 --> trasposizione NON necessaria
 *              1 --> trasposizione necessaria avvenuta in discesa
 *              2 --> trasposizione necessaria avvenuta in salita
 *              3 --> trasposizione NON necessaria
 *                    (flip avvenuto in salita e discesa)
 */
            if (*trasposizione == 2u)
            {
                *trasposizione = 0u;
            }
            else if (*trasposizione == 1u)
            {
                *trasposizione = 3u;
            }
            else if (*trasposizione == 3u) 
            {
                *trasposizione = 1u;
            }
            else if (*trasposizione == 0u)
            {
                *trasposizione = 2u;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
        else if (*trasposizione > 1u && lv_1 > *(livello + 0))
        {
            (*trasposizione) -= 2u;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    impulso *imp =
        init_impulso(sys, lv_0, lv_1,  M_PI, fase, modulazione);
        
    return imp;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che determina il corretto angolo dell'impulso che applica una
 *  rotazione Z
 *
 *  HP::    Stiamo supponendo una matrice di connessione del tipo Sy
 */
double determina_angolo_rotazione_Z(sistema_composto *sys,
                                    rotazione_Z *rot,
                                    size_t lv_0,
                                    size_t lv_1,
                                    uint8_t inversione)
{
    double phi = 0.0;

/*
 *  controlliamo che l'ordinamento dei livelli sia quello corretto
 */
    if (lv_1 < lv_0)
    {
        size_t tmp = lv_1;
        lv_1 = lv_0;
        lv_0 = tmp; 
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *      Se l'elemento della matrice di connessione sopra diagonale
 *      NON è positivo vuol dire che i livelli energetici non sono
 *      ordinati come gli autovalori di Sz e dunque ci "balla" una
 *      fase relativa di "pi".
 */
    if (test_ordine_livelli(sys->M, lv_0, lv_1))
    {
        phi = rot->phi;
    }
    else
    {
        phi = rot->phi - M_PI;
    }

    if (inversione > 0u && inversione < 3u)
    {
        phi *= -1.0;
    }

    return phi;
}
//---------------------------------------------------------------------------//

/*
 *  Funzione che determina la coretta fase dell'impulso che applica
 *  l'effettiva rotazione piana
 * 
 *  HP::    Stiamo supponendo una matrice di connessione del tipo Sy
 * 
 *  TODO::  Rimuovere l'HP sopra!!!
 */
double determina_fase_impulso(sistema_composto *sys,
                                rotazione_piana *rot,
                                size_t lv_0,
                                size_t lv_1,
                                uint8_t trasposizione)
{
    double fase = 0.0;

/*
 *  controlliamo che l'ordinamento dei livelli sia quello corretto
 */
    if (lv_1 < lv_0)
    {
        size_t tmp = lv_1;
        lv_1 = lv_0;
        lv_0 = tmp; 
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *      Se l'elemento della matrice di connessione sopra diagonale
 *      NON è positivo vuol dire che i livelli energetici non sono
 *      ordinati come gli autovalori di Sz e dunque ci balla una
 *      fase relativa di "pi".
 */
    if (test_ordine_livelli(sys->M, lv_0, lv_1))
    {
        fase = M_PI - rot->beta;
    }
    else
    {
/*
 *      per la periodicità del coseno abbiamo:
 *                      pi - beta + pi = 2*pi - beta = -beta
 */
        fase = -1.0 * rot->beta;
    }

/*
 *  Controlliamo se è necessario eseguire una trasposizione dell'operatore
 */
    if (trasposizione > 0u && trasposizione < 3u)
    {
        fase += M_PI;
        fase *= -1.0;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return fase;
}                               

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla la possibile risonanza di due impulsi. Ritorna 0
 *  se i due impulsi non sono in stretta risonanza, 1 altrimenti. Non protegge
 *  in alcun modo da piccoli leakage
 */
uint8_t controlla_risonanza_impulsi(impulso *imp_1, impulso *imp_2)
{
    double w1;
    double w2;
    double diff;
    double max;

    w1 = imp_1->omega;
    w2 = imp_2->omega;
    diff = fabs(w1 - w2);
    max = (fabs(w1) > fabs(w2)) ? fabs(w1) : fabs(w2);

    if (diff / max > 1e-6)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una schedule ed un sistema composto corregge la
 *  durata degli impulsi in base ai parametri del sistema stesso
 */
uint8_t correggi_impulsi(sistema_composto *sys, schedule *sch)
{
    if (sys == NULL || sch == NULL)
    {
        fprintf(stderr, "ERRORE nella correzione della durata degli impulsi.\n"
                        "Argomento nullo.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do nothing */
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

                double omega = gsl_vector_get(comp_sys->autovalori, imp->livelli[0]);
                omega -= gsl_vector_get(comp_sys->autovalori, imp->livelli[1]);

                omega = fabs(omega);

                if (omega < 1e-12)
                {
                    fprintf(stderr, "ERRORE nella correzione di un impulso.\n"
                                    "frequenza risonante NULLA.\n"
                                    "ABORTING...\n\n");
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }
                imp->omega = omega;

                uint8_t ris = imposta_durata_impulso(sys->M, imp);

                if (ris > 0)
                {
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }
            }
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che conta il numero di impulsi di una schedule
 */
size_t conta_impulsi_schedule(schedule *sch)
{
    size_t n_impulsi = 0;
    
    for (size_t i = 0; i < sch->dim; ++i)
    {
        n_impulsi += sch->arr_sequenze[i]->dim;
    }

    return n_impulsi;
}

//---------------------------------------------------------------------------//