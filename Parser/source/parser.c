/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20220114

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "./source/parser.y"
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

#include "../../Common/include/global.h"
#include "../../Common/include/gate.h"
#include "../include/parser_support.h"
#include "../include/parser.h"
#include "../include/lexer.h"

#include <string.h>
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 31 "./source/parser.y"
typedef union YYSTYPE {
    char        *string;
    size_t      int_val;
    double      float_val;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 65 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define NAME 257
#define GATE 258
#define MEASUREMENT 259
#define CUSTOM_LOGIC 260
#define INDEX 261
#define QUDIT 262
#define INTEGER 263
#define FLOAT 264
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    0,    0,    1,    1,    1,    1,    1,    4,
    4,    4,    4,    4,    3,    3,    2,    2,    2,    5,
    5,    5,    5,    5,    5,    5,
};
static const YYINT yylen[] = {                            2,
    0,    2,    3,    3,    8,    6,    5,    3,    3,    8,
    5,    5,    3,    2,    1,    2,    0,    1,    3,    0,
    1,    1,    1,    3,    3,    3,
};
static const YYINT yydefred[] = {                         1,
    0,    0,    0,    0,    0,    2,    0,    4,    0,    0,
    0,    3,    8,    9,    0,    0,    0,    0,    0,    0,
    0,    0,   15,    0,    0,   14,    0,    0,    0,    7,
   16,    0,    0,   18,    0,    0,   13,    6,    0,    0,
    0,    0,    0,   11,    0,   19,   12,    5,   21,   23,
   22,    0,    0,    0,   10,   24,   26,   25,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  266,  256,  257,  258,  262,   59,  267,   59,   61,   91,
   91,   59,  263,  264,  123,  263,  263,  257,  258,  259,
  260,  269,  270,   93,   93,   59,   91,   91,  257,  125,
  270,   61,   61,  263,  268,  268,   59,  261,  123,   93,
   44,   93,  268,   59,   40,  263,   59,  125,  257,  263,
  264,  271,   41,   44,   59,  257,  263,  264,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          1,
    7,   35,   22,   23,   52,
};
static const YYINT yysindex[] = {                         0,
  -59,  -46,  -35,  -76,  -62,    0,  -29,    0, -122, -232,
 -231,    0,    0,    0, -236,  -60,  -58,  -25,  -55,  -54,
 -219, -123,    0,  -22,  -21,    0, -222, -222,  -17,    0,
    0, -218,  -79,    0,  -39,  -38,    0,    0, -222,  -32,
 -217,  -14,  -41,    0, -247,    0,    0,    0,    0,    0,
    0,  -30,  -12, -245,    0,    0,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -37,  -37,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -40,    0,
    0,    0,    0,    0,  -16,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  -19,    0,   26,    0,
};
#define YYTABLESIZE 203
static const YYINT yytable[] = {                          6,
   15,   30,   41,   17,   41,   41,   17,   45,   36,   49,
   53,   56,    8,   54,   10,   50,   51,   57,   58,   43,
   18,   19,   20,   21,   20,    9,   44,   20,   11,   12,
   16,   17,   24,   26,   25,   27,   28,   29,   32,   33,
   34,   37,   38,   39,   47,   46,   55,   31,    0,    0,
    0,    0,    0,   40,   42,   17,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   48,   17,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   18,   19,   20,   21,    0,    0,    0,
   13,   14,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    2,    3,    4,    0,
    0,    0,    5,
};
static const YYINT yycheck[] = {                         59,
  123,  125,   44,   44,   44,   44,   44,   40,   28,  257,
   41,  257,   59,   44,   91,  263,  264,  263,  264,   39,
  257,  258,  259,  260,   41,   61,   59,   44,   91,   59,
  263,  263,   93,   59,   93,   91,   91,  257,   61,   61,
  263,   59,  261,  123,   59,  263,   59,   22,   -1,   -1,
   -1,   -1,   -1,   93,   93,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  125,  125,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  257,  258,  259,  260,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  256,  257,  258,   -1,
   -1,   -1,  262,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,
};
#endif
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 264
#define YYUNDFTOKEN 272
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,"'='",0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"error","NAME","GATE","MEASUREMENT","CUSTOM_LOGIC","INDEX","QUDIT","INTEGER",
"FLOAT","$accept","list","def","integer_sequence","inst_sequence","inst",
"float_sequence","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : list",
"list :",
"list : list ';'",
"list : list def ';'",
"list : list error ';'",
"def : QUDIT '[' INTEGER ']' '=' '{' integer_sequence '}'",
"def : GATE '[' INTEGER ']' '=' INDEX",
"def : NAME '=' '{' inst_sequence '}'",
"def : NAME '=' INTEGER",
"def : NAME '=' FLOAT",
"inst : GATE '[' integer_sequence ']' '(' float_sequence ')' ';'",
"inst : GATE '[' integer_sequence ']' ';'",
"inst : MEASUREMENT '[' integer_sequence ']' ';'",
"inst : CUSTOM_LOGIC NAME ';'",
"inst : NAME ';'",
"inst_sequence : inst",
"inst_sequence : inst_sequence inst",
"integer_sequence :",
"integer_sequence : INTEGER",
"integer_sequence : integer_sequence ',' INTEGER",
"float_sequence :",
"float_sequence : NAME",
"float_sequence : FLOAT",
"float_sequence : INTEGER",
"float_sequence : float_sequence ',' NAME",
"float_sequence : float_sequence ',' FLOAT",
"float_sequence : float_sequence ',' INTEGER",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 2:
#line 43 "./source/parser.y"
	{ /* Nothing */ }
#line 1096 "y.tab.c"
break;
case 3:
#line 44 "./source/parser.y"
	{ /* Nothing */ }
#line 1101 "y.tab.c"
break;
case 4:
#line 46 "./source/parser.y"
	{ 
    fprintf(stderr, "ERRORE nel parsing della linea: %ld.\n"
                    "ABORTING...\n\n", lineno);
    free_globals();
    exit(1);
}
#line 1111 "y.tab.c"
break;
case 5:
#line 55 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di set di Qudit.\n"
                    "Numero Qudit: %ld.\n", lineno, yystack.l_mark[-5].int_val);
#endif
/*
 *  Inizializziamo un array con gli elemnti della sequenza di numeri letti.
 *  Da tenere a mente che la sequenza di numeri è ordinata in verso opposto
 *  a quello di lettura
 */
    dimensioni_qudit = 
        (size_t*)realloc(dimensioni_qudit, sizeof(size_t)*(numero_qudit + yystack.l_mark[-5].int_val));

/*
 *  Verifichiamo l'esito della riallocazione.
 */
    if (dimensioni_qudit == NULL)
    {
        fprintf(stderr, "ERRORE nella riallocazione della lista delle "
                        "dimensioni dei qudit.\n"
                        "Memoria insufficiente.\n"
                        "ABORTING...\n\n");
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *tmp = int_seq;

    for (size_t i = 1; i <= yystack.l_mark[-5].int_val; ++i)
    {
/*
 *  Verifichiamo che il numero di qudit inizializzati e la dimensione
 *  della sequenza combacino
 */
        if (tmp == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni delle dimensioni "
                            "dei qudit.\n"
                            "Qudit dichiarati: %ld\n"
                            "Qudit inizializzati: %ld\n"
                            "ABORTING...\n\n",
                            lineno,
                            yystack.l_mark[-5].int_val + numero_qudit,
                            i + numero_qudit);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        dimensioni_qudit[numero_qudit + yystack.l_mark[-5].int_val - i] = tmp->valore.u_val;
        tmp = tmp->next;
    }

    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    fprintf(stdout, "valori sequenza letti:\n");
    
    for (size_t i = 0; i < yystack.l_mark[-5].int_val; ++i)
    {
        fprintf(stdout, "%ld-esimo valore: %ld\n",
                        i, dimensioni_qudit[numero_qudit + i]);
    }

    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Teniamo traccia del numero di qudit dichiarati.
 */
    numero_qudit += yystack.l_mark[-5].int_val;
}
#line 1197 "y.tab.c"
break;
case 6:
#line 138 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di un gate custom.\n"
                    "Nome Gate: %s.\n"
                    "Numero Qudit: %ld.\n"
                    "/path/to/file: %s\n"
                    "==========\n\n", lineno, yystack.l_mark[-5].string, yystack.l_mark[-3].int_val, yystack.l_mark[0].string);
#endif
/*
 *  Allochiamo un gate che conterrà la definizione attuale
 */
    gate *g = alloc_gate(yystack.l_mark[-3].int_val, 0);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: allocazione definizione gate non "
                        "riuscita.\n", lineno);
        free_globals();
        free_gate(g);
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Recuperiamo la stringa corretta
 */
    char *s = yystack.l_mark[0].string;
    ++s;
    size_t len = strlen(s);
    s[len - 1] = '\0';

    g->operatore.path_to_file = s;
    g->nome_gate = yystack.l_mark[-5].string;
    g->type = CSTM;

    esito_inserimento ris = inserisci_definizione(lst_def, g);

/*
 *  Verifichiamo l'esito dell'inserimento
 */
    if (ris == RDEF)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: Ridefinizione del gate %s.\n"
                        "Possibile che il nome sia già riservato per un gate "
                        "builtin.\n"
                        "Consultare il manuale dell'applicazione per capire "
                        "come stampare una lista esaustiva di tutti i gate "
                        "builtin.\n"
                        "ABORTING...\n\n", lineno, yystack.l_mark[-5].string);
        free_globals();
        free_gate(g);
        exit(1);
    }
    else if (ris == GERR)
    {
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
#line 1272 "y.tab.c"
break;
case 7:
#line 210 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di un blocco di gate.\n"
                    "Nome blocco: %s.\n"
                    "==========\n\n", lineno, yystack.l_mark[-4].string);
#endif

/*
 *  Verifichiamo che il blocco non sia già stato definito
 */
    blocco_gate *blk = cerca_blocco(tabella_blk, yystack.l_mark[-4].string);

    if (blk != NULL)
    {
        fprintf(stderr, "ln %ld.\n"
                        "ERROR:: Ridefinizione blocco '%s'.\n", lineno, yystack.l_mark[-4].string);
        free_globals();
        exit(1);
    }
    else
    {
        ;
    }

/*
 *  Allochiamo un blocco che andrà poi inserito all'interno della tabella
 *  dei blocchi
 */
    blk = alloc_blocco_gate();
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (blk == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: allocazione blocco gate non "
                        "riuscita.\n", lineno);
        free_blocco_gate(blk);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    blk->nome_blocco = yystack.l_mark[-4].string;
    blk->lst = lst_gate_supporto;
/*
 *  Liberiamo la variabile globale per poter leggere
 *  (se, in futuro, necessario) una nuova lista di gate
 */
    lst_gate_supporto = NULL;

    tabella_blk = inserisci_blocco(tabella_blk, blk);
}
#line 1334 "y.tab.c"
break;
case 8:
#line 269 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di una variabile.\n"
                    "Nome: %s.\n"
                    "Valore: %f\n"
                    "==========\n\n",
                    lineno, yystack.l_mark[-2].string, (double)yystack.l_mark[0].int_val);
#endif
/*
 *  Verifichiamo che la variabile non sia già definita. In tal caso
 *  sovrascriviamone il valore
 */
    variabile *var = ricerca_variabile(tabella_var, yystack.l_mark[-2].string);

    if (var == NULL)
    {
/*
 *      Allochiamo la variabile
 */ 
        variabile *var = alloc_variabile();
/*  
 *      Verifichiamo l'esito dell'allocazione
 */ 
        if (var == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: Allocazione variabile non riuscita.\n",
                            lineno);
            free_lista_variabili(&var);
            free_globals();
            exit(1);
        }
        else
        {
            ; /* Do Nothing */
        }
/*  
 *      Inizializziamo la variabile
 */ 
        var->valore = (double)yystack.l_mark[0].int_val;
        var->nome = yystack.l_mark[-2].string;

        tabella_var = inserisci_variabile(tabella_var, var);
    }
    else
    {
        var->valore = yystack.l_mark[0].int_val;
    } 
}
#line 1389 "y.tab.c"
break;
case 9:
#line 321 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta la definizione di una variabile.\n"
                    "Nome: %s.\n"
                    "Valore: %f\n"
                    "==========\n\n",
                    lineno, yystack.l_mark[-2].string, yystack.l_mark[0].float_val);
#endif
/*
 *  Verifichiamo che la variabile non sia già definita. In tal caso
 *  sovrascriviamone il valore
 */
    variabile *var = ricerca_variabile(tabella_var, yystack.l_mark[-2].string);

    if (var == NULL)
    {
/*
 *      Allochiamo la variabile
 */ 
        variabile *var = alloc_variabile();
/*  
 *      Verifichiamo l'esito dell'allocazione
 */ 
        if (var == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: Allocazione variabile non riuscita.\n",
                            lineno);
            free_lista_variabili(&var);
            free_globals();
            exit(1);
        }
        else
        {
            ; /* Do Nothing */
        }
/*  
 *      Inizializziamo la variabile
 */ 
        var->valore = yystack.l_mark[0].float_val;
        var->nome = yystack.l_mark[-2].string;

        tabella_var = inserisci_variabile(tabella_var, var);
    }
    else
    {
        var->valore = yystack.l_mark[0].float_val;
    }
}
#line 1444 "y.tab.c"
break;
case 10:
#line 375 "./source/parser.y"
	{

#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di gate builtin.\n"
                    "Nome gate: %s\n",
                    lineno, yystack.l_mark[-7].string);
#endif
/*
 *  Cerchiamo la definizione del gate
 */
    gate *g_def = cerca_definizione(lst_def, yystack.l_mark[-7].string);
/*
 *  verifichiamo l'esito della ricerca
 */
    if (g_def == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: possibile che il gate %s NON sia mai "
                        "stato definito.\n", lineno, yystack.l_mark[-7].string);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *tmp_float = float_seq;
    sequenza_numeri *tmp_int = int_seq;
/*
 *  Contiamo il numero di qudit su cui applicare il gate.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
#ifdef DEBUG_PARSER
        fprintf(stdout, "\nValori sequenza interi letti:");
#endif
    size_t n_qudit = 0;
    while(tmp_int != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, " %ld ", tmp_int->valore.u_val);
#endif
        ++n_qudit;
        tmp_int = tmp_int->next;
    }
#ifdef DEBUG_PARSER
        fprintf(stdout, "\n\n");
#endif

/*
 *  Contiamo il numero di argomenti forniti al gate.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
#ifdef DEBUG_PARSER
        fprintf(stdout, "Valori sequenza decimali letti:");
#endif
    size_t n_arg = 0;
    while(tmp_float != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, " %e ", tmp_float->valore.f_val);
#endif
        ++n_arg;
        tmp_float = tmp_float->next;
    }
#ifdef DEBUG_PARSER
        fprintf(stdout, "\n\n");
#endif

/*
 *  Reinizzializziamo tmp alla testa della sequenza di numeri
 */
    tmp_float = float_seq;
    tmp_int = int_seq;

/*
 *  Allochiamo i parametri della misura necessari all'interfaccia per
 *  l'allocazione di una struttura dati
 */
    parametri_gate *par = alloc_parametri_gate(n_qudit, n_arg);

/*
 *  Verifichiamo che i parametri passati corrispondano nella forma a quelli
 *  della definizione
 */
    if (g_def->parametri->n_qudit != par->n_qudit || 
        g_def->parametri->n_arg   != par->n_arg)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: i parametri forniti non rispettano la "
                        "definizione del gate.\n"
                        "Numero indici qudit richiesti: %d.\n"
                        "Numero indici qudit forniti:   %d.\n"
                        "Numero argomenti richiesti:    %ld.\n"
                        "Numero argomenti forniti:      %ld.\n"
                        "ABORTING...\n",
                        lineno,
                        g_def->parametri->n_qudit,
                        par->n_qudit,
                        g_def->parametri->n_arg,
                        par->n_arg);
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Riempiamo i parametri del gate
 *
 *  indici qudit
 */
    for (size_t i = 1; i <= n_qudit; ++i)
    {
        if (tmp_int == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_qudit da fornire: %ld\n"
                            "n_qudit forniti:    %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_qudit, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->idx_qudit[n_qudit - i] = tmp_int->valore.u_val;
        tmp_int = tmp_int->next;

/*
 *      Verifichiamo che l'indice del qudit passato sia legale, i.e., che il
 *      sistema sia composto da un numero sufficiente di qudit.
 */
        if (par->idx_qudit[n_qudit - i] > numero_qudit)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: indice qudit troppo grande.\n"
                            "Indice qudit fornito: %ld\n"
                            "Numero di qudit del sistema "
                            "(l'indicizzazione parte da 0): %ld\n"
                            "Definire prima i qudit dei blocchi di gate.\n"
                            "ABORTING...\n\n",
                            lineno,
                            par->idx_qudit[n_qudit - i],
                            numero_qudit);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }
/*
 *  Argomenti
 */
    for (size_t i = 1; i <= n_arg; ++i)
    {
        if (tmp_float == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_arg da fornire: %ld\n"
                            "n_arg forniti:    %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_arg, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->arg[n_arg - i] = tmp_float->valore.f_val;
        tmp_float = tmp_float->next;
    }

    free_sequenza_numeri(&float_seq);
    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    fprintf(stdout, "Indici qudit su cui applicare il gate: ");
    
    for (size_t i = 0; i < n_qudit - 1; ++i)
    {
        fprintf(stdout, "%ld, ", par->idx_qudit[i]);
    }
    
    fprintf(stdout, "%ld\n", par->idx_qudit[n_qudit - 1]);
    fprintf(stdout, "Argomenti del gate: ");
    
    for (size_t i = 0; i < n_arg - 1; ++i)
    {
        fprintf(stdout, "%f, ", par->arg[i]);
    }
    
    fprintf(stdout, "%f\n", par->arg[n_arg - 1]);
    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, par);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
#line 1681 "y.tab.c"
break;
case 11:
#line 609 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di gate custom.\n"
                    "Nome gate: %s\n",
                    lineno, yystack.l_mark[-4].string);
#endif
/*
 *  Cerchiamo la definizione del gate
 */
    gate *g_def = cerca_definizione(lst_def, yystack.l_mark[-4].string);
/*
 *  verifichiamo l'esito della ricerca
 */
    if (g_def == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: possibile che il gate %s NON sia mai "
                        "stato definito.\n", lineno, yystack.l_mark[-4].string);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *tmp = int_seq;
/*
 *  Contiamo il numero di qudit su cui applicare il gate.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
    size_t n_qudit = 0;
    while(tmp != NULL)
    {
        ++n_qudit;
        tmp = tmp->next;
    }

/*
 *  Reinizzializziamo tmp alla testa della sequenza di numeri
 */
    tmp = int_seq;

/*
 *  Allochiamo i parametri della misura necessari all'interfaccia per
 *  l'allocazione di una struttura dati
 */
    parametri_gate *par = alloc_parametri_gate(n_qudit, 0);

/*
 *  Verifichiamo che i parametri passati corrispondano nella forma a quelli
 *  della definizione
 */
    if (g_def->parametri->n_qudit != par->n_qudit || 
        g_def->parametri->n_arg   != par->n_arg)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: i parametri forniti non rispettano la "
                        "definizione del gate.\n"
                        "Numero indici qudit richiesti: %d.\n"
                        "Numero indici qudit forniti:   %d.\n"
                        "Numero argomenti richiesti:    %ld.\n"
                        "Numero argomenti forniti:      %ld.\n"
                        "ABORTING...\n",
                        lineno,
                        g_def->parametri->n_qudit,
                        par->n_qudit,
                        g_def->parametri->n_arg,
                        par->n_arg);
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Riempiamo i parametri del gate
 */
    for (size_t i = 1; i <= n_qudit; ++i)
    {
        if (tmp == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_qudit dichiarati: %ld\n"
                            "n_qudit inizializzati: %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_qudit, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->idx_qudit[n_qudit - i] = tmp->valore.u_val;
        tmp = tmp->next;

/*
 *      Verifichiamo che l'indice del qudit passato sia legale, i.e., che il
 *      sistema sia composto da un numero sufficiente di qudit.
 */
        if (par->idx_qudit[n_qudit - i] > numero_qudit)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: indice qudit troppo grande.\n"
                            "Indice qudit fornito: %ld\n"
                            "Numero di qudit del sistema "
                            "(l'indicizzazione parte da 0): %ld\n"
                            "Definire prima i qudit dei blocchi di gate.\n"
                            "ABORTING...\n\n",
                            lineno,
                            par->idx_qudit[n_qudit - i],
                            numero_qudit);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    fprintf(stdout, "Indici qudit su cui applicare il gate: ");
    
    for (size_t i = 0; i < n_qudit - 1; ++i)
    {
        fprintf(stdout, "%ld, ", par->idx_qudit[i]);
    }
    
    fprintf(stdout, "%ld\n", par->idx_qudit[n_qudit - 1]);
    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, par);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
#line 1849 "y.tab.c"
break;
case 12:
#line 774 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di misura.\n"
                    "Indici qudit da misurare: ", lineno);
#endif
    sequenza_numeri *tmp = int_seq;
/*
 *  Contiamo il numero di qudit da misurare.
 *  Dalla grammatica abbiamo la certezza di avere liste NON nulle.
 */
    size_t n_qudit = 0;
    while(tmp != NULL)
    {
        ++n_qudit;
        tmp = tmp->next;
    }

/*
 *  Reinizzializziamo tmp alla testa della sequenza di numeri
 */
    tmp = int_seq;

/*
 *  Allochiamo i parametri della misura necessari all'interfaccia per
 *  l'allocazione di una struttura dati
 */
    parametri_gate *par = alloc_parametri_gate(n_qudit, 0);

/*
 *  Riempiamo i parametri del gate
 */
    for (size_t i = 1; i <= n_qudit; ++i)
    {
        if (tmp == NULL)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERRORE nell'inizializzazioni dei parametri della "
                            "misura dei qudit.\n"
                            "n_qudit dichiarati: %ld\n"
                            "n_qudit inizializzati: %ld\n"
                            "ABORTING...\n\n",
                            lineno, n_qudit, i);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }

        par->idx_qudit[n_qudit - i] = tmp->valore.u_val;
        tmp = tmp->next;

/*
 *      Verifichiamo che l'indice del qudit passato sia legale, i.e., che il
 *      sistema sia composto da un numero sufficiente di qudit.
 */
        if (par->idx_qudit[n_qudit - i] > numero_qudit)
        {
            fprintf(stderr, "ln %ld\n"
                            "ERROR:: indice qudit troppo grande.\n"
                            "Indice qudit fornito: %ld\n"
                            "Numero di qudit del sistema "
                            "(l'indicizzazione parte da 0): %ld\n"
                            "Definire prima i qudit dei blocchi di gate.\n"
                            "ABORTING...\n\n",
                            lineno,
                            par->idx_qudit[n_qudit - i],
                            numero_qudit);
            free_parametri_gate(par);
            free_globals();
            exit(1);
        }
        else
        {
            ;   /* Do Nothing */
        }
    }

    free_sequenza_numeri(&int_seq);

#ifdef DEBUG_PARSER
    for (size_t i = 0; i < n_qudit - 1; ++i)
    {
        fprintf(stdout, "%ld, ", par->idx_qudit[i]);
    }
    
    fprintf(stdout, "%ld\n", par->idx_qudit[n_qudit - 1]);
    fprintf(stdout, "==========\n\n");
#endif

/*
 *  Allochiamo una definizione fittizia di gate per poter usare le interfacce
 *  di allocazione delle istruzioni
 */
    gate *g_def = alloc_gate(n_qudit, 0);

/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_def == NULL)
    {
        free_parametri_gate(par);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_def->nome_gate = "measure";
    g_def->type = MSNT;

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, par);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_parametri_gate(par);
        free_gate(g_def);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    free_gate(g_def);
}
#line 1991 "y.tab.c"
break;
case 13:
#line 913 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di custom_logic.\n"
                    "Nome custom_logic: %s.\n"
                    "==========\n\n",
                    lineno, yystack.l_mark[-1].string);
#endif
/*
 *  Verifichiamo che l'istruzione precedente sia stata correttamente inserita
 */
    if (ist_supporto != NULL)
    {
        fprintf(stderr, "ERRORE istruzione precedente NON correttamente "
                        "inserita nella tabella.\n"
                        "ABORTING...\n\n");
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Recuperiamo il puntatore alla funzione di custom logic dalla tabella
 */
    ist_supporto = alloc_istruzione_custom_logic(yystack.l_mark[-1].string);

/*
 *  Verifichiamo che l'istruzione precedente sia stata correttamente inserita
 */
    if (ist_supporto == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
}
#line 2038 "y.tab.c"
break;
case 14:
#line 957 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "\n==========\n"
                    "ln: %ld.\n"
                    "Riconosciuta l'istruzione di blocco.\n"
                    "Nome: %s.\n"
                    "==========\n\n",
                    lineno, yystack.l_mark[-1].string);
#endif
/*
 *  Verifichiamo che l'istruzione precedente sia stata correttamente inserita
 */
    if (ist_supporto != NULL)
    {
        fprintf(stderr, "ERRORE istruzione precedente NON correttamente "
                        "inserita nella tabella.\n"
                        "ABORTING...\n\n");
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  recuperiamo il blocco dalla tabella
 */
    blocco_gate *blk = cerca_blocco(tabella_blk, yystack.l_mark[-1].string);
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (blk == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: blocco %s NON definito.\n",
                        lineno, yystack.l_mark[-1].string);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Creiamo una definizione fittizia di gate
 */
    gate *g_def = alloc_gate(0,0);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (g_def == NULL)
    {
        free_gate(g_def);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    g_def->operatore.blk_ptr = blk;
    g_def->nome_gate = yystack.l_mark[-1].string;
    g_def->type = BLCK;

/*
 *  Allochiamo l'istruzione da eseguire
 */
    ist_supporto = alloc_istruzione_gate(g_def, NULL);
/*
 *  Verifichiamo l'esito dell'allocazione
 */
    if (ist_supporto == NULL)
    {
        free_gate(g_def);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    free_gate(g_def);
}
#line 2129 "y.tab.c"
break;
case 15:
#line 1047 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    fprintf(stdout, "Iniziato parsing sequenza di istruzioni");
#endif
/*
 *  Questo è il primo passo di parsing di una sequenza NON vuota di numeri
 *  interi.
 */
    if (lst_gate_supporto != NULL)
    {
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);

        free_lista_gate(&lst_gate_supporto);
    }
    else
    {
        ;   /* Do Nothing */
    }

    lst_gate_supporto = alloc_lista_gate();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (lst_gate_supporto == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la corretta all'ocazione di una nuova istruzione
 */
    if (ist_supporto == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: Inserimento di un'istruzione nulla "
                        "all'interno di una lista di gate.\n"
                        "ABORTING...\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    inserisci_istruzione(lst_gate_supporto, ist_supporto);
/*
 *  Liberiamo la variabile d'appoggio per una possibile futura lettura di
 *  un'altra istruzione di supporto
 */
    ist_supporto = NULL;
}
#line 2197 "y.tab.c"
break;
case 16:
#line 1112 "./source/parser.y"
	{
/*
 *  Questo è il primo passo di parsing di una sequenza NON vuota di numeri
 *  interi.
 */
    if (lst_gate_supporto == NULL)
    {
        fprintf(stdout, "ln %ld "
                        "ERROR:: seqeunza di istruzioni nulla quando "
                        "dovrebbe essere disponibile almeno un'istruzione.\n"
                        "ABORTING...\n\n", lineno);
        free_lista_gate(&lst_gate_supporto);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Verifichiamo la corretta all'ocazione di una nuova istruzione
 */
    if (ist_supporto == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERROR:: Inserimento di un'istruzione nulla "
                        "all'interno di una lista di gate.\n"
                        "ABORTING...\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    inserisci_istruzione(lst_gate_supporto, ist_supporto);
/*
 *  Liberiamo la variabile d'appoggio per una possibile futura lettura di
 *  un'altra istruzione di supporto
 */
    ist_supporto = NULL;
}
#line 2245 "y.tab.c"
break;
case 17:
#line 1158 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    if (int_seq != NULL)
    {
        fprintf(stderr, "Riconosciuta sequenza di numeri vuota alla linea: %ld "
                        "ma la sequenza di numeri non è NULLA.\n"
                        "Seqeunza di numeri deallocata improprioamente.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        fprintf(stdout, "Riconosciuta sequenza di numeri vuota alla linea: "
                        "%ld.\n", lineno);
    }
#endif
}
#line 2267 "y.tab.c"
break;
case 18:
#line 1177 "./source/parser.y"
	{
/*
 *  Questo è il primo passo di parsing di una sequenza NON vuota di numeri
 *  interi.
 */
    if (int_seq != NULL)
    {
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&int_seq);
    }
    else
    {
        ;   /* Do Nothing */
    }

    int_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (int_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    int_seq->valore.u_val = yystack.l_mark[0].int_val;
}
#line 2309 "y.tab.c"
break;
case 19:
#line 1216 "./source/parser.y"
	{
/*
 *  Secondo step di parsing di una sequenza di numeri interi. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (int_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri interi "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_sequenza_numeri(&elem);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.u_val = yystack.l_mark[0].int_val;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    int_seq = inserisci_valore_sequenza_numeri(int_seq, elem);
}
#line 2358 "y.tab.c"
break;
case 20:
#line 1263 "./source/parser.y"
	{
#ifdef DEBUG_PARSER
    if (float_seq != NULL)
    {
        fprintf(stderr, "Riconosciuta sequenza di numeri vuota alla linea: %ld "
                        "ma la sequenza di numeri non è NULLA.\n"
                        "Seqeunza di numeri deallocata improprioamente.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        fprintf(stdout, "Riconosciuta sequenza di numeri vuota alla linea: "
                        "%ld.\n", lineno);
    }
#endif
}
#line 2380 "y.tab.c"
break;
case 21:
#line 1282 "./source/parser.y"
	{
/*
 *  Questo è un possibile primo passo di parsing di una sequenza NON vuota di
 *  numeri razionali.
 */
    if (float_seq != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&float_seq);
#endif
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (float_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

/*
 *  Cerchiamo la variabile in questione
 */
    variabile *var = ricerca_variabile(tabella_var, yystack.l_mark[0].string);
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (var == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERRORE:: variabile %s NON definita\n", lineno, yystack.l_mark[0].string);
        free_lista_variabili(&var);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq->valore.f_val = var->valore;
}
#line 2444 "y.tab.c"
break;
case 22:
#line 1343 "./source/parser.y"
	{
/*
 *  Questo è un possibile primo passo di parsing di una sequenza NON vuota di
 *  numeri razionali.
 */
    if (float_seq != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&float_seq);
#endif
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (float_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq->valore.f_val = yystack.l_mark[0].float_val;
}
#line 2488 "y.tab.c"
break;
case 23:
#line 1384 "./source/parser.y"
	{
/*
 *  Questo è un possibile primo passo di parsing di una sequenza NON vuota di
 *  numeri razionali.
 */
    if (float_seq != NULL)
    {
#ifdef DEBUG_PARSER
        fprintf(stdout, "ln %ld "
                        "WARNING:: Iniziato il parsing di una nuova sequenza "
                        "senza aver prima deallocato correttamente la "
                        "precedente sequenza letta.\n"
                        "Deallocazione sequenza...\n\n", lineno);
        free_sequenza_numeri(&float_seq);
#endif
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (float_seq == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    float_seq->valore.f_val = (double)yystack.l_mark[0].int_val;
}
#line 2532 "y.tab.c"
break;
case 24:
#line 1425 "./source/parser.y"
	{
/*
 *  Secondo step di parsing di una sequenza di numeri. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (float_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }
/*
 *  Cerchiamo la variabile in questione
 */
    variabile *var = ricerca_variabile(tabella_var, yystack.l_mark[0].string);
/*
 *  Verifichiamo l'esito della ricerca
 */
    if (var == NULL)
    {
        fprintf(stderr, "ln %ld\n"
                        "ERRORE:: variabile %s NON definita\n", lineno, yystack.l_mark[0].string);
        free_sequenza_numeri(&elem);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.f_val = var->valore;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    float_seq = inserisci_valore_sequenza_numeri(float_seq, elem);
}
#line 2599 "y.tab.c"
break;
case 25:
#line 1489 "./source/parser.y"
	{
/*
 *  Secondo step di parsing di una sequenza di numeri. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (float_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.f_val = yystack.l_mark[0].float_val;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    float_seq = inserisci_valore_sequenza_numeri(float_seq, elem);
}
#line 2647 "y.tab.c"
break;
case 26:
#line 1534 "./source/parser.y"
	{
/*
 *  Secondo step di parsing di una sequenza di numeri. Verifichiamo
 *  che la sequenza sia già stata inizializzata
 */
    if (float_seq == NULL)
    {
        fprintf(stderr, "ln: %ld ERRORE:: Sequenza di numeri "
                        "inizializzata in modo errato.\n"
                        "Possibile la presenza di una virgola (,) "
                        "antecedente il primo numero.\n"
                        "ABORTING...\n\n", lineno);
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    sequenza_numeri *elem = alloc_valore_sequenza_numeri();

/*
 *  Verifichiamo l'esito dell'allocazione. Nel caso l'errore è già stato
 *  segnalato e non dobbiamo fare altro se non abortire l'esecuzione del
 *  programma
 */
    if (elem == NULL)
    {
        free_globals();
        exit(1);
    }
    else
    {
        ;   /* Do Nothing */
    }

    elem->valore.f_val = (double)yystack.l_mark[0].int_val;

/*
 *  Inseriamo il nuovo elemento in testa alla sequenza
 */
    float_seq = inserisci_valore_sequenza_numeri(float_seq, elem);
}
#line 2695 "y.tab.c"
break;
#line 2697 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
