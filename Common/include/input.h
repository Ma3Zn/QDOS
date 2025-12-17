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

#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include "rotation.h"
/*
 *  File contenente le dichiarazioni di tutte le funzioni e strutture dati per
 *  la gestione dell'input da file necessario al nostro programma
 */

//---------------------------------------------------------------------------//

typedef struct input
{
/*
 *  File di input per i parametri del sistema. In ordine:
 *      -   avviamento [tempo di avviamento che passa tra due impulsi]
 *      -   condizioni_iniziali [rho]
 */
    FILE *f_in_P;
/*
 *  File di input per le hamiltoniane dei qudit
 */
    FILE *f_in_H;
/*
 *  File di input per il tensore di connettività delle hamiltoniane
 */
    FILE *f_in_J;
/*
 *  File di input per la matrice di connettività dei livelli del sistema
 */
    FILE *f_in_M;
/*
 *  File di input per gli operatori di errore del sistema
 */
    FILE *f_in_E;
/*
 *  File di input per i ratei degli errori
 */
    FILE *f_in_R;
} input;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  di output
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione della struttura di input per il simulatore
 */
extern
input *alloc_input();

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione della struttura di output per il simulatore
 */
extern
void free_input(input *in);

//---------------------------------------------------------------------------//

/*
 *  Funzione che inizializza la variabile di input passata con i valori
 *  impostati dal parser delle opzioni
 */
extern
uint8_t init_input(input *in);

#endif