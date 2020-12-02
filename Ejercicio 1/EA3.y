/* 1 - Declaraciones */
%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "lib/tsimbolos.c"
#include "lib/arbol.c"
#include "lib/assembler.c"

FILE *yyin;
char *yyltext;

int yyerror();

nodoArbol *ptrProg = NULL;
nodoArbol *ptrSent = NULL;
nodoArbol *ptrRead = NULL; 
nodoArbol *ptrWrite = NULL;
nodoArbol *ptrAsig = NULL;
nodoArbol *ptrPosicion = NULL;
nodoArbol *ptrLista = NULL;
nodoArbol *ptrIf = NULL;
nodoArbol *ptrCuerpo = NULL;
nodoArbol *ptrCond = NULL;
nodoArbol *ptrTrue = NULL;

char varString[255];
char varCTE[255];
char pivot[30];
char varItoa[20];
nodoArbol* vectorCuerpo[20];

char msgError_1[30] = "\"El valor debe ser >= 1\"";
char msgListaVacia[50] = "\"La lista esta vacia\"";
char msgNoEncontro[50] = "\"Elemento no encontrado\"";

int pos = 0;
int cont = 0;
int listaVacia = 0;
int cantSent = 0;

%}

/* 2 - Tokens */

%union {
    int tipo_int;
    double tipo_double;
    char *tipo_string;
}

%token WRITE 
%token READ 
%token POSICION 
%token<tipo_int> CTE   
%token<tipo_string> ID    
%token ASIGNA
%token PARA  
%token PARC  
%token CA    
%token CC    
%token COMA  
%token PYC   
%token<tipo_string> CTE_S 


/* 3 - Reglas */
%%

s:          prog { 
                    printf("Compilacion OK\n"); 
                    guardarArbol(ptrProg); 
                    generarAssembler(&ptrProg);
                };

prog:       sent        { 
                            printf("Regla 1 (PROG->SENT)\n");
                            ptrProg = ptrSent;
                        } 
            | prog sent { 
                            printf("Regla 2 (PROG -> PROG SENT)\n");
                            strcpy(varString,"subArbol_");
                            itoa(cantSent,varItoa,10);
                            strcat(varString,varItoa);
                            cantSent++;
                            ptrProg = crearNodo(varString, ptrProg, ptrSent);
                        };

sent:       read    {
                        printf("Regla 3 (SENT -> READ)\n");
                        ptrSent = crearNodo("union",ptrRead,ptrIf);
                    }
            | write {
                        printf("Regla 3 (SENT -> WRITE)\n");
                        ptrSent = ptrWrite;
                    }
            | asig  {    
                        printf("Regla 3 (SENT -> ASIG)\n");
                        ptrSent = ptrAsig;
                    };

read:       READ ID {   
                        printf("Regla 4 (READ -> read id)\n");
                        strcpy(varString, $2);
                        insertarEnTS("_1", "CTE", "", 1, 0);
                        insertarEnTS("@msgMayor_1", "CTE_STRING", msgError_1, 0, 0);
                        ptrCond = crearNodo("<", crearHoja(varString), crearHoja("_1"));
                        ptrIf = crearNodo("IF", ptrCond, crearHoja("@msgMayor_1"));
                        ptrRead = crearNodo("READ",crearHoja(varString), NULL);
                    };

asig:       ID { strcpy(varString, $1);} ASIGNA posicion  {
                                    printf("Regla 5 (ASIG -> id asigna POSICION)\n");
                                    if(listaVacia == 0){
                                        nodoArbol* ptrUnion = crearNodo("=",crearHoja(varString), crearHoja("@aux"));
                                        ptrAsig = crearNodo("unionAsig", ptrPosicion, ptrUnion);
                                    } else{
                                        ptrAsig = crearNodo("=", crearHoja(varString),ptrPosicion);
                                    } 
                                };

posicion:   POSICION PARA ID PYC CA lista CC PARC   {   
                                                        printf("Regla 6 (POSICION -> posicion para id pyc ca LISTA cc parc)\n");
                                                        nodoArbol* ptrFalse = crearHoja("@msgNoEncontro");
                                                        nodoArbol* ptrCuerpoAnt = vectorCuerpo[cont-1];
                                                        (ptrCuerpoAnt)->ptrDer = ptrFalse;
                                                        ptrPosicion = ptrLista;
                                                        insertarEnTS("@msgNoEncontro", "CTE_STRING", msgNoEncontro, 0, 0);
                                                    }
            | POSICION PARA ID PYC CA CC PARC   {   
                                                    printf("Regla 7 (POSICION -> posicion para id pyc ca cc parc)\n");
                                                    listaVacia = 1;
                                                    insertarEnTS("@msgListaVacia", "CTE_STRING", msgListaVacia, 0, 0);
                                                    ptrPosicion = crearHoja("@msgListaVacia");
                                                };

lista:      CTE {   
                    printf("Regla 8 (LISTA -> CTE)\n");
                    pos++;
                    itoa($1,varItoa,10);
                    strcpy(varCTE,"_");
                    strcat(varCTE, varItoa);
                    ptrCond = crearNodo("==", crearHoja("pivot"),crearHoja(varCTE));
                    itoa(pos,varItoa,10);
                    strcpy(varCTE,"_");
                    strcat(varCTE, varItoa);
                    insertarEnTS(varCTE,"CTE","",pos,0);
                    insertarEnTS("@aux","auxCode","",0,0);
                    ptrTrue = crearNodo("=", crearHoja("@aux"),crearHoja(varCTE));
                    ptrCuerpo = crearNodo("CUERPOIF", ptrTrue, NULL);
                    vectorCuerpo[cont] = ptrCuerpo;
                    cont++;
                    ptrIf = crearNodo("IF", ptrCond, ptrCuerpo);
                    ptrLista = ptrIf;
                }
            | lista COMA CTE {
                    printf("Regla 9 (LISTA -> LISTA coma cte)\n");
                    pos++;
                    itoa($3,varItoa,10);
                    strcpy(varCTE,"_");
                    strcat(varCTE, varItoa);
                    ptrCond = crearNodo("==", crearHoja("pivot"),crearHoja(varCTE));
                    itoa(pos,varItoa,10);
                    strcpy(varCTE,"_");
                    strcat(varCTE, varItoa);
                    insertarEnTS(varCTE,"CTE","",pos,0);
                    ptrTrue = crearNodo("=", crearHoja("@aux"),crearHoja(varCTE));
                    ptrCuerpo = crearNodo("CUERPOIF", ptrTrue, NULL);
                    ptrIf = crearNodo("IF", ptrCond, ptrCuerpo);
                    nodoArbol* ptrCuerpoAnt = vectorCuerpo[cont-1];
                    (ptrCuerpoAnt)->ptrDer = ptrIf;
                    vectorCuerpo[cont] = ptrCuerpo;
                    cont++;
                };

write:      WRITE CTE_S {   
                            printf("Regla 10 (WRITE -> write cte_s)\n");
                            strcpy(varString, $2);
                            ptrWrite = crearNodo("WRITE",crearHoja(varString),NULL);
                        }
            | WRITE ID  {    
                            printf("Regla 11 (WRITE -> write id)\n");
                            strcpy(varString, $2);
                            ptrWrite = crearNodo("WRITE",crearHoja(varString),NULL);
                        };

%%

/* 4 - Código */
int yyerror() {
    printf("Error sintactico \n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if((yyin = fopen(argv[1], "rt")) == NULL) {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
    } else {
        crearTS();       
        yyparse();
        guardarTS();
    }
    fclose(yyin);
    return 0;
}
