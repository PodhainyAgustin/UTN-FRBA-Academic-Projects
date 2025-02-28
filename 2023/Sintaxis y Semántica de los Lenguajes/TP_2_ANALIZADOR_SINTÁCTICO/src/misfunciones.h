#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*********************************************************************************************************************/

/// FUNCIONES PARA LAS CONSTANTES OCTALES
int octalADecimal(int octal) {
    int numDecimal=0,exponente=0;
    while(octal>0){
        numDecimal= numDecimal + (octal%10 * pow(8,exponente)); 
        octal= octal/ 10; 
        exponente++;
    }
    return numDecimal;
 }


typedef struct NodoOctales {
    char* constanteOctal;
    int octalADecimal;
    struct NodoOctales* sgte;
} NodoOctales;

NodoOctales* crearNodoOctal(char* cteoctal) {
    NodoOctales* nuevoNodoOctal = (NodoOctales*)malloc(sizeof(NodoOctales));
    int numOctal;
    nuevoNodoOctal->constanteOctal = strdup(cteoctal);
    numOctal=atoi(cteoctal);
    nuevoNodoOctal->octalADecimal= octalADecimal(numOctal);
    nuevoNodoOctal->sgte = NULL;
    return nuevoNodoOctal;
}
void agregarNodoOctal(NodoOctales** cabeza, char* cteoctal) {
    NodoOctales* nuevoNodoOctal = crearNodoOctal(cteoctal);
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodoOctal;
    } else {
        NodoOctales* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodoOctal;
    }
}
  
void mostrarListaOctales(NodoOctales* cabeza) {
    NodoOctales* actual = cabeza;
    printf("\nListado de constantes octales: \n");
    while (actual != NULL) {
        printf("La constante octal %s convertido a decimal es : %d\n", actual->constanteOctal, actual->octalADecimal);
      
    NodoOctales* nodoAEliminar = actual;
        actual = actual->sgte;  
        free(nodoAEliminar); 
    }
    printf("\n");}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//FUNCIONES PARA LAS CONSTANTES HEXADECIMALES
int hexaADecimal(char hexa[],int tam){
    int numDecimal=0, exponente=0;
    // strtol(texto, puntero(null), base)
   for(int i=tam-1; i>=2; i--)
   { 
        char c = hexa[i];
        int digito = 0;
        if (c >= '0' && c <= '9')
            digito = c - '0';
        else if (c >= 'A' && c <= 'F')
            digito=10+(c-'A');
        else if (c >= 'a' && c <= 'f')
         digito=10+(c-'a');

        numDecimal = numDecimal + (digito * pow(16, exponente));
        exponente++;
    }
    return numDecimal;
}
 
typedef struct NodoHexadecimales {
    char* constanteHexa;
    int hexaAdec;
    struct NodoHexadecimales* sgte;
} NodoHexadecimales;


NodoHexadecimales* crearNodoHexa(char* ctehexa, int tam) {
    NodoHexadecimales* nuevoNodoHexa = (NodoHexadecimales*)malloc(sizeof(NodoHexadecimales));
    nuevoNodoHexa->hexaAdec=hexaADecimal(ctehexa,tam);
    nuevoNodoHexa->constanteHexa = strdup(ctehexa);  
    nuevoNodoHexa->sgte = NULL;
    return nuevoNodoHexa;
}

void agregarNodoHexa(NodoHexadecimales** cabeza, char* ctehexa, int tam) {
    NodoHexadecimales* nuevoNodoHexa = crearNodoHexa(ctehexa, tam);
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodoHexa;
    } else {
        NodoHexadecimales* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodoHexa;
    }
}

void mostrarListaHexa(NodoHexadecimales* cabeza) {
    NodoHexadecimales* actual = cabeza;
    printf("Listado de constantes hexadecimales: \n");
    while (actual != NULL) {
        printf("La constante hexadecimal %s convertido a decimal es : %d \n", actual->constanteHexa, actual->hexaAdec);

        NodoHexadecimales* nodoAEliminar = actual;
        actual = actual->sgte; 
        free(nodoAEliminar); 
    }
    printf("\n");}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES PARA LAS CONSTANTES DECIMALES
typedef struct NodoDecimales {
    char* constanteDec;
    struct NodoDecimales* sgte;
} NodoDecimales;


NodoDecimales* crearNodoDec(char* ctedec) {
    NodoDecimales* nuevoNodoDec = (NodoDecimales*)malloc(sizeof(NodoDecimales));
    nuevoNodoDec->constanteDec = strdup(ctedec);  
    nuevoNodoDec->sgte = NULL;
    return nuevoNodoDec;
}

// Función para añadir un nodo al final de la lista
void agregarNodoDecimal(NodoDecimales** cabeza, char* ctedec) {
    NodoDecimales* nuevoNodoDec = crearNodoDec(ctedec);
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodoDec;
    } else {
        NodoDecimales* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodoDec;
    }
}


void mostrarListaDecimal(NodoDecimales* cabeza) {
    NodoDecimales* actual = cabeza;
    printf("Listado de constantes decimales: \n");
    while (actual != NULL) {
        printf("Se encontro la constante decimal : %s \n", actual->constanteDec);
        NodoDecimales* nodoAEliminar = actual;
        actual = actual->sgte;  
        free(nodoAEliminar);
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES PARA LAS CONSTANTES Reales

int cantidadSinSufijo(char numero[]) { 
    int cant = strlen(numero);
    if (numero[cant - 1] == 'f' || numero[cant - 1] == 'F' || numero[cant - 1] == 'l' || numero[cant - 1] == 'L') {
        numero[cant - 1] = '\0';
        cant--;
    }
    return cant;
}

int calcularEntero(char numero[], int tam) { 
    tam=cantidadSinSufijo(numero);
    int posicion = 0;
    int entero = 0;
    int signo = 1;
    if (numero[0] == '-') {
        signo = -1;
        posicion++; 
    }
    while (numero[posicion] != '.') {
        if (numero[posicion] == 'e' || numero[posicion] == 'E') {
            break;
        }
        entero = entero * 10 + (numero[posicion] - '0');
        posicion++;
    }

    return entero * signo; 
}
double calcularMantisa(char numero[], int tam) {
    tam=cantidadSinSufijo(numero);
    int posicion = 0;
    double mantisa = 0.0;
    double divisor = 10.0; 
    int signo = 1;
    if (numero[0] == '-') {
        signo = -1;
        posicion++; 
    }

    while (numero[posicion] != '.' && numero[posicion] != '\0') {
        posicion++;
    }

    for (int i = posicion + 1; i < tam; i++) {
        if (numero[i] == 'e' || numero[i] == 'E') { 
            break;
        }
        mantisa = mantisa + ((double)(numero[i] - '0') / divisor);
        divisor *= 10.0;
    }

    return mantisa * signo;
}


int obtenerExponente(char numero[], int tam) {
    tam=cantidadSinSufijo(numero);
    int exponente = 0;
    int posicion = 0;

    for (int i = 0; i < tam; i++) {
        if (numero[i] == 'e' || numero[i] == 'E') {
            int posicionexp = i + 1;
            int signoexp = 1;    

            if (numero[posicionexp] == '-') {
                signoexp = -1;
                posicionexp++; 
            }

            exponente = atoi(&numero[posicionexp]) * signoexp;
            break;
        }
    }
    return exponente;
}

double resultado(char numero[], int tam) {
    int parte_entera = calcularEntero(numero, tam);
    double mantisa = calcularMantisa(numero, tam);
    int exponente = obtenerExponente(numero, tam);
    // funcion modf
    if (exponente != 0) {
        double resultado_final = (double)parte_entera + mantisa;

        if (exponente > 0) {
            while (exponente > 0) {
                resultado_final *= 10;
                exponente--;
            }
        } else if (exponente < 0) {
            while (exponente < 0) {
                resultado_final /= 10;
                exponente++;
            }
        }
        return resultado_final;
    }
    return parte_entera + mantisa;
}


double obtenerMantisa(char numero[], int tam) {
    double resultado_real = resultado(numero, tam);
    int parte_entera = (int)resultado_real;
    double parte_decimal = resultado_real - parte_entera;
    return parte_decimal;
}

int obtenerEntero(char numero[], int tam) {
    double resultado_real = resultado(numero, tam);
    int parte_entera = (int)resultado_real;
    return parte_entera;
}

typedef struct NodoReales {
    char* constantereal;
    int entero;
    float mantisa;
    struct NodoReales* sgte;
} NodoReales;


NodoReales* crearNodoReal(char* ctereal, int tam) {
    NodoReales* nuevoNodoReal = (NodoReales*)malloc(sizeof(NodoReales));
    nuevoNodoReal->constantereal = strdup(ctereal);  
    nuevoNodoReal->entero=obtenerEntero(ctereal,tam);
    nuevoNodoReal->mantisa=obtenerMantisa(ctereal,tam);
    nuevoNodoReal->sgte = NULL;
    return nuevoNodoReal;
}

void agregarNodoReal(NodoReales** cabeza, char* ctereal, int tam) {
    NodoReales* nuevoNodoReal = crearNodoReal(ctereal, tam);
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodoReal;
    } else {
        NodoReales* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodoReal;
    }
}

void mostrarListaReales(NodoReales* cabeza) {
    NodoReales* actual = cabeza;
    printf("Listado de constantes reales: \n");
    while (actual != NULL) {
        printf("Se encontro la constante real : %s \n", actual->constantereal);
        printf("Su parte entera es %d \n", actual->entero);
        printf("Su mantisa es %f \n", actual->mantisa);

        NodoReales* nodoAEliminar = actual;
        actual = actual->sgte;  
        free(nodoAEliminar);   
        }
    printf("\n");}

////////////////////////////////////////////////////////////////////////////////////////
//FUNCIONES PARA LAS CONSTANTES CARACTER
typedef struct NodoCaracteres {
    char* constanteCaracter;
    struct NodoCaracteres* sgte;
} NodoCaracteres;

NodoCaracteres* crearNodoCaracteres(char* ctecaracter) {
    NodoCaracteres* nuevoNodoCaracter = (NodoCaracteres*)malloc(sizeof(NodoCaracteres));
    nuevoNodoCaracter->constanteCaracter = strdup(ctecaracter);
    nuevoNodoCaracter->sgte = NULL;
    return nuevoNodoCaracter;
}

void agregarNodoCaracter(NodoCaracteres** cabeza, char* ctecaracter) {
    NodoCaracteres* nuevoNodoCaracter = crearNodoCaracteres(ctecaracter);

    if (*cabeza == NULL) {
        *cabeza = nuevoNodoCaracter;
    } else {
        NodoCaracteres* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodoCaracter;
    }
}

void mostrarListaCaracteres(NodoCaracteres* cabeza) {
    NodoCaracteres* actual = cabeza;
    printf("Listado de constantes caracter:\n");
    int numcaracter = 1;
    while (actual != NULL) {
        printf("%d) La constante caracter %s\n", numcaracter, actual->constanteCaracter);
        NodoCaracteres* nodoAEliminar = actual;
        actual = actual->sgte;  
        free(nodoAEliminar); 
        numcaracter++;
    }
    printf("\n");
    }

//Palabras Reservadas
typedef struct NodoPalabrasReservadas{
     char* palabraReservada;
     struct NodoPalabrasReservadas* sgte;
}NodoPalabrasReservadas;

NodoPalabrasReservadas* crearNodoPR(char* palabra) {
    NodoPalabrasReservadas* nuevoNodo = (NodoPalabrasReservadas*)malloc(sizeof(NodoPalabrasReservadas));
    nuevoNodo->palabraReservada = strdup(palabra);
    nuevoNodo->sgte = NULL;
    return nuevoNodo;
};

void insertarUltimo(NodoPalabrasReservadas **cabeza, char* aInsertar){
    NodoPalabrasReservadas* nuevoNodo = crearNodoPR(aInsertar);
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoPalabrasReservadas* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodo;
    }
}

void mostrarListaPR(NodoPalabrasReservadas* cabeza) {
    NodoPalabrasReservadas* actual = cabeza;
    printf("Listado de palabras reservadas: \n");
    while (actual != NULL) {
        printf("palabra reservada: %s\n", actual->palabraReservada);
        actual = actual->sgte;
    }
    printf("\n");
}

/*********************************************************************************************************************/
typedef struct NodoIdentificadores{
    char* identificador;
    int apariciones;
    struct NodoIdentificadores* sgte;
}NodoIdentificadores;

NodoIdentificadores* crearNodoID(char* palabra) {
    NodoIdentificadores* nuevoNodo = (NodoIdentificadores*)malloc(sizeof(NodoIdentificadores));
    nuevoNodo->identificador = strdup(palabra);
    nuevoNodo->apariciones = 1;
    nuevoNodo->sgte = NULL;
    return nuevoNodo;
}

NodoIdentificadores* buscarNodoID(NodoIdentificadores* cabeza, char* cadena) {
    NodoIdentificadores* actual = cabeza;

    while (actual != NULL) {
        if (strcmp(actual->identificador, cadena) == 0) {
            actual->apariciones += 1;
            return actual;
        }
        actual = actual->sgte;
    }

    return NULL;
}

void insertarEnOrdenAlfabetico(NodoIdentificadores** cabeza, char* id) {
    NodoIdentificadores* nuevoNodo = crearNodoID(id);
    
    if (*cabeza == NULL || strcmp((*cabeza)->identificador, id) > 0) {
        // Insertar al principio de la lista o si la lista está vacía
        nuevoNodo->sgte = *cabeza;
        *cabeza = nuevoNodo;
    } else{
     NodoIdentificadores* actual = *cabeza;
        while (actual->sgte != NULL && (strcmp(actual->sgte->identificador, id) < 0))
            actual = actual->sgte;
        // Insertar el nuevo nodo después del nodo actual
        nuevoNodo->sgte = actual->sgte;
        actual->sgte = nuevoNodo;
    }
}

void insertarEnOrdenAlfabeticoYContandoApariciones(NodoIdentificadores** lista, char* cadena) {
    NodoIdentificadores* encontro = buscarNodoID(*lista, cadena);
    if (encontro == NULL) {
        insertarEnOrdenAlfabetico(lista, cadena);
    }
}
void mostrarListaID(NodoIdentificadores* cabeza) {
    NodoIdentificadores* actual = cabeza;
    printf("Listado de identificadores: \n");
    while (actual != NULL) {
        printf("identificador: %s \n", actual->identificador);
        printf("apariciones: %d \n", actual->apariciones);
        actual = actual->sgte;
    }
    printf("\n");
}

/*********************************************************************************************************************/

//Literales Cadena
typedef struct NodoLiteralesCadena{
     char* literalCadena;
     int longitud;
     struct NodoLiteralesCadena* sgte;
} NodoLiteralesCadena;

NodoLiteralesCadena* crearNodoLC(char* palabra, int longitudCadena) {
    NodoLiteralesCadena* nuevoNodo = (NodoLiteralesCadena*)malloc(sizeof(NodoLiteralesCadena));
    nuevoNodo->literalCadena = strdup(palabra);
    nuevoNodo->longitud = longitudCadena;
    nuevoNodo->sgte = NULL; 
    return nuevoNodo;
}

void insertarOrdenadoPorLongitud(NodoLiteralesCadena** cabeza, char* cadena, int longitudActual) {
    NodoLiteralesCadena* nuevoNodo = crearNodoLC(cadena, longitudActual);

    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else if ((*cabeza)->longitud >= longitudActual) {
        nuevoNodo->sgte = *cabeza;
        *cabeza = nuevoNodo;
    } else {
        NodoLiteralesCadena* aux = *cabeza;
        while (aux->sgte != NULL && aux->sgte->longitud < longitudActual) {
            aux = aux->sgte;
        }
        nuevoNodo->sgte = aux->sgte;
        aux->sgte = nuevoNodo;
    }
}

NodoLiteralesCadena* buscarNodoLC(NodoLiteralesCadena* cabeza, char* cadena) {
    NodoLiteralesCadena* actual = cabeza;

    while (actual != NULL) {
        if (strcmp(actual->literalCadena, cadena) == 0) {
            return actual;
        }
        actual = actual->sgte;
    }

    return NULL;
}

void insertarOrdenadoPorLongitudYsinRepetir(NodoLiteralesCadena** lista, char* cadena, int longitud) {
    NodoLiteralesCadena* encontro = buscarNodoLC(*lista, cadena);
    if (encontro == NULL) {
        insertarOrdenadoPorLongitud(lista, cadena, longitud);
    }
}

void mostrarListaLC(NodoLiteralesCadena* cabeza) {
    NodoLiteralesCadena* actual = cabeza;
    printf("Listado de literales cadena: \n");
    while (actual != NULL) {
        printf("literal cadena: %s \n", actual->literalCadena);
        printf("longitud: %d \n", actual->longitud);
        actual = actual->sgte;
    }
    printf("\n");
}

// operadores/caracteres de puntuación
typedef struct NodoOpCarac {
    char* caracter;
    int cant;
    struct NodoOpCarac* sgte;
} NodoOpCarac;

NodoOpCarac* buscarNodo(NodoOpCarac* lista, char* dato) {
    NodoOpCarac* actual = lista;

    
    while (actual != NULL) {
        if (strcmp(actual->caracter, dato) == 0) {
           
            return actual;
        }
        actual = actual->sgte;
    }

    return NULL;
}

// Función para crear un nuevo nodo
NodoOpCarac* crearNodo(char* dato) {
    NodoOpCarac* nuevoNodo = (NodoOpCarac*)malloc(sizeof(NodoOpCarac));
    nuevoNodo->cant = 1;
    nuevoNodo->caracter = strdup(dato);  // Usamos strdup para copiar el string
    nuevoNodo->sgte = NULL;

    return nuevoNodo;
}

// Función para añadir un nodo al final de la lista
NodoOpCarac agregarNodo(NodoOpCarac** cabeza, char* dato) {
    NodoOpCarac* nuevoNodo = crearNodo(dato);
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoOpCarac* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodo;
    }
}

// Función para imprimir los elementos de la lista
void imprimirLista(NodoOpCarac* cabeza) {
    printf("Lista de operadores y caracteres:\n");
    NodoOpCarac* actual = cabeza;
    int contador = 0;

    while (actual != NULL) {
        printf("El caracter \"%s\" aparecio %i veces\n", actual->caracter, actual->cant);
        contador = contador + actual->cant;
        actual = actual->sgte;
    }
    printf("El total de caracteres u operadores encontrado fue de: %i\n\n",contador);
}

void manejarOperadoresCaracteres(NodoOpCarac** lista, char* yytext) {
    // imprimirLista(*lista);
    NodoOpCarac* nodo = buscarNodo(*lista, yytext);

    if(nodo != NULL){
        nodo->cant++;
    } else {
        agregarNodo(lista, yytext);
    }
    // printf("Caracteres de puntuación/operadores de C: %s \n", yytext);
};

// Comentarios de una linea
typedef struct NodoComent {
    char* comentario;
    struct NodoComent* sgte;
} NodoComent;

// Comentarios de múltiple lineas


void agregarNodoComent(NodoComent** cabeza, char* dato) {
    NodoComent* nuevoNodo = (NodoComent*)malloc(sizeof(NodoComent));
    nuevoNodo->comentario = strdup(dato); 
    nuevoNodo->sgte = NULL;
    
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoComent* actual = *cabeza;
        while (actual->sgte != NULL) {
            actual = actual->sgte;
        }
        actual->sgte = nuevoNodo;
    }
}

void imprimirListaComentarios(NodoComent* cabeza) {
    NodoComent* actual = cabeza;

    while (actual != NULL) {
        actual->comentario = actual->comentario;
        printf("%s ", actual->comentario);
        actual = actual->sgte;
    }

    printf("\n");
}

// Lista de caracteres no reconocidos
typedef struct NodoCaractNoReconocidos {
    char* caracter;
    int linea;
    struct NodoCaractNoReconocidos* sgte;
} NodoCaractNoReconocidos;

NodoCaractNoReconocidos* buscarUltimoNodo(NodoCaractNoReconocidos* lista) {
    NodoCaractNoReconocidos* actual = lista;

    while (actual != NULL && actual->sgte != NULL) {
        actual = actual->sgte;
    }

    return actual;
}

void agregarNodoCaracnoReco (NodoCaractNoReconocidos** cabeza, char* dato) {
    NodoCaractNoReconocidos* nuevoNodo = (NodoCaractNoReconocidos*)malloc(sizeof(NodoCaractNoReconocidos));
    nuevoNodo->caracter = strdup(dato); 
    nuevoNodo->sgte = NULL;
    
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

void concatenarACadenaEnNodo(NodoCaractNoReconocidos* nodo, char* nuevaCadena) {
    size_t len_cadena_existente = strlen(nodo->caracter);
    size_t len_nueva_cadena = strlen(nuevaCadena);

    nodo->caracter = (char*)realloc(nodo->caracter,len_cadena_existente + len_nueva_cadena + 1);

    strcat(nodo->caracter, nuevaCadena);

}

void caracteresNoReconocidos (NodoCaractNoReconocidos** listaCaracNoRecc, char* caracter, int lineaAct, int flag){
    NodoCaractNoReconocidos* ultimoNodo = buscarUltimoNodo(*listaCaracNoRecc);

    if(ultimoNodo != NULL && ultimoNodo->linea == lineaAct){
        if(flag == 0){
            concatenarACadenaEnNodo(ultimoNodo," ");
        }
        concatenarACadenaEnNodo(ultimoNodo,caracter);       
    } else {
        NodoCaractNoReconocidos* nuevoNodo = (NodoCaractNoReconocidos*)malloc(sizeof(NodoCaractNoReconocidos));
        nuevoNodo->caracter = strdup(caracter); 
        nuevoNodo->linea = lineaAct; 
        nuevoNodo->sgte = NULL;
        
        if(ultimoNodo != NULL) {
            ultimoNodo->sgte = nuevoNodo;
        } else {
            *listaCaracNoRecc = nuevoNodo;
        }
    }
}

void imprimirListaCaracNoReco(NodoCaractNoReconocidos* listaCaracNoRecc){
    printf("\nListado de caracteres no reconocidos:\n");
    NodoCaractNoReconocidos* actual = listaCaracNoRecc;

    while (actual != NULL) {

        printf("Cadenas no reconocidas: \"");

        char* cadena = actual->caracter;
        while (*cadena != '\0') {
            if(*cadena == ' '){
                printf("\" \"");
            } else {
                printf("%c", *cadena);
            }

           cadena++;  // Movemos el puntero al siguiente caracter
        }

        printf("\" aparecieron en la linea %i \n", actual->linea);

        actual = actual->sgte;
    }
}

/*********************************************************************************************************************/
