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

#include "../include/physical_system.h"
#include "../include/spin_operators.h"
#include "../include/global.h"
#include "../include/output.h"
#include "../include/tensor.h"
#include "../include/input.h"
#include "../include/eigen.h"

/*
 *  Header File contenente le definizioni di tutte le funzioni
 *  necessarie alla descrizione del sistema fisico preso
 *  in esame.
 */
//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione dello spazio di lavoro
 *  necessario alla simulazione dell'errore di dephasing
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione dello spazio di lavoro per la simulazione
 *  dell'errore di dephasing
 */
spazio_di_lavoro_errore *alloc_spazio_di_lavoro_errore(size_t dim)
{
    spazio_di_lavoro_errore *sle =
        (spazio_di_lavoro_errore*)malloc(sizeof(spazio_di_lavoro_errore));

    if (sle == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per "
                        "lo spazio di lavoro per la simulazione "
                        "dell'errore.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sle->tmp1 = gsl_matrix_complex_calloc(dim, dim);
    sle->tmp2 = gsl_matrix_complex_calloc(dim, dim);
    sle->tmp3 = gsl_matrix_complex_calloc(dim, dim);
    sle->tmp4 = gsl_matrix_complex_calloc(dim, dim);
    sle->tmp5 = gsl_matrix_complex_calloc(dim, dim);

    if (sle->tmp1 == NULL || sle->tmp2 == NULL || sle->tmp3 == NULL
        || sle->tmp4 == NULL || sle->tmp5 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione delle matrici di supporto "
                        "per la simulazione dell'errore di dephasing.\n"
                        "ABORTING...\n\n");
        free_spazio_di_lavoro_errore(sle);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return sle;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione dello spazio di lavore per la simulazione
 *  dell'errore di dephasing
 */
void free_spazio_di_lavoro_errore(spazio_di_lavoro_errore *sle)
{
    if (sle->tmp1 != NULL)
    {
        gsl_matrix_complex_free(sle->tmp1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sle->tmp1 != NULL)
    {
        gsl_matrix_complex_free(sle->tmp2);
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sle->tmp1 != NULL)
    {
        gsl_matrix_complex_free(sle->tmp3);
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sle->tmp1 != NULL)
    {
        gsl_matrix_complex_free(sle->tmp4);
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sle->tmp1 != NULL)
    {
        gsl_matrix_complex_free(sle->tmp5);
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(sle);
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione del sistema fisico
 *  studiato
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema sastratto NON affetto da errore
 */
sistema_astratto *alloc_sistema_astratto_NO_ERR(size_t N)
{
    sistema_astratto *sys =
        (sistema_astratto*)malloc(sizeof(sistema_astratto));

    if (sys == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il sistema "
                        "astratto.\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->hamiltoniana = 
        (gsl_matrix_complex**)calloc(sizeof(gsl_matrix_complex*), N);

    if (sys->hamiltoniana == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per le "
                        "hamiltoniane.\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->M = (gsl_matrix_complex**)calloc(sizeof(gsl_matrix_complex*), N);

    if (sys->M == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per le "
                        "hamiltoniane.\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->n_livelli = (size_t*)calloc(sizeof(size_t), N);

    if (sys->n_livelli == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il vettore"
                        " dei livelli dei qudit."
                        "\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->operatore_errore = NULL;
    sys->M_tot = NULL;
    sys->rateo = NULL;

    sys->n_qudit = N;

    return sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema sastratto affetto da errore
 */
sistema_astratto *alloc_sistema_astratto_ERR(size_t N)
{
    sistema_astratto *sys = 
        (sistema_astratto*)malloc(sizeof(sistema_astratto));

    if (sys == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il sistema."
                        "\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->hamiltoniana = 
        (gsl_matrix_complex**)calloc(sizeof(gsl_matrix_complex*), N);

    if (sys->hamiltoniana == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per le "
                        "hamiltoniane.\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->M = (gsl_matrix_complex**)calloc(sizeof(gsl_matrix_complex*), N);

    if (sys->M == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per le "
                        "hamiltoniane.\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->n_livelli = (size_t*)calloc(sizeof(size_t), N);

    if (sys->n_livelli == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il vettore"
                        " dei livelli dei qudit."
                        "\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->operatore_errore = 
        (gsl_matrix_complex**)calloc(sizeof(gsl_matrix_complex*), N);

    if (sys->operatore_errore == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per l'array"
                        " degli operatori d'errore."
                        "\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->rateo = (double*)calloc(sizeof(double), N);
    if (sys->rateo == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per l'array"
                        " dei tempi di dephasing."
                        "\n"
                        "ABORTING...\n\n");

        free_sistema_astratto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->M_tot = NULL;
    sys->n_qudit = N;

    return sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione di un sistema sastratto
 */
void free_sistema_astratto(sistema_astratto *sys)
{
    if (sys == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->hamiltoniana != NULL)
    {
        for (size_t i = 0; i < sys->n_qudit; ++i)
        {
            if (sys->hamiltoniana[i] != NULL)
            {
                gsl_matrix_complex_free(sys->hamiltoniana[i]);
                sys->hamiltoniana[i] = NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }

        free(sys->hamiltoniana);
        sys->hamiltoniana = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->M != NULL)
    {
        for (size_t i = 0; i < sys->n_qudit; ++i)
        {
            if (sys->M[i] != NULL)
            {
                gsl_matrix_complex_free(sys->M[i]);
                sys->M[i] = NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }

        free(sys->M);
        sys->M = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->J != NULL)
    {
        gsl_matrix_complex_free(sys->J);
        sys->J = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->M_tot != NULL)
    {
        gsl_matrix_complex_free(sys->M_tot);
        sys->M_tot = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->operatore_errore != NULL)
    {
        for (size_t i = 0; i < sys->n_qudit; ++i)
        {
            if (sys->operatore_errore[i] != NULL)
            {
                gsl_matrix_complex_free(sys->operatore_errore[i]);
                sys->operatore_errore[i] = NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }

        free(sys->operatore_errore);
        sys->operatore_errore = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->rateo != NULL)
    {
        free(sys->rateo);
        sys->rateo = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->n_livelli != NULL)
    {
        free(sys->n_livelli);
        sys->n_livelli = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(sys);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di una struttura
 *  dati "sistema_composto"
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema composto NON affetto da errore
 */
sistema_composto *alloc_sistema_composto_NO_ERR(size_t N, size_t dim)
{
    sistema_composto *sys =
        (sistema_composto*)malloc(sizeof(sistema_composto));

    if (sys == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il sistema "
                        "astratto.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->autovalori = gsl_vector_calloc(dim);

    if (sys->autovalori == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione del vettore contenente gli "
                        "autovalori del Hamiltoniana.\n"
                        "ABORTING...\n\n");
        free_sistema_composto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->operatore_errore = NULL;
    sys->rateo = NULL;
    sys->sle = NULL;  

    sys->n_qudit = N;
    sys->dim = dim;

    return sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema composto affetto da errore
 */
sistema_composto *alloc_sistema_composto_ERR(size_t N, size_t dim)
{
    sistema_composto *sys =
        (sistema_composto*)malloc(sizeof(sistema_composto));

    if (sys == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il sistema "
                        "astratto.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->autovalori = gsl_vector_calloc(dim);

    if (sys->autovalori == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione del vettore contenente gli "
                        "autovalori del Hamiltoniana.\n"
                        "ABORTING...\n\n");
        free_sistema_composto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->operatore_errore = 
        (gsl_matrix_complex**)calloc(sizeof(gsl_matrix_complex*), N);

    if (sys->operatore_errore == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per l'array"
                        " degli operatori d'errore."
                        "\n"
                        "ABORTING...\n\n");
        free_sistema_composto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < N; ++i)
    {
        sys->operatore_errore[i] = gsl_matrix_complex_calloc(dim, dim);

        if (sys->operatore_errore == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione di uno degli operatori "
                            "di errore.\n"
                            "ABORTING...\n\n");
            free_sistema_composto(sys);

            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    sys->rateo = (double*)calloc(sizeof(double), N);

    if (sys->rateo == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per l'array"
                        " dei tempi di dephasing."
                        "\n"
                        "ABORTING...\n\n");
        free_sistema_composto(sys);

        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->sle = alloc_spazio_di_lavoro_errore(dim);

    if (sys->sle == NULL)
    {
        free_sistema_composto(sys);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sys->n_qudit = N;
    sys->dim = dim;

    return sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la reinizializzazione di un sistema composto
 */
void reset_sistema_composto(sistema_composto *sys)
{
    gsl_vector_free(sys->autovalori);
    sys->autovalori = NULL;

    gsl_matrix_complex_free(sys->M);
    sys->M = NULL;
}


/*
 *  Funzione per la deallocazione di un sistema composto
 */
void free_sistema_composto(sistema_composto *sys)
{
    if (sys == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->autovalori != NULL)
    {
        gsl_vector_free(sys->autovalori);
        sys->autovalori = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->M != NULL)
    {
        gsl_matrix_complex_free(sys->M);
        sys->M = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->operatore_errore != NULL)
    {
        for (size_t i = 0; i < sys->n_qudit; ++i)
        {
            if (sys->operatore_errore[i] != NULL)
            {
                gsl_matrix_complex_free(sys->operatore_errore[i]);
                sys->operatore_errore[i] = NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }

        free(sys->operatore_errore);
        sys->operatore_errore = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->rateo != NULL)
    {
        free(sys->rateo);
        sys->rateo = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sys->sle != NULL)
    {
        free_spazio_di_lavoro_errore(sys->sle);
        sys->sle = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(sys);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che, passate tutte le informazioni di input necessarie attraverso
 *  la struttura dati "input", alloca e ritorna un opportuno "sistema_astratto"
 * 
 *  Il parametro 'qudit_flag' ci dirà se la dimensione ed il numero di ogni
 *  qudit è stato definito all'interno di un circuito fornito come input
 *  oppure se viene passato tramite file. Viene inserito questo flag per
 *  ragioni di retrocompatibilità, i.e., per permettere all' utente di svolgere
 *  simulazioni fornendo direttamente sequenze di impulsi/rotazioni o unitarie
 *  generiche.
 */
sistema_astratto *init_sistema_astratto(void *in_ptr)
{
    input *in = (input*)in_ptr;

    size_t N;
    size_t ris;

/*
 *  Recuperiamo il numero di qudit del sistema
 */
    N = numero_qudit;

/*
 *  Allochiamo il sistema astratto
 */
    sistema_astratto *sys;
/*
 *  Verifichiamo se le simulazioni comprendono o meno l'errore
 */
    if (errore_abilitato > 0)
    {
        sys = alloc_sistema_astratto_ERR(N);
/*
 *      Leggiamo i ratei di dephasing
 */
        ris = fread(sys->rateo,sizeof(double),sys->n_qudit, in->f_in_R);

/*
 *      Controlliamo la corretta lettura dei ratei
 */
        if (ris != sys->n_qudit)
        {
            fprintf(stderr, "ERRORE nella lettura dei ratei di dephasing.\n"
                            "ABORTING...\n\n");
            free_sistema_astratto(sys);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
        sys = alloc_sistema_astratto_NO_ERR(N);
    }

/*
 *  Controlliamo il successo dell'allocazione del sistema
 */
    if (sys == NULL)
    {
        free_sistema_astratto(sys);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Recuperiamo le dimensioni di ogni qudit
 */
    for (size_t i = 0; i < sys->n_qudit; ++i)
    {
        sys->n_livelli[i] = dimensioni_qudit[i];
    }

/*
 *  Calcoliamo la dimensione del sistema completo
 */
    sys->dim = sys->n_livelli[0];
    for (size_t i = 1; i < sys->n_qudit; ++i)
    {
        sys->dim *= sys->n_livelli[i];
    }

/*
 *  Leggiamo le hamiltoniane del sistema e gli operatori d'errore se necessario
 */
    for (size_t i = 0; i < sys->n_qudit; ++i)
    {
        if (errore_abilitato > 0)
        {
/*
 *          Allochiamo la memoria per un operatorre di errore
 */
            sys->operatore_errore[i] =
                gsl_matrix_complex_calloc(sys->dim, sys->dim);
/*
 *          Verifichiamo l'esito dell'allocazione
 */
            if (sys->operatore_errore[i] == NULL)
            {
                fprintf(stderr, "ERRORE nell'allocazione dell'operatore "
                                "di errore %ld.\n"
                                "ABORTING...\n\n", i+1);
                
                free_sistema_astratto(sys);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }

/*
 *          Allochiamo la memoria per la matrice contenente l'operatore di
 *          errore sul qudit
 */
            size_t tmp_dim = dimensioni_qudit[i];
            gsl_matrix_complex *tmp_matrix =
                gsl_matrix_complex_calloc(tmp_dim, tmp_dim);
/*
 *          Verifichiamo l'esito dell'allocazione
 */
            if (tmp_matrix == NULL)
            {
                fprintf(stderr, "ERRORE nell'allocazione dell'operatore "
                                "di errore %ld.\n"
                                "ABORTING...\n\n", i+1);
                
                free_sistema_astratto(sys);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }           

            ris = gsl_matrix_complex_fread(in->f_in_E, tmp_matrix);
            if (ris == GSL_EFAILED)
            {
                fprintf(stderr, "ERRORE nella lettura degli "
                                "operatori di errore.\n"
                                "ABORTING...\n\n");
                gsl_matrix_complex_free(tmp_matrix);
                free_sistema_astratto(sys);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }

#ifdef DEBUG
            fprintf(stdout, "\nOperatore Errore %ld-esimo letto:\n", i);
            stampa_matrice(stdout, tmp_matrix);
#endif

/*
 *          Costruiamo l'operatore di errore totale. Altro non è che
 *          l'operatore letto tensor I opportune volte
 */
            gsl_matrix_complex *base = NULL;
            
            if (i == 0)
            {
                base = tmp_matrix;
            }
            else
            {
                tmp_dim = dimensioni_qudit[0];
                base = gsl_matrix_complex_calloc(tmp_dim, tmp_dim);

                if (base == NULL)
                {
                    fprintf(stderr, "ERRORE nell'allocazione dell'operatore "
                                    "di errore %ld.\n"
                                    "ABORTING...\n\n", i+1);
                    gsl_matrix_complex_free(tmp_matrix);
                    free_sistema_astratto(sys);
                    return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }

                gsl_matrix_complex_set_identity(base);
            }

            for (size_t j = 1; j < sys->n_qudit; ++j)
            {
                gsl_matrix_complex *nuovo_termine = NULL;

                if (j != i)
                {
                    tmp_dim = dimensioni_qudit[j];
                    nuovo_termine = 
                        gsl_matrix_complex_calloc(tmp_dim, tmp_dim);
                    
                    if (nuovo_termine == NULL)
                    {
                        fprintf(stderr, "ERRORE nell'allocazione dell'operatore "
                                        "di errore %ld.\n"
                                        "ABORTING...\n\n", i+1);
                        gsl_matrix_complex_free(tmp_matrix);
                        gsl_matrix_complex_free(base);
                        free_sistema_astratto(sys);
                        return NULL;
                    }
                    else
                    {
                        ;   /* Do Nothing */
                    }
                    
                    gsl_matrix_complex_set_identity(nuovo_termine);
                }
                else
                {
                    nuovo_termine = tmp_matrix;
                }

                gsl_matrix_complex *prodotto = 
                    complex_kron(base, nuovo_termine);

                if (prodotto == NULL)
                {
                        gsl_matrix_complex_free(nuovo_termine);
                        gsl_matrix_complex_free(tmp_matrix);
                        gsl_matrix_complex_free(base);
                        free_sistema_astratto(sys);
                        return NULL;
                }
                else
                {
                    ;   /* Do Nothing */
                }

                gsl_matrix_complex_free(nuovo_termine);
                gsl_matrix_complex_free(base);

                base = prodotto;          
            }

            gsl_matrix_complex_memcpy(sys->operatore_errore[i], base);

#ifdef DEBUG
            fprintf(stdout, "\nOperatore Errore %ld-esimo:\n", i);
            stampa_matrice(stdout, sys->operatore_errore[i]);
#endif
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Allochiamo la memoria per un'Hamiltoniana
 */
        sys->hamiltoniana[i] =
            gsl_matrix_complex_calloc(sys->n_livelli[i], sys->n_livelli[i]);
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (sys->hamiltoniana[i] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione del Hamiltoniana "
                            "%ld.\n"
                            "ABORTING...\n\n", i+1);
            
            free_sistema_astratto(sys);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        ris = gsl_matrix_complex_fread(in->f_in_H, sys->hamiltoniana[i]);

#ifdef DEBUG
        fprintf(stdout, "Hamiltoniana %ld-esima:\n", i);
        stampa_matrice(stdout, sys->hamiltoniana[i]);
#endif
        if (ris == GSL_EFAILED)
        {
            fprintf(stderr, "ERRORE nella lettura delle hamiltoniane.\n"
                            "ABORTING...\n\n");
            free_sistema_astratto(sys);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Allochiamo la memoria per una matrice di connettività
 */
        sys->M[i] =
            gsl_matrix_complex_calloc(sys->n_livelli[i], sys->n_livelli[i]);
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (sys->M[i] == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della matrice di "
                            "connesione %ld.\n"
                            "ABORTING...\n\n", i+1);
            
            free_sistema_astratto(sys);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        ris = gsl_matrix_complex_fread(in->f_in_M, sys->M[i]);

        if (ris == GSL_EFAILED)
        {
            fprintf(stderr, "ERRORE nella lettura delle hamiltoniane.\n"
                            "ABORTING...\n\n");
            free_sistema_astratto(sys);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

/*
 *  Allochiamo la memoria per il tensore di connessione dei qudit
 */
    sys->J = gsl_matrix_complex_calloc(sys->n_qudit, sys->n_qudit);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (sys->J == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione del tensore di connessione.\n"
                        "ABORTING...\n\n");
        
        free_sistema_astratto(sys);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Leggiamo il tensore di connettività delle hamiltoniane
 */
    ris = gsl_matrix_complex_fread(in->f_in_J, sys->J);

    if (ris == GSL_EFAILED)
    {
        fprintf(stderr, "ERRORE nella lettura del tensore di connettivita "
                        "delle hamiltoniane.\n"
                        "ABORTING...\n\n");
        free_sistema_astratto(sys);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo la memoria per la matrice di connessione totale
 */
    sys->M_tot = gsl_matrix_complex_calloc(sys->dim, sys->dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (sys->M_tot == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della matrice di connessione.\n"
                        "ABORTING...\n\n");
        
        free_sistema_astratto(sys);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t idx[sys->n_qudit];
    for (size_t i = 0; i < sys->n_qudit; ++i)
    {
        idx[i] = i;
    }

    ris = costruisci_matrice_connettivita(sys->M_tot, sys, idx, sys->n_qudit);

/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (ris > 0)
    {
        free_sistema_astratto(sys);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return sys;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiunge il termine i-esimo all'hamiltoniana del sistema.
 */
uint8_t aggiungi_termine(gsl_matrix_complex *hamiltoniana,
                            sistema_astratto *sys,
                            size_t *idx_qudit,
                            size_t idx,
                            size_t N)
{
    size_t n_qudit_gate = N;

    size_t idx_qudit_ordinato[n_qudit_gate];

    uint8_t flag[numero_qudit];
    for (size_t i = 0; i < numero_qudit; ++i)
    {
        flag[i] = 0;
    }

    for (size_t i = 0; i < n_qudit_gate; ++i)
    {
        flag[idx_qudit[i]] = 1;
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

    size_t dim = sys->n_livelli[idx_qudit_ordinato[0]];
    gsl_matrix_complex *tmp1 = 
        gsl_matrix_complex_calloc(dim, dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (tmp1 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per una"
                        "una delle matrici di supporto per la "
                        "formazione del Hamiltoniana.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (idx == 0)
    {
        gsl_matrix_complex_memcpy(tmp1, 
            sys->hamiltoniana[idx_qudit_ordinato[0]]);
    }
    else
    {
        gsl_matrix_complex_set_identity(tmp1);
    }

    for (size_t j = 1; j < N; ++j)
    {
        size_t idx_attuale = idx_qudit_ordinato[j];
        dim = sys->n_livelli[idx_attuale];
        gsl_matrix_complex *tmp2 = 
            gsl_matrix_complex_calloc(dim, dim);
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (tmp2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per una"
                            "una delle matrici di supporto per la "
                            "formazione del Hamiltoniana.\n"
                            "ABORTING...\n\n");
            gsl_matrix_complex_free(tmp1);
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        if (idx_attuale == idx_qudit_ordinato[idx])
        {
            gsl_matrix_complex_memcpy(tmp2, sys->hamiltoniana[idx]);
        }
        else
        {
            gsl_matrix_complex_set_identity(tmp2);
        }

        gsl_matrix_complex *tmp3 = complex_kron(tmp1, tmp2);

        gsl_matrix_complex_free(tmp1);
        gsl_matrix_complex_free(tmp2);

        tmp1 = tmp3;
    }

#ifdef DEBUG
    fprintf(stdout, "\nHAMILTONIANA:\t\tTermine %ld-esimo\n", idx);
    stampa_matrice(stdout, tmp1);
#endif

    gsl_matrix_complex_add(hamiltoniana, tmp1);

    gsl_matrix_complex_free(tmp1);

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiunge il termine di connessione (se necessario)
 *  all'hamiltoniana. Questo ha la forma:
 *  J_ij * [ I  x .... x I x Sz_i x I x .... x I x Sz_j x I x .... x I ]
 *
 *  HP [IMPORTANTISSIMA]
 *          supponiamo che ogni qudit sia direttamente collegabile con ogni altro
 *          qudit. Se proviamo a fare un'operazione tra due qudit il cui termine
 *          di connessione è nullo (e il meccanismo di switch on/off delle
 *          connessioni è abilitato) anzichè realizzare un'opportuna sequenza di
 *          swap (che andrebbe fatta al livello della traduzione da istruzione a
 *          operazione all'interno di 'parser_interface') andiamo ad abortire
 *          l'esecuzione del programma
 * 
 *  TODO:: Rimuovere l'HP sopra [FONDAMENTALE]
* 
 *  indici_qudit_connessione --> gli indici dei qudit da connettere
 *                               [ATTENZIONE:: è supposto ordinato]
 *  indici_qudit --> gli indici dei qudit che compongono il sistema composto
 *  N --> numero dei qudit totali che compongono il sistema composto
 */
uint8_t aggiungi_termine_connesso(gsl_matrix_complex *hamiltoniana,
                                    sistema_astratto *sys,
                                    size_t indici_qudit_connessione[2],
                                    size_t *indici_qudit,
                                    size_t N)
{
/*
 *  Ordiniamo gli indici dei qudit passati
 */
    size_t n_qudit_gate = N;
    size_t idx_qudit_ordinato[n_qudit_gate];

    uint8_t flag[numero_qudit];
    for (size_t i = 0; i < numero_qudit; ++i)
    {
        flag[i] = 0;
    }

    for (size_t i = 0; i < n_qudit_gate; ++i)
    {
        flag[indici_qudit[i]] = 1;
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

    size_t i = 0;
    gsl_matrix_complex *tmp1 = NULL;
    gsl_matrix_complex *tmp2 = NULL;

    if (indici_qudit_connessione[0] == idx_qudit_ordinato[0])
    {
/*
 *      Recuperiamo uno spin opportuno
 */
        double spin = ((double)(sys->n_livelli[0] - 1u)) / 2.0;
/*
 *      Generiamo l'operatore di connessione
 */
        tmp1 = SZ(spin);
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (tmp1 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per una"
                            "una delle matrici di supporto per la "
                            "formazione del Hamiltoniana.\n"
                            "ABORTING...\n\n");
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }
        ++i;
    }
    else
    {
        tmp1 = gsl_matrix_complex_calloc(sys->n_livelli[0], sys->n_livelli[0]);
        gsl_matrix_complex_set_identity(tmp1);
    }

    for (size_t j = 1; j < N; ++j)
    {
        size_t j_idx = idx_qudit_ordinato[j];
        tmp2 = gsl_matrix_complex_calloc(sys->n_livelli[j_idx], sys->n_livelli[j_idx]);
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (tmp2 == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria per una"
                            "una delle matrici di supporto per la "
                            "formazione del Hamiltoniana.\n"
                            "ABORTING...\n\n");
            gsl_matrix_complex_free(tmp1);
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        if (indici_qudit_connessione[i] == j_idx)
        {
/*
 *          Recuperiamo uno spin opportuno
 */
            double spin = ((double)(sys->n_livelli[j_idx] - 1u)) / 2.0;
/*
 *          Generiamo l'operatore di connessione
 */
            tmp2 = SZ(spin);

            if (i == 0)
            {
                ++i;
            }
            else
            {
/*
 *              NON è più necessario incrementare i in quanto la connessione
 *              è tra soli 2 qudit. Se si vorrà mai generalizzare a connessioni
 *              più ampie allora qua va modificato.
 */
                ;   /* Do Nothing */
            }
        }
        else
        {
            gsl_matrix_complex_set_identity(tmp2);
        }

        gsl_matrix_complex *tmp3 = complex_kron(tmp1, tmp2);

        gsl_matrix_complex_free(tmp1);
        gsl_matrix_complex_free(tmp2);

        tmp1 = tmp3;
    }

    gsl_complex J_mn =
        gsl_matrix_complex_get(sys->J, 
                                indici_qudit_connessione[0],
                                indici_qudit_connessione[1]);

    if (gsl_complex_abs(J_mn) < 1e-12)
    {
        fprintf(stderr, "ERRORE nell'aggiunta del termine di connessione.\n"
                        "Termine nullo, i.e., qudit non collegabili.\n"
                        "In versioni future si proverà ad implementare una "
                        "catena automatica di swap tra qudit per collegare "
                        "anche due qudit non direttamente collegabili.\n"
                        "Al momento però vige l'HP di qudit collebali tra "
                        "loro senza vincolo alcuno.\n"
                        "ABORTING...\n\n");
        gsl_matrix_complex_free(tmp1);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_matrix_complex_scale(tmp1, J_mn);

#ifdef DEBUG
    fprintf(stdout, "\nHAMILTONIANA:\t\tTermine di connessione\n");
    stampa_matrice(stdout, tmp1);
#endif

    gsl_matrix_complex_add(hamiltoniana, tmp1);

    gsl_matrix_complex_free(tmp1);

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che costruisce la matrice di connettività opportuna una volta
 *  che forniti gli indici dei qudit coinvolti
 */
uint8_t costruisci_matrice_connettivita(gsl_matrix_complex *M,
                                        sistema_astratto *sys,
                                        size_t *indici_qudit,
                                        size_t n_qudit)
{
    if (M == NULL)
    {
        fprintf(stderr, "ERRORE passato un riferimento nullo alla "
                        "funzione per la costruzione della matrice di "
                        "connettività.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Ordiniamo gli indici dei qudit
 */
    size_t idx_qudit_ordinato[n_qudit];
    uint8_t flag[sys->n_qudit];

    for (size_t i = 0; i < sys->n_qudit; ++i)
    {
        flag[i] = 0;
    }

    for (size_t i = 0; i < n_qudit; ++i)
    {
        flag[indici_qudit[i]] = 1;
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

    size_t dim_sp = 1;

    for (size_t i = 0; i < n_qudit; ++i)
    {
        dim_sp *= sys->n_livelli[idx_qudit_ordinato[i]];
    }

/*
 *  Costruiamo la matrice di connessione
 */
    gsl_matrix_complex *tmp  = gsl_matrix_complex_calloc(dim_sp, dim_sp);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (tmp == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            return 1;
        }
        {
            ;   /* Do nothing */
        }

    for (size_t i = 0; i < n_qudit; ++i)
    {
        uint8_t ris = 
            aggingi_termine_matrice_connettivita(tmp,
                                                    sys, 
                                                    idx_qudit_ordinato,
                                                    idx_qudit_ordinato[i],
                                                    n_qudit);
/*
 *      Verifichiamo l'esito dell'operazione
 */
        if (ris > 0)
        {
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

    }

    gsl_matrix_complex_memcpy(M, tmp);
    gsl_matrix_complex_free(tmp);

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiunge un termine alla matrice di connessione
 */
uint8_t aggingi_termine_matrice_connettivita(gsl_matrix_complex *ris,
                                                sistema_astratto *sys,
                                                size_t *idx_qudit,
                                                size_t idx,
                                                size_t N)
{
    gsl_matrix_complex *tmp1 = NULL;
    gsl_matrix_complex *tmp2 = NULL;
    gsl_matrix_complex *tmp3 = NULL;

    for (size_t i = 0; i < N; ++i)
    {
        size_t dim = sys->n_livelli[idx_qudit[i]];

        if (i == 0)
        {
            tmp1 = gsl_matrix_complex_calloc(dim, dim);
/*
 *          Verifichiamo l'esito dell'allocazione
 */ 
            if (tmp1 == NULL)
            {
                fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
                return 1;
            }
            {
                ;   /* Do nothing */
            }

            if (idx_qudit[i] == idx)
            {
                gsl_matrix_complex_memcpy(tmp1, sys->M[idx]);
            }
            else
            {
                gsl_matrix_complex_set_identity(tmp1);
            }
        }
        else
        {
            tmp2 = gsl_matrix_complex_calloc(dim, dim);

            if (tmp2 == NULL)
            {
                fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
                return 1;
            }
            {
                ;   /* Do nothing */
            }

            if (idx_qudit[i] == idx)
            {
                gsl_matrix_complex_memcpy(tmp2, sys->M[idx]);
            }
            else
            {
                gsl_matrix_complex_set_identity(tmp2);
            }
        }

        if (i != 0)
        {
            tmp3 = complex_kron(tmp1, tmp2);
            gsl_matrix_complex_free(tmp1);
            gsl_matrix_complex_free(tmp2);
        }
        else
        {
            tmp3 = gsl_matrix_complex_calloc(dim, dim);
/*
 *          Verifichiamo l'esito dell'allocazione
 */ 
            if (tmp3 == NULL)
            {
                fprintf(stderr, "ERRORE nell'allocazione di una matrice.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
                return 1;
            }
            {
                ;   /* Do nothing */
            }

            gsl_matrix_complex_memcpy(tmp3, tmp1);
            gsl_matrix_complex_free(tmp1);
        }

        tmp1 = tmp3;
    }

    gsl_matrix_complex_add(ris, tmp3);
    gsl_matrix_complex_free(tmp3);

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che verfica l'ordinamento dei livelli in corrispondenza agli
 *  autovalori della matrice di connessione.
 * 
 *  Ritorna 1 se i livelli sono ordinati in modo consistente con gli
 *  autovalori, 0 altrimenti
 * 
 *  ATTENZIONE::    Questa funione al momento lavora solo con matrici di
 *                  connessione formate da operatori di spin (Sx/Sy).
 * 
 *  HP::    Lavoriamo con sistemi di spin
 * 
 *  TODO::  Rimuovere l'HP sopra
 */
uint8_t test_ordine_livelli(gsl_matrix_complex *M, size_t lv_0, size_t lv_1)
{
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

    gsl_complex tmp = gsl_matrix_complex_get(M, lv_0, lv_1);

    size_t max = 0u;

    if (fabs(tmp.dat[1]) > fabs(tmp.dat[0]))
    {
        max = 1u;
    }
    else
    {
        ;   /* Do Nothing */
    }

    uint8_t ris = 0u;

    if (tmp.dat[max] > 0.0)
    {
        ris = 1u;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return ris;
}

//---------------------------------------------------------------------------//