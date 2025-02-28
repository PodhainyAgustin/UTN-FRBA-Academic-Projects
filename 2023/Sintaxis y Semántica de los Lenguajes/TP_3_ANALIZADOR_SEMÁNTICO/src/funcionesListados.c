#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* FUNCIONES TABLA DE SIMBOLOS DE FUNCIONES */
//Nodo auxiliar para almacenar temporalmente el tipo, id y parametros

typedef struct NodoParametros {
    char* identificador;
    char* tipo;
    struct NodoParametros* siguiente;
} NodoParametros;

typedef struct NodoAuxiliarF {
    char* identificador;
    char* tipo;
    int numLinea;
    struct NodoParametros* parametros;    
} NodoAuxiliarF;

typedef struct NodoErrorSintatico {
    int numLinea;
    struct NodoErrorSintatico* siguiente;
} NodoErrorSintatico;


//Funciones para crear nodos de los auxiliares
NodoParametros* crearNodoParametros(char* tipo, char* id) {
    NodoParametros* nuevoNodo = (NodoParametros*)malloc(sizeof(NodoParametros));
    nuevoNodo->identificador = strdup(id);
    nuevoNodo->tipo = strdup(tipo);
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

//Funcion para insertar parametros a la lista de cada funcion
void insertarUltimoParametro(NodoParametros** cabeza, char* tipo, char* id){
    NodoParametros* nuevoNodo = crearNodoParametros(tipo, id);
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoParametros* actual = *cabeza;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
}

//Funcion para chequear si los parametros de la funcion definida coinciden con los parametros de la funcion declarada
int parametrosSonIguales(NodoParametros* parametrosDeclarados, NodoParametros* parametrosDefinidos) {
    
    while (parametrosDeclarados != NULL && parametrosDefinidos != NULL) { //mientras sigo habiendo parametros en ambas listas sigo comparando
        
        if (strcmp(parametrosDeclarados->tipo, parametrosDefinidos->tipo)) {
            return 0; //Si los parametros tienen distinto id o tipo la lista de parametros son diferentes

        }
        parametrosDeclarados = parametrosDeclarados->siguiente;
        parametrosDefinidos = parametrosDefinidos->siguiente;
    }
    
   
    if (parametrosDeclarados != NULL || parametrosDefinidos != NULL) {
        return 0; // Si una lista es más larga que la otra, la lista de parametros es diferente
    }
    
    return 1; // Si no pasa ninguno de esos casos, las listas de parametros son iguales
}

//Nodo Funciones declaradas y definidas
typedef struct NodoFunciones {
    char* identificador;
    char* tipo;
    int numLinea;
    struct NodoParametros* parametros;
    struct NodoFunciones* siguiente;
    int flagError;
    int flagDefinida;    
} NodoFunciones;

//Funcion para crear el nodo de las funciones
NodoFunciones* crearNodoFunciones(char* tipo, char* id, int numeroLinea, NodoParametros* parametros, int tipoError, int flagDefinida) {
    NodoFunciones* nuevoNodo = (NodoFunciones*)malloc(sizeof(NodoFunciones));
    nuevoNodo->identificador = strdup(id);
    nuevoNodo->tipo = strdup(tipo);
    nuevoNodo->numLinea = numeroLinea;
    nuevoNodo->parametros = parametros;
    nuevoNodo->siguiente = NULL;
    nuevoNodo->flagError = tipoError;
    nuevoNodo->flagDefinida= flagDefinida;
    return nuevoNodo;
}

//Funcion para insertar ultimo la funcion en su respectiva lista
void insertarUltimoFunciones(NodoFunciones** cabeza, char* idFuncion, char* tipo, int linea, NodoParametros* parametros, int tipoError, int flagDefinida){
    NodoFunciones* nuevoNodo = crearNodoFunciones(tipo, idFuncion, linea, parametros, tipoError, flagDefinida);
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoFunciones* actual = *cabeza;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
}

//Funcion para buscar si una funcion con ese identificador ya fue declarada
NodoFunciones* buscarNodoFunciones(NodoFunciones* cabeza, char* idFuncion) {
    NodoFunciones* actual = cabeza;

    while (actual != NULL) {
        if (strcmp(actual->identificador, idFuncion) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }

    return NULL;
}

//Funcion para insertar funciones declaradas
void insertarFuncionesDeclaradas(NodoFunciones** lista, char* idFuncion, int linea, char* tipo, NodoParametros* parametros) {
    NodoFunciones* encontro = buscarNodoFunciones(*lista, idFuncion);
    if(encontro==NULL){
        insertarUltimoFunciones(lista, idFuncion, tipo, linea, parametros, 0, 0); // 0 porque no hay error 
    }else{
        insertarUltimoFunciones(lista, idFuncion, tipo, linea, parametros, 1, 0); //1 porque hay error de doble declaracion de esa funcion, por lo que es invalida
    }
    }

//Funcion para insertar funciones definidas
void insertarFuncionesDefinidas(NodoFunciones** listaDeclaradas, char* idFuncion, int linea, char* tipo, NodoParametros* listaParametros) {
    NodoFunciones* encontroDeclarada = buscarNodoFunciones(*listaDeclaradas, idFuncion);
    
    
    //Si no fue declarada insertar en la lista de declaradas
    if(encontroDeclarada == NULL ){
        insertarUltimoFunciones(listaDeclaradas, idFuncion, tipo, linea, listaParametros, 0, 1); //1 porque se define la función 
        //como no fue declarada, al definirla se la declara sin error

    }else if((strcmp(encontroDeclarada->tipo,tipo) == 0) && parametrosSonIguales(encontroDeclarada->parametros, listaParametros) && (encontroDeclarada->flagDefinida == 0)){
        encontroDeclarada->flagDefinida=1;
        //La función ya fue declarada y solo se la está definiendo, no hay error

    }else if(strcmp(encontroDeclarada->tipo,tipo) != 0){
        insertarUltimoFunciones(listaDeclaradas, idFuncion, tipo, linea, listaParametros, 2, 1);
        //Error de tipo 2 porque la funcion ya fue declarada previamente y con otro tipo

    }else if(parametrosSonIguales(encontroDeclarada->parametros,listaParametros) !=1 ){
        insertarUltimoFunciones(listaDeclaradas, idFuncion, tipo, linea, listaParametros, 3, 1);
        //error de tipo 3 porque la funcion ya fue declarada previamente y con distintos parametros 

    }else if(encontroDeclarada->flagDefinida){
        insertarUltimoFunciones(listaDeclaradas, idFuncion, tipo, linea, listaParametros, 4, 1);}
        //error de tipo 4 porque la funcion ya fue definida previamente
}
    

/* 
    NodoFunciones* encontroDefinida = buscarNodoFunciones(*listaDefinidas, idFuncion);

    //Insertar en la lista de definidas
    if(encontroDefinida==NULL){
        insertarUltimoFunciones(listaDefinidas, idFuncion, tipo, linea, parametros, TRUE); //true porque es tipoError sintacticamente
    }else{
        insertarUltimoFunciones(listaDefinidas, idFuncion, tipo, linea, parametros, FALSE); //false porque hay doble declaracion de esa funcion, por lo que es invalida
    }
*/
    

//Funciones para imprimir Listados de las tablas de simbolos de las funciones declaradas y definidas

void mostrarListaFuncionesDeclaradas(NodoFunciones* cabeza) {
    NodoFunciones* actual = cabeza;

    printf("\nListado de funciones declaradas:\n");
    printf("+-----------------+-----------------+-----------------+---------------------------------------------------------+\n");
    printf("| %-15s | %-15s | %-15s | %-30s \n", "Id_Funcion", "Tipo", "Nro Linea", "Mensaje                                   ");
    printf("+-----------------+-----------------+-----------------+---------------------------------------------------------+\n");

    while (actual != NULL) {
        switch(actual->flagError) {
            case 0:
                printf("| %-15s | %-15s | %-15d | %-30s                                    \n", actual->identificador, actual->tipo, actual->numLinea, "Funcion declarada correctamente.");
                break;
            case 1:
                printf("| %-15s | %-15s | %-15d | %-30s                                   \n", actual->identificador, actual->tipo, actual->numLinea, "ERROR: Ya fue declarada previamente.");
                break;
            case 2:
                printf("| %-15s | %-15s | %-15d | %-30s                                   \n", actual->identificador, actual->tipo, actual->numLinea, "ERROR: Ya fue declarada previamente con otro tipo.");
                break;
            case 3:
                printf("| %-15s | %-15s | %-15d | %-30s                                   \n", actual->identificador, actual->tipo, actual->numLinea, "ERROR: Ya fue declarada previamente con otros parametros.");
                break;
            case 4:
                printf("| %-15s | %-15s | %-15d | %-30s                                   \n", actual->identificador, actual->tipo, actual->numLinea, "ERROR: Ya fue definida previamente.");
                break;
        }
        printf("+-----------------+-----------------+-----------------+---------------------------------------------------------+\n");
        actual = actual->siguiente;
    }
    printf("\n");
}


/*
void mostrarListaFuncionesDefinidas(NodoFunciones* cabeza) {
    NodoFunciones* actual = cabeza;
    printf("Listado de funciones definidas: \n");
    while (actual != NULL){
        printf("Identificador: %s, Tipo: %s, Nro Linea: %d \n", actual->identificador, actual -> tipo, actual->nroLinea);
        actual = actual->siguiente;
    }
    printf("\n");
}
*/

/*TP INTEGRADOR*/
//Funcion para chequear si en una invocacion de funcion los parametros son correctos
int parametrosBienInvocados(NodoParametros* parametrosDeclarados, NodoParametros* parametrosInvocados){
    
    while (parametrosDeclarados != NULL && parametrosInvocados != NULL) { //mientras sigo habiendo parametros en ambas listas sigo comparando
        
        if (strcmp(parametrosDeclarados->tipo, parametrosInvocados->tipo)) {
            return 0; //Si los parametros tienen distinto tipo, la funcion fue invocada erroneamente

        }
        parametrosDeclarados = parametrosDeclarados->siguiente;
        parametrosInvocados = parametrosInvocados->siguiente;
    }
    
   
    if (parametrosDeclarados != NULL || parametrosInvocados != NULL) {
        return 0; //Si la cantidad de parametros es distinta, la funcion fue invocada erroneamente
    }
    
    return 1; // Si no pasa ninguno de esos casos, la funcion fue invocada correctamente
}

void imprimirRestoParametros(NodoParametros* parametrosDeclarados){
    NodoParametros* actual = parametrosDeclarados;
    actual = actual->siguiente;
     while (actual != NULL) {
        const char* auxId = actual->identificador;
        const char* auxEspacio = " ";
        char* auxTipo = strcat(actual -> tipo, auxEspacio);
        char* parametro = strcat(auxTipo, auxId);
        printf("\n|-----------------|-----------------| %-15s |-----------------|---------------------------------------------------------|\n", parametro);
        actual = actual->siguiente;
     }
}

const char* primerParametro(NodoParametros* parametrosDeclarados){
    NodoParametros* actual = parametrosDeclarados;
    
    if(actual){
    const char* auxId = actual->identificador;
    char* auxTipo = strcat(actual -> tipo, " ");
    char* parametro = strcat(auxTipo, auxId);
    return parametro;
    }else{
    char* parametro = strdup(" ");
    return parametro;
    }
}

void mostrarListaFuncionesDeclaradasIntegrador(NodoFunciones* cabeza) {
    NodoFunciones* actual = cabeza;

    printf("\nListado de funciones declaradas:\n");
    printf("+-----------------+-----------------+-----------------+-----------------+---------------------------------------------------------+\n");
    printf("| %-15s | %-15s | %-15s | %-15s | %-30s \n", "Id_Funcion", "Tipo", "Paremetros", "Nro Linea", "Mensaje                            |");
    printf("+-----------------+-----------------+-----------------+-----------------+---------------------------------------------------------+\n");

    while (actual != NULL) {
        switch(actual->flagError) {
            case 0:
                printf("| %-15s | %-15s | %-15s | %-15d | %-30s \n", actual->identificador, actual->tipo, primerParametro(actual->parametros), actual->numLinea, "Funcion declarada correctamente.");
                //imprimirRestoParametros(actual -> parametros);
                break;
            case 1:
                printf("| %-15s | %-15s | %-15s | %-15d | %-30s \n", actual->identificador, actual->tipo, primerParametro(actual->parametros), actual->numLinea, "ERROR: Ya fue declarada previamente.");
                //imprimirRestoParametros(actual -> parametros);
                break;
            case 2:
                printf("| %-15s | %-15s | %-15s | %-15d | %-30s \n", actual->identificador, actual->tipo, primerParametro(actual->parametros), actual->numLinea, "ERROR: Ya fue declarada previamente con otro tipo.");
                //imprimirRestoParametros(actual -> parametros);
                break;
            case 3:
                printf("| %-15s | %-15s | %-15s | %-15d | %-30s \n", actual->identificador, actual->tipo, primerParametro(actual->parametros), actual->numLinea, "ERROR: Ya fue declarada previamente con otros parametros.");
                //imprimirRestoParametros(actual -> parametros);
                break;
            case 4:
                printf("| %-15s | %-15s | %-15s | %-15d | %-30s \n", actual->identificador, actual->tipo, primerParametro(actual->parametros), actual->numLinea, "ERROR: Ya fue definida previamente.");
                //imprimirRestoParametros(actual -> parametros);
                break;
        }
        printf("+-----------------+-----------------+-----------------+-----------------+---------------------------------------------------------+\n");
        actual = actual->siguiente;
    }
    printf("\n");
}



/* FUNCIONES TABLA DE SIMBOLOS DE VARIABLE */
// Definicion de la estructura del nodo tipo de variable
typedef struct NodoVarible {
    char* nombre;
    char* tipo;
    int numLinea;
    struct NodoVarible* siguiente;
} NodoVarible;

// Funcion para crear un nuevo nodo del tipo de variable
NodoVarible* crearNodo( char* tipo, char* nombre, int numeroLinea ) {
    NodoVarible* nuevoNodo = (NodoVarible*)malloc(sizeof(NodoVarible));
    nuevoNodo->nombre = strdup(nombre);
    nuevoNodo->tipo = strdup(tipo);
    nuevoNodo->numLinea = numeroLinea;
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

// Función para añadir un nodo al final de la lista para el tipo de varible
void agregarNodo(NodoVarible** lista, char* tipo, char* nombre, int numeroLinea ) {
    NodoVarible* nuevoNodo = crearNodo(tipo,nombre, numeroLinea);
    
    if (*lista == NULL) {
        *lista = nuevoNodo;
    } else {
        NodoVarible* actual = *lista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
}

void imprimirListaDeVars (NodoVarible* lista) {

    printf("\nListado de variables definidas:\n");
    printf("+--------+---------------+-------+\n");
    printf("| Tipo   | Identificador | Linea |\n");
    printf("+--------+---------------+-------+\n");

    NodoVarible* actual = lista;

    while (actual != NULL) {
        printf("| %-6s | %-13s | %-5i |\n", actual->tipo, actual->nombre, actual->numLinea );
        actual = actual->siguiente;
    }

    printf("+--------+---------------+-------+\n");
}

NodoVarible* listaDeVars = NULL;

void manejarDeclaraciones(char* tipo,char* identificador,int numLinea){
    agregarNodo(&listaDeVars ,tipo, identificador,numLinea);
}

// Funciones de sentencias

typedef struct NodoSentencias {
    char* tipoSentencia;
    int linea;
    struct NodoSentencias* sgte;
} NodoSentencias;

NodoSentencias* crearNodoSentencia(int tipoSent, int lin) {
    NodoSentencias* nuevoNodoSent = (NodoSentencias*)malloc(sizeof(NodoSentencias));
    nuevoNodoSent->tipoSentencia = (char*)malloc(50);  
    switch(tipoSent) {
        case 1: strcpy(nuevoNodoSent->tipoSentencia, "Sentencia Compuesta"); break;
        case 2: strcpy(nuevoNodoSent->tipoSentencia, "Sentencia de Expresion"); break;
        case 3: strcpy(nuevoNodoSent->tipoSentencia, "Sentencia de Seleccion"); break;
        case 4: strcpy(nuevoNodoSent->tipoSentencia, "Sentencia de Iteracion"); break;
        case 5: strcpy(nuevoNodoSent->tipoSentencia, "Sentencia de Salto"); break;
        default: break;
    }
    nuevoNodoSent->linea=lin;
    nuevoNodoSent->sgte = NULL;
    return nuevoNodoSent;
}

void agregarNodoSentencia(NodoSentencias** cabeza, int tipoSent, int lin) {
    NodoSentencias* nuevoNodoSent = crearNodoSentencia(tipoSent,lin);
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodoSent;
    } else {
        NodoSentencias* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodoSent;
    }
}
  
void mostrarListaSent(NodoSentencias* cabeza) {
    NodoSentencias* actual = cabeza;
    printf("\nListado de sentencias: \n");
    printf("+------------------------+--------------+\n");
    printf("|          Tipo          |     Linea    |\n");
    printf("+------------------------+--------------+\n");

    while (actual != NULL) {
        printf("| %-22s | %-12d |\n", actual->tipoSentencia, actual->linea);
        printf("+------------------------+--------------+\n");

        actual = actual->sgte;
    }        

}

// funciones para la impresion de errores sintaticos

NodoErrorSintatico* crearNodoErrorSintatico( int numeroLinea ) {
    NodoErrorSintatico* nuevoNodo = (NodoErrorSintatico*)malloc(sizeof(NodoErrorSintatico));
    nuevoNodo->numLinea = numeroLinea;
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

void agregarNodoErrorSintactico(NodoErrorSintatico** listaErrorSintatico, int numeroLinea ){
    NodoErrorSintatico* nuevoNodo = crearNodoErrorSintatico(numeroLinea);
    
    if (*listaErrorSintatico == NULL) {
        *listaErrorSintatico = nuevoNodo;
    } else {
        NodoErrorSintatico* actual = *listaErrorSintatico;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
};

void imprimirListaDeErrorSintactico (NodoErrorSintatico* listaErrorSintatico){
    NodoErrorSintatico* actual = listaErrorSintatico;
    printf("\nLista de Errores Sintacticos: \n");
    printf("+------------------------------+\n");
    printf("|             Linea            |\n");
    printf("+------------------------------+\n");

    while (actual != NULL) {
        printf("| %-28d |\n", actual->numLinea);
        printf("+------------------------------+\n");

        actual = actual->siguiente;
    }        

};

