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

#include "../include/eigen.h"
#include "../include/output.h"

/* 
 *  File che contiene la definizione delle funzioni per la ricerca di 
 *  autovalori ed autovettori e di tutte le funzioni per il cambio di base
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione che assicura il fatto che la componente reale di modulo
 *  maggiore di ogni autovalore abbia sempre segno positivo
 * 
 *  [ pare che il codice fornito da GSL assivuri già questa cosa ma ]
 *  [ per sicurezza la implemntiamo comunque                        ]
 */
uint8_t correggi_fase(gsl_matrix_complex *autovettori)
{
    size_t size = autovettori->size1;

    double max;

    for (size_t j = 0; j < size; ++j)
    {
        gsl_vector_complex_view col = gsl_matrix_complex_column(autovettori, j);
        max = 0.0;

        for (size_t i = 0; i < size; ++i)
        {
            gsl_complex tmp = gsl_vector_complex_get(&(col.vector), i);
            
            if (fabs(tmp.dat[0]) > fabs(max))
            {
                max = tmp.dat[0];
            }
            else
            {
                ; /* Do nothing */
            }
        }

        if (max < 0)
        {
            gsl_complex tmp = gsl_complex_rect(-1.0, 0.0);
            gsl_vector_complex_scale(&(col.vector), tmp);
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che verifica il corretto ordinamento degli autovettori
 *  passati. Quello a noi necessario è quello che ha la componente
 *  maggiore di modulo dell'autovettore sull'antidiagonale.
 *  In questo modo gli stati del sistema fisico sono ordinati
 *  consistentemente con quelli virtuali
 */
uint8_t correggi_ordine_autov(gsl_matrix_complex *autovettori,
                                gsl_vector *autovalori)
{
#ifdef DEBUG
    fprintf(stdout, "\nAUTOVETTORI:\n");
    stampa_matrice(stdout, autovettori);
#endif
    size_t dim = autovalori->size;

    gsl_permutation *perm = gsl_permutation_calloc(dim);
    gsl_permutation_init(perm);

    size_t ordine[dim];

    for (size_t i = 0; i < dim; ++i)
    {

        gsl_complex tmp = 
            gsl_matrix_complex_get(autovettori, (dim-1) - i, i);

        if (gsl_complex_abs(tmp) < 0.5)
        {
            size_t idx_max = 0;
            size_t max_abs = 0;
            for (size_t j = 0; j < dim; ++j)
            {
                tmp = gsl_matrix_complex_get(autovettori, j, i);

                if (gsl_complex_abs(tmp) > max_abs)
                {
                    idx_max = j;
                    max_abs = gsl_complex_abs(tmp);
                }
                else
                {
                    ;   /* Do Nothing */
                }
            }

            ordine[i] = dim - 1 - idx_max;
        }
        else
        {
            ordine[i] = i;
        }
    }

    for (size_t i = 0; i < dim; ++i)
    {
        perm->data[ordine[i]] = i;
    }

    gsl_permute_matrix_complex(perm, autovettori);
    gsl_permute_vector(perm, autovalori);

    gsl_permutation_free(perm);

    return 0;
}

//---------------------------------------------------------------------------//

/* 
 *  Funzione che data una matrice H hermitiana ne calcola autovalori ed
 *  autovettori sovrascrivendo i parametri "autovalori" ed "autovettori".
 *  La parte triangolare inferiore di H viene distrutta.
 */
uint8_t cerca_autovalori(gsl_matrix_complex *H,
                            gsl_matrix_complex *autovettori,
                            gsl_vector *autovalori)
{
    gsl_eigen_hermv_workspace *w = gsl_eigen_hermv_alloc(H->size1);

    if (w == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione del workspace per eseguire "
                        "la diagonalizzazione del Hamiltoniana.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_eigen_hermv(H, autovalori, autovettori, w);
    gsl_eigen_hermv_sort(autovalori, autovettori, GSL_EIGEN_SORT_VAL_ASC);

/*
 *  Noi ci aspettiamo una matrice degli autovettori in cui la componente
 *  maggiore sta sulla 'anti-diagonale'. Così facendo siamo certi del corretto
 *  ordinamento degli stati fisici del sistema
 */
    uint8_t ris = correggi_ordine_autov(autovettori, autovalori);
    ris += correggi_fase(autovettori);

    if (ris > 0)
    {
        fprintf(stderr, "ERRORE nella ricerca degli autovalori di "
                        "una matrice.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_eigen_hermv_free(w);
    return 0;
}

//---------------------------------------------------------------------------//

/* 
 *  Funzione che data una matrice rappresentata in base canonica
 *  (il prodotto tensoriale di autovettori di Sz nel nostro caso)
 *  ed una nuova base, calcola la rappresentazione della matrice nella
 *  nuova base.
 * 
 *  Ricordiamo che per fare ciò, data la matrice del cambio di base U
 *  e la matrice di cui vogliamo la nuova rappresentazione X, ci basta
 *  eseguire il prodotto
 * 
 *                            (U*)XU
 * 
 *  dove con * si intende la traspozizione e coniugio di U.
 */

uint8_t cambio_base(gsl_matrix_complex *X, gsl_matrix_complex *U)
{
    gsl_matrix_complex *tmp1 = gsl_matrix_complex_calloc(X->size1, X->size2);
    gsl_matrix_complex *tmp2 = gsl_matrix_complex_calloc(X->size1, X->size2);

    if (tmp1 == NULL || tmp2 == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il cambio "
                        "base di una matrice.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*  Calcoliamo XU.
 *
 *  La chiamata seguente all'interfaccia BLAS calcola il prodotto
 * 
 *                      1.0 * X*U + 0.0*X
 */
    gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, gsl_complex_rect(1.0,0.0),
                    X, U, gsl_complex_rect(0.0,0.0), tmp1);

/*  Calcoliamo (U*)X.
 *
 *  La chiamata sefuente all'interfaccia BLAS calcola il prodotto
 * 
 *                      1.0 * (U*)*X + 0.0*X
 */
    
    gsl_blas_zgemm(CblasConjTrans, CblasNoTrans, gsl_complex_rect(1.0,0.0),
                    U, tmp1, gsl_complex_rect(0.0,0.0), tmp2);
    
    gsl_matrix_complex_memcpy(X, tmp2);

    gsl_matrix_complex_free(tmp1);
    gsl_matrix_complex_free(tmp2);

    return 0;
}

//---------------------------------------------------------------------------//