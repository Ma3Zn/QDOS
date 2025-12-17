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

#ifndef PHYSICAL_SYSTEM_H
#define PHYSICAL_SYSTEM_H

#include "common.h"

/*
 *  Header File contenente le dichiarazioni di tutte le funzioni e le
 *  strutture dati necessarie alla descrizione del sistema fisico preso
 *  in esame.
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che contiene tutte le varibaili d'appoggio necessarie
 *  al simulatore per poter simulare l'errore di dephasing.
 *  
 *  La struttura è semplicemente composta da cinque matrici tutte della stessa
 *  dimensione dell'Hamiltoniana del sistema
 */
typedef struct 
{
/*
 *  La struttura è semplicemente composta da cinque matrici tutte della stessa
 *  dimensione dell'Hamiltoniana del sistema
 */
    gsl_matrix_complex *tmp1;
    gsl_matrix_complex *tmp2;
    gsl_matrix_complex *tmp3;
    gsl_matrix_complex *tmp4;
    gsl_matrix_complex *tmp5;
} spazio_di_lavoro_errore;


//---------------------------------------------------------------------------//

/*
 *  Struttura dati che conterrà le hamiltoniane dei vari qudit ed il tensore
 *  di connessione. Rappresenta il sistema fisico in considerazione da un punto
 *  di vista astratto in quanto l'hamiltoniana NON è formata e tutto è scritto
 *  in base canonica.
 * 
 *  HP: Il tensore di connessione è del secondo ordine. Ovvero è una matrice.
 * 
 *  Questa struttura dati è necessaria quando il programma viene invocato con
 *  l'opzione -J1, ovvero richiedendo che ci siano switch on/off delle
 *  connessioni tra i vari qudit. Se però le connessioni le si voglino lasciare
 *  sempre accese, con l'opzione -J0, allora questa struttura dati verrà
 *  semplicemente ignorata e la catena di chiamte invocherà direttamente
 *  l'opportuno inizializzatore per il sistema composto che però sarà fisso
 *  per tutta l'evoluzione temporale.
 */
typedef struct
{
/*
 *  Puntatori alle Hamiltoniane di ogni qubit
 */
    gsl_matrix_complex **hamiltoniana;
/*
 *  Tensore di connettività delle hamiltoniane
 */
    gsl_matrix_complex *J;
/*
 *  Vettore contennete le matrici di connettivitò dei livelli di ogni qudit
 *  del sistema
 */
    gsl_matrix_complex **M;
/*
 *  Matrice di connettività totale del sistema
 */
    gsl_matrix_complex *M_tot;
/*
 *  Puntatori agli operatori di errore
 */
    gsl_matrix_complex **operatore_errore;
/*
 *  Rateo di depasing per ogni qubit
 */
    double *rateo;
/*
 *  Numero di qudit nel sistema
 */
    size_t n_qudit;
/*
 *  Vettore contenente per ad ogni indice il numero di livelli di ogni qudit
 */
    size_t *n_livelli;
/*
 *  Dimensione del sistema completo
 */
    size_t dim;
} sistema_astratto;

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che costituirà il sistema composto. Questa è una delle
 *  strutture dati necessarie alle interfacce con il simulatore per poter
 *  approssimare numericamente l'evoluzione del sistema stesso.
 */
typedef struct 
{
/*
 *  Vettore contenente gli autovalori dell'hamiltoniana del sistema
 */
    gsl_vector *autovalori;
/*
 *  Matrice di connettività del sistema già ruotata sulla base degli
 *  autovettori dell'hamiltoniana
 */
    gsl_matrix_complex *M;
/*
 *  Vettore di matrici, ognuna delle quali rappresenta l'operatore di errore
 *  da simulare per il corrispondente qudit. Già ruotate sulla base
 *  degli autovettori dell'Hamiltoniana
 */
    gsl_matrix_complex **operatore_errore;
/*
 *  Vettore che contiene i ratei di dephasing associati ad ogni operatore
 *  d'errore
 */
    double *rateo;
/*
 *  Variabili d'appoggio per la simulazione dell'errore
 */
    spazio_di_lavoro_errore *sle;
/*
 *  Numero qudit del sistema
 */
    size_t n_qudit;
/*
 *  Dimensione del sistema
 */
    size_t dim;
}sistema_composto;

//---------------------------------------------------------------------------//

/*
 *  Istanze globali del sistema fisico
 */

/*
 *  Sistema astratto
 */
extern sistema_astratto *abs_sys;

/*
 *  Sistema composto
 */
extern sistema_composto *comp_sys;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione dello spazio di lavoro
 *  necessario alla simulazione dell'errore di dephasing
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione dello spazio di lavoro per la simulazione
 *  dell'errore di dephasing
 */
extern
spazio_di_lavoro_errore *alloc_spazio_di_lavoro_errore(size_t dim);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione dello spazio di lavore per la simulazione
 *  dell'errore di dephasing
 */
extern
void free_spazio_di_lavoro_errore(spazio_di_lavoro_errore *sle);

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione del sistema fisico
 *  studiato
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema sastratto NON affetto da errore
 */
extern
sistema_astratto *alloc_sistema_astratto_NO_ERR(size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema sastratto affetto da errore
 */
extern
sistema_astratto *alloc_sistema_astratto_ERR(size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la deallocazione di un sistema sastratto
 */
extern
void free_sistema_astratto(sistema_astratto *sys);

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di una struttura
 *  dati "sistema_composto"
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema composto NON affetto da errore
 */
extern
sistema_composto *alloc_sistema_composto_NO_ERR(size_t N, size_t dim);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un sistema composto affetto da errore
 */
extern
sistema_composto *alloc_sistema_composto_ERR(size_t N, size_t dim);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la reinizializzazione di un sistema composto
 */
extern
void reset_sistema_composto(sistema_composto *sys);

/*
 *  Funzione per la deallocazione di un sistema composto
 */
extern
void free_sistema_composto(sistema_composto *sys);

//---------------------------------------------------------------------------//

/*
 *  Funzione che, passate tutte le informazioni di input necessarie attraverso
 *  la struttura dati "input", alloca e ritorna un opportuno "sistema_astratto"
 * 
 *  Il parametro 'qudit_flag' ci dirà se la dimensione ed il numero di ogni
 *  qudit è stato definito all'interno di un circuito fornito come input
 *  oppure se viene passato tramite file. Viene inserito questo flag per
 *  ragioni di retrocompatibilità, i.e., per permettere all' utente di svolgere
 *  simulazioni fornendo direttamente sequenze di impulsi/rotazioni o unitarie
 *  generiche.
 */
extern
sistema_astratto *init_sistema_astratto(void *in_ptr);

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiunge il termine i-esimo all'hamiltoniana del sistema.
 *  Tale termine è semplicemente:
 *              I x I x .... x I x H_i x I x .... x I x I
 */
extern
uint8_t aggiungi_termine(gsl_matrix_complex *hamiltoniana,
                            sistema_astratto *sys,
                            size_t *idx_qudit,
                            size_t idx,
                            size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiunge il termine di connessione (se necessario)
 *  all'hamiltoniana. Questo ha la forma:
 *  J_ij * [ I  x .... x I x Sz_i x I x .... x I x Sz_j x I x .... x I ]
 * 
 *  HP [IMPORTANTISSIMA]
 *          supponiamo che ogni qudit sia direttamente collegabile con ogni altro
 *          qudit. Se proviamo a fare un'operazione tra due qudit il cui termine
 *          di connessione è nullo (e il meccanismo di switch on/off delle
 *          connessioni è abilitato) anzichè realizzare un'opportuna sequenza di
 *          swap (che andrebbe fatta al livello della traduzione da istruzione a
 *          operazione all'interno di 'parser_interface') andiamo ad abortire
 *          l'esecuzione del programma
 * 
 *  TODO:: Rimuovere l'HP sopra [FONDAMENTALE] 
 * 
 *  indici_qudit_connessione --> gli indici dei qudit da connettere
 *  indici_qudit --> gli indici dei qudit che compongono il sistema composto
 *  N --> numero dei qudit totali che compongono il sistema composto
 */
extern
uint8_t aggiungi_termine_connesso(gsl_matrix_complex *hamiltoniana,
                                    sistema_astratto *sys,
                                    size_t indici_qudit_connessione[2],
                                    size_t *indici_qudit,
                                    size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione che costruisce la matrice di connettività opportuna una volta
 *  che forniti gli indici dei qudit coinvolti
 */
extern
uint8_t costruisci_matrice_connettivita(gsl_matrix_complex *M,
                                        sistema_astratto *sys,
                                        size_t *indici_qudit,
                                        size_t n_qudit);

//---------------------------------------------------------------------------//

/*
 *  Funzione che aggiunge un termine alla matrice di connessione
 */
extern
uint8_t aggingi_termine_matrice_connettivita(gsl_matrix_complex *ris,
                                                sistema_astratto *sys,
                                                size_t *idx_qudit,
                                                size_t idx,
                                                size_t N);

//---------------------------------------------------------------------------//

/*
 *  Funzione che verfica l'ordinamento dei livelli in corrispondenza agli
 *  autovalori della matrice di connessione.
 * 
 *  Ritorna 1 se i livelli sono ordinati in modo consistente con gli
 *  autovalori, 0 altrimenti
 * 
 *  ATTENZIONE::    Questa funione al momento lavora solo con matrici di
 *                  connessione formate da operatori di spin (Sx/Sy).
 * 
 *  HP::    Lavoriamo con sistemi di spin
 * 
 *  TODO::  Rimuovere l'HP sopra
 */
extern
uint8_t test_ordine_livelli(gsl_matrix_complex *M, size_t lv_0, size_t lv_1);

#endif