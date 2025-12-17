%{
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
#include "../../Common/include/common.h"

#include "../../Common/include/global.h"
#include "../../Common/include/gate.h"
#include "../include/lexer_support.h"
#include "../include/parser.h"
%}


DIGIT                   [0-9]
INT_NUMBER              -?{DIGIT}+
FLOAT_NUMBER            -?{INT_NUMBER}*"."?{INT_NUMBER}
GATE_NAME               $*[A-Z][A-Z_]*[0-9]*
FUNC_VAR_BLOCK_NAME     [a-z_]+[0-9_]*
STRING                  \"[a-zA-Z _/"."]*\"

%%

"("|"["|"{"|")"|"]"|"}"|";"|"="|"," {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 0. Valore letto: %s\n", yytext);
#endif
/*
 *  Ritorniamo semplicemente il carattere speciale letto
 */
    return *yytext;
}
"q" {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 1. Valore letto: %s\n", yytext);
#endif
/*
 *  Ritorniamo semplicemente la keyword letta
 */
    yylval.string = strdup(yytext);
    return QUDIT;
}
"measure" {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 2. Valore letto: %s\n", yytext);
#endif
/*
 *  Lasciamo al parser il compito di allocare l'istruzione relativa e di
 *  inserirla nell'opportuno blocco
 */
    yylval.string = strdup(yytext);
    return MEASUREMENT;
}
"custom_logic::" {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 3. Valore letto: %s\n", yytext);
#endif
/*
 *  Ritorniamo semplicemente la keyword letta
 */
    yylval.string = strdup(yytext);
    return CUSTOM_LOGIC;
}
{STRING} {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 4. Valore letto: %s\n", yytext);
#endif
/* 
 *  Questa è la regola per il riconoscimento degli INDEX
 */
    yylval.string = strdup(yytext);
    return INDEX;
}
{INT_NUMBER} {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 5. Valore letto: %s\n", yytext);
#endif

    int tmp = atoi(yytext);
/*
 *  Verifichiamo la legalità dell'input.
 */
    if (tmp < 0)
    {
        fprintf(stderr, "ERRORE alla riga %ld: numeri interi negativi NON "
                        "ammessi.\n"
                        "Valore letto: %d.\n"
                        "ABORTING...\n\n", lineno, tmp);
        exit(1);
    }
    else
    {
        yylval.int_val = (size_t)atoi(yytext);
    }

    return INTEGER;
}
{FLOAT_NUMBER} {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 6. Valore letto: %s\n", yytext);
#endif

    yylval.float_val = atof(yytext);
    return FLOAT;
}
{GATE_NAME} {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 7. Valore letto: %s\n", yytext);
#endif
    
    yylval.string = strdup(yytext);
    return GATE;
}
{FUNC_VAR_BLOCK_NAME} {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 8. Valore letto: %s\n", yytext);
#endif

    yylval.string = strdup(yytext);
    return NAME;
}
[ ?\t?]* {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 9. Valore letto: %s\n", yytext);
#endif
/*
 *  Niente da fare in quanto vogliamo semplicemente rimuovere dal file di input
 *  questi caratteri
 */
    yylval.string = strdup(yytext);
}
[\n] {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 10. Valore letto: %s\n", yytext);
#endif
/*
 *  Teniamo traccia della riga attuale
 */
++lineno;
/*
 *  Nient altro da fare in quanto vogliamo semplicemente rimuovere dal file di
 *  input questi caratteri
 */
    yylval.string = strdup(yytext);
}
. {
#ifdef DEBUG_LEXER
    fprintf(stdout, "IDR: 11. Valore letto: %s\n", yytext);
#endif
/*
 *  Niente da fare in quanto vogliamo semplicemente rimuovere dal file di input
 *  questi caratteri
 */
    yylval.string = strdup(yytext);
}

%%