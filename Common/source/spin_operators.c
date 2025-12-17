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

#include "../include/spin_operators.h"

/*
 *  File che contiene la definizione delle funzioni per la costruzione
 *  degli operatori di Spin a noi necessari. Per evitare noie sulla conversione
 *  dei tipi di matrici, lavoreremo fin da subito con una rappresentazione
 *  complessa degli operatori
 */

/*
 *  Funzione che costruisce l'operatore di creazione partendo dallo Spin del
 *  sistema.
 */
gsl_matrix_complex* S_plus(double spin)
{
/*
 *  Calcoliamo la dimesnsione dello spazio, i.e., (2 * spin) + 1
 */
    uint16_t dim = (uint16_t) (2 * spin + 1);

/*
 *  Allocazione della memoria per una matrice quadrata, rappresentazione
 *  dell'operatore in questione nella base di S_z
 */
    gsl_matrix_complex* operatore = gsl_matrix_complex_calloc(dim, dim);

/*  
 *  Generazione dell'operatore S_+. Considerando gli autovalori di S_z
 *  ordinati in senso decrescente si ha che il generico elemento matriciale
 *  è della forma
 * 
 *      (S_+)_{i,j} = d_{i+1,j}[spin*(spin+1) - (spin-i)(spin-(i+1))]^(1/2)
 * 
 *  ovvero ha solo la prima diagonale superiore non nulla
 */

/*
 *  Recuperiamo la prima sopra-diagonale dell'operatore
 */
    gsl_vector_complex_view diag = 
        gsl_matrix_complex_superdiagonal(operatore, 1);

    for (uint16_t i = 0; i < dim - 1; ++i)
    {
        double tmp = spin*(spin+1) - (spin - i)*(spin - (i+1));
        tmp = sqrt(tmp);

/*
 *      tmp è sempre un numero reale
 */
        gsl_complex valore = gsl_complex_rect(tmp, 0.0);

/*
 *      inizializzazione dell'elemento della vista
 */
        gsl_vector_complex_set(&diag.vector, i, valore);
    }

    return operatore;
}

/* 
 *  Funzione che costruisce l'operatore di distruzione partendo dallo Spin del
 *  sistema.
 */
gsl_matrix_complex* S_minus(double spin)
{
/* 
 *  S_- = (S_+)*
 */

/*
 *  Creazione di S_+
 */
    gsl_matrix_complex* tmp = S_plus(spin);

    gsl_matrix_complex* operatore = 
        gsl_matrix_complex_calloc(tmp->size1, tmp->size1);
    
/*
 *  Trasposizione di S_+
 */
    gsl_matrix_complex_transpose_memcpy(operatore, tmp);

    gsl_matrix_complex_free(tmp);

    return operatore;
}

/* 
 *  Funzione che costruisce l'operatore X di Spin partendo dallo Spin del
 *  sistema.
 */
gsl_matrix_complex* SX(double spin)
{
/*
 *  S_X = (1/2) * [S_+ + S_-]
 */

    gsl_matrix_complex* operatore = S_plus(spin);
    gsl_matrix_complex* tmp = S_minus(spin);
    
/* 
 *  Calcolo di S_X
 */
    gsl_complex fattore = gsl_complex_rect(0.5,0.0);
    gsl_matrix_complex_add(operatore, tmp);
    gsl_matrix_complex_scale(operatore, fattore);

    gsl_matrix_complex_free(tmp);

    return operatore;
}

/* 
 *  Funzione che costruisce l'operatore Y di Spin partendo dallo Spin del
 *  sistema.
 */
gsl_matrix_complex* SY(double spin)
{
/*
 *  S_Y = -i*(1/2)*[S_+ - S_-]
 */

    gsl_matrix_complex* operatore = S_plus(spin);
    gsl_matrix_complex* tmp = S_minus(spin);
    
/*
 * Calcolo di S_Y
 */
    gsl_complex fattore = gsl_complex_rect(0.0,-0.5);
    gsl_matrix_complex_sub(operatore, tmp);
    gsl_matrix_complex_scale(operatore, fattore);

    gsl_matrix_complex_free(tmp);

    return operatore;
}

/* 
 *  Funzione che costruisce l'operatore Z di Spin partendo dallo Spin del
 *  sistema.
 */
gsl_matrix_complex* SZ(double spin)
{
/*
 *  Calcoliamo la dimesnsione dello spazio, i.e., (2 * spin) + 1
 */
    uint16_t dim = (uint16_t) (2 * spin + 1);

    gsl_matrix_complex* operatore = gsl_matrix_complex_calloc(dim, dim);

/*  Nella base scelta S_Z sarà semplicemente una matrice diagonale, il cui
 *  generico eleemnto è dato da:
 *                          (S_Z)_{i,i} = spin - i
 */
    gsl_vector_complex_view diag = gsl_matrix_complex_diagonal(operatore);

    for (uint16_t i = 0; i < dim; ++i)
    {
        double tmp = spin - i;

/*
 *      tmp è sempre un numero reale
 */
        gsl_complex valore = gsl_complex_rect(tmp, 0.0);

/*
 *      inizializzazione dell'elemento della vista
 */
        gsl_vector_complex_set(&diag.vector, i, valore);
    }

    return operatore;
}
