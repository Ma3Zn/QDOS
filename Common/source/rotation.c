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

/*
 *  Allocazione di una rotazione piana
 */
rotazione_piana *alloc_rot_piana()
{
    return (rotazione_piana*)malloc(sizeof(rotazione_piana));
}

/*
 *  Deallocazione di una rotazione piana
 */
void free_rot_piana(rotazione_piana *rot)
{
    free(rot);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una rotazione Z
 */
rotazione_Z *alloc_rot_Z()
{
    return (rotazione_Z*)malloc(sizeof(rotazione_Z));
}

/*
 *  Deallocazione di una rotazione Z
 */
void free_rot_Z(rotazione_Z *rot)
{
    free(rot);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una generica rotazione
 */
rotazione *alloc_rotazione(size_t id)
{
    rotazione *rot = (rotazione*)malloc(sizeof(rotazione));
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (rot == NULL)
    {
        fprintf(stderr, "Errore nell'allocazione di una rotazione.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* TBD */
    }

    rot->type = id;

    if (rot->type == ID_ROT_PIANA)
    {
        rotazione_piana *rot_piana = alloc_rot_piana();
/*
 *      Errore gia segnalato
 */
        if (rot_piana == NULL)
        {
            free_rotazione(&rot);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        rot->ptr.piana = rot_piana;
    }
    else if (rot->type == ID_ROT_Z)
    {
        rotazione_Z *rot_Z = alloc_rot_Z();
/*
 *      Errore gia segnalato
 */
        if (rot_Z == NULL)
        {
            free_rotazione(&rot);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        rot->ptr.Z = rot_Z;
    }
    else
    {
        fprintf(stderr, "ERRORE nell'id di identificazione di una "
                        "rotazione.\n"
                        "ID: %ld sconosciuto.\n\n"
                        "ID = %d per una rotazione piana.\n"
                        "ID = %d per una rotazione Z.\n\n"
                        "ABORTING...\n\n", id, ID_ROT_PIANA, ID_ROT_Z);
        free_rotazione(&rot);
        return NULL;
    }

    return rot;
}

/*
 *  Deallocazione di una generica rotazione
 */
void free_rotazione(rotazione **rot)
{
    if (rot == NULL || *rot == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    switch ((*rot)->type)
    {
        case ID_ROT_PIANA:
            free_rot_piana((*rot)->ptr.piana);
            (*rot)->ptr.piana = NULL;
            break;

        case ID_ROT_Z:
            free_rot_Z((*rot)->ptr.Z);
            (*rot)->ptr.Z = NULL;
            break;

        default:
            break;
    }

    free(*rot);
    *rot = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Allocazione di una sequenza di rotazioni
 */
sequenza_rotazioni *alloc_seq_rot(size_t N_rot)
{
    sequenza_rotazioni *seq = 
        (sequenza_rotazioni*)malloc(sizeof(sequenza_rotazioni));
    
    if (seq == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->rot = (rotazione**)calloc(sizeof(rotazione*), N_rot);

    if (seq->rot == NULL && N_rot != 0)
    {
        free_seq_rot(&seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->dim = N_rot;
    return seq;
}

/*
 *  Riallocazione di una sequenza di rotazioni
 */
uint8_t realloc_seq_rot(sequenza_rotazioni *seq, size_t N_rot)
{
    rotazione **ptr =
        (rotazione**)realloc(seq->rot, sizeof(rotazione*) * N_rot);
    
    if (ptr == seq->rot)
    {
        seq->dim = N_rot;
        return 0u;
    }
    else if (ptr != NULL)
    {
        seq->rot = ptr;
        seq->dim = N_rot;
        return 0u;
    }
    else if (N_rot == 0)
    {
        seq->dim = N_rot;
        return 0u;
    }
    else
    {
        return 1u;
    }
}

/*
 *  Deallocazione di una sequenza di rotazioni
 */
void free_seq_rot(sequenza_rotazioni **seq)
{
    if (seq == NULL || *seq == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if ((*seq)->rot != NULL)
    {
        for (size_t i = 0; i < (*seq)->dim; ++i)
        {
            if ((*seq)->rot[i] != NULL)
            {
                free_rotazione(&((*seq)->rot[i]));
            }
            else
            {
                ;   /* Do Nothing */
            }
        }

    free((*seq)->rot);
    (*seq)->rot = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    free(*seq);
    *seq = NULL;

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per il controllo del corretto ordinamento dei livelli tra cui
 *  fare la rotazione Z. Forziamo che livello[0] < livello[1], poichè
 *  l'algoritmo da noi implementato lavora esattamente in questo modo.
 */
void controlla_ordine_livelli_rotazione_Z(rotazione_Z *rot)
{
/*
 *  Controlliamo che la rotazine passata non sia nulla
 */
    if (rot == NULL)
    {
        fprintf(stderr, "ATTENZIONE la rotazione passata e' nulla.\n");
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (rot->livello[0] > rot->livello[1])
    {
        fprintf(stderr, "ATTENZIONE livelli di rotazione ordinati in modo"
                        " errato. Riordinamento dei livelli eseguito"
                        " automaticamente.\n");

        size_t tmp = rot->livello[1];
        rot->livello[1] = rot->livello[0];
        rot->livello[0] = tmp;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

/*
 *  Funzione per il controllo del corretto ordinamento dei livelli tra cui
 *  fare la rotazione piana. Forziamo che livello[0] < livello[1], poichè
 *  l'algoritmo da noi implementato lavora esattamente in questo modo.
 */
void controlla_ordine_livelli_rotazione_piana(rotazione_piana *rot)
{
/*
 *  Controlliamo che la rotazine passata non sia nulla
 */
    if (rot == NULL)
    {
        fprintf(stderr, "ATTENZIONE la rotazione passata e' nulla.\n");
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (rot->livello[0] > rot->livello[1])
    {
        fprintf(stderr, "ATTENZIONE livelli di rotazione ordinati in modo"
                        " errato. Riordinamento dei livelli eseguito"
                        " automaticamente.\n");

        size_t tmp = rot->livello[1];
        rot->livello[1] = rot->livello[0];
        rot->livello[0] = tmp;
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che si assicura che l'angolo di rotazione di una rotazione
 *  Z sia sempre compreso tra (-pi, pi]
 */
void controlla_ottimalita_parametri_rot_Z(rotazione_Z *rot)
{
    if (rot == NULL)
    {
        fprintf(stderr, "ATTENZIONE la rotazione passata e' nulla.\n");
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (rot->phi > M_PI)
    {
        fprintf(stderr, "ATTENZIONE angolo di rotazione non ottimale.\n"
                        "Automatica traslazione dell'angolo di rotazione "
                        "nel range (-pi, pi]\n\n");
        
        do { rot->phi -= M_PI; } while (rot->phi > M_PI);
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}


/*
 *  Funzione che si assicura che i parametri di rotazione di una rotazione
 *  piana siano ottimali, ovvero l'angolo di rotazione in [0,pi]
 * 
 *  SBAGLIATA!!!
 */
void controlla_ottimalita_parametri_rot_piana(rotazione_piana *rot)
{
    if (rot == NULL)
    {
        fprintf(stderr, "ATTENZIONE la rotazione passata e' nulla.\n");
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (rot->theta < 0.0)
    {
        fprintf(stderr, "ATTENZIONE angolo di rotazione non ottimale.\n"
                        "Automatica traslazione dell'angolo di rotazione."
                        "\n\n");
        
        do {
            rot->theta += 4.0 * M_PI;
        } while (rot->theta < 0.0);
    }
    else if (rot->theta > 4.0 * M_PI)
    {
        fprintf(stderr, "ATTENZIONE angolo di rotazione non ottimale.\n"
                        "Automatica traslazione dell'angolo di rotazione."
                        "\n\n");
        
        do {
            rot->theta += 4.0 * M_PI;
        } while (rot->theta < -4.0 * M_PI);
    }

/*
 *  Se l'angolo di rotazione è maggiore di 2*PI possiamo diminuirlo invertendo
 *  due volte la matrice di rotazione piana
 */
    if (rot->theta > 2.0 * M_PI)
    {
        rot->theta -=  4.0 * M_PI;
/*
 *      Invertiamo due volte la matrice
 */
        rot->theta *= -1.0;
        rot->beta  +=  M_PI;
    }
    else
    {
        ;   /* Do Nothing */
    }

    while (rot->beta < 0.0) 
    {
/*
 *      Invertiamo due volte la matrice di rotazione piana
 */
        fprintf(stderr, "ATTENZIONE fase di rotazione negativa. Aggiustamento "
                        "automatico della fase tramite una doppia inversione "
                        "della matrice\n\n");
        rot->beta  += M_PI;
        rot->theta *= -1.0;
    }

/*
 *  Se l'angolo di rotazione è ancora negativo rendiamolo positivo
 *  invertendo due volte la matrice
 */
    if (rot->theta < 0)
    {
        rot->beta  += M_PI;
        rot->theta *= -1.0;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Sfruttiamo la periodicità in beta di 2*PI per riportare beta tra 0 e 2*PI
 */
    while (rot->beta > 2.0 * M_PI)
    {
        fprintf(stderr, "ATTENZIONE fase di rotazione NON ottimale. "
                        "Traslazione automatica della fase nel range ottimale"
                        "\n\n");
        rot->beta -= 2.0 * M_PI;
    }

    return;
}
