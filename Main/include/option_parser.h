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

#ifndef OPTION_PARSER_H
#define OPTION_PARSER_H

#include "../../Common/include/common.h"

/* 
 *  Header file che conterrà la dichiarazione di funzioni e dati necessarie
 *  al parser delle opzioni passate da linea di comando al programma
 */

//---------------------------------------------------------------------------//

/*
 *  Definizioni dei valori di ritorno speciali per la funzione di parsing
 *  delle opzioni
 */
#define OPT_OK      0u
#define OPT_STOP    1u
#define OPT_ERR     2u

/*
 *  Numero delle opzioni da linea di comando riconosciute
 */
#define N_CLO       16
//---------------------------------------------------------------------------//

/*
 *  Lista delle opzioni riconosciute dal parser
 */
extern char *opzioni_riconosciute[N_CLO];

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa un help per l'utilizzo del programma
 */
extern
void stampa_help();

//---------------------------------------------------------------------------//

/*
 *  Funzione che fornita l'opzione ed il valore associato esegue le operazioni
 *  necessarie
 */
extern
uint8_t esegui_opzione(char *opt, char *val);

//---------------------------------------------------------------------------//

/*
 *  Funzione che esegue il parsing delle opzioni fornite da linea di comando
 */
extern
uint8_t parse_options(int argc, char **argv);

#endif