#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "arbol.h"

void generarAssembler(ptrArbol*);
void escribir_seccion_datos(FILE *);
void escribir_seccion_codigo(FILE *, ptrArbol*);
void escribir_etiquetas_validacion(FILE *);
int esCmp(char*);
int esOperacion(char*);

int cantCTE = 1;
char msjAux[50];
t_nodo* lexema;

#endif