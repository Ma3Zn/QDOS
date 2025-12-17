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

#ifndef CUSTOM_LOGIC
#define CUSTOM_LOGIC

#include "../../Simulator/include/measure.h"

/*
 *  Header file che conterrà le dichiarazioni di tutte le funzioni di
 *  'custom logic' definite da utente
 */

/*
 *  Macro per una definizione consistente della dimensione dell'array delle
 *  custom logic e del numero di custom logic
 */
#define N_CSL   1

/*
 *  Custom logic function for the Embedded Quantum Error Correction
 *  scheme proposed in Section 3.2 of the Thesis.
 * 
 *  It simply check the las measurement outcome of the ancilla.
 *  Then if we get a down (0) it will schedule the 'blocco_gate'
 *  recovery_error. While if we get an up (1) it will schedule
 *  the recovery_no_error block.
 */
extern
blocco_gate *EQEC_check_ancilla(lista_misure *tabella_misure);

#endif