#include <stdio.h>
#include <stdlib.h>
#include "tsimbolos.h"
#include "assembler.h"
#include "arbol.h"

void generarAssembler(ptrArbol* pa) {
    FILE *archivo;

    if((archivo = fopen("Final.asm", "w"))==NULL){
        printf("No se puede crear el archivo \"Final.asm\"\n");
        exit(1);
    }

    fprintf(archivo, "include macros2.asm\ninclude number.asm\n.MODEL LARGE\n.386\n.STACK 200h\n\n");

    escribir_seccion_datos(archivo);

    fprintf(archivo, ".CODE\n\n");
    
    escribir_etiquetas_validacion(archivo);
    
    fprintf(archivo,"\nSTART:\n\nMOV AX,@DATA\nMOV DS, AX\nMOV ES, AX\n\n");

    escribir_seccion_codigo(archivo,pa);

    fprintf(archivo, "MOV EAX, 4C00h\nINT 21h\n\nEND START\n");

    fclose(archivo);
}

void escribir_seccion_datos(FILE *archivoAssembler) {
    int i;
	char valorAux[100];
    char buffer[10];

	fprintf(archivoAssembler, ".DATA\n");
    	
    t_nodo *aux;
    t_nodo *tabla = tablaSimbolos.primero;
    char linea[100];

	while(tabla)
	{
        aux = tabla;
        //printf("Aux data.nombre: %s\n", aux->data.nombre);
        if(strcmp(aux->data.tipo, "CTE") == 0)
        {
            strcpy(buffer,".0");
            itoa(aux->data.valor.valor_int,valorAux,10);
            strcat(valorAux,buffer);
            sprintf(linea, "%s dd %s\n", aux->data.nombre, valorAux);        
        } 
        else if(strcmp(aux->data.tipo, "CTE_STRING") == 0)
        {
            strcpy(valorAux,aux->data.valor.valor_string);
            sprintf(linea, "%s db %s , '$', %d dup (?)\n", aux->data.nombre, valorAux, aux->data.longitud);         
        }
        else if(strcmp(aux->data.tipo, "ID") == 0 || strcmp(aux->data.tipo, "auxCode") == 0)
        {
            sprintf(linea, "%s dd ?\n", aux->data.nombre);
        }

        fprintf(archivoAssembler, "%s",linea);
        tabla = tabla->siguiente;
	}

	fprintf(archivoAssembler, "\n");	
}


void escribir_seccion_codigo(FILE *archivoAssembler, ptrArbol* pa)
{
    if(!*pa)
        return;

    // para recorrer por izquierda
    escribir_seccion_codigo(archivoAssembler, &(*pa)->ptrIzq);
    //printf("Valor: %s\n", (*pa)->valor);

    if((*pa)->ptrDer != NULL || (*pa)->ptrIzq != NULL) {
        switch(esOperacion(((*pa)->valor)))
        {
            case 1:
                lexema = getLexema((*pa)->ptrIzq->valor);
                if(strcmp(lexema->data.tipo,"ID") == 0){
                    fprintf(archivoAssembler,"DisplayFloat %s,2\nNEWLINE\n\n", (*pa)->ptrIzq->valor);
                } else {
                    fprintf(archivoAssembler,"mov dx,OFFSET %s\nmov ah,9\nint 21h\nnewline 1\n\n", (*pa)->ptrIzq->valor);
                }
                break;
            case 2:
                fprintf(archivoAssembler,"GetFloat %s \n\n", (*pa)->ptrIzq->valor);
                break;
            case 3:
                if(strcmp((*pa)->ptrDer->valor,"@msgListaVacia")==0) {
                    fprintf(archivoAssembler,"JMP errorListaVacia\n\n");
                    return;
                }
                fprintf(archivoAssembler,"fld %s\nfstp %s\n\n",(*pa)->ptrDer->valor,(*pa)->ptrIzq->valor);
                break;
        }
        if(strcmp((*pa)->valor,"CUERPOIF")==0) {
            fprintf(archivoAssembler,"JMP etiqAsig\n\n");
            strcpy(msjAux,"etiqElem");
            char varItoa[5];
            itoa(cantCTE,varItoa,10);
            strcat(msjAux,varItoa);
            strcat(msjAux,":\n");
            fprintf(archivoAssembler, msjAux);
            if(strcmp((*pa)->ptrDer->valor,"@msgNoEncontro")==0) {
                fprintf(archivoAssembler,"JMP errorNoEncontro\n\n");
            }
        }

        if(strcmp((*pa)->valor,"unionAsig")==0) {
            strcpy(msjAux,"etiqAsig:\n");
            fprintf(archivoAssembler, msjAux);
        }

        if(esCmp((*pa)->valor)) {
            fprintf(archivoAssembler,"fld %s\nfld %s\nfxch\nfcomp\nfstsw ax\nsahf\n", (*pa)->ptrIzq->valor,(*pa)->ptrDer->valor);
        }


        switch(esCmp(((*pa)->valor)))
        {
            case 1:
                fprintf(archivoAssembler,"JNAE etiqError_1\n\n");
                break;
            case 2:
                strcpy(msjAux,"JNE etiqElem");
                cantCTE++;
                char varItoa[5];
                itoa(cantCTE,varItoa,10);
                strcat(msjAux,varItoa);
                fprintf(archivoAssembler,msjAux);
                fprintf(archivoAssembler,"\n\n");
                break;
            
        }
        
    }

    //Recorro por derecha
    escribir_seccion_codigo(archivoAssembler, &(*pa)->ptrDer);		
}

int esOperacion(char* valor)
{
	if(strcmp(valor,"WRITE")==0)
		return 1;											
	if(strcmp(valor,"READ")==0)
		return 2;
    if(strcmp(valor,"=")==0)
        return 3;
	return 0;
}

int esCmp(char* valor)
{	
	if(strcmp(valor,"<")==0)		
	    return 1;
    if(strcmp(valor,"==")==0) 
        return 2;
	return 0;
}

void escribir_etiquetas_validacion(FILE *archivoAssembler) {
    if(getLexema("@msgMayor_1") != NULL) {
        fprintf(archivoAssembler,"etiqError_1:\n");
        fprintf(archivoAssembler,"mov dx,OFFSET %s\nmov ah,9\nint 21h\nnewline 1\n\n", "@msgMayor_1");
        fprintf(archivoAssembler, "MOV EAX, 4C00h\nINT 21h\n\n");
    }

    if(getLexema("@msgNoEncontro") != NULL) {
        fprintf(archivoAssembler,"errorNoEncontro:\n");
        fprintf(archivoAssembler,"mov dx,OFFSET %s\nmov ah,9\nint 21h\nnewline 1\n\n", "@msgNoEncontro");
        fprintf(archivoAssembler, "MOV EAX, 4C00h\nINT 21h\n\n");
    }

    if(getLexema("@msgListaVacia") != NULL) {
        fprintf(archivoAssembler,"errorListaVacia:\n");
        fprintf(archivoAssembler,"mov dx,OFFSET %s\nmov ah,9\nint 21h\nnewline 1\n\n", "@msgListaVacia");
        fprintf(archivoAssembler, "MOV EAX, 4C00h\nINT 21h\n\n");
    }
}