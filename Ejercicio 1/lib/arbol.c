#include "arbol.h"
#include "util.h"
#include "assembler.h"

void inicializarArbol(ptrArbol arbol){
  arbol = NULL;
}

ptrArbol crearHoja(char* valor){
    ptrArbol ptrNodo;
    ptrNodo = malloc(sizeof(nodoArbol));
    
    strcpy((ptrNodo)->valor,valor);
    //printf("Consola: Guardando Valor %s en la hoja con el tipo %s\n", (ptrNodo)->valor,  ptrNodo->tipoNodo);
    (ptrNodo)->ptrIzq = NULL;
    (ptrNodo)->ptrDer = NULL;

    return ptrNodo;
}

ptrArbol crearNodo(char* valor, ptrArbol ptrArbolIzq, ptrArbol ptrArbolDer) {
    ptrArbol ptrNodo;
    ptrNodo = malloc(sizeof(nodoArbol));
   
    strcpy((ptrNodo)->valor,valor);
    //printf("Consola: Guardando Valor %s en el nodo con el tipo %s\n", (ptrNodo)->valor, (ptrNodo)->tipoNodo);
    (ptrNodo)->ptrIzq = ptrArbolIzq;
    (ptrNodo)->ptrDer = ptrArbolDer;

    return ptrNodo;
}


void inOrder(ptrArbol ptrArbol)
{ 
   if (ptrArbol != NULL) {
    inOrder(ptrArbol->ptrIzq);
    fprintf(file, "%s ", ptrArbol->valor);
    if(strstr(ptrArbol->valor,"subArbol_") != 0){
        fprintf(file, "\n");
    }
    inOrder(ptrArbol->ptrDer);
  }
}

void graf(ptrArbol ptrArbol)
{ 
   if (ptrArbol != NULL) {
    if((ptrArbol)->ptrIzq != NULL){
        strcpy(aux,ptrArbol->ptrIzq->valor);
        removeChar(aux,'@');
        fprintf(fileGraphviz, "\"%s\" -> \"%s\" ;\n\t", ptrArbol->valor, aux);
        graf(ptrArbol->ptrIzq);
    }
    if((ptrArbol)->ptrDer != NULL){
        strcpy(aux,ptrArbol->ptrDer->valor);
        removeChar(aux,'@');
        fprintf(fileGraphviz, "\"%s\" -> \"%s\" ;\n\t", ptrArbol->valor, aux);
        graf(ptrArbol->ptrDer);
    }
  }
}

void guardarArbol(nodoArbol * arbol) {
    nodoArbol* aux = arbol;
    file = fopen("intermedia.txt", "w");
    if (file == NULL)
    {
        printf("No se pudo crear la tabla intermedia.\n");
        exit(1);
    }

    fileGraphviz = fopen("graphviz.dot", "w");
    if (fileGraphviz == NULL)
    {
        printf("No se pudo crear el árbol en Graphviz.\n");
        exit(1);
    }
    inOrder(aux);
    fprintf(fileGraphviz, "digraph G {\n\t");
    graf(aux);
    fprintf(fileGraphviz,"\n}");
    fclose(file);
    fclose(fileGraphviz);
}
