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

#ifndef EIGEN_H
#define EIGEN_H

#include "common.h"

/* 
 *  File che contiene la dichiarazione delle funzioni per la ricerca di 
 *  autovalori ed autovettori e di tutte le funzioni per il cambio di base
 */

/*
 *  Funzione che assicura il fatto che la componente reale di modulo
 *  maggiore di ogni autovalore abbia sempre segno positivo
 * 
 *  [ pare che il codice fornito da GSL assivuri già questa cosa ma ]
 *  [ per sicurezza la implementiamo comunque                       ]
 */
extern
uint8_t correggi_fase(gsl_matrix_complex *autovettori);

/*
 *  Funzione che verifica il corretto ordinamento degli autovettori
 *  passati. Quello a noi necessario è quello che ha la componente
 *  maggiore di modulo dell'autovettore sull'antidiagonale.
 *  In questo modo gli stati del sistema fisico sono ordinati
 *  consistentemente con quelli virtuali
 */
extern
uint8_t correggi_ordine_autov(gsl_matrix_complex *autovettori,
                                gsl_vector *autovalori);

/* 
 *  Funzione che data una matrice H hermitiana ne calcola autovalori ed
 *  autovettori sovrascrivendo i parametri "autovalori" ed "autovettori".
 *  La parte triangolare inferiore di H viene distrutta.
 */
extern 
uint8_t cerca_autovalori(gsl_matrix_complex *H,
                            gsl_matrix_complex *autovettori,
                            gsl_vector *autovalori);

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
extern
uint8_t cambio_base(gsl_matrix_complex *X, gsl_matrix_complex *U);

#endif