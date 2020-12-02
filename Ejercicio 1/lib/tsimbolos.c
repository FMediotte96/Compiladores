#include "tsimbolos.h"

void crearTS() {
    tablaSimbolos.primero = NULL;
}

int insertarEnTS(const char* nombre, const char* tipo, const char* valStr, int valInt, double valDouble) {
    t_nodo *tabla = tablaSimbolos.primero;
    char nombreCTE[100] = "_";
    strcat(nombreCTE, nombre);

    //Muevo el punto hasta el último elemento de la tabla de simbolos
    while(tabla) {
        //Reviso si ya existe en la tabla de simbolos, si ya existe no lo inserto
        if(strcmp(tabla->data.nombre, nombre) == 0 || strcmp(tabla->data.nombre, nombreCTE) == 0){
            return 1;
        }
        
        if(tabla->siguiente == NULL){
            break;
        }
        tabla = tabla->siguiente;
    }

    //Reservo memoria
    t_data *data = (t_data*)malloc(sizeof(t_data));
    data = crearDatos(nombre,tipo,valStr,valInt,valDouble);

    if(data == NULL) {
        return 1;
    }

    t_nodo* nuevo = (t_nodo*)malloc(sizeof(t_nodo));
    if(nuevo == NULL){
        return 2;
    }

    nuevo->data = *data;
    nuevo->siguiente = NULL;

    if(tablaSimbolos.primero==NULL) {
        tablaSimbolos.primero = nuevo;
    } else {
        tabla->siguiente = nuevo;
    }

    return 0;
}

t_data* crearDatos(const char* nombre, const char* tipo, const char* valStr, int valInt, double valDouble){
    char full[100] = "_";
    char aux[20];
    t_data *data = (t_data*)calloc(1, sizeof(t_data));
    if(data == NULL){
        return NULL;
    }
    data->tipo = (char*)malloc(sizeof(char) * (strlen(tipo) + 1));
    strcpy(data->tipo, tipo);
    //printf("Nombre: %s, Tipo: %s\n", nombre, data->tipo);
    if(strcmp(tipo, "CTE_STRING") == 0) {
        data->valor.valor_string = (char*)malloc(sizeof(char) * strlen(valStr) +1);
        data->nombre = (char*)malloc(sizeof(char) * (strlen(nombre) + 1));
        data->longitud = strlen(valStr);
        strcpy(data->nombre,nombre);
        strcpy(data->valor.valor_string,valStr);
    }
    if(strcmp(tipo, "CTE") == 0){
        sprintf(aux, "%d", valInt);
        strcat(full, aux);
        data->nombre = (char*)malloc(sizeof(char) * strlen(full));
        strcpy(data->nombre, full);
        data->valor.valor_int = valInt;
    }
    if(strcmp(tipo, "ID") == 0 || strcmp(tipo, "auxCode") == 0){
        data->nombre = (char*)malloc(sizeof(char) * strlen(nombre) + 1);
        strcpy(data->nombre, nombre);
    }
    return data;
}

void guardarTS() {
    FILE* archivo;
    if((archivo = fopen("ts.txt", "wt")) == NULL){
        printf("\nNo se pudo crear la tabla de simbolos.\n\n");
        return;
    } else if(tablaSimbolos.primero == NULL){
        return;
    }
    
    fprintf(archivo, "%-60s%-30s%-50s%-50s\n", "NOMBRE", "TIPO", "VALOR", "LONGITUD");

    t_nodo *aux;
    t_nodo *tabla = tablaSimbolos.primero;
    char linea[300];

    while(tabla) {
        aux = tabla;
        tabla = tabla->siguiente;

        if(strcmp(aux->data.tipo, "CTE_STRING") == 0) {
            sprintf(linea, "%-60s%-30s%-50s%d\n", aux->data.nombre, aux->data.tipo, aux->data.valor.valor_string, aux->data.longitud);
        } else if (strcmp(aux->data.tipo, "CTE") == 0) {
            sprintf(linea, "%-60s%-30s%d\n", aux->data.nombre, aux->data.tipo, aux->data.valor.valor_int);
        } else if (strcmp(aux->data.tipo, "ID") == 0 || strcmp(aux->data.tipo, "auxCode") == 0) {
            sprintf(linea, "%-60s\n", aux->data.nombre);
        }
        fprintf(archivo, "%s", linea);
    }
    fclose(archivo);
}


t_nodo * getLexema(const char *valor){
    t_nodo *lexema;
    t_nodo *tablaSimb = tablaSimbolos.primero;

    int encontro = -1;

    while(tablaSimb){ 
        encontro = strcmp(tablaSimb->data.nombre, valor);
       
        if(encontro == 0) 
        { 
            lexema = tablaSimb;
            return lexema;
        }
        tablaSimb = tablaSimb->siguiente;
    }
    return NULL;
}