%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "funcionesListados.c"
//#include "funcionesFlex.c"
 //#include "funcionesFlex.h"



extern FILE *yyin; //para evitar el error que teniamos en Windows con el archivo de entrada
extern struct NodoCaractNoReconocidos* listaErroresLexicos; //para que flex guarde los errores lexicos en esta lista
extern int linActual;
extern int colActual;
extern void imprimirListaCaracNoReco();

int yylex();
int yyerror (const char *s);
int yywrap(){ return(1); }

char* tipo = NULL;
char* identificador;
int tipoSent;
NodoSentencias* listaSentencias = NULL;

NodoAuxiliarF auxiliarFun;

NodoFunciones* listaFunDecla = NULL;

NodoParametros* listaAuxParametros = NULL;

NodoErrorSintatico* listaErrorSintactico = NULL;




%}

%union {
    char op;
    int dval;
    char* str;
}

%locations



%token <dval> NUM
%token <str> NOMBRE_TIPO
%token <str> IDENTIFICADOR
%token <str> LITERAL_CADENA
%token <str> OR
%token <str> AND
%token <str> OP_ADD
%token <str> OP_SUB
%token <str> OP_MAS_IG
%token <str> OP_COMP
%token <str> SIMB_DIFF
%token <str> RETURN
%token <str> DO
%token <str> WHILE
%token <str> IF
%token <str> ELSE
%token <str> FOR
%token <str> SWITCH



%%

input:    
    | input line
;


line: declaracion
    | exp
    | sentencia
    | error ;

declaracion: NOMBRE_TIPO {tipo=$<str>1;} bifurcacionDeclaraciones
;

bifurcacionDeclaraciones: declaracionFuncion 
    | declaracionVariable
;

declaracionVariable: listaDeDeclaradores ';'
;

listaDeDeclaradores: declarador op_RecursionListaDeDeclaradores
;

op_RecursionListaDeDeclaradores:/*vacio*/
    |',' declarador op_RecursionListaDeDeclaradores
;
    

declarador: IDENTIFICADOR { manejarDeclaraciones(tipo, $<str>1, linActual);} inicializacionOpcional {}
;

inicializacionOpcional: /* vacio */
    | '=' exp
;

// Arrancan las expresiones

expPrimaria: IDENTIFICADOR 
    | NUM                  
    | LITERAL_CADENA       
    | '(' exp ')'
;

exp: expAsignacion;

expAsignacion: expCondicional
    | expUnaria operAsignacion expAsignacion
;

operAsignacion: '=' | OP_MAS_IG;

expCondicional: expOr
    | expOr '?' exp ':' expCondicional 
;

expOr: expAnd opRecursionOr
; 

opRecursionOr: /*vacio*/
    | OR expAnd opRecursionOr 
;

expAnd: expIgualdad opRecursionAnd
;

opRecursionAnd: /*vacio */
    | AND expIgualdad opRecursionAnd

expIgualdad: expRelacional noTerminal3;

noTerminal3: noTerminal2 noTerminal3
    |
;

noTerminal2:
    OP_COMP expRelacional 
    | SIMB_DIFF expRelacional 
;

expRelacional: expAditiva opRecursionExpRelacional
;

opRecursionExpRelacional: /*vacio*/ 
    |desigualdad opRecursionExpRelacional

desigualdad: '>''=' expAditiva
    |'>' expAditiva
    |'<''=' expAditiva
    |'<'    expAditiva
;

expAditiva: expMultiplicativa opRecursionExpAditiva
;

opRecursionExpAditiva: /*vacio*/
    |op_Suma_Resta opRecursionExpAditiva

op_Suma_Resta: '+' expMultiplicativa
    | '-' expMultiplicativa
;

expMultiplicativa: expUnaria primerOperando
;

primerOperando: segundoOperando primerOperando
    |
;

segundoOperando: 
    '*' expUnaria 
    | '/' expUnaria 
;


expUnaria: expPostfijo eliminaRecursividad
    | OP_ADD expUnaria     {}
    | OP_SUB expUnaria     {}
    | operUnario expUnaria {} 
    
;

eliminaRecursividad:
    parteOpcional eliminaRecursividad 
    |
;

parteOpcional: 
    OP_ADD 
    | OP_SUB 
;
    

operUnario: '*' | '&' | '-' | '!' ;

expPostfijo: expPrimaria opRecursionExpPostfijo
;

opRecursionExpPostfijo: /*vacio*/
    |op_Llave_Parentesis opRecursionExpPostfijo
;

op_Llave_Parentesis:'[' exp ']'
    |'(' op_listaArgumentos ')'////TP INTEGRADOR INVOCACION
;

op_listaArgumentos: /*vacio*/
    |listaArgumentos

listaArgumentos: expAsignacion opRecursionListaArgum
;

opRecursionListaArgum: /* vacio */ 
    |',' expAsignacion 
;

//Arrancan la declaracion y definicion de funciones

declaracionFuncion: IDENTIFICADOR {auxiliarFun.numLinea = linActual; auxiliarFun.tipo = tipo; auxiliarFun.identificador = $1; auxiliarFun.parametros = NULL;} '(' op_listaParametros ')' division_decla_y_def ;


division_decla_y_def: ';'{insertarFuncionesDeclaradas(&listaFunDecla, auxiliarFun.identificador, auxiliarFun.numLinea, auxiliarFun.tipo, listaAuxParametros); listaAuxParametros = NULL; } 
    |sentenciaCompuesta {insertarFuncionesDefinidas(&listaFunDecla, auxiliarFun.identificador, auxiliarFun.numLinea, auxiliarFun.tipo, listaAuxParametros); listaAuxParametros = NULL; }

op_listaParametros: /*vacio*/
    |listaParametros 
    
listaParametros: parametro op_masParametros

op_masParametros: /*vacio*/
    |',' parametro op_masParametros

parametro: NOMBRE_TIPO IDENTIFICADOR {insertarUltimoParametro(&listaAuxParametros, $1, $2);}

// Arrancan sentencias

sentencias:/*vacío*/
    | sentencia sentencias
    | declaracion sentencias
    ;

sentencia: sentenciaCompuesta
        | sentenciaExpresion 
        | sentenciaSalto    
        | sentenciaIteracion
        | sentenciaSeleccion
;

sentenciaCompuesta: '{'{tipoSent=1; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);} sentencias '}' 
;



sentenciaExpresion:  {tipoSent=2; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);} opc';'
;

sentenciaSalto: RETURN {tipoSent=5; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);} opc';'
;

opc:/*vacio*/ 
    |exp ;

sentenciaIteracion: WHILE{tipoSent=4; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);}'('exp')' sentencia
    | DO {tipoSent=4; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);} sentencia WHILE'('exp')'';'
    | FOR{tipoSent=4; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);}'(' opc ';' opc ';' opc ')' sentencia
;

sentenciaSeleccion: IF{tipoSent=3; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);} '(' exp ')' sentencia opcionElse 
    | SWITCH {tipoSent=3; agregarNodoSentencia(&listaSentencias,tipoSent,linActual);}'('exp')' sentencia
;

opcionElse: | ELSE sentencia

%%

/* Llamada por yyparse ante un error */
int yyerror (const char *s) { agregarNodoErrorSintactico (&listaErrorSintactico, linActual); return 0; } 

int main() {
    
    //Definiendo el archivo a analizar
    const char *nombre_archivo = "testPresentacion.c";

    //Asignación del archivo de entrada
    yyin = fopen(nombre_archivo, "r");
    if (yyin == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    yyparse(); // Llamar a la función generada por Bison para analizar la entrada
   
    fclose(yyin);

    char opcion;
    do {
        printf("Menu: Presione una tecla \n");
        printf("1. Imprimir declaraciones de funciones\n");
        printf("2. Imprimir sentencias\n");
        printf("3. Imprimir lista de variables\n");
        printf("4. Imprimir errores lexicos\n");
        printf("5. Imprimir errores sintacticos\n");
        printf("Presiona 0 para salir\n");
        
        opcion = getchar();  
        
        switch (opcion) {
            case '1':
                mostrarListaFuncionesDeclaradas(listaFunDecla);
                break;
            case '2':
                mostrarListaSent(listaSentencias);
                break;
            case '3':
                imprimirListaDeVars(listaDeVars);
                break;
            case '4':
                imprimirListaCaracNoReco(listaErroresLexicos);
            break;
            case '5':
                imprimirListaDeErrorSintactico(listaErrorSintactico);
            break;
            case '0':  
                printf("Saliendo...\n");
                return 0;
            default:
                printf("Opcion no valida. Por favor, elige una opcion valida.\n");
        }
        getchar();
    } while(1);

    getchar();

    return 0;
}