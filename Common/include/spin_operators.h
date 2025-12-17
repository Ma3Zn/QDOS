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

#ifndef SPIN_OPERATORS_H
#define SPIN_OPERATORS_H

#include "common.h"

/* 
 *  File che contiene la dichiarazione delle funzioni per la
 *  costruzione degli operatori di Spin. Per evitare noie sulla conversione
 *  dei tipi di matrici, lavoreremo fin da subito con una rappresentazione
 *  complessa degli operatori
 */

/* 
 *  Funzione che costruisce l'operatore di creazione partendo dallo Spin del
 *  sistema
 */
extern gsl_matrix_complex* S_plus(double spin);

/* 
 *  Funzione che costruisce l'operatore di distruzione partendo dallo Spin del
 *  sistema
 */
extern gsl_matrix_complex* S_minus(double spin);

/* 
 *  Funzione che costruisce l'operatore X di Spin partendo dallo Spin del
 *  sistema
 */
extern gsl_matrix_complex* SX(double spin);

/* 
 *  Funzione che costruisce l'operatore Y di Spin partendo dallo Spin del
 *  sistema
 */
extern gsl_matrix_complex* SY(double spin);

/* 
 *  Funzione che costruisce l'operatore Z di Spin partendo dallo Spin del
 *  sistema
 */
extern gsl_matrix_complex* SZ(double spin);

#endif
