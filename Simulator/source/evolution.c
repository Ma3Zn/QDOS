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

#include "../include/evolution.h"

/* 
 *  File che contiene la definizione di tutte le funzioni necessarie
 *  alle prcedure  presenti in GSL per la risoluzione di un sistema di ODE
 */

/*
 *  Funzione che aggiunge l'errore secondo la Limblad Master Equation
 *
 *  TODO::  Direi di spostare tutte queste allocazioni e deallocazioni al di
 *          fuori del forante e di passare un ambiente come anche GSL fa per
 *          il solover del sistema di ODE o per la diagonalizzazione delle
 *          matrici
 */
uint8_t aggiungi_errore(gsl_matrix_complex *commmutatore,
                        gsl_matrix_complex *rho,
                        sistema_composto *sys)
{

    gsl_matrix_complex *tmp1 = sys->sle->tmp1;
    gsl_matrix_complex *tmp2 = sys->sle->tmp2;
    gsl_matrix_complex *tmp3 = sys->sle->tmp3;
    gsl_matrix_complex *tmp4 = sys->sle->tmp4;
    gsl_matrix_complex *tmp5 = sys->sle->tmp5;


    if (tmp1 == NULL || tmp2 == NULL || tmp3 == NULL
                     || tmp4 == NULL || tmp5 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione delle matrice di supporto "
                        "per la simulazione dell'errore di dephasing.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_complex minus_one       = gsl_complex_rect(-1.0, 0.0);
    gsl_complex zero            = gsl_complex_rect( 0.0, 0.0);
    gsl_complex one             = gsl_complex_rect( 1.0, 0.0);
    gsl_complex two             = gsl_complex_rect( 2.0, 0.0);


    for (size_t i = 0; i < sys->n_qudit; ++i)
    {
        if (sys->operatore_errore[i] == NULL)
        {
            fprintf(stderr, "ERRORE nella simulazione dell'errore.\n"
                            "Numero di operatori d'errore fornito "
                            "insufficiente.\n"
                            "ABORTING...\n\n");
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        if (sys->rateo[i] < 0.0)
        {
            fprintf(stderr, "ERRORE nella simulazione dell'errore.\n"
                            "Tempo di dephasing fornito illegale (NEGATIVO).\n"
                            "ABORTING...\n\n");
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex *A_i = sys->operatore_errore[i];
        gsl_complex param = gsl_complex_rect(sys->rateo[i], 0.0);

#ifdef DEBUG_ERRORE
fprintf(stdout, "Matrice di densità\n");
stampa_matrice(stdout, rho);

fprintf(stdout, "Operatore d'errore\n");
stampa_matrice(stdout, A_i);
#endif

/*
 *      Calcoliamo il secondo termine della Lindblad
 *                      2*A*rho*A'
 */
        gsl_blas_zgemm(CblasNoTrans, CblasNoTrans,
                        one, A_i, rho,
                        zero, tmp1);
        gsl_blas_zgemm(CblasNoTrans, CblasConjTrans,
                        two, tmp1, A_i,
                        zero, tmp2);
#ifdef DEBUG_ERRORE
fprintf(stdout, "Primo termine lindblad\n");
stampa_matrice(stdout, tmp2);
#endif
/*
 *      Calcoliamo il termine comune della Lindblad
 *                      -A'*A
 */
        gsl_blas_zgemm(CblasConjTrans, CblasNoTrans,
                        minus_one, A_i, A_i,
                        zero, tmp3);
#ifdef DEBUG_ERRORE
fprintf(stdout, "Termine comune lindblad\n");
stampa_matrice(stdout, tmp3);
#endif
/*
 *      Calcoliamo il terzo e quarto termine della Lindblad
 *          -A'*A*rho   e   -rho*A'*A
 */
        gsl_blas_zgemm(CblasNoTrans, CblasNoTrans,
                        one, tmp3, rho,
                        zero, tmp4);
        gsl_blas_zgemm(CblasNoTrans, CblasNoTrans,
                        one, rho, tmp3,
                        zero, tmp5);
#ifdef DEBUG_ERRORE
fprintf(stdout, "Secondo termine lindblad\n");
stampa_matrice(stdout, tmp4);

fprintf(stdout, "terzo termine lindblad\n");
stampa_matrice(stdout, tmp5);
#endif
/*
 *      Sommiamo tutti i termini ottenuti
 */
        gsl_matrix_complex_add(tmp2, tmp4);
        gsl_matrix_complex_add(tmp2, tmp5);
/*
 *      Scaliamo con l'opportuno parametro di dephasing
 */
        gsl_matrix_complex_scale(tmp2, param);

#ifdef DEBUG_ERRORE
fprintf(stdout, "Termine di errore\n");
stampa_matrice(stdout, tmp2);
#endif

/*
 *      Aggiungiamo il contributo calcolato al commutatore
 */
        gsl_matrix_complex_add(commmutatore, tmp2);
    }

    return 0;
}

/* 
 *  Il Forzante risulta essere semplicemente il commutatore di rho(t) con VI(t)
 *  dove con VI si intende l'impulso in interaction picture.
 * 
 *  TODO::  Direi di spostare tutte queste allocazioni e deallocazioni al di
 *          fuori del forante e di passare un ambiente come anche GSL fa per
 *          il solover del sistema di ODE o per la diagonalizzazione delle
 *          matrici
 */
int forzante(double t, const double y[], double f[], void *params)
{
    parametri_forzante *data = (parametri_forzante *)params;

    impulsi_paralleli *imp_par = data->imp_par;
    sistema_composto *sys = data->sys;

    size_t dim = sys->dim;
    size_t size = dim * dim;

/*
 *  Variabili d'appoggio per il calcolo del forzante
 */
    gsl_matrix_complex *commutatore = gsl_matrix_complex_calloc(dim, dim);
    gsl_matrix_complex *rho = gsl_matrix_complex_calloc(dim, dim);

/*
 *  Vogliamo creare una matrix view del dato iniziale, ma siccome y NON deve
 *  essere modificato, utiliziamo l'array di output f. Dunque come prima cosa
 *  inizializziamo f = y, ricordando che y contiene 2n entrate le cui prime n
 *  sono le parti reali della soluzione e le restanti sono le parti immaginarie
 *
 *  Siccome vogliamo una vista complessa del vettore y e le interfacce di gsl
 *  leggono i numeri complessi da vettori di double a coppie, inizializziamo
 *  f nel modo corretto
 */
    for (size_t i = 0, j = 0; j < size; i += 2, ++j)
    {
/*
 *      Copia delle parti reali della soluzione
 */
        f[i]    = y[j];
/*
 *      Copia delle parti immaginarie della soluzione
 */
        f[i+1]  = y[j+size];
    }

/*
 *  Creazione di una matrix view di f di 2n righe ed n colonne.
 *  Il primo blocco (n*n) conterrà la parte reale della soluzione mentre il
 *  secondo la parte complessa
 */
    gsl_matrix_complex_view M =
        gsl_matrix_complex_view_array(f, dim, dim);

/*
 *  NON vogliamo modificare erroneamente i valori di f quindi li copiamo
 *  in una nuova matrice
 */
    gsl_matrix_complex_memcpy(rho, &M.matrix);

/*
 *  Dobbiamo effettivamente fare la simulazione di un impulso parallelo
 *  altrimenti sarebbe la simulazione di una fase di "fire-up"
 */
    if (imp_par != NULL)
    {
/*
 *      Aggiorniamo la valutazione temporale dell'impulso
 */
        gsl_complex ampiezza = gsl_complex_rect(0.0, 0.0);

        for (size_t i = 0; i < imp_par->dim; ++i)
        {
            impulso *imp = imp_par->arr_impulsi[i];

            gsl_complex tmp = imp->modulazione(t, imp);

            ampiezza = gsl_complex_add(ampiezza, tmp);
        }

/*
 *      Recuperiamo la perturbazione
 */
        gsl_matrix_complex *V = gsl_matrix_complex_calloc(dim, dim);
        gsl_matrix_complex_memcpy(V, sys->M);
/*
 *      Scaliamo la perturbazione dell'ampiezza calcolata
 */
        gsl_matrix_complex_scale(V, ampiezza);

/*
 *      Calcoliamo la rappresentazione in interaction picture del potenziale V
 */
        for (size_t i = 0; i < dim; ++i)
        {
            for (size_t j = 0; j < dim; ++j)
            {
/*
 *              Solo i termini fuori diagonale devono essere modificati
 */
                if (i != j)
                {
                    double w_ij = gsl_vector_get(sys->autovalori, i);
                    w_ij       -= gsl_vector_get(sys->autovalori, j);
                    gsl_complex V_ij = gsl_matrix_complex_get(V, i, j);

                    double cos_ij = cos(w_ij * t);
                    double sin_ij = sin(w_ij * t);

                    gsl_complex fattore = gsl_complex_rect(cos_ij, sin_ij);

                    gsl_complex VI_ij = gsl_complex_mul(V_ij, fattore);

                    gsl_matrix_complex_set(V, i, j, VI_ij);
                }
                else
                {
                    ; /* Do Nothing */
                }
            }
        }

/*
 *      Calcoliamo il commutatore [rho,VI]
 */
        gsl_complex tmp1 = gsl_complex_rect(-1.0, 0.0);
        gsl_complex tmp2 = gsl_complex_rect( 1.0, 0.0);
        gsl_complex tmp3 = gsl_complex_rect( 0.0, 0.0);

/*
 *      commutatore = VI * rho
 */
        gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, tmp2, 
                        V, rho, tmp3, commutatore);
/*
 *      commutatore = rho * VI - commutatore
 */
        gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, tmp2,
                        rho, V, tmp1, commutatore);

/*
 *      commutatore = i * commutatore
 */
        gsl_complex imaginary_unit = gsl_complex_rect(0.0, 1.0);
        gsl_matrix_complex_scale(commutatore, imaginary_unit);

        gsl_matrix_complex_free(V);
    }

/*
 *  Verifichiamo se la simulazione comprende anche la simulazione dell'errore
 *  o meno
 */
    if (sys->operatore_errore != NULL)
    {
/*
 *      verifichiamo l'esito dell'applicazione dell'errore
 */
        if (aggiungi_errore(commutatore, rho, sys))
        {
            return GSL_EBADFUNC;
        }
        else
        {
            ;   /* Do Nothig */
        }
    }

#ifdef DEBUG_ERRORE
fprintf(stderr, "Forzante\n");
stampa_matrice(stdout, commutatore);
#endif

/*
 *  Aggiorniamo i valori di f. In questo caso utilizziamo la stessa convenzione
 *  usata per y. Ovvero le prime n entrate sono la parte reale della soluzione
 *  mentre le ultime n sono la parte immaginaria
 */
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            gsl_complex rho_ij = gsl_matrix_complex_get(commutatore, i, j);
            f[(i * dim) + j]        = rho_ij.dat[0];
            f[(i * dim) + j + size] = rho_ij.dat[1];
        }
    }

    gsl_matrix_complex_free(commutatore);
    gsl_matrix_complex_free(rho);

    return GSL_SUCCESS;
}

/* 
 * Lo Jacobiano è richiesto dalle interfacce GSL ma non tutti i solver lo
 * utilizzano. In particolare il solver d noi implementato non ne fa uso,
 * dunque la funzione torna semplicemente un valore che causerà l'arresto
 * immediato di tutte le routine dell'ambiente odeiv2 di GSL.
 */
int jacobiano(double t, const double y[], double *dfdy, double dfdt[],
                void *params)
{
    return GSL_EBADFUNC;
}