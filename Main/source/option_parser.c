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

#include "../include/option_parser.h"
#include "../../Common/include/common.h"
#include "../../Common/include/global.h"
#include "../../Parser/include/lexer.h"

#include <time.h>
/* 
 *  File che conterrà la dichiarazione di funzioni necessarie al parser delle
 *  opzioni passate da linea di comando al programma
 */

//---------------------------------------------------------------------------//

/*
 *  Lista delle opzioni riconosciute dal parser
 */
char *opzioni_riconosciute[N_CLO] =
{
    "--connection_enabled",
    "--errors_enabled",
    "--frequency_output",
    "--help",
    "--in_dir",
    "--measurement_error",
    "--measure_memory",
    "--n_shots",
    "--out_dir",
    "--out_rho_final",
    "--out_rho_simulation",
    "--out_schedule",
    "--random_seed",
    "--simulation_enabled",
    "--simulation_progres",
    "--version",
};

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa un help per l'utilizzo del programma
 */
void stampa_help()
{
    fprintf(stdout, 
            "Usage: qdos [options] file\n"
            "Options:\n"
            "\t--connection_enabled={true|false}\t"
            "Enable connections among qudit to resolve levels.\n"
            "\t--errors_enabled={true|false}\t"
            "Enable the simulation of \"Lindblad form\" errors.\n"
            "\t--frequency_output={integer}\t\t"
            "Sets the number of times the density matrix will be printed when "
            "simulating a pulse.\n"
            "\t--help\t\t\t\t\t"
            "Display this Information.\n"
            "\t--in_dir={string}\t\t\t"
            "Set the directory for input files.\n"
            "\t--measurement_error={double}\t\t"
            "Set the probability of errors performing a measurement of a "
            "qudit.\n"
            "\t--measure_memory={integer}\t\t"
            "Set the number of measurement to save.\n"
            "\t--n_shots={integer}\t\t\t"
            "Set the number of complete measurements to be performed at the "
            "end of the simulation.\n"
            "\t--out_dir={string}\t\t\t"
            "Set the directory for the output files.\n"
            "\t--out_rho_final={false|bin|dec}\t\t"
            "Set the format of the final (and before any measurement) density "
            "matrix to be printed.\n"
            "\t--out_rho_simulation={false|bin|dec}\t"
            "Set the format of the intermediate density matrix to be "
            "printed.\n"
            "\t--out_schedule={false|bin|dec}\t\t"
            "Set the format of the sequence of pulses to be printed.\n"
            "\t--random_seed={false|true|integer}\t\t"
            "Set a custom seed for the RNG of QDOS or auto-generate a random seed for it.\n"
            "\t--simulation_enabled={true|false}\t"
            "Enable the simulation of the circuit provided as input.\n"
            "\t--simulation_progres={0|1|2}\t\t"
            "Set the level of verbosity of the simulator.\n"
            "\t--version\t\t\t\t"
            "Get the version of the installed software.\n"
            "\n"
            "The options above have default values shuch that a plain call to "
            "QDOS (i.e., qdos circ.q) is equivalent to:\n"
            "qdos "
            "--connection_enabled=true "
            "--dephasing_enabled=false "
            "--frequency_output=32 "
            "--in_dir=\"./input/\" "
            "--measurement_error=0.0 "
            "--measure_memory=10 "
            "--n_shots=0 "
            "--out_dir=\"./output/\" "
            "--out_rho_final=dec "
            "--out_rho_simulation=dec "
            "--out_schedule=dec "
            "--random_seed=false "
            "--simulation_enabled=true "
            "--simulation_progres=2 "
            "circ.q\n");
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che fornita l'opzione ed il valore associato esegue le operazioni
 *  necessarie
 */
uint8_t esegui_opzione(char *opt, char *val)
{
/*
 *  Individuiamo la corretta opzione
 */
    if (strcmp(opzioni_riconosciute[0], opt) == 0)
    {
        /* --connection_enabled */
        if(strcmp(val, "false") == 0)
        {
            flag_connessione = CONNESSIONE_DISABILITATA;
        }
        else if (strcmp(val, "true") == 0)
        {
            flag_connessione = CONNESSIONE_ABILITATA;
        }
        else
        {
            flag_connessione = CONNESSIONE_ABILITATA;
            fprintf(stderr, "ATTENZIONE: valore %s NON riconosciuto per "
                            "l'opzione %s\n"
                            "Utilizzziamo il valore di default \'true\'\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[1],  opt) == 0)
    {
        /* --dephasing_enabled */
        if(strcmp(val, "true") == 0)
        {
            errore_abilitato = 1;
        }
        else if (strcmp(val, "false") == 0)
        {
            errore_abilitato = 0;
        }
        else
        {
            errore_abilitato = 0;
            fprintf(stderr, "ATTENZIONE: valore %s NON riconosciuto per "
                            "l'opzione %s\n"
                            "Utilizzziamo il valore di default \'false\'\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[2],  opt) == 0)
    {
        /* --frequency_output */
        int ival = atoi(val);
/*
 *      Verifichiamo la validità del valore passato
 */
        if (0 < ival && ival < 1024)
        {
            freq_output = ival;
        }
        else
        {
            freq_output = 32;
            fprintf(stderr, "ATTENZIONE: valore %s NON permesso per "
                            "l'opzione %s\n"
                            "Range ammissibile [1,1023].\n"
                            "Utilizzziamo il valore di default 32\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[3],  opt) == 0)
    {
        /* --help */
        stampa_help();
        return OPT_STOP;
    }
    else if (strcmp(opzioni_riconosciute[4],  opt) == 0)
    {
        /* --in_dir */
        size_t ln_val = strlen(val);

        in_dir = (char*)calloc(ln_val + 1, sizeof(char));
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (in_dir == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                            "ABORTING...\n\n");
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        in_dir[ln_val] = '\0';

        strcpy(in_dir, val);
    }
    else if (strcmp(opzioni_riconosciute[5],  opt) == 0)
    {
        /* --measurement_error */
        double dval = atof(val);
/*
 *      Verifichiamo la validità del valore passato
 */
        if (0 < dval && dval < 1)
        {
            p_errore = dval;
        }
        else
        {
            p_errore = 0.0;
            fprintf(stderr, "ATTENZIONE: valore %s NON permesso per "
                            "l'opzione %s\n"
                            "Range ammissibile [0.0,1.0)."
                            "Utilizzziamo il valore di default 0.0\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[6],  opt) == 0)
    {
        /* --measure_memory */
        int ival = atoi(val);

/*
 *      Verifichiamo la validità del valore passato
 */
        if (0 < ival && ival < 1e4)
        {
            memoria_misure = ival;
        }
        else
        {
            memoria_misure = 10;
            fprintf(stderr, "ATTENZIONE: valore %s NON permesso per "
                            "l'opzione %s\n"
                            "Range ammissibile [1,10'000)."
                            "Utilizzziamo il valore di default 10\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[7],  opt) == 0)
    {
        /* --n_shots */
        int ival = atoi(val);

/*
 *      Verifichiamo la validità del valore passato
 */
        if (0 < ival && ival <= 1e6)
        {
            n_shots = ival;
        }
        else
        {
            n_shots = 0;
            fprintf(stderr, "ATTENZIONE: valore %s NON permesso per "
                            "l'opzione %s\n"
                            "Range ammissibile [1,1'000'000]."
                            "Utilizzziamo il valore di default 0\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[8],  opt) == 0)
    {
        /* out_dir */
        size_t ln_val = strlen(val);

        out_dir = (char*)calloc(ln_val + 1, sizeof(char));
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (out_dir == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                            "ABORTING...\n\n");
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        out_dir[ln_val] = '\0';

        strcpy(out_dir, val);
    }
    else if (strcmp(opzioni_riconosciute[9],  opt) == 0)
    {
        /* --out_rho_final */
        if(strcmp(val, "false") == 0)
        {
            out_rho_final = 0;
        }
        else if (strcmp(val, "bin") == 0)
        {
            out_rho_final = 1;
        }
        else if (strcmp(val, "dec") == 0)
        {
            out_rho_final = 2;
        }
        else
        {
            out_rho_final = 2;
            fprintf(stderr, "ATTENZIONE: valore %s NON riconosciuto per "
                            "l'opzione %s\n"
                            "Utilizzziamo il valore di default \'dec\'\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[10],  opt) == 0)
    {
        /* --out_rho_simulation */
        if(strcmp(val, "false") == 0)
        {
            out_rho_simulation = 0;
        }
        else if (strcmp(val, "bin") == 0)
        {
            out_rho_simulation = 1;
        }
        else if (strcmp(val, "dec") == 0)
        {
            out_rho_simulation = 2;
        }
        else
        {
            out_rho_simulation = 2;
            fprintf(stderr, "ATTENZIONE: valore %s NON riconosciuto per "
                            "l'opzione %s\n"
                            "Utilizzziamo il valore di default \'dec\'\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[11], opt) == 0)
    {
        /* --out_schedule */
        if(strcmp(val, "false") == 0)
        {
            out_schedule = 0;
        }
        else if (strcmp(val, "bin") == 0)
        {
            out_schedule = 1;
        }
        else if (strcmp(val, "dec") == 0)
        {
            out_schedule = 2;
        }
        else
        {
            out_schedule = 2;
            fprintf(stderr, "ATTENZIONE: valore %s NON riconosciuto per "
                            "l'opzione %s\n"
                            "Utilizzziamo il valore di default \'dec\'\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[12], opt) == 0)
    {
        /* --random_seed */
        if(strcmp(val, "false") == 0)
        {
            ; /* Do Nothing */
        }
        else if (strcmp(val, "true") == 0)
        {
            srandom(time(NULL));
            uint32_t seed = random();

            gsl_rng_set(rng, seed);
        }
        else
        {
            int64_t ival = strtoll(val, '\0', 10);
/*
 *          Verifichiamo la validità del valore passato
 */
            if (0 < ival)
            {
               gsl_rng_set(rng, ival);
            }
            else
            {
                fprintf(stderr, "ATTENZIONE: valore %s NON permesso per "
                                "l'opzione %s\n"
                                "Il seme deve essere positivo."
                                "Utilizzziamo il valore di default false\n\n",
                                val, opt);
            }
        }
    }
    else if (strcmp(opzioni_riconosciute[13], opt) == 0)
    {
        /* --simulation_enabled */
        if(strcmp(val, "false") == 0)
        {
            simulation_enabled = 0;
        }
        else if (strcmp(val, "true") == 0)
        {
            simulation_enabled = 1;
        }
        else
        {
            simulation_enabled = 1;
            fprintf(stderr, "ATTENZIONE: valore %s NON riconosciuto per "
                            "l'opzione %s\n"
                            "Utilizzziamo il valore di default \'true\'\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[14], opt) == 0)
    {
        /* --simulation_progress */
        int ival = atoi(val);

        if (0 <= ival && ival <= 2)
        {
            simulation_progress = ival;
        }
        else
        {
            simulation_progress = 2;
            fprintf(stderr, "ATTENZIONE: valore %s NON permesso per "
                            "l'opzione %s\n"
                            "Valori ammissibili {0,1,2}.\n"
                            "Utilizzziamo il valore di default 2\n\n",
                            val, opt);
        }
    }
    else if (strcmp(opzioni_riconosciute[15], opt) == 0)
    {
        /* --version */
        fprintf(stderr, "%s\n"
                        "This program is free software: you can "
                        "redistribute it and/or modify\n"
                        "it under the terms of the GNU General Public "
                        "License as published by\n"
                        "the Free Software Foundation, either version 3 "
                        "of the License, or\n"
                        "(at your option) any later version.\n"
                        "\n"
                        "This program is distributed in the hope that "
                        "it will be useful,\n"
                        "but WITHOUT ANY WARRANTY; without even the "
                        "implied warranty of\n"
                        "MERCHANTABILITY or FITNESS FOR A PARTICULAR "
                        "PURPOSE.  See the\n"
                        "GNU General Public License for more details.\n"
                        "\n"
                        "You should have received a copy of the GNU "
                        "General Public License\n"
                        "along with this program.  If not, see "
                        "<https://www.gnu.org/licenses/>.\n"
                        "\n",
                        versione_programma);
        return OPT_STOP;
    }
    else 
    {
        if (opt[0] != '-' && opt[1] != '-' && val[0] == '\0')
        {
            yyin = fopen(opt, "r");

            if (yyin == NULL)
            {
                fprintf(stderr, "ERRORE nell'apertura del file di input '%s'\n"
                                "ABORTING...\n\n", opt);
                return 1;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
        else
        {
            fprintf(stderr, "ERRORE: opzione %s NON riconosciuta.\n"
                            "Utilizzare l'opzione --help per avere un lista "
                            "delle opzioni accettate.\n", opt);
            return OPT_ERR;
        }
    }
    
    return OPT_OK;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che esegue il parsing delle opzioni fornite da linea di comando
 */
uint8_t parse_options(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i)
    {
        size_t strlen = 0;
        size_t idx = 0;
/*
 *      Recuperiamo la lunghezza dell'opzione
 */
        for (idx = 0; argv[i][idx] != '=' && argv[i][idx] != '\0'; ++idx)
        {
            ;   /* Do Nothing */
        }

        strlen = idx;
        char *opt = (char *)calloc(idx + 1, sizeof(char));
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (opt == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                            "ABORTING...\n\n");
            return OPT_ERR;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Copiamo l'opzione
 */
        for (size_t j = 0; j < idx; ++j)
        {
            opt[j] = argv[i][j];
        }
        opt[idx] = '\0';

/*
 *      Recuperiamo la lunghezza del valore
 */
        size_t idx2 = 0;
        for (idx2 = 0; argv[i][strlen] != '\0'; ++strlen)
        {
            ++idx2;   /* Do Nothing */
        }

        char *val = (char *)calloc(idx2 + 1, sizeof(char));
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (val == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria.\n"
                            "ABORTING...\n\n");
            free(opt);
            return OPT_ERR;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Copiamo il valore
 */
        for (size_t j = 0; j < idx2; ++j)
        {
            val[j] = argv[i][idx + j];
        }
        val[idx2] = '\0';

        uint8_t flag = 0;
        if (val[0] == '=')
        {
            ++val;
            flag = 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

/*
 *      Eseguiamo le azioni necessarie in base all'azione ed al valore forniti
 */
        uint8_t ris = esegui_opzione(opt, val);

        if (flag == 1)
        {
            --val;
        }
        else
        {
            ;   /* Do Nothing */
        }

        free(opt);
        free(val);

        if (ris != OPT_OK)
        {
            return ris;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    return 0;
}