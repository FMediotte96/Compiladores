include macros2.asm
include number.asm
.MODEL LARGE
.386
.STACK 200h

.DATA
_Ingrese_un_valor_pivot_mayor_o_igual_a_1_ db "Ingrese un valor pivot mayor o igual a 1: " , '$', 44 dup (?)
pivot dd ?
_1 dd 1.0
@msgMayor_1 db "El valor debe ser >= 1" , '$', 24 dup (?)
resul dd ?
_4 dd 4.0
@aux dd ?
_10 dd 10.0
_2 dd 2.0
@msgNoEncontro db "Elemento no encontrado" , '$', 24 dup (?)
_Elemento_encontrado_en_posicion_ db "Elemento encontrado en posicion: " , '$', 35 dup (?)

.CODE

etiqError_1:
mov dx,OFFSET @msgMayor_1
mov ah,9
int 21h
newline 1

MOV EAX, 4C00h
INT 21h

errorNoEncontro:
mov dx,OFFSET @msgNoEncontro
mov ah,9
int 21h
newline 1

MOV EAX, 4C00h
INT 21h


START:

MOV AX,@DATA
MOV DS, AX
MOV ES, AX

mov dx,OFFSET _Ingrese_un_valor_pivot_mayor_o_igual_a_1_
mov ah,9
int 21h
newline 1

GetFloat pivot 

fld pivot
fld _1
fxch
fcomp
fstsw ax
sahf
JNAE etiqError_1

fld pivot
fld _4
fxch
fcomp
fstsw ax
sahf
JNE etiqElem2

fld _1
fstp @aux

JMP etiqAsig

etiqElem2:
fld pivot
fld _10
fxch
fcomp
fstsw ax
sahf
JNE etiqElem3

fld _2
fstp @aux

JMP etiqAsig

etiqElem3:
JMP errorNoEncontro

etiqAsig:
fld @aux
fstp resul

mov dx,OFFSET _Elemento_encontrado_en_posicion_
mov ah,9
int 21h
newline 1

DisplayFloat resul,2
NEWLINE

MOV EAX, 4C00h
INT 21h

END START
