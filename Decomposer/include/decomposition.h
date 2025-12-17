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

#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "../../Common/include/common.h"
#include "../../Common/include/lib.h"

/*
 *  Header file che conterrà le dichiarazioni di tutte le funzioni e di
 *  tutte le strutture dati necessarie per la realizzazione dell'algoritmo
 *  di decomposizione in rotazioni piane e lungo Z
 */

//---------------------------------------------------------------------------//

/*
 *  Moltiplicazione esplicita di U per la rotazione piana rot
 *
 *                          U * rot
 */
extern
void molt_mat_rot_piana(rotazione_piana *rot,
                        gsl_matrix_complex *U);

//---------------------------------------------------------------------------//

/*
 *  Moltiplicazione esplicita di U per la rotazione Z rot
 *
 *                          U * rot
 */
extern
void molt_mat_rot_Z(rotazione_Z *rot,
                    gsl_matrix_complex *U,
                    size_t r);

//---------------------------------------------------------------------------//

/*
 *  Decomposizione della matrice U in rotazioni piane. La decomposizione è
 *  distruttiva, i.e., U verrà modificata, in particolare U diventerà una
 *  matrice diagonale
 *
 *  La funzione torna 0 se la riallocazione è andata a buon fine, mentre un
 *  qualsiasi altro valore di ritorno indica il ferificarsi di un errore
 */
extern
uint8_t decomponi_in_rotazioni_piane(gsl_matrix_complex *U,
                                        sequenza_rotazioni *seq,
                                        size_t *N_rot_eseguite);

//---------------------------------------------------------------------------//

/*
 *  Controlla che la matrice U sia effettivamente unitaria speciale, i.e, ha
 *  determinante 1. Ritorna 0 se il determinante è 1 mentre ritorna 1
 *  altrimenti.
 * 
 *  ATTENZIONE::    si presuppone che U sia diagonale.
 * 
 *  TODO::  aggiungere riferimento alla tesi in cui si dimostra perchè
 *          questa procedura è lecita e ci ridà quello che ci serve 
 *
 *  La funzione torna 0 se la riallocazione è andata a buon fine, mentre un
 *  qualsiasi altro valore di ritorno indica il ferificarsi di un errore
 */
extern
uint8_t controlla_appartenenza_SU(gsl_matrix_complex *U);

//---------------------------------------------------------------------------//

/*
 *  Decomposizione della matrice diagonale ottenuta al termine di
 *                  "decomponi_in_rotazioni_piane"
 *  in una sequenza di rotazioni Z
 *
 *  La funzione torna 0 se la riallocazione è andata a buon fine, mentre un
 *  qualsiasi altro valore di ritorno indica il ferificarsi di un errore
 */
extern
uint8_t decomponi_in_rotazioni_Z(gsl_matrix_complex *U,
                                    sequenza_rotazioni *seq,
                                    size_t *N_rot_eseguite);

//---------------------------------------------------------------------------//

/*
 *  Interfaccia esterna da chiamare per la decomposizione di una matrice in SU.
 *  Un controllo per verificare che U sia effettivamente una matrice unitaria
 *  speciale viene eseguito, in caso non lo sia un messaggio di errore viene
 *  stampato su stderr. Se si verifica un'errore e la decomposizione NON 
 *  può essere terminata con successo allora un puntatore nullo viene ritornato
 */
extern
sequenza_rotazioni *decomponi(gsl_matrix_complex *U);

#endif