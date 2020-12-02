#ifndef ARBOL_H
#define ARBOL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct nodoArbol {
  char valor[255];
  struct nodoArbol *ptrIzq;
  struct nodoArbol *ptrDer;
} nodoArbol;

typedef nodoArbol *ptrArbol;

FILE* file;
FILE* fileGraphviz;

void inicializarArbol();
ptrArbol crearHoja(char*);
ptrArbol crearNodo(char*, ptrArbol, ptrArbol);
void guardarArbol(nodoArbol *);
void inOrder(ptrArbol);
void graf(ptrArbol);

char aux[100];
char auxPadre[100];

#endif