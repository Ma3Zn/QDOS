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

#ifndef GATE_H
#define GATE_H

#include "common.h"
#include "../../Simulator/include/measure.h"

/*
 *  Definizone dello step di aumento di dimensioni di una lista di definizioni di gate.
 *
 *  Ogni volta che una lista di definizioni di gate avrà esurito lo spazio allocato
 *  verrà invocata una realloc di dimensione dim_attuale + STEP_DIM_LISTA
 * 
 *  L'allocazione iniziale avrà dimensione pari a STEP_DIM_LISTA
 */
#define STEP_DIM_LISTA 10u

/*
 *  Definizione dei codici tramite i quali si stabilisce se un custom gate è
 *  fornito come unitaria, sequenza di rotazioni o schedule
 */
#define CODICE_UNITARIA             0
#define CODICE_SEQUENZA_ROTAZIONI   1
#define CODICE_SCHEDULE             2

/*
 *  Header file contenente tutte le definizioni dei tipi di dato per
 *  le tabelle del parser. Per semplicictà al momento implementiamo
 *  tali tabelle come liste
 * 
 *  TODO::  implementare la lista delle definizoni come una tabella Hash.
 *          NON so quanto possa tornare utile visto che le definizioni dubito
 *          saranno mai tante ed anche i programmi me li aspetto piuttosto
 *          corti
 * 
 *  TODO::  Per il momento implementiamo la tabella dei blocchi come una
 *          semplice lista. In una futura versione si può pensare di
 *          implementarla con lo stesso approccio della lista delle
 *          definizoni per avere almeno una ricerca dicotomica dei
 *          blocchi. Vero è anche che mi aspetto pochi blocchi e dunque
 *          l'overhead introdotto dall'approccio dicotomico potrebbe essere
 *          più grande del guadagno che permetterebbe l'algoritmo di ricerca.
 *          Da tenere anche a mente che non mi aspetto programmi molto profondi
 *          e dunque anche nel caso di QEC credo che la ripetizione dei blocchi
 *          di error detection/correction possa essere minimale.
 */

//---------------------------------------------------------------------------//

/*
 *  Struttura dati per il passaggio dei parametri alle funzioni che generano
 *  i gate built-in includi nel programma
 * 
 *  Piccolo reminder::  questa struttura dati è per i BLTN, quindi sai già
 *                      tutto su di lei
 */
typedef struct parametri_gate
{
/*
 *  Numero qudit coinvolti dal gate
 */
    uint8_t n_qudit;

/*
 *  Indici qudit coinvolti dal gate
 */
    size_t *idx_qudit;

/*
 *  n_parametri
 */
    size_t n_arg;
/*
 *  Parametri richiesti dal gate
 */
    double *arg;
} parametri_gate;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'parametri_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione dei parametri del gate
 */
extern
parametri_gate *alloc_parametri_gate(uint8_t n_qudit, size_t n_parametri);

/*
 *  Funzione per la deallocazione dei parametri del gate
 */
extern
void free_parametri_gate(parametri_gate *parametri);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che definisce il tipo di ritorno della funzione associata
 *  ad ogni gate BLTN
 */
typedef struct
{
    enum
    {
        COD_UNT = CODICE_UNITARIA,
        COD_SQR = CODICE_SEQUENZA_ROTAZIONI,
        COD_SCH = CODICE_SCHEDULE  
    } type;

    union 
    {
        gsl_matrix_complex          *unt;
        struct sequenza_rotazioni   *seq;
        struct schedule             *sch;
    } val;
    
}gate_operator;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'BLTN_return_type'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un gate_operator
 */
extern
gate_operator *alloc_gate_operator();

/*
 *  Funzione per la deallocazione di un gate_operator
 */
extern
void free_gate_operator(gate_operator **ptr);

//---------------------------------------------------------------------------//

/*
 *  Enumerazione che va a definire il tipo di GATE ammissibile.
 *
 *  Acronimata per non avere conflitti con le definizioni del parser
 */
typedef enum
{
    BLTN,
    CSTM,
    MSNT,
    BLCK,
    UNDF
}gate_type;

/*
 *  Struttura dati che va a contenere tutte le informazioni necessarie
 *  ad un gate generalizzato, ovvero, un gate BUILTIN (BLTN), un gate
 *  CUSTOM (CSTM) definito da utente, una misura MEASUREMENT (MSNT)
 *  oppure un blocco di gate BLOCK (BLCK) definito da utente.
 *  Infine è prevista l'opzione UNDEFINED (UNDF) come valore di
 *  inizializzazione
 */
typedef struct gate
{
/*
 *  Nome del gate
 */
    char *nome_gate;

/*
 *  Array contenente il tipo del gate: BUILTIN / CUSTOM / MEASUREMENT
 */
    gate_type type;

/*
 *  Parametri richiesti dal gate
 */
    parametri_gate *parametri;

    union
    {    
/*
 *      Se si tratta di un gate built-in: la funziona che lo genera
 */
        gate_operator* (*func_ptr) (parametri_gate *parametri);
/*
 *      Se si tratta di un gate definito da utente: il file che lo contiene
 */
        char *path_to_file;
/*
 *      Se si tratta di un blocco: il puntatore al blocco nella tabella
 *      dei blocchi
 * 
 *      Se si tratta di una misura la procedura è standard e l'unica cosa di
 *      cui dobbiamo tenere traccia sono gli indici dei qudit da misurare e
 *      dunque settiamo NULLO uno qualsiasi dei puntatori della union.
 *      In particolare setteremo nullo il blk_ptr
 */
        struct blocco_gate *blk_ptr;
    } operatore;
} gate;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un gate
 */
extern
gate *alloc_gate(uint8_t n_qudit, size_t n_arg);

/*
 *  Funzione per la deallocazione di un gate
 */
extern
void free_gate(gate *g);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati 
 */
typedef struct lista_definizioni_gate
{
/*
 *  Array contenente i puntatori alle definizioni dei gate per rendere
 *  possibile una ricerca dicotomica della lista delle definizoni
 */
    gate **indici_lista;
/*
 *  Dimensione massima della lista: coincide con lo spazio allocato per
 *  indici_lista
 */
    size_t dim_max;
/*
 *  Dimensione effettiva della lista: coincide con il numero di puntatori
 *  NON nulli presenti in indici_lista. lista_indicidim_eff - 1 coincide
 *  con il puntatore alla coda della lista
 */
    size_t dim_eff;
}lista_definizioni_gate;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'lista_definizioni_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di una lista di definizioni di gate
 */
extern
lista_definizioni_gate *alloc_lista_definizioni_gate(void);

/*
 *  Funzione per la riallocazione di una lista di definizioni di gate
 */
extern
lista_definizioni_gate 
    *realloc_lista_definizioni_gate(lista_definizioni_gate *lst, size_t step);

/*
 *  Funzione per la deallocazione di una lista di definizioni di gate
 */
extern
void free_lista_definizioni_gate(lista_definizioni_gate *lst);

//---------------------------------------------------------------------------//

/*
 *  Tipo del puntatore a funzione di una 'custom_logic'
 */
typedef struct blocco_gate* (*custom_logic_ptr) (lista_misure *tabella_misure);

/*
 *  Strutture dati necessarie per la gestione della tabella delle funzioni
 *  di custom logic
 */
typedef struct custom_logic
{
/*
 *  Nome funzione (usata come chiave di ricerca)
 */
    char *nome;

/*
 *  Puntatore alla funzione relativa al nome
 */
    custom_logic_ptr f_ptr;

}custom_logic;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per la gestione delle custom_logic definite da utente
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per la ricerca di una custom logic all'interno della tabella
 */
extern
custom_logic_ptr cerca_custom_logic(custom_logic *tabella_custom_logic,
                                    size_t dim,
                                    char *f_name);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati di supporto per la realizzazione di una lisa di gate
 */
typedef struct istruzione
{
/*
 *  Enumerazione necessaria per tenere traccia del tipo di istruzione
 */
    enum {
        GATE_INST,
        CUSTOM_LOGIC_INST
    } tipo;
/*
 *  Unione dei tipi possibili di istruzioni presenti in una 'lista_gate'
 */
    union {
/*
 *      Puntatore al gate attuale
 */
        gate *g;
/*
 *      Puntatore alla funzione relativa al nome
 */
        custom_logic_ptr custom_logic_function;
    } val;

/*
 *  Puntatore al prossimo gate della lista
 */
    struct istruzione *next;
}istruzione;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'blocco_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di un gate da eseguire. Prende
 *  come argomenti la definizione del gate da implementare ed i parametri
 *  del gate da eseguire
 */
extern
istruzione *alloc_istruzione_gate(gate *def_gate, parametri_gate *param);

/*
 *  Funzione per l'allocazione di una custom logic da eseguire. Prende in
 *  input il puntatore alla funzione da eseguire.
 */
extern
istruzione *alloc_istruzione_custom_logic(char *f_name);

/*
 *  Funzione per la deallocazione di un'istruzione
 */
extern
void free_istruzione(istruzione **ist);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati per la realizzazione della lisa di gate di cui è
 *  composto un blocco
 */
typedef struct lista_gate
{
/*
 *  Puntatore alla testa della lista.
 */
    istruzione *testa;
/*
 *  Puntatore alla coda della lista
 */
    istruzione *coda;
}lista_gate;

//---------------------------------------------------------------------------//

/*
 *  Interfacce per l'allocazione e la deallocazione della struttura dati
 *  'lista_gate'
 */

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'allocazione di una 'lista_gate'
 */
extern
lista_gate *alloc_lista_gate(void);

/*
 *  Funzione per la deallocazione di una 'lista_gate'
 */
extern
void free_lista_gate(lista_gate **lst);

//---------------------------------------------------------------------------//

/*
 *  Struttura dati che permette la definizione di liste di gate da eseguire
 */
typedef struct blocco_gate
{
/*
 *  Nome del blocco
 */
    char *nome_blocco;
/*
 *  Lista di gate da eseguire
 */
    lista_gate *lst;
/*
 *  Puntatore al prossimo blocco della tabella
 */
    struct blocco_gate *next;
}blocco_gate;

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
blocco_gate *alloc_blocco_gate(void);

/*
 *  Funzione per la deallocazione di una lista di definizioni di gate
 */
extern
void free_blocco_gate(blocco_gate *blk);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la gestione della dimensione del vettore dwgli indici
 *  della lista delle definizioni. Riorna 0 se tutto è andato a buon fine,
 *  altrimenti un valore positivo.
 */
extern
uint8_t aumenta_dimensione_lista_ordinata(lista_definizioni_gate *lst);

//---------------------------------------------------------------------------//

/*
 *  Funzione che esegue la ricerca binaria del gate di nome [nome]
 *  nella lista [lst_def]. Il valore di [*idx] al termine della funzione,
 *  se il gate è stato trovato, ne conterrà l'indice all'interno della lista.
 *  Il valore di ritorno della funzione ci dice se il gate è stato trovato [1]
 *  o meno [0].
 */
extern
uint8_t ricerca_definizione_gate(lista_definizioni_gate *lst,
                                    char *nome,
                                    size_t *idx);

//---------------------------------------------------------------------------//

/*
 *  Enumerazione che ci permette di distinguere gli errori riscontrati
 *  durante l'inserimento di un gate nella lista delle definizoni
 */
typedef enum
{
    OK,     /* Nessun Errore */
    RDEF,   /* Il gate era già presente e si vuole ridefinirlo */
    GERR    /* Errore Generico */
} esito_inserimento;

/*
 *  Funzione per l'inserimento di un gate all'interno della lista
 *  delle definizioni
 */
extern
esito_inserimento inserisci_definizione(lista_definizioni_gate *lst, gate *g);

//---------------------------------------------------------------------------//

/*
 *  Funzione per la ricerca di un gate all'interno della lista
 *  delle definizioni
 */
extern
gate *cerca_definizione(lista_definizioni_gate *lst, char *nome_gate);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un'istruzione all'interno di una 'lista_gate'
 */
extern
void inserisci_istruzione(lista_gate *lst, istruzione *ist);

//---------------------------------------------------------------------------//

/*
 *  Funzione per l'inserimento di un blocco di gate all'interno della
 *  tabella dei blocchi
 */
extern
blocco_gate *inserisci_blocco(blocco_gate *tabella, blocco_gate *blk);

//---------------------------------------------------------------------------//

/*
 *  Funzione per verificare la presenza di un blocco di gate all'interno della
 *  tabella dei blocchi. Ritorna un puntatore nullo se il blocco NON è presente
 */
extern
blocco_gate *cerca_blocco(blocco_gate *tabella, char *nome_blocco);

//---------------------------------------------------------------------------//

#endif