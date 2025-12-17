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

#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "../../Common/include/common.h"
#include "../../Common/include/lib.h"

/*
 *  Header file contenente le dichiarazioni di tutte le funzioni e
 *  strutture dati necessarie alle interfacce GSL per poter eseguire 
 *  l'approssimazione numerica del sistema di equazioni differenziali
 *  che governano lo stato del sistema quantistico
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati necessaria al calcolo del forzante del sistema differenziale
 */
typedef struct
{
/*
 *  Impulso da simulare
 */
    impulsi_paralleli *imp_par;
/*
 *  Sistema fisico in considerazione
 */
    sistema_composto *sys;
} parametri_forzante;

/*
 *  Struttura dati necessaria al calcolo dello jacobiano del sistema
 *  differenziale
 */
typedef struct
{
    ; /* Vuota */
} parametri_jacobiano;

/* 
 *  Dichiarazione delle due funzioni che permettono di valutare il sistema
 *  di ODE tramite le procedure fornite da GSL
 */
extern int forzante(double t, const double y[], double f[], void *params);
extern int jacobiano(double t, const double y[], double *dfdy, double dfdt[],
                        void *params);

/*
 *  Funzione che aggiunge l'errore secondo la Limblad Master Equation
 */
extern
uint8_t aggiungi_errore(gsl_matrix_complex *commmutatore,
                        gsl_matrix_complex *rho,
                        sistema_composto *sys);
#endif