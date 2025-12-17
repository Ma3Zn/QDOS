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

#ifndef ROTATION_H
#define ROTATION_H

#include "common.h"

/*
 *  Header file contenente tuttel le strutture dati rappresentati le rotazioni
 *  in cui l'algoritmo di decomposizione andrà a decomporre la matrice unitaria
 *  speciale fornita come input
 */

#define ID_ROT_PIANA 0u
#define ID_ROT_Z     1u

//---------------------------------------------------------------------------//

/*
 *  Struttura dati rappresentate una rotazione piana
 */
typedef struct
{
/*
 *  I due livelli del qudit a cui vogliamo applicare la rotazione piana
 */
    uint64_t livello[2];

/*
 *  Angolo della rotazione piana
 */
    double theta;

/*
 *  Fase della rotazione piana
 */
    double beta;
} rotazione_piana;

//---------------------------------------------------------------------------//

/*
 *  Struttura dati rappresentate una rotazione Z 
 */
typedef struct 
{
/*
 *  I due livelli del qudit a cui vogliamo applicare la rotazione piana
 */
    uint64_t livello[2];

/*
 *  Angolo di rotazione
 */
    double phi;

} rotazione_Z;

//---------------------------------------------------------------------------//

/*
 *  Struttura dati rappresentate una generica rotazione (sia Z che piana)
 */
typedef struct 
{
/*
 *  Il tipo della rotazione
 */
    enum
    {
        ROT_PIANA   = ID_ROT_PIANA,
        ROT_Z       = ID_ROT_Z
    } type;

/*
 *  Puntatore alla rotazione effettiva
 */
    union
    {
        rotazione_piana *piana;
        rotazione_Z *Z;
    } ptr;

} rotazione;

//---------------------------------------------------------------------------//

/*
 *  Struttura dati rappresentante una sequenza di rotazioni
 */
typedef struct sequenza_rotazioni
{
/*
 *  Array contenente i puntatori alle varie rotazioni
 */
    rotazione **rot;

/*
 *  Numero delle rotazioni nella sequenza
 */
    size_t dim;
} sequenza_rotazioni;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di sequenza di rotazioni
 */

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una rotazione piana
 */
extern
rotazione_piana *alloc_rot_piana();

/*
 *  Deallocazione di una rotazione piana
 */
extern
void free_rot_piana(rotazione_piana *rot);

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una rotazione Z
 */
extern
rotazione_Z *alloc_rot_Z();

/*
 *  Deallocazione di una rotazione Z
 */
extern
void free_rot_Z(rotazione_Z *rot);

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una generica rotazione
 */
extern
rotazione *alloc_rotazione(size_t id);

/*
 *  Deallocazione di una generica rotazione
 */
extern
void free_rotazione(rotazione **rot);

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una sequenza di rotazioni
 */
extern
sequenza_rotazioni *alloc_seq_rot(size_t N_rot);

/*
 *  Riallocazione di una sequenza di rotazioni
 */
extern
uint8_t realloc_seq_rot(sequenza_rotazioni *seq, size_t N_rot);

/*
 *  Deallocazione di una sequenza di rotazioni
 */
extern
void free_seq_rot(sequenza_rotazioni **seq);

//---------------------------------------------------------------------------//

/*
 *  Funzione per il controllo del corretto ordinamento dei livelli tra cui
 *  fare la rotazione Z. Forziamo che livello[0] < livello[1], poichè
 *  l'algoritmo da noi implementato lavora esattamente in questo modo.
 */
extern
void controlla_ordine_livelli_rotazione_Z(rotazione_Z *rot);

/*
 *  Funzione per il controllo del corretto ordinamento dei livelli tra cui
 *  fare la rotazione piana. Forziamo che livello[0] < livello[1], poichè
 *  l'algoritmo da noi implementato lavora esattamente in questo modo.
 */
extern
void controlla_ordine_livelli_rotazione_piana(rotazione_piana *rot);

/*
 *  Funzione che si assicura che l'angolo di rotazione di una rotazione
 *  Z sia sempre compreso tra (-pi, pi]
 */
extern
void controlla_ottimalita_parametri_rot_Z(rotazione_Z *rot);

/*
 *  Funzione che si assicura che i parametri di rotazione di una rotazione
 *  piana siano ottimali, ovvero l'angolo di rotazione in [0,pi]
 */
extern
void controlla_ottimalita_parametri_rot_piana(rotazione_piana *rot);

#endif