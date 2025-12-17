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

#include "../include/gate_built_in.h"

#include "../../Common/include/rotation.h"
#include "../../Common/include/global.h"
#include "../../Common/include/common.h"
#include <math.h>

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una matrice unitaria la rende unitaria speciale
 */
gsl_matrix_complex *normalizza(gsl_matrix_complex *u)
{
    if (u == NULL)
    {
        fprintf(stderr, "ERRORE nella normalizzazione di un operatore "
                        "unitario.\n"
                        "Operatore fornito come argomento NULLO.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t dim = u->size1;

    gsl_matrix_complex *u_copy = gsl_matrix_complex_calloc(dim, dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (u_copy == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di memoria.\n"
                        "ABORTING...\n\n");
        gsl_matrix_complex_free(u);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_matrix_complex_memcpy(u_copy, u);

/*
 *  Allochiamo la permutazione richiesta dall'algoritmo di decomposizione
 */
    gsl_permutation *p = gsl_permutation_calloc(dim);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (p == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di memoria.\n"
                        "ABORTING...\n\n");
        gsl_matrix_complex_free(u_copy);
        gsl_matrix_complex_free(u);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Segno della permutazione
 */
    int sgn = 1;

/*
 *  Fattorizzazione LU di u
 */
    gsl_linalg_complex_LU_decomp(u_copy, p, &sgn);
/*
 *  Calcolo del determinante di u
 */
    gsl_complex det = gsl_linalg_complex_LU_det(u_copy, sgn);
/*
 *  Recuperiamo l'argomento del determinante
 */
    double arg = gsl_complex_arg(det);
/*
 *  Calcoliamo il fattore di normalizzazione
 */
    arg = ((-1.0) * arg) / ((double)dim);
    gsl_complex scale = gsl_complex_polar(1.0, arg);
/*
 *  Normalizziamo u
 */
    gsl_matrix_complex_scale(u, scale);

    return u;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di una specifica rotazione Z
 */
gate_operator *imp_nullo(parametri_gate *param)
{
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 1)
    {
        fprintf(stderr, "ERRORE il gate '$$IDLE' richiede 1 argomenti.\n"
                        "1. Durata IDLE (ns)\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Verifichiamo la legalità dei parametri
 */
    if (param->arg[0] < 0.0)
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate '$$IDLE'.\n"
                        "Durata fornita negativa.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Allochiamo l'impulso nullo per simulare l'evoluzione libera
 */
    impulso *imp = alloc_impulso();

    if (imp == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per un impulso"
                        "IDLE.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inizializzazione dell'impulso
 */
    imp->modulazione = modulazione_impulso_zero;
    imp->T = param->arg[0];

    imp->livelli[0] = 0;
    imp->livelli[1] = 1;
    imp->gamma = 0.0;
    imp->theta = 0.0;
    imp->omega = 0.0;

    impulsi_paralleli *imp_par = alloc_impulsi_paralleli(1);

    if (imp_par == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per un impulso"
                        "IDLE.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_impulso(imp);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    imp_par->arr_impulsi[0] = imp;
    imp_par->stato = NON_SIMULATO;

    sequenza_impulsi *seq = alloc_sequenza_impulsi(1);

    if (seq == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per un impulso"
                        "IDLE.\n"
                        "ABORTING...\n\n");
        free_impulsi_paralleli(imp_par);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    seq->arr_impulsi_paralleli[0] = imp_par;

    schedule *sch = alloc_schedule(1);

    if (sch == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della memoria per un impulso"
                        "IDLE.\n"
                        "ABORTING...\n\n");
        free_sequenza_impulsi(seq);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    sch->arr_sequenze[0] = seq;

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        free_schedule(sch);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->type    = COD_SCH;
    g_op->val.sch = sch;

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di una specifica rotazione Z
 */
gate_operator *Z_rot(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate '$RZ'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 1)
    {
        fprintf(stderr, "ERRORE il gate '$RZ' agisce solo su un unico qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 3)
    {
        fprintf(stderr, "ERRORE il gate '$RZ' richiede 3 argomenti.\n"
                        "1. lv_0\n"
                        "2. lv_1\n"
                        "3. theta\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

    size_t dim = dimensioni_qudit[param->idx_qudit[0]];
/*
 *  Verifichiamo la legalità dei parametri
 */
    if ((dim - 1 < param->arg[0] || param->arg[0] < 0) ||
        (dim - 1 < param->arg[1] || param->arg[1] < 0))
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate '$RZ'.\n"
                        "Indici livelli forniti illegali.\n"
                        "Indici forniti: %ld, %ld\n"
                        "Range indici consentito [0, %ld]\n"
                        "ABORTING...\n\n",
                        (size_t)param->arg[0],
                        (size_t)param->arg[1],
                        dim - 1);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Generiamo l'operatore del gate
 */
    g_op->type    = COD_SQR;
    sequenza_rotazioni *seq = alloc_seq_rot(1);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (seq == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una sequenza di "
                        "rotazioni.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    rotazione *rot = alloc_rotazione(ID_ROT_Z);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (rot == NULL)
    {
        free_seq_rot(&seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
#ifdef DEBUG
    fprintf(stderr, "\nParametri gate '$RZ': %f, %f, %f\n", 
            param->arg[0], param->arg[1], param->arg[2]);
#endif
/*
 *  verifichiamo il corretto ordinamento dei livelli
 */
    if (param->arg[0] < param->arg[1])
    {
        rot->ptr.Z->livello[0] = (uint64_t)param->arg[0];
        rot->ptr.Z->livello[1] = (uint64_t)param->arg[1];
        rot->ptr.Z->phi        = param->arg[2];
    }
    else if (param->arg[0] > param->arg[1])
    {
        rot->ptr.Z->livello[0] = (uint64_t)param->arg[1];
        rot->ptr.Z->livello[1] = (uint64_t)param->arg[0];
        rot->ptr.Z->phi        = (-1.0) * param->arg[2];
    }
    else
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate '$RZ'.\n"
                        "Rotazione operante tra due livelli identici.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }

    seq->rot[0] = rot;
    g_op->val.seq = seq;

#ifdef DEBUG
fprintf(stdout, "\nOperatore gate '$RZ':\n");
stampa_rotazione_Z(stdout, rot->ptr.Z);
fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di una specifica rotazione piana
 */
gate_operator *P_rot(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate '$RP'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 1)
    {
        fprintf(stderr, "ERRORE il gate '$RP' agisce solo su un unico qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 4)
    {
        fprintf(stderr, "ERRORE il gate '$RP' richiede 4 argomenti.\n"
                        "1. lv_0\n"
                        "2. lv_1\n"
                        "3. theta\n"
                        "4. beta\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

    size_t dim = dimensioni_qudit[param->idx_qudit[0]];
/*
 *  Verifichiamo la legalità dei parametri
 */
    if ((dim - 1 < param->arg[0] || param->arg[0] < 0) ||
        (dim - 1 < param->arg[1] || param->arg[1] < 0))
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate '$RP'.\n"
                        "Indici livelli forniti illegali.\n"
                        "Indici forniti: %ld, %ld\n"
                        "Range indici consentito [0, %ld]\n"
                        "ABORTING...\n\n",
                        (size_t)param->arg[0],
                        (size_t)param->arg[1],
                        dim - 1);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Generiamo l'operatore del gate
 */
    g_op->type = COD_SQR;

    sequenza_rotazioni *seq = alloc_seq_rot(1);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (seq == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione di una sequenza di "
                        "rotazioni.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    rotazione *rot = alloc_rotazione(ID_ROT_PIANA);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (rot == NULL)
    {
        free_seq_rot(&seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  verifichiamo il corretto ordinamento dei livelli
 */
    if (param->arg[0] < param->arg[1])
    {
        rot->ptr.piana->livello[0] = (uint64_t)param->arg[0];
        rot->ptr.piana->livello[1] = (uint64_t)param->arg[1];
        rot->ptr.piana->theta      = param->arg[2];
        rot->ptr.piana->beta       = param->arg[3];
    }
    else if (param->arg[0] > param->arg[1])
    {
        rot->ptr.Z->livello[0] = (uint64_t)param->arg[1];
        rot->ptr.Z->livello[1] = (uint64_t)param->arg[0];
        rot->ptr.Z->phi        = param->arg[2];
        rot->ptr.piana->beta   = (-1.0) * (M_PI + param->arg[3]);
    }
    else
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate '$RP'.\n"
                        "Rotazione operante tra due livelli identici.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }

    seq->rot[0] = rot;
    g_op->val.seq = seq;

#ifdef DEBUG
fprintf(stdout, "\nOperatore gate '$RP':\n");
stampa_rotazione_piana(stdout, rot->ptr.piana);
fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate X generalizzato
 */
gate_operator *X_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'X'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 1)
    {
        fprintf(stderr, "ERRORE il gate 'X' agisce solo su un unico qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 0)
    {
        fprintf(stderr, "ERRORE il gate 'X' richiede 0 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Generiamo l'operatore del gate
 */
    g_op->type = COD_UNT;

/*
 *  Allochiamo la matrice che conterrà la rappresentazione del gate
 */
    size_t dim = dimensioni_qudit[param->idx_qudit[0]];
    gsl_matrix_complex *u = gsl_matrix_complex_calloc(dim, dim);

    if (u == NULL)
    {
        fprintf(stderr, "Errore nell'allocazione della matrice "
                        "rappresentante il gate 'X'.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    gsl_complex one = gsl_complex_rect(1.0, 0.0);

/*
 *  Costruiamo la rappresentazione dell'operatore
 */
    gsl_matrix_complex_set(u, 0, dim - 1, one);
    for (size_t i = 1; i < dim; ++i)
    {
        gsl_matrix_complex_set(u, i, i - 1, one);
    }
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
fprintf(stdout, "\nOperatore gate 'X':\n");
stampa_matrice(stdout, u);
fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate Z generalizzato
 */
gate_operator *Z_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'Z'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 1)
    {
        fprintf(stderr, "ERRORE il gate 'Z' agisce solo su un unico qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 0)
    {
        fprintf(stderr, "ERRORE il gate 'Z' richiede 0 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Generiamo l'operatore del gate
 */
    g_op->type = COD_UNT;

/*
 *  Allochiamo la matrice che conterrà la rappresentazione del gate
 */
    size_t dim = dimensioni_qudit[param->idx_qudit[0]];
    gsl_matrix_complex *u = gsl_matrix_complex_calloc(dim, dim);

    if (u == NULL)
    {
        fprintf(stderr, "Errore nell'allocazione della matrice "
                        "rappresentante il gate 'Z'.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Costruiamo la rappresentazione dell'operatore
 */
    for (size_t i = 0; i < dim; ++i)
    {
        double angle = (2.0 * M_PI * (double)i)/ ((double)dim);
        gsl_complex omega = gsl_complex_polar(1.0, angle);
        gsl_matrix_complex_set(u, i, i, omega);
    }
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
fprintf(stdout, "\nOperatore gate 'Z':\n");
stampa_matrice(stdout, u);
fprintf(stdout, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate H generalizzato
 */
gate_operator *H_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'H'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 1)
    {
        fprintf(stderr, "ERRORE il gate 'H' agisce solo su un unico qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 0)
    {
        fprintf(stderr, "ERRORE il gate 'H' richiede 0 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Generiamo l'operatore del gate
 */
    g_op->type = COD_UNT;

/*
 *  Allochiamo la matrice che conterrà la rappresentazione del gate
 */
    size_t dim = dimensioni_qudit[param->idx_qudit[0]];
    gsl_matrix_complex *u = gsl_matrix_complex_calloc(dim, dim);

    if (u == NULL)
    {
        fprintf(stderr, "Errore nell'allocazione della matrice "
                        "rappresentante il gate 'H'.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Costruiamo la rappresentazione dell'operatore
 */
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            double angle = (2.0 * M_PI * (double)(i * j)) / ((double)dim);
            gsl_complex omega = gsl_complex_polar(1, angle);

            if (fabs(omega.dat[0]) < 1e-12)
            {
                omega.dat[0] = 0.0;
            }
            else
            {
                ;   /* Do Nothing */
            }

            if (fabs(omega.dat[1]) < 1e-12)
            {
                omega.dat[1] = 0.0;
            }
            else
            {
                ;   /* Do Nothing */
            }

            gsl_matrix_complex_set(u, i, j, omega);
        }
    }

    gsl_complex scale = gsl_complex_rect(1.0/(sqrt(dim)), 0.0);
    gsl_matrix_complex_scale(u, scale);
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
fprintf(stdout, "\nOperatore gate 'H':\n");
stampa_matrice(stdout, u);
fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate S generalizzato
 */
gate_operator *S_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'S'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 1)
    {
        fprintf(stderr, "ERRORE il gate 'S' agisce solo su un unico qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 0)
    {
        fprintf(stderr, "ERRORE il gate 'S' richiede 0 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Generiamo l'operatore del gate
 */
    g_op->type = COD_UNT;
/*
 *  Allochiamo la matrice che conterrà la rappresentazione del gate
 */
    size_t dim = dimensioni_qudit[param->idx_qudit[0]];
    gsl_matrix_complex *u = gsl_matrix_complex_calloc(dim, dim);

    if (u == NULL)
    {
        fprintf(stderr, "Errore nell'allocazione della matrice "
                        "rappresentante il gate 'S'.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Forziamo la definizione di S per d = 2 a quella canonica, i.e.
 *
 *                          | 1     0 |
 *                          |         |
 *                          | 0     i |
 * 
 * in quanto la definizione generalizzata per dim = 2 farebbe si che
 * 
 *                             S = Z
 */
    if (dim == 2)
    {
        gsl_complex one = gsl_complex_rect(1.0, 0.0);
        gsl_complex I = gsl_complex_rect(0.0, 1.0);

        gsl_matrix_complex_set(u, 0, 0, one);
        gsl_matrix_complex_set(u, 1, 1, I);
    }
    else
    {
/*
 *      Costruiamo la rappresentazione dell'operatore generalizzato
 */ 
        for (size_t i = 0; i < dim; ++i)
        {
            double angle = (M_PI * (double)(i * (i + 1))) / ((double)dim);
            gsl_complex omega = gsl_complex_polar(1.0, angle);

            gsl_matrix_complex_set(u, i, i, omega);
        }
    }
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
    fprintf(stdout, "\nOperatore gate 'S':\n");
    stampa_matrice(stdout, u);
    fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CX generalizzato, i.e., un
 *  C_inc
 */
gate_operator *CX_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'CX'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 2)
    {
        fprintf(stderr, "ERRORE il gate 'CX' agisce su due qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 0)
    {
        fprintf(stderr, "ERRORE il gate 'CX' richiede 0 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Recuperiamo gli indici dei qudit
 */
    size_t idx_0 = param->idx_qudit[0];
    size_t idx_1 = param->idx_qudit[1];

    gsl_matrix_complex *u = NULL;
/*
 *     Generiamo l'operatore del gate
 */
        g_op->type = COD_UNT;
/*
 *  Verifichiamo l'ordinamento del controll e del target
 */
    if (idx_0 < idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CX'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        gsl_complex zero = gsl_complex_rect(0.0, 0.0);
        gsl_complex one  = gsl_complex_rect(1.0, 0.0);

        for (size_t j = 0; j < dim_1 - 1; ++j)
        {
            size_t colonna = (dim_0 - 1) * dim_1 + j;
            gsl_matrix_complex_set(u, colonna, colonna, zero);
            gsl_matrix_complex_set(u, colonna + 1, colonna, one);
        }

        gsl_matrix_complex_set(u, (dim_0 - 1) * dim_1, dim - 1, one);
        gsl_matrix_complex_set(u, dim - 1, dim - 1, zero);
    }
    else if (idx_0 > idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CX'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        gsl_complex zero = gsl_complex_rect(0.0, 0.0);
        gsl_complex one  = gsl_complex_rect(1.0, 0.0);

        for (size_t i = 1; i < dim_0; ++i)
        {
            size_t colonna = dim_1 * i - 1;

            gsl_matrix_complex_set(u, colonna, colonna, zero);
            gsl_matrix_complex_set(u, colonna, colonna + dim_1, one);
        }

        gsl_matrix_complex_set(u, dim - 1, dim_1 - 1, one);
        gsl_matrix_complex_set(u, dim - 1, dim - 1, zero);
    }
    else
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate 'CX'.\n"
                        "Gate invocato con target coincidente con al "
                        "control.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
    fprintf(stdout, "\nOperatore gate 'CX':\n");
    stampa_matrice(stdout, u);
    fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CZ generalizzato
 */
gate_operator *CZ_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'CZ'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 2)
    {
        fprintf(stderr, "ERRORE il gate 'CZ' agisce su due qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 0)
    {
        fprintf(stderr, "ERRORE il gate 'CZ' richiede 0 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Recuperiamo gli indici dei qudit
 */
    size_t idx_0 = param->idx_qudit[0];
    size_t idx_1 = param->idx_qudit[1];

    gsl_matrix_complex *u = NULL;
/*
 *     Generiamo l'operatore del gate
 */
        g_op->type = COD_UNT;
/*
 *  Verifichiamo l'ordinamento del controll e del target
 */
    if (idx_0 < idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CZ'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        for (size_t j = 0; j < dim_1; ++j)
        {
            double angle = (2.0 * M_PI * (double)j)/ ((double)dim_1);
            gsl_complex omega = gsl_complex_polar(1.0, angle);

            size_t colonna = (dim_0 - 1) * dim_1 + j;
            gsl_matrix_complex_set(u, colonna, colonna, omega);
        }
    }
    else if (idx_0 > idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CZ'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        for (size_t i = 0; i < dim_0; ++i)
        {
            double angle = (2.0 * M_PI * (double)i)/ ((double)dim_0);
            gsl_complex omega = gsl_complex_polar(1.0, angle);

            size_t colonna = dim_1 * (i + 1) - 1;
            gsl_matrix_complex_set(u, colonna, colonna, omega);
        }
    }
    else
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate 'CZ'.\n"
                        "Gate invocato con target coincidente con al "
                        "control.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
    fprintf(stdout, "\nOperatore gate 'CZ':\n");
    stampa_matrice(stdout, u);
    fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CR generalizzato, gate fondamentale
 *  per l'implementazione della QFT
 */
gate_operator *CR_generalizzato(parametri_gate *param)
{
    
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'CR'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 2)
    {
        fprintf(stderr, "ERRORE il gate 'CR' agisce su due qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 1)
    {
        fprintf(stderr, "ERRORE il gate 'CR' richiede 1 argomenti.\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Recuperiamo gli indici dei qudit
 */
    size_t idx_0 = param->idx_qudit[0];
    size_t idx_1 = param->idx_qudit[1];
/*
 *  Recuperiamo il parametro del gate che definirà l'angolo di rotazione
 */
    double k = param->arg[0];
    gsl_matrix_complex *u = NULL;
/*
 *     Generiamo l'operatore del gate
 */
        g_op->type = COD_UNT;
/*
 *  Verifichiamo l'ordinamento del controll e del target. In questo caso i due
 *  casi coincidono, quindi verifichiamo solo la diversità degli indici
 */
    if (idx_0 != idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CR'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        for (size_t i = 1; i < dim_0; ++i)
        {
            for (size_t j = 0; j < dim_1; ++j)
            {
                double angle = (2.0 * M_PI * (double)(i * j))/ (pow((double)dim_0, k));
                gsl_complex omega = gsl_complex_polar(1.0, angle);

                size_t colonna = i * dim_1 + j;
                gsl_matrix_complex_set(u, colonna, colonna, omega);
            }
        }
    }
    else
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate 'CR'.\n"
                        "Gate invocato con target coincidente con al "
                        "control.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
#ifdef DEBUG
    fprintf(stdout, "\nOperatore gate 'CR':\n");
    stampa_matrice(stdout, u);
    fprintf(stderr, "\n");
#endif
/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
    fprintf(stdout, "\nOperatore gate 'CR':\n");
    stampa_matrice(stdout, u);
    fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la generazione di un gate CX generalizzato modificato, i.e.,
 *  un C_exc
 */
gate_operator *CEX_generalizzato(parametri_gate *param)
{
    if (param == NULL)
    {
        fprintf(stderr, "ERRORE nell'utilizzo del gate 'CEX'\n"
                        "Paramtri NULLI.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_qudit != 2)
    {
        fprintf(stderr, "ERRORE il gate 'CEX' agisce su due qudit.\n"
                        "E' stato invocato su %d qudit\n"
                        "ABORTING...\n\n", param->n_qudit);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }
/*
 *  Verifichiamo la correttezza dei parametri
 */
    if (param->n_arg != 3)
    {
        fprintf(stderr, "ERRORE il gate 'CEX' richiede 0 argomenti.\n"
                        "1. lv_control\n"
                        "2. lv_0\t(to be exchanged)\n"
                        "3. lv_1\t(to be exchanged)\n"
                        "Sono stati forniti %ld argomenti\n"
                        "ABORTING...\n\n", param->n_arg);
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */ 
    }

/*
 *  Allochiamo la memoria per l'operatore del gate da restituire
 */
    gate_operator *g_op = alloc_gate_operator();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_op == NULL)
    {
        free_parametri_gate(param);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Recuperiamo gli indici dei qudit
 */
    size_t idx_0 = param->idx_qudit[0];
    size_t idx_1 = param->idx_qudit[1];

    gsl_matrix_complex *u = NULL;
/*
 *     Generiamo l'operatore del gate
 */
        g_op->type = COD_UNT;
/*
 *  Verifichiamo l'ordinamento del controll e del target
 */
    if (idx_0 < idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CEX'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        gsl_complex zero = gsl_complex_rect(0.0, 0.0);
        gsl_complex one  = gsl_complex_rect(1.0, 0.0);
/*
 *      Recuperiamo il livello di controllo
 */
        size_t lv_control = (size_t)param->arg[0];
/*
 *      Verifichiamo la legalità del livello di controllo
 */
        if (lv_control > dim_0 - 1)
        {
            fprintf(stderr, "ERRORE nell'invocazione del gate 'CEX'.\n"
                            "Il livello di controllo è superiore alla "
                            "dimensione del qudit di controllo.\n"
                            "ABORTING...\n\n");
            gsl_matrix_complex_free(u);
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Recuperiamo i livelli del target da scambiare
 */
        size_t lv_target_0 = (size_t)param->arg[1];
        size_t lv_target_1 = (size_t)param->arg[2];
/*
 *      Verifichiamo la legalità dei livelli da scambiare
 */
        if (lv_target_0 > dim_1 - 1 || lv_target_1 > dim_1 - 1)
        {
            fprintf(stderr, "ERRORE nell'invocazione del gate 'CEX'.\n"
                            "I livelli del target da scambiare sono superiori "
                            "alla dimensione del qudit target.\n"
                            "ABORTING...\n\n");
            gsl_matrix_complex_free(u);
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        size_t indice_0 = lv_control * dim_1 + lv_target_0;
        size_t indice_1 = lv_control * dim_1 + lv_target_1;

        gsl_matrix_complex_set(u, indice_0, indice_0, zero);
        gsl_matrix_complex_set(u, indice_1, indice_1, zero);

        gsl_matrix_complex_set(u, indice_0, indice_1, one);
        gsl_matrix_complex_set(u, indice_1, indice_0, one);
    }
    else if (idx_0 > idx_1)
    {
/*
 *      Allochiamo la matrice che conterrà la rappresentazione del gate
 */
        size_t dim_0 = dimensioni_qudit[idx_0];
        size_t dim_1 = dimensioni_qudit[idx_1];
        size_t dim = dim_0 * dim_1;

        u = gsl_matrix_complex_calloc(dim, dim);

        if (u == NULL)
        {
            fprintf(stderr, "Errore nell'allocazione della matrice "
                            "rappresentante il gate 'CEX'.\n"
                            "Memoria insufficiente.\n"
                            "ABORTING...\n\n");
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        gsl_matrix_complex_set_identity(u);

        gsl_complex zero = gsl_complex_rect(0.0, 0.0);
        gsl_complex one  = gsl_complex_rect(1.0, 0.0);
/*
 *      Recuperiamo il livello di controllo
 */
        size_t lv_control = (size_t)param->arg[0];
/*
 *      Verifichiamo la legalità del livello di controllo
 */
        if (lv_control > dim_0 - 1)
        {
            fprintf(stderr, "ERRORE nell'invocazione del gate 'CEX'.\n"
                            "Il livello di controllo è superiore alla "
                            "dimensione del qudit di controllo.\n"
                            "ABORTING...\n\n");
            gsl_matrix_complex_free(u);
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *      Recuperiamo i livelli del target da scambiare
 */
        size_t lv_target_0 = (size_t)param->arg[1];
        size_t lv_target_1 = (size_t)param->arg[2];
/*
 *      Verifichiamo la legalità dei livelli da scambiare
 */
        if (lv_target_0 > dim_1 - 1 || lv_target_1 > dim_1 - 1)
        {
            fprintf(stderr, "ERRORE nell'invocazione del gate 'CEX'.\n"
                            "I livelli del target da scambiare sono superiori "
                            "alla dimensione del qudit target.\n"
                            "ABORTING...\n\n");
            gsl_matrix_complex_free(u);
            free_parametri_gate(param);
            free_gate_operator(&g_op);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        size_t indice_0 = lv_target_0 * dim_1 + lv_control;
        size_t indice_1 = lv_target_1 * dim_1 + lv_control;

        gsl_matrix_complex_set(u, indice_0, indice_0, zero);
        gsl_matrix_complex_set(u, indice_1, indice_1, zero);

        gsl_matrix_complex_set(u, indice_0, indice_1, one);
        gsl_matrix_complex_set(u, indice_1, indice_0, one);
    }
    else
    {
        fprintf(stderr, "ERRORE nell'invocazione del gate 'CEX'.\n"
                        "Gate invocato con target coincidente con al "
                        "control.\n"
                        "ABORTING...\n\n");
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }

/*
 *  Assicuriamoci che questa sia in SU(n)
 */
    u = normalizza(u);
/*
 *  Verifichiamo l'esito della normalizzazione
 */
    if (u == NULL)
    {
        free_parametri_gate(param);
        free_gate_operator(&g_op);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_op->val.unt = u;

#ifdef DEBUG
    fprintf(stdout, "\nOperatore gate 'CEX':\n");
    stampa_matrice(stdout, u);
    fprintf(stderr, "\n");
#endif

    return g_op;
}

//---------------------------------------------------------------------------//

/*
 *  Array che conterrà la definizione delle custom logic definite da utente
 */
gate_BLTN gate_builtin_definitions[N_BLTN] = 
{
    {"$$IDLE", imp_nullo        , 1, 1},
    {"$RZ"   , Z_rot            , 1, 3},
    {"$RP"   , P_rot            , 1, 4},
    {"X"     , X_generalizzato  , 1, 0},
    {"Z"     , Z_generalizzato  , 1, 0},
    {"H"     , H_generalizzato  , 1, 0},
    {"S"     , S_generalizzato  , 1, 0},
    {"CX"    , CX_generalizzato , 2, 0},
    {"CZ"    , CZ_generalizzato , 2, 0},
    {"CR"    , CR_generalizzato , 2, 1},
    {"CEX"   , CEX_generalizzato, 2, 3}
};

/*
 *  Numero di funzioni di custom logic definite. Sarà la dimensione
 *  dell'array 'custom_logic_definitions'
 */
size_t N_gate_builtin = N_BLTN;

//---------------------------------------------------------------------------//

/*
 *  Funzione che installa i gate built in all'interno della lista delle
 *  definizioni
 */
uint8_t install_BLTN(lista_definizioni_gate *lista_def)
{
    if (lista_def == NULL)
    {
        fprintf(stderr, "ERRORE nell'installazione delle definzioni dei "
                        "gate built in.\n"
                        "Lista definizioni NULLA.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    for (size_t i = 0; i < N_gate_builtin; ++i)
    {
        gate_BLTN tmp = gate_builtin_definitions[i];
/*
 *      Allochiamo la definizone del gate
 */
        gate *g = alloc_gate(tmp.n_qudit, tmp.n_arg);
/*
 *      Verifichiamo l'esito dell'allocazione
 */
        if (g == NULL)
        {
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }

        g->operatore.func_ptr = tmp.func_ptr;
        g->nome_gate = tmp.nome;
        g->type = BLTN;

        esito_inserimento ris = inserisci_definizione(lista_def, g);

/*
 *      Verifichiamo l'esito dell'inserimento
 */
        if (ris == RDEF)
        {
            fprintf(stderr, "ERRORE ridefinizione del gate built in %s\n"
                            "ABORTING...\n\n", g->nome_gate);
            return 1;
        }
        else if (ris == GERR)
        {
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    return 0;
}
