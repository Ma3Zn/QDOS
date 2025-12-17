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

#ifndef SIMULATION_INTERFACES_H
#define SIMULATION_INTERFACES_H


#include "../../Common/include/common.h"
#include "../../Common/include/global.h"
#include "../../Common/include/pulse.h"
#include "../../Common/include/gate.h"
#include "../../Common/include/lib.h"

#include "../../Decomposer/include/decomposition.h"

#include "../../Scheduler/include/scheduling.h"

#include "../../Simulator/include/simulation.h"

#include "../../Parser/include/parser.h"

#include "parser_interfaces.h"

//---------------------------------------------------------------------------//

/*
 *  Funzione che, forniti gli indici dei qudit coinvolti dal gate, va a 
 *  fare il set-up del sistema composto opportuno per la simulazione
 * 
 *  TODO::  liberare la memoria punatta precedentemente dai campi che ora
 *          andiamo a riutilizzare
 */
extern
sistema_composto *init_sistema_composto_sim(parametri_operazione *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione che conta il numero di operazioni all'interno di un blocco
 */
extern
size_t conta_numero_operazioni_blocco(blocco_operazioni *blk);

//---------------------------------------------------------------------------//


/*
 *  Funzione che dato un blocco di operazioni ed un ambiente di simulazione
 *  esegue la simulazione del blocco NON stesso utilizzando lo switch on/off
 *  delle connessioni dei qudit
 */
extern
simulazione *simula_blocco_operazioni(blocco_operazioni *op_blk,
                                        simulazione *sim,
                                        blocco_operazioni *tabella_op);

//---------------------------------------------------------------------------//

/*
 *  Funzione che nel caso di simulazioni con switch on/off delle connessioni
 *  tra qudit disabilitate (-J0) gestirà la corretta simulazione del circuito
 *  fornito come input.
 *  Ritorna un puntatore nullo se si è verificato un errore
 */
extern
simulazione *simula_circuito(input *in, output *out);

#endif