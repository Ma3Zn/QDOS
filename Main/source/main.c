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

#ifdef BLIS_CBLAS
#include <blis/blis.h>
#include <blis/cblas.h>
#endif

#include "../../Common/include/common.h"
#include "../../Common/include/global.h"
#include "../../Common/include/output.h"
#include "../../Common/include/input.h"

#include "../../P-S Interface/include/simulation_interfaces.h"

#include "../../Gate Built-in/include/gate_built_in.h"

#include "../../Simulator/include/simulation.h"

#include "../include/option_parser.h"

/*
 *  Entry point di QDOS
 */
int main(int argc, char **argv)
{
    --argc;
    ++argv;

    uint8_t ris = alloc_globals();
    output *out = alloc_output();
    input *in = alloc_input();
/*
 *  Verifichiamo gli esiti delle allocazioni
 */
    if (ris > 0 || out == NULL || in == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione iniziale della memoria.\n"
                        "ABORTING...\n\n");
        free_globals();
        free_output(out);
        free_input(in);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Controlliamo se sono state fornite o meno delle opzioni
 */
    if (argc > 0)
    {
        ris = parse_options(argc, argv);
/*
 *      Verifichiamo il risultato del parsing delle opzioni
 */
        if (ris == OPT_ERR)
        {
            return 1;
        }
        else if (ris == OPT_STOP)
        {
            return 0;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inizializziamo le interfacce di input ed output
 */
    ris  = init_output(out);
    ris += init_input(in);
/*
 *  Verifichiamo l'inizializzazione
 */
    if (ris > 0)
    {
        free_globals();
        free_output(out);
        free_input(in);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Installiamo i gate builtin all'interno della lista delle definizioni
 */
    ris = install_BLTN(lst_def);
/*
 *  Verifichismo l'installazione
 */
    if (ris > 0)
    {
        free_globals();
        free_output(out);
        free_input(in);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Lanciamo la simulazione del circuito fornito in input
 */
    simulazione *sim = simula_circuito(in, out);

/*
 *  Liberiamo la memoria ancora allocata
 */
    free_simulazione(sim);
    free_globals();

    return 0;
}