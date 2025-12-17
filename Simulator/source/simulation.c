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

#include "../include/simulation.h"
#include "../include/evolution.h"

/*
 *  File che conterra' le definizioni di tutte le funzioni e
 *  strutture dati necessarie alla simulazione dell'evoluzione dello stato
 *  del nostro sistema quantistico
 */

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per l'allocazione di una simulazione
 */
simulazione *alloc_simulazione()
{
    return (simulazione*)malloc(sizeof(simulazione));
}

//---------------------------------------------------------------------------//

/*
 *  Interfaccia per la deallocazione di una simulazione
 */
void free_simulazione(simulazione *sim)
{
    if (sim == NULL)
    {
        return;
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    if (sim->rho != NULL)
    {
        gsl_matrix_complex_free(sim->rho);
    }
    else
    {
        ;   /* Do Nothing */
    }

    if (sim->sch != NULL)
    {
        free_schedule(sim->sch);
    }
    else
    {
        ;   /* Do Nothing */
    }
    
    if (sim->sys != NULL)
    {
        free_sistema_composto(sim->sys);
        
        if (sim->sys == comp_sys)
        {
            comp_sys = NULL;
        }
        else
        {
            ;   /* Do Nothing */
        }

        sim->sys = NULL;
    }
    else
    {
        ;   /* Do Nothing */
    }

    free(sim);
}

//---------------------------------------------------------------------------//

/*
 *  Funzione che simula effettivamente l'evoluzione del sistema sotto
 *  l'influenza della perturbazione data dall'impulso passato
 */
uint8_t simula_impulso(impulsi_paralleli *imp_par,
                        gsl_matrix_complex *rho,
                        sistema_composto *sys,
                        output *out, 
                        double avviamento)
{
/*
 *  Durata dell'impulso
 */
    double T = 0.0;

    if (imp_par != NULL)
    {
    
        for (size_t i = 0; i < imp_par->dim; ++i)
        {
            if (imp_par->arr_impulsi[i]->T > T)
            {
                T = imp_par->arr_impulsi[i]->T;
            }
            else
            {
                ;   /* Do Nothing */
            }
        }
    }
    else
    {
        T = avviamento;
    }
    
/*
 *  Controlliamo se abbiamo un impulso nullo da simulare
 */
    if (T < 1e-18)
    {
        fprintf(stderr, "ATTENZIONE duranta impulso troppo piccola "
                        "o negativa.\n"
                        "Simulazione saltata.\n");
        return 0;
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Inizializziamo il passo temporale dopo cui voglio stampare in output la
 *  soluzione approssimata
 */
    double dt = T / (double)out->N;

/* 
 *  Spezziamo le condizioni iniziali complesse in parte reale
 *  e parte immaginaria
 */
    size_t dim  = sys->dim;
    size_t size = dim * dim;

    double ci[2 * size];

/*
 *  IMPORTANTE: Siccome poi utilizzeremo delle matrix view di questo array
 *              e' necesario generare le condizioni iniziali nell'ordine
 *              corretto. Ovvero, scorre la matrice rho prima per righe
 *              poi per colonne. In altre parole, si fissa la riga e si
 *              scorrono le colonne
 */
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            gsl_complex tmp = gsl_matrix_complex_get(rho, i, j);
            ci[(i * dim) + j]        = tmp.dat[0];
            ci[(i * dim) + j + size] = tmp.dat[1];
        }
    }

/*
 *  Inizializziamo l'ambiente richiesto dalle interfacce GSL per la
 *  risoluzione del nostro sistema di ODE
 */
    parametri_forzante parametri = {imp_par, sys};
    gsl_odeiv2_system s = {forzante, jacobiano, 2*size, &parametri};

/*
 *  Ciclo per l'approssimazione della soluzione nei vari intervalli
 *  richiesti
 */
    for (size_t j = 0; j < out->N; ++j)
    {
        double t_i = j * dt;        /* tempo iniziale del passo */
        double t_f = (j + 1) * dt;  /* tempo finale del passo */
        
/*
 *      In ordine:
 *          sistema
 *          metodo numerico, in questo caso il metodo predictor
 *          corrector (Adams-Bashforth) (Adams-Moulton)
 *          passo
 *          errore_assoluto
 *          errore_relativo
 *          peso_errore_funzione
 *          peso_errore_derivata
 */      
        double passo =  dt * 1e-5;

        gsl_odeiv2_driver *d_PC_ABAM =
            gsl_odeiv2_driver_alloc_standard_new(&s,
                gsl_odeiv2_step_msadams, passo, 1e-12, 1e-12, 1.0, 1.0);
        
        int ris = gsl_odeiv2_driver_apply(d_PC_ABAM, &t_i, t_f, ci);

/*
 *      Controlliamo l'esito dell'evoluzione
 */
        if (ris != GSL_SUCCESS)
        {
            if (ris == GSL_EBADFUNC)
            {
                fprintf(stderr, "Possibile richiamo dello jacobiano da "
                                "parte del solver\n\n");
            }
            fprintf(stderr, "ERRORE nell'evoluzione temporale del "
                            "sistema\n");
            fprintf(stderr, "CODICE ERRORE: %d\n\n", ris);
            fprintf(stderr, "ABORTING...\n");
            return 1;
        }

        if (out->output_simulazione_abilitato)
        {
/*
 *          Aggiorniamo la matrice di densità
 */
            for (size_t i = 0; i < dim; ++i)
            {
                for (size_t j = 0; j < dim; ++j)
                {
                    double tmp1 = ci[(i * dim) + j];
                    double tmp2 = ci[(i * dim) + j + size];
                    gsl_complex tmp3 = gsl_complex_rect(tmp1, tmp2);

                    gsl_matrix_complex_set(rho, i, j, tmp3);
                }
            }
/*
 *           Stampiamo su file i risultati
 */
            controllo_stampa_output_simulazione(out, t_f, rho);
        }
        else
        {
            ;   /* Do Nothing */
        }
            
        if (out->avanzamento_simulazione > 1)
        {
            fprintf(stdout, "\33[2K\r"
                            "Avanzamento: %.0f%s",
                            t_f / T * 100.0,
                            "%");
            fflush(stdout);
        }
        else
        {
            ;   /* Do Nothing */
        }
        
        gsl_odeiv2_driver_free(d_PC_ABAM);
    }

/*
 *  Aggiorniamo la matrice di densità
 */
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            double tmp1 = ci[(i * dim) + j];
            double tmp2 = ci[(i * dim) + j + size];
            gsl_complex tmp3 = gsl_complex_rect(tmp1, tmp2);

            gsl_matrix_complex_set(rho, i, j, tmp3);
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

/*
 *  Interfaccia primaria con il simulatore. Simula la simulazione passata.
 *  Il risultato di questa sarà salvato in rho all'interno della struttura
 *  dati sim passata. Ritorna 0 se tutto è andato a buon fine, un qualsiasi
 *  altro valore positivo se vi è stato un errore
 */
uint8_t simula(simulazione *sim)
{
/*
 *  Se l'output e' abilitato stampiamo le condizioni iniziali
 *  del problema
 */
    controllo_stampa_output_simulazione(sim->out, 0.0, sim->rho);
    
    schedule *sch = sim->sch;

    if (sch == NULL)
    {
        fprintf(stderr, "ERRORE nella simulazione, una delle schedule "
                        "fornite e' nulla.\n"
                        "ABORTING...\n\n");
        return 1;
    }
    else
    {
        ;   /* Do Nothing */
    }

    size_t n_imp_attuale = 1;
    size_t n_imp_totale = 0;
    
    if (sim->out->avanzamento_simulazione > 1)
    {
        n_imp_totale = conta_impulsi_schedule(sch);
    }
    else
    {
        ;   /* Do Nothing */
    }


/* 
 *  ciclo sulle seq della schedule
 */
    for (size_t j = 0; j < sch->dim; ++j) 
    {
        sequenza_impulsi *seq = sch->arr_sequenze[j];
            
        if (sch == NULL)
        {
            fprintf(stderr, "ERRORE nella simulazione, una delle sequenze "
                            "fornite e' nulla.\n"
                            "ABORTING...\n\n");
            return 1;
        }
        else
        {
            ;   /* Do Nothing */
        }
/*
 *          ciclo sugli impulsi della seq
 */
        for (size_t k = 0; k < seq->dim; ++k)
        {
            impulsi_paralleli *imp_par = seq->arr_impulsi_paralleli[k];

            if (sch == NULL)
            {
                fprintf(stderr, "ERRORE nella simulazione, uno degli "
                                "impulsi fornite e' nullo.\n"
                                "ABORTING...\n\n");
                return 1;
            }
            else
            {
                ;   /* Do Nothing */
            }

/*
 *          Simulazione di fireup. eseguita passando un puntatore nullo
 *          al simulatore al posto dell'impulso parallelo
 */
            if (sim->avviamento > 1e-6)
            {
                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "Simulazione fase di avviamento\n"
                                    "Avanzamento: 0%s", "%");
                    fprintf(stdout, "\33[2K\r\33[2K\r");
                    fflush(stdout);
                }
                else
                {
                    ;   /* Do Nothing */
                }

                uint8_t ris = 
                    simula_impulso(NULL, sim->rho, sim->sys,
                                    sim->out, sim->avviamento);
/*
 *              Controlliamo se c'e' stato un errore o meno
 */
                if (ris)
                {
                    return 1;
                }
                else
                {
                    ;   /* Do Nothing */
                }

                if (sim->out->avanzamento_simulazione > 1)
                {
                    fprintf(stdout, "\n");
                }
                else
                {
                    ;   /* Do Nothing */
                }
            }

            if (sim->out->avanzamento_simulazione > 1)
            {
                fprintf(stdout, "Simulazione impulso %ld/%ld\n"
                                "Avanzamento: 0%s",
                                n_imp_attuale,
                                n_imp_totale,
                                "%");
            }
            else
            {
                ;   /* Do Nothing */
            }
/*
 *          Simulazione dell'impulso parallelo
 */
            simula_impulso(imp_par, sim->rho, sim->sys, sim->out, 0.0);

            if (sim->out->avanzamento_simulazione > 1)
            {
                fprintf(stdout, "\33[2K\r");
                fflush(stdout);
            }
            else
            {
                ;   /* Do Nothing */
            }
                ;

            ++n_imp_attuale;
        }
    }

    return 0;
}
