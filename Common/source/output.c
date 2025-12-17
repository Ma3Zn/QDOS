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

#include "../include/output.h"

#include "../../Common/include/global.h"

#include "../../Simulator/include/measure.h"

/*
 *  File contenente le definizioni di tutte le funzioni per
 *  la gestione dell'output su file necessario al nostro programma
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione della struttura di output per il simulatore
 */
output *alloc_output()
{
    return (output*)malloc(sizeof(output));
}

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione della struttura di output per il simulatore
 */
void free_output(output *out)
{
    if (out == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (out->f_out_rho_finale != NULL)
    {
        fclose(out->f_out_rho_finale);
        out->f_out_rho_finale = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (out->f_out_rho != NULL)
    {
        fclose(out->f_out_rho);
        out->f_out_rho = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (out->f_out_t)
    {
        fclose(out->f_out_t);
        out->f_out_t = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (out->f_out_sch != NULL)
    {
        fclose(out->f_out_sch);
        out->f_out_sch = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (out->f_out_rot != NULL)
    {
        fclose(out->f_out_rot);
        out->f_out_rot = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(out);

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la matrice [M]
 */
void stampa_matrice(FILE *F, const gsl_matrix_complex *M)
{
    size_t size1 = M->size1;
    size_t size2 = M->size2;

#ifndef DEBUG
    for (size_t i = 0; i < size1; ++i)
    {
        for (size_t j = 0; j < size2; ++j)
        {
            gsl_complex tmp = gsl_matrix_complex_get(M, i, j);
            fprintf(F, "%.18e ", tmp.dat[0]);
        }

        for (size_t j = 0; j < size2; ++j)
        {
            gsl_complex tmp = gsl_matrix_complex_get(M, i, j);
            fprintf(F, "%.18e ", tmp.dat[1]);
        }
        fprintf(F, "\n");
    }
#else
    for (size_t i = 0; i < size1; ++i)
    {
        for (size_t j = 0; j < size2; ++j)
        {
            gsl_complex tmp = gsl_matrix_complex_get(M, i, j);
            fprintf(F, "%.4f ", tmp.dat[0]);
        }

        fprintf(F, "\t");

        for (size_t j = 0; j < size2; ++j)
        {
            gsl_complex tmp = gsl_matrix_complex_get(M, i, j);
            fprintf(F, "%.4f ", tmp.dat[1]);
        }
        fprintf(F, "\n");
    }
#endif
    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la matrice [M] in formato binario
 */
void stampa_matrice_bin(FILE *F, const gsl_matrix_complex *M)
{
    gsl_matrix_complex_fwrite(F, M);
    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in quale formato stampare l'evoluzione della
 *  simulazione
 */
void controllo_stampa_output_simulazione(output *out,
                                            const double t,
                                            const gsl_matrix_complex *M)
{
    if (out->output_simulazione_abilitato == 1)
    {
        if (out->f_out_rho == NULL || out->f_out_t == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare l'evoluzione "
                            "della simulazione.\n"
                            "Problema con l'apertura dei file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_simulazione_abilitato = 0;
        }
        else
        {
            uint8_t ris = fwrite(&t, sizeof(double), 1, out->f_out_t);

            if (ris != 1)
            {
                fprintf(stderr, "ERRORE nella stampa su file del tempo attuale"
                                ".\n"
                                "ABORTING OUTPUT...\n\n");
                out->output_simulazione_abilitato = 0;
                return;
            }
            else
            {
                ; /* Do Nothing */
            }

            stampa_matrice_bin(out->f_out_rho, M);
        }
    }
    else if (out->output_simulazione_abilitato == 2)
    {
        if (out->f_out_rho == NULL || out->f_out_t == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare l'evoluzione "
                            "della simulazione.\n"
                            "Problema con l'apertura dei file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_simulazione_abilitato = 0;
        }
        else
        {
            fprintf(out->f_out_t, "%.18f\n", t);
            stampa_matrice(out->f_out_rho, M);
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in quale formato stampare la soluzione finale
 *  approssimata dal simulatore
 */
void controllo_stampa_soluzione_finale(output *out,
                                        const gsl_matrix_complex *M)
{
    if (out->output_soluzione_finale_abilitato == 1)
    {
        if (out->f_out_rho_finale == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare l'evoluzione "
                            "della simulazione.\n"
                            "Problema con l'apertura dei file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_soluzione_finale_abilitato = 0;
        }
        else
        {
            stampa_matrice_bin(out->f_out_rho_finale, M);
        }
    }
    else if (out->output_soluzione_finale_abilitato == 2)
    {
        if (out->f_out_rho == NULL || out->f_out_t == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare l'evoluzione "
                            "della simulazione.\n"
                            "Problema con l'apertura dei file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_soluzione_finale_abilitato = 0;
        }
        else
        {
            stampa_matrice(out->f_out_rho_finale, M);
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la rotazione Z [rot]]
 */
void stampa_rotazione_Z(FILE *F, const rotazione_Z *rot)
{
    fprintf(F, "{ livello 0: %ld, livello 1: %ld, angolo: %f }\n",
                rot->livello[0], rot->livello[1], rot->phi);
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la rotazione piana [rot]]
 */
void stampa_rotazione_piana(FILE *F, const rotazione_piana *rot)
{
    fprintf(F, "{ livello 0: %ld, livello 1: %ld, angolo: %f, fase: %f }\n",
                rot->livello[0], rot->livello[1], rot->theta, rot->beta);
    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la seqeunza di rotazioni [seq]
 */
void stampa_sequenza_rotazioni(FILE *F, const sequenza_rotazioni *seq)
{
    fprintf(F, "Inizio stampa sequenza rotazioni.\n");

    for (size_t i = 0; i < seq->dim; ++i)
    {
        switch (seq->rot[i]->type)
        {
        case ID_ROT_PIANA:
            fprintf(F, "Rotazione piana:\n");
            stampa_rotazione_piana(F, seq->rot[i]->ptr.piana);
            fprintf(F, "\n");

            break;
        
        case ID_ROT_Z:
            fprintf(F, "Rotazione Z:\n");
            stampa_rotazione_Z(F, seq->rot[i]->ptr.Z);
            fprintf(F, "\n");
            
            break;

        default:
            fprintf(stderr, "ERRORE: ID rotazione sconosciuto.\n"
                            "ID: %d\n\n"
                            "ABORTING OUTPUT...\n\n", seq->rot[i]->type);
            return;
        }
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la seqeunza di rotazioni [seq] in
 *  formato binario direttamente leggibile dal simulatore
 */
uint8_t stampa_sequenza_rotazioni_bin(FILE *F,
                                        const sequenza_rotazioni *seq,
                                        const size_t indici[2])
{
/*
 *  Stampiamo gli indici di connnessione dei qudit 
 */
    uint8_t ris = fwrite(indici, sizeof(size_t), 2, F);

    if (ris != 2)
    {
        fprintf(stderr, "ERRORE nella scrittura su file degli attuali livelli di "
                        "connessione dei qudit.\n"
                        "ABORTING OUTPUT...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t N_rot;

/*
 *  Stampiamo il numero totale di rotazioni
 */
    N_rot = seq->dim;

    ris = fwrite(&N_rot, sizeof(size_t), 1, F);

    if (ris != 1)
    {
        fprintf(stderr, "ERRORE nella scrittura su file della sequenza "
                        "di rotazioni in formato binario.\n"
                        "ABORTING OUTPUT...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Stampa delle rotazioni
 */
    for (size_t i = 0; i < seq->dim; ++i)
    {
        rotazione *rot = seq->rot[i];

        switch (rot->type)
        {
            case ID_ROT_PIANA:
                ris = 0;

                ris += fwrite(&(rot->type),     sizeof(size_t), 1, F);
                ris += fwrite(rot->ptr.piana->livello,  sizeof(size_t), 2, F);
                ris += fwrite(&(rot->ptr.piana->theta), sizeof(double), 1, F);
                ris += fwrite(&(rot->ptr.piana->beta),  sizeof(double), 1, F);

                if (ris != 5)
                {
                    fprintf(stderr, "ERRORE nella scrittura su file della "
                                    "sequenza di rotazioni in formato "
                                    "binario.\n"
                                    "ABORTING OUTPUT...\n\n");
                    return 1;
                 }
                else
                {
                    ;   /* Do Nothing */
                }

                break;

            case ID_ROT_Z:
                ris = 0;

                ris += fwrite(&(rot->type), sizeof(size_t), 1, F);
                ris += fwrite(rot->ptr.Z->livello,  sizeof(size_t), 2, F);
                ris += fwrite(&(rot->ptr.Z->phi),   sizeof(double), 1, F);

                if (ris != 4)
                {
                    fprintf(stderr, "ERRORE nella scrittura su file della "
                                    "sequenza di rotazioni in formato "
                                    "binario.\n"
                                    "ABORTING OUTPUT...\n\n");
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }

                break;

            default:
                fprintf(stderr, "ERRORE: ID rotazione sconosciuto.\n"
                                "ID: %d\n\n"
                                "ABORTING OUTPUT...\n\n", rot->type);
                return 1;
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in che formato stampare la seqeunza di
 *  rotazioni [seq]
 */
void controllo_output_rotazioni(output *out,
                                const sequenza_rotazioni *seq,
                                const size_t indici[2])
{
    if (out->output_rotazioni_abilitato == 1)
    {
        if (out->f_out_sch == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare la sequenza "
                            "di rotazioni.\n"
                            "Problema con l'apertura del file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_rotazioni_abilitato = 0;
        }
        else
        {
            if (stampa_sequenza_rotazioni_bin(out->f_out_rot, seq, indici))
            {
                out->output_rotazioni_abilitato = 0;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
    }
    else if (out->output_rotazioni_abilitato == 2)
    {
        if (out->f_out_rot == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare la sequenza "
                            "di rotazioni.\n"
                            "Problema con l'apertura del file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_rotazioni_abilitato = 0;
        }
        else
        {
            stampa_sequenza_rotazioni(out->f_out_rot, seq);
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che legge dal file [F] una seqeunza di rotazioni, la lettura
 *  degli indici di connessione spetta al chiamamte che così ha modo di
 *  sapere quante schedule differenti sono presenti sul file
 */
sequenza_rotazioni *leggi_sequenza_rotazioni(FILE *F)
{
/*
 *  Numero di rotazioni da eseguire
 */
    size_t N_rot = 0;

/*
 *  Leggiamo il numero di rotazioni da eseguire
 */
    uint8_t ris = 0;
    ris += fread(&N_rot, sizeof(size_t), 1, F);
/*
 *  Verifichiamo la lettura
 */
    if (ris != 1)
    {
        fprintf(stderr, "ERRORE nella lettura del numero di rotazioni.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo una sequenza di rotazioni opportuna
 */
    sequenza_rotazioni *seq = alloc_seq_rot(N_rot);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (seq == NULL)
    {
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Leggiamo le rotazioni
 */
    for (size_t i = 0; i < N_rot; ++i)
    {
        size_t id_rot = 2u;
/*
 *      Leggiamo l'id della rotazione attuale
 */
        ris = fread(&(id_rot), sizeof(size_t), 1, F);

/*
 *  Verifichiamo la lettura dell'id
 */
        if (ris != 1)
        {
            fprintf(stderr, "ERRORE nella lettura dell'ID di una rotazione.\n"
                            "ABORTING...\n\n");
            free_seq_rot(&seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        rotazione *rot = alloc_rotazione(id_rot);
        if (rot == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria di una "
                            "rotazione Z.\n"
                            "ABORTING...\n\n");
            free_seq_rot(&seq);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        seq->rot[i] = rot;

        ris = 0;

        switch (id_rot)
        {
        case ID_ROT_PIANA:
/*  
 *          Leggiamo la rotazione
 */
            ris += fread(rot->ptr.piana->livello,  sizeof(size_t), 2, F);
            ris += fread(&(rot->ptr.piana->theta), sizeof(double), 1, F);
            ris += fread(&(rot->ptr.piana->beta),  sizeof(double), 1, F);

/*  
 *          Verifichiamo l'esito della lettura
 */ 
            if (ris != 4)
            {
                fprintf(stderr, "ERRORE nella lettura di una rotazione "
                                "piana.\n"
                                "ABORTING...\n\n");
                free_seq_rot(&seq);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
            break;
        
        case ID_ROT_Z:
/*  
 *          Leggiamo la rotazione
 */ 
            ris += fread(rot->ptr.Z->livello, sizeof(size_t), 2, F);
            ris += fread(&(rot->ptr.Z->phi),  sizeof(double), 1, F);
/*  
 *          Verifichiamo l'esito della lettura
 */ 
            if (ris != 3)
            {
                fprintf(stderr, "ERRORE nella lettura di una rotazione Z.\n"
                                "ABORTING...\n\n");
                free_seq_rot(&seq);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }
            break;

        default:
                fprintf(stderr, "ERRORE, ID rotazione sconosciuto.\n"
                                "ID: %d\n\n"
                                "ABORTING OUTPUT...\n\n", seq->rot[i]->type);
                free_seq_rot(&seq);
                return NULL;
        }
    }

    return seq;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] l'impulso [imp]
 */
void stampa_impulso(FILE *F, const impulso *imp)
{
    fprintf(F, "(lv_0: %ld, lv_1: %ld, tetha: %f, gamma: %f, omega: %f, T: %f)\n",
                imp->livelli[0], imp->livelli[1], imp->theta, imp->gamma,
                imp->omega, imp->T);
    
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] l'impulso parallelo [imp]
 */
void stampa_impulsi_paralleli(FILE *F, const impulsi_paralleli *imp)
{
    fprintf(F, "{{\n");
    
    for (size_t i = 0; i < imp->dim; ++i)
    {
        stampa_impulso(F, imp->arr_impulsi[i]);
    }

    fprintf(F,"}}\n");
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la sequenza di impulsi [seq]
 */
void stampa_sequenza_impulsi(FILE *F, const sequenza_impulsi *seq)
{
    fprintf(F, "[[[\n");
    
    for (size_t i = 0; i < seq->dim; ++i)
    {
        stampa_impulsi_paralleli(F, seq->arr_impulsi_paralleli[i]);
    }
    
    fprintf(F, "]]]\n");
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la schedule [sch]
 */
void stampa_schedule(FILE *F, const schedule *sch)
{
    if (F == NULL || sch == NULL)
    {
        return;
    }
    
    for (size_t i = 0; i < sch->dim; ++i)
    {
        stampa_sequenza_impulsi(F, sch->arr_sequenze[i]);
        fprintf(F, "\n");
    }
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che stampa sullo stream [F] la schedule [sch] in formato binario
 */
uint8_t stampa_schedule_bin(FILE *F, const schedule *sch)
{
/*
 *  Calcoliamo e stampiamo il numero totale di impulsi paralleli da stampare
 */
    size_t N_impulsi_paralleli = 0;

    for (size_t i = 0; i < sch->dim; ++i)
    {
        N_impulsi_paralleli += sch->arr_sequenze[i]->dim;
    }

    uint8_t ris = fwrite(&N_impulsi_paralleli, sizeof(size_t), 1, F);

    if (ris != 1)
    {
        fprintf(stderr, "ERRORE nella scrittura su file della schedule "
                        "in formato binario.\n"
                        "ABORTING OUTPUT...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Stampiamo uno ad uno tutti gli impulsi paralleli nel formato:
 *      - N_impulsi
 *      - impulso1
 *      -   ....
 *      - impulsoN
 */
    for (size_t j = 0; j < sch->dim; ++j)
    {
        for (size_t i = 0; i < sch->arr_sequenze[j]->dim; ++i)
        {
            impulsi_paralleli *imp_par =
                sch->arr_sequenze[j]->arr_impulsi_paralleli[i];

            ris = fwrite(&(imp_par->dim), sizeof(size_t), 1, F);

            if (ris != 1)
            {
                fprintf(stderr, "ERRORE nella scrittura su file della "
                                "schedule in formato binario.\n"
                                "ABORTING OUTPUT...\n\n");
                return 1;
            }
            else
            {
                ;   /* Do Nothing */
            }

            for (size_t j = 0; j < imp_par->dim; ++j)
            {
                impulso *imp = imp_par->arr_impulsi[j];

                ris = 0;

                ris += fwrite(imp->livelli,  sizeof(size_t), 2, F);
                ris += fwrite(&(imp->theta), sizeof(double), 1, F);
                ris += fwrite(&(imp->gamma), sizeof(double), 1, F);


                if (imp->modulazione == modulazione_impulso_hs)
                {
                    size_t id = MOD_IMP_HS;
                    ris += fwrite(&(id), sizeof(size_t), 1, F);
                }
                else if (imp->modulazione == modulazione_impulso_gauss)
                {
                    size_t id = MOD_IMP_GAUSS;
                    ris += fwrite(&(id), sizeof(size_t), 1, F);
                }
                else
                {
                    size_t id = MOD_IMP_ERR;
                    ris += fwrite(&(id), sizeof(size_t), 1, F);  
                }

                if (ris != 5)
                {
                    fprintf(stderr, "ERRORE nella scrittura su file della "
                                    "schedule in formato binario.\n"
                                    "ABORTING OUTPUT...\n\n");
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }
            }
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che controlla se e in che formato stampare la schedule [sch]
 */
void controllo_output_schedule(output *out, const schedule *sch)
{
    if (out->output_schedule_abilitato == 1)
    {
        if (out->f_out_sch == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare la schedule.\n"
                            "Problema con l'apertura del file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_schedule_abilitato = 0;
        }
        else
        {
            if (stampa_schedule_bin(out->f_out_sch, sch))
            {
                out->output_schedule_abilitato = 0;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
    }
    else if (out->output_schedule_abilitato == 2)
    {
        if (out->f_out_sch == NULL)
        {
            fprintf(stderr, "ATTENZIONE impossibile stampare la schedule.\n"
                            "Problema con l'apertura del file di output.\n"
                            "ABORTING OUTPUT...\n\n");
            out->output_schedule_abilitato = 0;
        }
        else
        {
            stampa_schedule(out->f_out_sch, sch);
        }
    }
    else
    {
        ;   /* Do Nothing */
    }

    return;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che legge dal file [F] una schedule, la lettura degli indici
 *  di connessione spetta al chiamamte che così ha modo di sapere quante
 *  schedule differenti sono presenti sul file
 */
schedule *leggi_schedule(FILE *F, gsl_matrix_complex *M, gsl_vector *aut)
{
/*
 *  Leggiamo il numero di impulsi paralleli da recuperare
 */
    size_t N_imp_par = 0;
    uint8_t ris = fread(&N_imp_par, sizeof(size_t), 1, F);

/*
 *  Verifichiamo gli esiti delle letture
 */
    if (ris != 1)
    {
        fprintf(stderr, "ERRORE nella lettura di una schedule da file.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Allochiamo schedule
 */
    schedule *sch = alloc_schedule(1);
/*
 *  Verifichiamo l'allocazione
 */
    if (sch == NULL) 
    {
        fprintf(stderr, "ERRORE nell'allocazione della schedule.\n"
                        "ABORTING...\n\n");
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Allochiamo un opportuna seqeunza di impulsi
 */
    sequenza_impulsi *seq = alloc_sequenza_impulsi(N_imp_par);
/*
 *  Verifichiamo l'allocazione
 */
    if (seq == NULL)
    {
        fprintf(stderr, "ERRORE nell'allocazione della sequenza di impulsi.\n"
                        "ABORTING...\n\n");

        free_schedule(sch);
        return NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Set-up della schedule
 */
    sch->arr_sequenze[0] = seq;

/*
 *  Leggiamo gli impulsi da applicare e riempiamo la schedule
 */
    for (size_t i = 0; i < N_imp_par; ++i)
    {
/*
 *      Leggiamo il numero di impulsi che compongono l'impulso parallelo
 *      attuale
 */
        size_t N_imp = 0;
        ris = fread(&N_imp, sizeof(size_t), 1, F);
/*
 *      Verifichiamo la lettura
 */
        if (ris != 1)
        {
            fprintf(stderr, "ERRORE nella lettura della dimensione "
                            "dell'impulso parallelo.\n"
                            "ABORTING...\n\n");
            free_schedule(sch);
            return NULL;
        }

/*
 *      Allochiamo un impulso parallelo delle dimensioni opportune
 */
        impulsi_paralleli *imp_par = alloc_impulsi_paralleli(N_imp);
/*
 *      Verifichiamo l'allocazione
 */
        if (imp_par == NULL)
        {
            fprintf(stderr, "ERRORE nell'allocazione della memoria di un "
                            "impulso parallelo durante la lettura di una "
                            "schedule da file.\n"
                            "ABORTING...\n\n");
            free_schedule(sch);
            return NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        seq->arr_impulsi_paralleli[i] = imp_par;

        imp_par->stato = NON_SIMULATO;

/*
 *      Leggiamo gli impulsi
 */
        for (size_t j = 0; j < N_imp; ++j)
        {
            impulso *imp = alloc_impulso();

            if (imp == NULL)
            {
                fprintf(stderr, "ERRORE nell'allocazione della memoria "
                                "di un impulso durante la "
                                "lettura di una schedule da file.\n"
                                "ABORTING...\n\n");
                free_schedule(sch);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }

            imp_par->arr_impulsi[j] = imp;

            ris = 0;

            size_t cod_mod = MOD_IMP_ERR;

            ris += fread(imp->livelli,  sizeof(size_t), 2, F);
            ris += fread(&(imp->theta), sizeof(double), 1, F);
            ris += fread(&(imp->gamma), sizeof(double), 1, F);
            ris += fread(&cod_mod,      sizeof(size_t), 1, F);

/*
 *          Verifichiamo l'esito delle letture
 */
            if (ris != 5)
            {
                fprintf(stderr, "ERRORE nella lettura dei parametri di "
                                "un impulso.\n"
                                "ABORTING...\n\n");
                free_schedule(sch);
                return NULL;
            }
            else
            {
                ;   /* Do Nothing */
            }

/*
 *          Assegniamo l'opportuna funzione di modulazione
 */
            if (cod_mod == MOD_IMP_HS)
            {
                imp->modulazione = modulazione_impulso_hs;
            }
            else if (cod_mod == MOD_IMP_GAUSS)
            {
                imp->modulazione = modulazione_impulso_gauss;
            }
            else
            {
                fprintf(stderr, "ATTENZIONE codice funzione di modulazione "
                                "nel file di input sconosciuto.\n"
                                "Automatica impostazione della funzione di "
                                "modulazione gaussiana.\n"
                                "ABORTING...\n\n");
            }
/*
 *          Controlliamo ordine livelli
 */
            if (imp->livelli[0] > imp->livelli[1])
            {
                size_t tmp = imp->livelli[1];

                imp->livelli[1] = imp->livelli[0];
                imp->livelli[0] = tmp;
            }
            else
            {
                ;   /* Do Nothing */
            }
/*
 *          Impostiamo la frequenza dell'impulso
 */
            imp->omega = gsl_vector_get(aut, imp->livelli[1]) 
                       - gsl_vector_get(aut, imp->livelli[0]);
/*
 *          Impostiamo la durata dell'impulso passato
 */
            imposta_durata_impulso(M, imp);
        }
    }

    return sch;
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che inizializza la variabile di output passata con i valori
 *  impostati dal parser delle opzioni
 */
uint8_t init_output(output *out)
{
    out->output_soluzione_finale_abilitato  = out_rho_final;
    out->output_simulazione_abilitato       = out_rho_simulation;
    out->output_rotazioni_abilitato         = out_rotation;
    out->output_schedule_abilitato          = out_schedule;
    out->avanzamento_simulazione            = simulation_progress;
    out->simulazione_abilitata              = simulation_enabled;
    out->N                                  = freq_output;

    char *f_out_rho_finale = concatena(out_dir, "rho_finale.out");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_out_rho_finale == NULL)
    {
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_out_rot = concatena(out_dir, "rot.out");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_out_rot == NULL)
    {
        free(f_out_rho_finale);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_out_sch = concatena(out_dir, "sch.out");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_out_sch == NULL)
    {
        free(f_out_rho_finale);
        free(f_out_rot);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_out_rho = concatena(out_dir, "rho.out");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_out_rho == NULL)
    {
        free(f_out_rho_finale);
        free(f_out_rot);
        free(f_out_sch);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }
    char *f_out_t = concatena(out_dir, "t.out");
/*
 *  Verifichiamo l'esito dell'operazione
 */
    if (f_out_t == NULL)
    {
        free(f_out_rho_finale);
        free(f_out_rot);
        free(f_out_sch);
        free(f_out_rho);
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    out->f_out_rho_finale   = fopen(f_out_rho_finale, "w");
    out->f_out_rot          = fopen(f_out_rot, "w");
    out->f_out_sch          = fopen(f_out_sch, "w");
    out->f_out_rho          = fopen(f_out_rho, "w");
    out->f_out_t            = fopen(f_out_t,   "w");
/*
 *  Verifichiamo gli esiti delle aperture
 */
    if (    out->f_out_rho_finale   == NULL 
        ||  out->f_out_rot          == NULL
        ||  out->f_out_sch          == NULL
        ||  out->f_out_rho          == NULL
        ||  out->f_out_t            == NULL)
    {
        fprintf(stderr, "ERRORE nell'apertura dei file di output.\n"
                        "ABORTING...\n\n");

        free(f_out_rho_finale);
        free(f_out_rot);
        free(f_out_sch);
        free(f_out_rho);
        free(f_out_t);

        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(f_out_rho_finale);
    free(f_out_rot);
    free(f_out_sch);
    free(f_out_rho);
    free(f_out_t);

    return 0;
}
