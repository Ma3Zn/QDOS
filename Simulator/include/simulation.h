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

#ifndef SIMULATION_H
#define SIMULATION_H

#include "../../Common/include/physical_system.h"
#include "../../Common/include/output.h"
#include "../../Common/include/common.h"
#include "../../Common/include/pulse.h"


/*
 *  Header file che conterra' le dichiarazione di tutte le funzioni e
 *  strutture dati necessarie alla simulazione dell'evoluzione dello stato
 *  del nostro sistema quantistico
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati contenente tutte le informazioni necessarie alla simulazione
 */
typedef struct
{
/*
 *  Il sistema fisico da simulare
 */
    sistema_composto *sys;
/*
 *  Le condizioni iniziali da simulare
 */
    gsl_matrix_complex *rho;
/*
 *  Le oprazioni da simulare
 */
    schedule *sch;
/*
 *  Struttura dati contenente tutte le informazioni per l'output dei risultati
 */
    output *out;
/*
 *  Tempo da fare intercorrere tra l'esecuzione di due impulsi paralleli in cui
 *  solo l'errore viene simulato agire sul sistema. Tale intervallo temporale
 *  viene simulato anche prima del primo impulso in quanto vuole simulare
 *  il tempo di avviamente tra il termine della sequenza di operazioni di
 *  inizializzazione dello stato iniziale ed il primo vero impulso della
 *  sequenza
 */
    double avviamento;
}simulazione;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per la deallocazione di una simulazione. 
 */

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'allocazione di una simulazione
 */
extern
simulazione *alloc_simulazione();

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per la deallocazione di una simulazione
 */
extern
void free_simulazione(simulazione *sim);

//---------------------------------------------------------------------------//

/*
 *  Funzione che simula effettivamente l'evoluzione del sistema sotto
 *  l'influenza della perturbazione data dall'impulso passato
 */
extern
uint8_t simula_impulso(impulsi_paralleli *imp_par,
                        gsl_matrix_complex *rho,
                        sistema_composto *sys,
                        output *out,
                        double avviamento);

//---------------------------------------------------------------------------//

/*
 *  Interfaccia primaria con il simulatore. Simula la simulazione passata e
 *  ritorna la matrice di densita' che descrive lo stato del sistema alla
 *  fine della simulazione
 */
extern
uint8_t simula(simulazione *sim);

#endif