#ifndef _yy_defines_h_
#define _yy_defines_h_

#define NAME 257
#define GATE 258
#define MEASUREMENT 259
#define CUSTOM_LOGIC 260
#define INDEX 261
#define QUDIT 262
#define INTEGER 263
#define FLOAT 264
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
    char        *string;
    size_t      int_val;
    double      float_val;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
