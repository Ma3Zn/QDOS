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

#include "../include/rotation.h"
#include "../include/global.h"
#include "../include/pulse.h"
#include "../include/input.h"

/*
 *  File contenente le definizioni di tutte le funzioni per
 *  la gestione dell'input da file necessario al nostro programma
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione della struttura di input per il simulatore
 */
input *alloc_input()
{
    return (input*)malloc(sizeof(input));
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione della struttura di output per il simulatore
 */
void free_input(input *in)
{
    if (in == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (in->f_in_P != NULL)
    {
        fclose(in->f_in_P);
        in->f_in_P = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    if (in->f_in_H != NULL)
    {
        fclose(in->f_in_H);
        in->f_in_H = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    if (in->f_in_J != NULL)
    {
        fclose(in->f_in_J);
        in->f_in_J = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    if (in->f_in_M != NULL)
    {
        fclose(in->f_in_M);
        in->f_in_M = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (errore_abilitato > 0)
    {
        if (in->f_in_E != NULL)
        {
            fclose(in->f_in_E);
            in->f_in_E = NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
        if (in->f_in_R != NULL)
        {
            fclose(in->f_in_R);
            in->f_in_R = NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free(in);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che inizializza la variabile di input passata con i valori
 *  impostati dal parser delle opzioni
 */
uint8_t init_input(input *in)
{
    char *f_in_P = concatena(in_dir, "in.P.bin");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_in_P == NULL)
    {
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_in_H = concatena(in_dir, "in.H.bin");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_in_H == NULL)
    {
        free(f_in_P);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_in_J = concatena(in_dir, "in.J.bin");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_in_J == NULL)
    {
        free(f_in_P);
        free(f_in_H);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_in_M = concatena(in_dir, "in.M.bin");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_in_M == NULL)
    {
        free(f_in_P);
        free(f_in_H);
        free(f_in_J);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    char *f_in_E = NULL;
    char *f_in_R = NULL;
/*
 *  Verifichiamo se l'errore è abilitato o meno
 */
    if (errore_abilitato > 0)
    {
        f_in_E = concatena(in_dir, "in.E.bin");
/*
 *      Verifichiamo l'esito dell'operazione
 */ 
        if (f_in_E == NULL)
        {
            free(f_in_P);
            free(f_in_H);
            free(f_in_J);
            free(f_in_M);
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }
        f_in_R = concatena(in_dir, "in.R.bin");
/*  
 *      Verifichiamo l'esito dell'operazione
 */ 
        if (f_in_R == NULL)
        {
            free(f_in_P);
            free(f_in_H);
            free(f_in_J);
            free(f_in_M);
            free(f_in_E);
            return 1;
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
    
    in->f_in_P = fopen(f_in_P, "r");
    in->f_in_H = fopen(f_in_H, "r");
    in->f_in_J = fopen(f_in_J, "r");
    in->f_in_M = fopen(f_in_M, "r");
/*
 *  Verifichiamo se l'errore è abilitato o meno
 */
    if (errore_abilitato > 0)
    {
        in->f_in_E = fopen(f_in_E, "r");
        in->f_in_R = fopen(f_in_R, "r");

        if (in->f_in_E == NULL || in->f_in_R == NULL)
        {
            fprintf(stderr, "ERRORE nell'apertura dei file di input.\n"
                            "ABORTING...\n\n");

            free(f_in_P);
            free(f_in_H);
            free(f_in_J);
            free(f_in_M);
            free(f_in_E);
            free(f_in_R);

            return 1;
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

    if (    in->f_in_P == NULL 
        ||  in->f_in_H == NULL
        ||  in->f_in_J == NULL
        ||  in->f_in_M == NULL)
    {
        fprintf(stderr, "ERRORE nell'apertura dei file di input.\n"
                        "ABORTING...\n\n");

        free(f_in_P);
        free(f_in_H);
        free(f_in_J);
        free(f_in_M);
        free(f_in_E);
        free(f_in_R);

        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(f_in_P);
    free(f_in_H);
    free(f_in_J);
    free(f_in_M);
    free(f_in_E);
    free(f_in_R);

    return 0;
}