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

#ifndef PARSER_INTERFACES_H
#define PARSER_INTERFACES_H

#include "../../Common/include/common.h"
#include "../../Common/include/lib.h"

#include "../../Decomposer/include/decomposition.h"

#include "../../Scheduler/include/scheduling.h"

/*
 *  Header file contenente tutte le dichiarazioni dei tipi di dati e delle
 *  funzioni necessarie alla creazione di sequenze di operazioni simulabili
 *  dal simulatore (questo includeil set-up degli ambienti di simulazione)
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che conterrà gli indici di connessione dei qudit da 
 *  accendere o meno in base se il meccanismo di connessione è stato abilitato
 *  o meno dall'utente (opzione -J0/1)
 */
typedef struct parametri_operazione
{
/*
 *  Vettore contenente le coppie di indici di connessione da abilitare
 */
    size_t *indici;
/*
 *  Numero di qudit da connettere tra di loro per ogni gate.
 *  Così facendo, se abbiamo J_ij != J_kh per ogni coppia di indici possiamo
 *  implementare gate tra un numero arbitrario di qudit semplicemente
 *  accendendo ogni connessione tra tutte le coppie possibili di qudit su cui
 *  opera il gate stesso
 */
    size_t numero_connessioni;
} parametri_operazione;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione dei parametri di
 *  un'operazione
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione dei parametri di un'operazione. Richiede il
 *   numero di qudit coinvolti dall'operazione
 */
extern
parametri_operazione *alloc_parametri_operazione(size_t N);

/*
 *  Funzione per la deallocazione dei parametri di un'operazione
 */
extern
void free_parametri_operazione(parametri_operazione **param);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che definisce le operazioni eseguibili dal simulatore
 */
typedef struct operazione
{
/*
 *  Tipo dell'operazione
 */
    enum
    {
        SCH,
        BLK,
        MSN,
        CSL
    } tipo;
/*
 * Puntatore all'operazione effettiva da simulare. In caso di una
 * misura il puntatore sarà NULL.
 */
    union
    {
/*
 *      Puntatore alla schedule da eseguire
 */
        schedule *sch;
/*
 *      Nome del blocco di operazioni da eseguire
 */
        char *nome_blocco;
/*
 *      Puntatore alla custom logic da eseguire
 */
        custom_logic_ptr csl;
    } val;

/*
 *  Parametri dell'operazione, contengono gli indici di connessione dei qudit
 *  per l'operazione stessa. Sarà un puntatore NULLO in caso di 'custom_logic'
 */
    parametri_operazione *parametri;

/*
 *  Puntatore alla prossima operazione della lista
 */
    struct operazione *next;
} operazione;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione di un'operazione
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un'operazione. Richiede il numero di qudit
 *  coinvolti dall'operazione
 */
extern
operazione *alloc_operazione(size_t N);

/*
 *  Funzione per la deallocazione di un'operazione
 */
extern
void free_operazione(operazione **op);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati per la realizzazione della lista di operazioni in cui ogni
 *  blocco verrà tradotto
 */
typedef struct lista_operazioni
{
/*
 *  Puntatore alla testa della lista.
 */
    operazione *testa;
/*
 *  Puntatore alla coda della lista
 */
    operazione *coda;
}lista_operazioni;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'lista_operazioni'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di una 'lista_gate'
 */
extern
lista_operazioni *alloc_lista_operazioni(void);

/*
 *  Funzione per la deallocazione di una 'lista_gate'
 */
extern
void free_lista_operazioni(lista_operazioni **lst);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che permette la definizione di tabelle di liste di
 *  operazioni da eseguire
 */
typedef struct blocco_operazioni
{
/*
 *  Nome del blocco
 */
    char *nome_blocco;
/*
 *  Lista di gate da eseguire
 */
    lista_operazioni *lst;
/*
 *  Puntatore al prossimo blocco della tabella
 */
    struct blocco_operazioni *next;
} blocco_operazioni;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'blocco_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un blocco di gate
 */
extern
blocco_operazioni *alloc_blocco_operazioni(void);

/*
 *  Funzione per la deallocazione di una lista di definizioni di gate
 */
extern
void free_blocco_operazioni(blocco_operazioni **blk);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un'istruzione all'interno di una
 *  'lista_operazioni'
 */
extern
void inserisci_operazione(lista_operazioni *lst, operazione *op);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un blocco di operazioni all'interno della
 *  tabella dei blocchi
 */
extern
blocco_operazioni *inserisci_blocco_operazioni(blocco_operazioni *tabella,
                                                blocco_operazioni *blk);

//---------------------------------------------------------------------------//

/*
 *  Funzione per verificare la presenza di un blocco di operazioni all'interno
 *  della tabella dei blocchi. Ritorna un puntatore nullo se il blocco
 *  NON è presente
 */
extern
blocco_operazioni *cerca_blocco_operazioni(blocco_operazioni *tabella,
                                            char *nome_blocco);

//---------------------------------------------------------------------------//

/*
 *  Funzione che, forniti gli indici dei qudit coinvolti dal gate, va a 
 *  fare il set-up del sistema composto opportuno per la simulazione
 *  
 *  TODO::  liberare la memoria punatta precedentemente dai campi che ora
 *          andiamo a riutilizzare
 */
extern
sistema_composto *init_sistema_composto_sch(parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione che esegue uno shift dei livelli delle rotazioni in base ai qudit
 *  a cui devono essere applicate le rotazioni
 */
extern
uint8_t shift_livelli_schedule(schedule *sch, parametri_gate *param);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un operatore derivante dall'invocazione di un gate
 *  lo traduce in una schedule
 */
extern
schedule *traduci_operatore_gate(gate_operator *operatore);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un gate g lo traduce in schedule
 */
extern
schedule *traduci_gate(gate *g);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data un'istruzione la traduce in un'operazione
 */
extern
operazione *traduci_istruzione(istruzione **ist);

//---------------------------------------------------------------------------//

/*
 *  Funzione che dato un blocco di gate lo traduce in un blocco di
 *  operazioni
 */
extern
blocco_operazioni *traduci_blocco_gate(blocco_gate *gate_blk);

//---------------------------------------------------------------------------//

/*
 *  Funzione che data una tabella di blocchi di gate li traduce e li installa
 *  in una tabella di blocchi di operazioni
 */
extern
blocco_operazioni *traduci_tabella_blocchi(blocco_gate *tabella_gate);
#endif