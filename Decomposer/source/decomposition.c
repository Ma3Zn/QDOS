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

#include "../include/decomposition.h"

/*
 *  File contenente la definizione di tutte le funzioni per la decomposizione
 *  di una matrice U in rotazioni piane e lungo Z
 */

//---------------------------------------------------------------------------//

/*
 *  Moltiplicazione esplicita di U per la rotazione piana rot
 *
 *                          U * rot
 */
void molt_mat_rot_piana(rotazione_piana *rot,
                        gsl_matrix_complex *U)
{
    double cos_ = cos(rot->theta);
    double sin_ = sin(rot->theta);

    gsl_complex exp1 = gsl_complex_rect(0.0, rot->beta);
    gsl_complex exp2 = gsl_complex_rect(0.0, -1.0 * rot->beta);

    gsl_complex rot11 = gsl_complex_rect(cos_, 0.0);
    gsl_complex rot12 = gsl_complex_mul_real(gsl_complex_exp(exp2), -1.0 * sin_);
    gsl_complex rot21 = gsl_complex_mul_real(gsl_complex_exp(exp1), sin_);
    gsl_complex rot22 = gsl_complex_rect(cos_, 0.0);

/*
 *  Creiamo la matrice di rotazione piana
 */
    gsl_matrix_complex *tmp1 =
        gsl_matrix_complex_calloc(U->size1, U->size1);

    gsl_matrix_complex_set_identity(tmp1);  /* fondamentale */

    gsl_matrix_complex_set(tmp1, rot->livello[0],
                                 rot->livello[0],
                                 rot11);
        
    gsl_matrix_complex_set(tmp1, rot->livello[0],
                                 rot->livello[1],
                                 rot12);

    gsl_matrix_complex_set(tmp1, rot->livello[1],
                                 rot->livello[0],
                                 rot21);
        
    gsl_matrix_complex_set(tmp1, rot->livello[1],
                                 rot->livello[1],
                                 rot22);

/*
 *  Eseguiamo il prodotto U * rot. Per la struttura delle interfacce BLAS
 *  abbiamo bisogno di una copia della matrice X
 */

    gsl_matrix_complex *tmp2 =
        gsl_matrix_complex_calloc(U->size1, U->size1);

    gsl_matrix_complex_memcpy(tmp2, U);

    gsl_complex zero = gsl_complex_rect(0.0, 0.0);
    gsl_complex uno = gsl_complex_rect(1.0, 0.0);

    gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, uno, tmp2, tmp1, zero, U);

#ifdef DEBUG_DECOMPOSER
    stampa_matrice(stdout, tmp1);
#endif

    gsl_matrix_complex_free(tmp1);
    gsl_matrix_complex_free(tmp2);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Moltiplicazione esplicita di U per la rotazione Z rot
 *
 *                          U * rot
 */
void molt_mat_rot_Z(rotazione_Z *rot,
                    gsl_matrix_complex *U,
                    size_t r)
{
    gsl_complex Urr  = gsl_matrix_complex_get(U, r, r);
    gsl_complex Urr_ = gsl_matrix_complex_get(U, r+1, r+1);

    gsl_complex tmp1 = gsl_complex_polar(1.0, -1.0 * rot->phi);
    gsl_complex tmp2 = gsl_complex_polar(1.0,        rot->phi);

    gsl_matrix_complex_set(U, r, r, gsl_complex_mul(Urr, tmp1));
    gsl_matrix_complex_set(U, r+1, r+1, gsl_complex_mul(Urr_, tmp2));

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Decomposizione della matrice U in rotazioni piane. La decomposizione è
 *  distruttiva, i.e., U verrà modificata, in particolare U diventerà una
 *  matrice diagonale
 *
 *  La funzione torna 0 se la riallocazione è andata a buon fine, mentre un
 *  qualsiasi altro valore di ritorno indica il ferificarsi di un errore
 */
uint8_t decomponi_in_rotazioni_piane(gsl_matrix_complex *U,
                                        sequenza_rotazioni *seq,
                                        size_t *N_rot_eseguite)
{
/*
 *  Seguiremo lo schema proposto da Domenico D'Alessandro in "Introduction to
 *  Quantum Control and Dynamics"
 * 
 *  Inoltre bisogna tenere a mente che il numero massimo di rotazioni piane
 *  necessarie per la trasformazione è (n-1)*n/2 dove n è la dimensione della
 *  matrice unitaria fornita
 */

    size_t N_max_rot = ((U->size1 - 1u) * U->size1) / 2u;
    rotazione **rot = seq->rot;

/*
 *  TODO::  Riferimento alla pagina della tesi in cui faccio
 *          questi conti
 */

    size_t size = U->size1;     /* Dimensioni della matrice */
    size_t dim_eff = 0;         /* Numero effettivo di decomposizioni */

    for (size_t n = size; n > 0  ; --n)
    {
        size_t riga = n - 1;    /* Indice di riga */
    
        for (size_t j = riga; j > 0; --j)
        {
            size_t colonna = j - 1;     /* Indice di colonna */

/*
 *          Recuperiamo gli elementi di U coinvolti in questo fase di
 *          decomposizione
 */
            gsl_complex U21 = gsl_matrix_complex_get(U, riga, colonna);
            gsl_complex U22 = gsl_matrix_complex_get(U, riga, riga);

/*
 *          Verifichiamo la NON banalità della decomposizione
 *
 *          Se è uno zero in doppia precisione lo consideriamo come uno zero e
 *          dunque nessuna rotazione deve essere eseguita
 */
            if (gsl_complex_abs(U21) > 1e-12)
            {
/*  
 *               Controlliamo di avere spazio a sufficienza per inserire
 *               un'altra rotazione piana
 */ 
                if (dim_eff >= N_max_rot)
                {
                    fprintf(stderr, "ERRORE nella decomposizione della"
                                        " matrice fornita.\n"
                                        "Spazio allocato per le rotazioni"
                                        " rotazioni piane insufficiente.\n"
                                        "ABORTING...\n\n");
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }

/*
 *              Allochiamo la rotazione piana da eseguire
 */
                rot[dim_eff] = (rotazione*)alloc_rotazione(ID_ROT_PIANA);

                if (rot[dim_eff] == NULL)
                {
                    fprintf(stderr, "ERRORE nella decomposizione della"
                                        " matrice fornita.\n"
                                        "Spazio insufficiente per"
                                        " l'allocazione di una nuova"
                                        " rotazione piana.\n"
                                        "ABORTING...\n\n");
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }

/* 
 *              Inizializzazione livelli rotazione piana
 */
                rot[dim_eff]->ptr.piana->livello[0] = colonna;
                rot[dim_eff]->ptr.piana->livello[1] = riga;

                if (gsl_complex_abs(U22) > 1e-12)
                {
/*
 *                  Calcoliamo i due parametri della rotazione piana
 *                  (theta, beta)
 */
                    double U21_r = U21.dat[0];
                    double U21_i = U21.dat[1];
                    double U22_r = U22.dat[0];
                    double U22_i = U22.dat[1];
/* 
 *                  TODO::  Riferimento alla pagina della tesi in cui
 *                          faccio questi conti
 * 
 *                  TODO::  Verifiare che in nessun altro punto venga
 *                          utilizzato impropriamente gsl_complex_abs
 *                          al posto di gsl_complex_abs2
 */    
                    double A = (U21_r * U22_r + U21_i * U22_i) 
                                / gsl_complex_abs2(U21);
                    double B = (U21_r * U22_i - U21_i * U22_r) 
                                / gsl_complex_abs2(U21);
/* 
 *                  Inizializzazione rotazione piana
 */
                    rot[dim_eff]->ptr.piana->livello[0] = colonna;
                    rot[dim_eff]->ptr.piana->livello[1] = riga;
                    if (A == 0 && B != 0)
                    {
                        rot[dim_eff]->ptr.piana->theta = atan(1.0 / B);
                        rot[dim_eff]->ptr.piana->beta  = M_PI_2;
                    }
                    else if (B == 0 && A != 0)
                    {
                        rot[dim_eff]->ptr.piana->theta = atan(-1.0 / A);
                        rot[dim_eff]->ptr.piana->beta  = 0.0;
                    }
                    else
                    {
                        rot[dim_eff]->ptr.piana->beta  = atan(-1.0 * B / A);
                        rot[dim_eff]->ptr.piana->theta = 
                            atan(-1.0 / A * cos(rot[dim_eff]->ptr.piana->beta));
                    }
                }
                else
                {
/*  
 *                  Inizializzazione della rotazione piana
 */
                    rot[dim_eff]->ptr.piana->theta = M_PI_2;
                    rot[dim_eff]->ptr.piana->beta = 0.0;
                }

/*
 *              Implementiamo il prodotto matriciale. Visto che la matrice
 *              può avere grosse dimensioni mentre gli elementi che
 *              agiscono non banalmente sono quattro conviene esplicitare 
 *              il prodotto a mano.
 */ 
                molt_mat_rot_piana(rot[dim_eff]->ptr.piana, U);
/*  
 *              Siccome noi appliceheremo una rotazione piana tramite
 *              un'impulso lineare lungo Y sfasato di un angolo beta,
 *              l'angolo di rotazione va moltiplicato per 2. Infatti
 *  
 *                                  |   cos(o/2)    -sin(o/2)   |         
 *                      R_y(o) =    |                           |,
 *                                  |   sin(o/2)     cos(o/2)   |
 *  
 *              fino a questo punto abbiamo trovato o/2. Per calcolare la
 *              durata effettiva dell'impulso da simulare abbiamo bisogno di o.
 */ 
                rot[dim_eff]->ptr.piana->theta *= 2.0;
/*  
 *              Dobbiamo salvare l'inversa della rotazione piana, i.e., la
 *              sua aggiunta. Per rotazioni piane questo è semplice e basta
 *              cambiare o segno all'angolo theta o aggiungere una fase di
 *              pi.
 *  
 *              ATTENZIONE::    questo va fatto DOPO il prodotto tra U e la
 *                                      rotazione piana
 */ 
                if (rot[dim_eff]->ptr.piana->beta < 0)
                {
                    rot[dim_eff]->ptr.piana->beta += M_PI;
                }
                else if (rot[dim_eff]->ptr.piana->theta < 0)
                {
                    rot[dim_eff]->ptr.piana->theta *= -1.0;
                }
                else
                {
                    rot[dim_eff]->ptr.piana->beta += M_PI;
                }

/*  
 *              Aggiorniamo il numero di rotazioni piane in cui U viene
 *              effettivamente decomposta.
 */ 
                ++dim_eff;
            }
            else
            {
                ;    /* Nessuna decomposizione decessaria */
            }
        }
    }

/*
 *  Propaghiamo il numero di rotazioni eseguite
 */
    *N_rot_eseguite = dim_eff;

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Decomposizione della matrice diagonale ottenuta al termine di
 *                  "decomponi_in_rotazioni_piane"
 *  in una sequenza di rotazioni Z
 *
 *  La funzione torna 0 se la riallocazione è andata a buon fine, mentre un
 *  qualsiasi altro valore di ritorno indica il ferificarsi di un errore
 * 
 *  FIXME:: Può essere fatto in modi molto più efficienti
 */
uint8_t decomponi_in_rotazioni_Z(gsl_matrix_complex *U,
                                    sequenza_rotazioni *seq,
                                    size_t *N_rot_eseguite)
{
/*  ATTENZIONE::
 *
 *          Ricordiamo la forma di R_z(o)
 * 
 *                          |   e^(-i*o/2)      0       |
 *              R_z(o) =    |                           |
 *                          |       0       e^(i*o/2)   |
 * 
 *          La nostra procedura per l'applicazione di un impulso che effettui
 *          un R_z ha questo effetto:
 *
 *                          |   e^(i*o)         0       |
 *              $R_z(o) =   |                           |
 *                          |       0       e^(-i*o)    |
 * 
 *          Per la struttura dell'algoritmo è attualmente più conveniente avere
 *          rotazioni del tipo $R_z che del tipo R_z. Tieni a mente che non
 *          stai eseguendo degli effettivi R_z. Rimane che le due forme sono
 *          collegate da
 * 
 *                          R_z(o) = $R_z(-o/2)
 * 
 *          Quindi se vi è bisogno di simulare un R_z(o) basta simulare
 *          un $R_z(-o/2).
 * 
 *  Bisogna poi ricordare che queste vanno messe dopo le rotazioni piane già
 *  ricavate, dunque dobbiamo partire con un offset di *N_rot_eseguite.
 *
 *  Inoltre dobbimo eseguire al più n-1rotazioni Z, dove n è la dimensione
 *  della matrice unitaria fornita, per redendere l'identità la matrice
 *  diagonale passata.
 */
    size_t N_max_rot = U->size1 - 1;
    rotazione **rot = seq->rot;

/*
 *  TODO::  Riferimento alla pagina della tesi in cui faccio
 *          questi conti
 */

    size_t dim_eff = *N_rot_eseguite;

    for (size_t i = 0; i < N_max_rot; ++i)
    {
        gsl_complex Uii = gsl_matrix_complex_get(U, i, i);
        double phi = gsl_complex_arg(Uii);

/*
 *      Se NON abbiamo un uno sulla diagonale una decomposizione va eseguita
 */
        if (fabs(phi) > 1e-12)
        {
/*
 *          Allochiamo la rotazione Z da eseguire
 */
            rot[dim_eff] = (rotazione*)alloc_rotazione(ID_ROT_Z);

            if (rot[dim_eff] == NULL)
            {
                fprintf(stderr, "ERRORE nella decomposizione della"
                                    " matrice fornita.\n"
                                    "Spazio insufficiente per"
                                    " l'allocazione di una nuova"
                                    " rotazione Z.\n"
                                    "ABORTING...\n\n");
                return 1;
            }
            else
            {
                ;   /* Do Nothing */
            }

/*
 *          Inizializzazione rotazione Z in cui decomporre la matrice
 */
            rot[dim_eff]->ptr.Z->livello[0] = i;
            rot[dim_eff]->ptr.Z->livello[1] = i + 1;
            rot[dim_eff]->ptr.Z->phi = phi;

            molt_mat_rot_Z(rot[dim_eff]->ptr.Z, U, i);
            ++dim_eff;
        }
        else
        {
            ;   /* Nessuna decomposizione necessaria */
        }
    }

/*
 *  Propaghiamo il numero di rotazioni eseguite
 */
    *N_rot_eseguite = dim_eff;

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Controlla che la matrice U sia effettivamente unitaria speciale, i.e, ha
 *  determinante 1. Ritorna 0 se il determinante è 1 mentre ritorna 1
 *  altrimenti.
 * 
 *  ATTENZIONE::    si presuppone che U sia diagonale.
 * 
 *  TODO::  aggiungere riferimento alla tesi in cui si dimostra perchè
 *          questa procedura è lecita e ci ridà quello che ci serve
 *
 *  La funzione torna 0 se il controllo è andato a buon fine, mentre un
 *  qualsiasi altro valore di ritorno indica il verificarsi di un errore 
 */
uint8_t controlla_appartenenza_SU(gsl_matrix_complex *U)
{
    gsl_complex tmp = gsl_matrix_complex_get(U, U->size1-1, U->size1-1);

    double real = tmp.dat[0];
    double imag = tmp.dat[1];

    if (fabs(real - 1.0) < 1e-6 && fabs(imag) < 1e-6)
    {
        return 0;
    }
    else
    {
        fprintf(stderr, "ATTENZIONE nella decomposizione della matrice fornita.\n"
                        "La matrice NON è risultata unitaria specile.\n"
                        "Il modulo del determinante vale: %.6e\n"
                        "Possibili errori numerici nei risultati.\n\n",
                        sqrt(fabs(real)*fabs(real) + fabs(imag)*fabs(imag)));
        return 0;
    }
}

//---------------------------------------------------------------------------//

/*
 *  Interfaccia esterna da chiamare per la decomposizione di una matrice in SU.
 *  Un controllo per verificare che U sia effettivamente una matrice unitaria
 *  speciale viene eseguito, in caso non lo sia un messaggio di errore viene
 *  stampato su stderr. Se si verifica un'errore e la decomposizione NON 
 *  può essere terminata con successo allora un puntatore nullo viene ritornato
 */
sequenza_rotazioni *decomponi(gsl_matrix_complex *U)
{
    size_t size = U->size1;

/*
 *  Checking for a possible overflow
 */
    if (size > sqrt((double)UINT64_MAX))
    {
        fprintf(stderr, "ERRORE nelle dimensioni della matrice da decomporre.\n"
                        "Dimensione massima ammissibile: %ld\nABORTING...\n\n",
                        (size_t)( sqrt((double)UINT64_MAX) ));
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Massimo numero di rotazioni piane in cui U viene decomposta utilizzando
 *  il nostro algoritmo
 *
 *  La seguente esoressione in aritmetica intera è lecita essendo size sempre
 *  pari (size = 2^k per un qualche k)
 */
    size_t N_rot_piane = ((size - 1) * size) / 2;
/*
 *  Massimo numero di rotazioni Z in cui U viene decomposta utilizzando il
 *  nostro algoritmo
 */
    size_t N_rot_Z = size - 1;

/*
 *  Riserviamo lo spazio (massimo) necessario alla decomposizione di U
 */
    sequenza_rotazioni *seq = alloc_seq_rot(N_rot_piane + N_rot_Z);
/*
 *  Verifichiamo l'esito dell'allocazione di memoria
 */
    if (seq == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della sequenza di rotazioni"
                        " in cui decomporre la matrice U.\nABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Contiamo il numero effettivo di rotazioni eseguite
 */
    size_t N_rot_eseguite = 0u;
/*
 *  Generiamo la sequenza di rotazioni piane che deconpongono U e verifichiamo
 *  la generazione di possibili errori
 */
    if (decomponi_in_rotazioni_piane(U, seq, &N_rot_eseguite))
    {
        free_seq_rot(&seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Decomponiamo la parte restante di U in rotazioni Z e verifichiamo la
 *  generazione di errori
 * 
 *  ATTENZIONE::    Le rotazioni Z ottenute tramite questo algoritmo commutano
 *                  le une con le altre. Dunque una inversione del loro ordine
 *                  di esecuzione non è necessaria.
 */
    if(decomponi_in_rotazioni_Z(U, seq, &N_rot_eseguite))
    {
        free_seq_rot(&seq);
        return NULL;
    }

/*
 *  Verifichiamo che la decomposizione che abbiamo eseguito sia corretta.
 *  
 *  La funzione controlla che U sia effettivamente una matrice unitaria
 *  speciale verificando che questa sia la matrice identità. A questo punto
 *  della decomposizione basta verifiare che l'ultimo elemento diagonale
 *  sia 1.
 */
    if (controlla_appartenenza_SU(U))
    {
        free_seq_rot(&seq);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Se necessario ridimensioniamo la sequenze di rotazioni piane
 */
    if (N_rot_eseguite < seq->dim)
    {
/*
 *      Controlliamo il possibile fallimento della riallocazione della memoria.
 *      In tal caso si prosegue con la sequenza di dimensione originale.
 *      La funzione torna 0 se la riallocazione è andata a buon fine, mentre un
 *      qualsiasi altro valore di ritorno indica il ferificarsi di un errore
 */
        if (realloc_seq_rot(seq, N_rot_eseguite))
        {
            fprintf(stderr, "ATTENZIONE impossibile ridurre le dimensioni "
                            "della sequenza di rotazioni.\n"
                            "Proseguimento con la sequenza originale");
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
    
    return seq;
}
