#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/// Funciones Listado Caracteres No Reconocidos

typedef struct NodoCaractNoReconocidos {
    const char* caracter;
    int linea;
    int primerCol;
    int ultCol;
    struct NodoCaractNoReconocidos* sgte;
} NodoCaractNoReconocidos;

NodoCaractNoReconocidos* crearNodoCNR(const char* caracter, int linea, int col) {
    NodoCaractNoReconocidos* nuevoNodo = (NodoCaractNoReconocidos*)malloc(sizeof(NodoCaractNoReconocidos));
    nuevoNodo->caracter = strdup(caracter);
    nuevoNodo->linea = linea;
    nuevoNodo->primerCol = col;
    nuevoNodo->ultCol = col;
    nuevoNodo->sgte = NULL;
    return nuevoNodo;
}

NodoCaractNoReconocidos* buscarUltimoNodo(NodoCaractNoReconocidos* lista) {
    NodoCaractNoReconocidos* actual = lista;

    while (actual != NULL && actual->sgte != NULL) {
        actual = actual->sgte;
    }

    return actual;
}

void agregarNodoCaracNoReco(NodoCaractNoReconocidos** cabeza, const char* caracter, int linea, int col) {
    NodoCaractNoReconocidos* nuevoNodo = crearNodoCNR(caracter, linea, col);
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoCaractNoReconocidos* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodo;
    }
}


void insertarCaracteresNoReconocidos(NodoCaractNoReconocidos** listaCaracNoRecc, const char* palabra, int lineaAct, int colAct){
    NodoCaractNoReconocidos* ultimoNodo = buscarUltimoNodo(*listaCaracNoRecc);
    
    agregarNodoCaracNoReco(listaCaracNoRecc, palabra, lineaAct, colAct);
}

void imprimirListaCaracNoReco(NodoCaractNoReconocidos* listaCaracNoReco) {
    printf("\nListado de Errores Lexicos:\n");
    printf("+------+--------+-------------------------------+\n");
    printf("|Fila  |Columna |            Cadena             |\n");
    printf("+------+--------+-------------------------------+\n");

    NodoCaractNoReconocidos* actual = listaCaracNoReco;

    while (actual != NULL) {
        printf("| %-4d | %-6d | %-30s| \n", actual->linea, actual->primerCol, actual->caracter);
        actual = actual->sgte;
    }

    printf("+------+--------+-------------------------------+\n");
}



