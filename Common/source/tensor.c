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

#include "../include/common.h"

/* 
 *  File che contiene la definizione delle funzioni per il calcolo tensoriale
 *  a noi necessarie (principalmente impiegato nella costruzione
 *  dell'hamiltoniana)
 */

/*
 *  Calcola il prodotto tensoriale delle due matrici complesse X ed Y.
 *  Se X è una matrice 2x2 ed Y di dimensioni quasiasi. Il risultatao della
 *  funzione sarà la matrice siffatta:
 * 
 *          |                         |
 *          |  X[0,0]*Y     X[0,1]*Y  |
 *          |                         |
 *          |                         |
 *          |  X[1,0]*Y     X[1,1]*Y  |
 *          |                         |
 * 
 */
gsl_matrix_complex* complex_kron(gsl_matrix_complex* X,
                                    gsl_matrix_complex* Y)
{
/*
 *  Calcoliamo le dimensioni del prodotto tensoriale
 */
    uint16_t size_1 = X->size1 * Y->size1; /* numero di righe   */
    uint16_t size_2 = X->size2 * Y->size2; /* numero di colonne */

    gsl_matrix_complex* prodotto = gsl_matrix_complex_calloc(size_1, size_2);

    if (prodotto == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per il prodotto "
                        "tensoriale di due operatori.\n"
                        "ABORTING...\n\n");
        return NULL;
    }

    for (uint16_t riga = 0; riga < size_1; ++riga)
    {
/*
 *      Recuperiamo gli indici di riga di X ed Y
 */
        uint16_t i_1 = riga / Y->size1;
        uint16_t i_2 = riga % Y->size1;

        for (uint16_t colonna = 0; colonna < size_2; ++colonna)
        {
/*
 *          Recuperiamo gli indici di colonna di X ed Y
 */
            uint16_t j_1 = colonna / Y->size2;
            uint16_t j_2 = colonna % Y->size2;

/*
 *          Calcoliamo il valore dell'elemento della matrice prodotto
 */
            gsl_complex valore = 
                gsl_complex_mul(gsl_matrix_complex_get(X, i_1, j_1),
                                gsl_matrix_complex_get(Y, i_2, j_2));

/*
 *          Assegniamo il valore apportuno all'elemento della matrice
 */
            gsl_matrix_complex_set(prodotto, riga, colonna, valore);
        }
    }

    return prodotto;
}

/*
 *  Funzione che calcola il prodotto esterno dei due vettori v e w scalato di un
 *  fattore a
 * 
 *  Ovvero, a*|v><w|
 */
gsl_matrix_complex* complex_outer_product(gsl_vector_complex *v,
                                            gsl_vector_complex *w,
                                            double a)
{
    double size_v = v->size;
    double size_w = w->size;
/*
 *  Se le size dei due vettori NON combaciano non ritorniamo un
 *  putatore nullo
 */
    if (size_v != size_w)
    {
        return NULL;
    }
    else
    {
        ;   /*  Do nothing  */
    }

/*
 *  Usiamo l'interfaccia blas per poter calcolare la matrice richiesta
 */
    gsl_matrix_complex_view v_view = 
        gsl_matrix_complex_view_vector(v, size_v, 1);
    gsl_matrix_complex_view w_view = 
        gsl_matrix_complex_view_vector(w, size_v, 1);
    
    gsl_matrix_complex *operatore = gsl_matrix_complex_calloc(size_v, size_v);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (operatore == NULL)
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
    
    gsl_complex alpha = gsl_complex_rect(a, 0.0);
    gsl_complex beta = gsl_complex_rect(0.0, 0.0);

    gsl_blas_zgemm(CblasNoTrans, CblasConjTrans, alpha,
                    &v_view.matrix, &w_view.matrix,
                    beta, operatore);

    return operatore;
}
