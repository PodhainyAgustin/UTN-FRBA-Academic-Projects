%option noyywrap
%option yylineno
%{

// Definimos las librerías que incluímos, entre ellas "misfunciones.h"

#include <stdlib.h>
#include <stdio.h>
#include "misfunciones.h"

// Podemos definir variables globales para nuestro analizador léxico

NodoOctales* listaoctales = NULL;
NodoHexadecimales* listahexadecimales = NULL;
NodoDecimales* listadecimales=NULL;
NodoReales* listareales=NULL;
NodoCaracteres* listacaracteres=NULL;
int numeroDec=0;
int acum=0;
int flag = 0;

NodoPalabrasReservadas* listaPalabras = NULL;
NodoIdentificadores* listaIds = NULL;
NodoLiteralesCadena* listaLC = NULL;
NodoOpCarac* listaOpCarac = NULL;
NodoComent* listaComentSimple = NULL;
NodoComent* listaComentMultiple = NULL;
NodoCaractNoReconocidos* ListaCaracNoReco = NULL;


%}

PALABRAS_RESERVADAS (auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)

%%

\n {yylineno;flag = 0;}


0[0-7]* {agregarNodoOctal(&listaoctales, yytext);flag = 0;}

0[xX][0-9a-fA-F]+ {agregarNodoHexa(&listahexadecimales, yytext, yyleng);flag = 0;}

[1-9][0-9]* {numeroDec=atoi(yytext); acum=acum+numeroDec;agregarNodoDecimal(&listadecimales, yytext);flag = 0;}

{PALABRAS_RESERVADAS} {insertarUltimo(&listaPalabras, yytext);flag = 0;}

[a-zA-Z_][a-zA-Z0-9_]* {insertarEnOrdenAlfabeticoYContandoApariciones(&listaIds, yytext);flag = 0;}

\"[^\"]*\" {insertarOrdenadoPorLongitudYsinRepetir(&listaLC, yytext, yyleng - 2);flag = 0;}

\/\/[^\n]* { agregarNodoComent(&listaComentSimple, yytext);flag = 0; }

\/\*(.[^\*\/]|[\r\n])*?\*\/ { agregarNodoComent(&listaComentMultiple, yytext);flag = 0;}

(\|\|)?(&&)?|[+\-*/%&|!~^<>=?:;.,()\[\]{}] {manejarOperadoresCaracteres(&listaOpCarac, yytext); flag = 0;}

[0-9]*\.([eE][+-]?[0-9]+)?[fFlL]?|[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?[fFlL]?|[0-9]+([eE][+-]?[0-9]+)[fFlL]? {agregarNodoReal(&listareales, yytext,yyleng);flag = 0;}

('([^'\\\n]|\\['?\\abfnrtv]|[0-7]{1,3}|x[0-9a-fA-F]{1,2})'|L'([^'\\\n]|\\['"?\\abfnrtv]|[0-7]{1,3}|x[0-9a-fA-F]{1,2})') {agregarNodoCaracter(&listacaracteres, yytext);flag = 0;}

[^ ] { caracteresNoReconocidos(&ListaCaracNoReco, yytext, yylineno, flag); flag=1;}

%%

int main() {
   
    /* Acciones previas a la invocación den analizador léxico */

    yyin = fopen("entrada.txt", "r");
    yyout = fopen("salida.txt", "w");	

    /* Invocación del analizador léxico */
    yylex();
	
    /* Acciónes posteriores a la ejecución del analizador léxico, 
    sirven por ejemplo, para hacer un informen de resumen.1 */

    mostrarListaOctales(listaoctales);
    mostrarListaHexa(listahexadecimales);
    mostrarListaDecimal(listadecimales);
    printf("La suma total de todas las constantes decimales que aparecieron es %d\n\n", acum);
    mostrarListaReales(listareales);
    mostrarListaCaracteres(listacaracteres);

    mostrarListaID(listaIds);
    mostrarListaLC(listaLC);
    mostrarListaPR(listaPalabras);
    imprimirLista(listaOpCarac);

    printf("Comentarios de una linea:\n");
    imprimirListaComentarios(listaComentSimple);
    
    printf("\nComentarios multiples lineas:\n");
    imprimirListaComentarios(listaComentMultiple);

    imprimirListaCaracNoReco(ListaCaracNoReco);

    

    fclose(yyin);
    fclose(yyout);

    getchar();
    return 0;
}

