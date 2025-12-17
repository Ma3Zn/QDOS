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

#ifndef TENSOR_H
#define TENSOR_H

#include "common.h"

/* 
 *  File che contiene la dichiarazione delle funzioni per il calcolo tensoriale
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
extern gsl_matrix_complex* complex_kron(gsl_matrix_complex *X,
                                        gsl_matrix_complex *Y);

/*
 *  Funzione he calcola il prodotto esterno dei due vettori v e w scalato di un
 *  fattore a
 * 
 *  Ovvero, a*|v><w|
 */
extern gsl_matrix_complex* complex_outer_product(gsl_vector_complex *v,
                                                 gsl_vector_complex *w,
                                                 double a);

#endif