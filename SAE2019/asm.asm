.586
.MODEL flat, stdcall 

includelib kernel32.lib 
includelib libucrt.lib
includelib ..\Debug\Lib.lib

ExitProcess PROTO: DWORD
EXTRN ToConsoleSTR: PROC
EXTRN ToConsoleNUM: PROC
EXTRN negative: PROC
EXTRN powb: PROC
EXTRN OVERFLOWEXCEPTION : PROC
EXTRN NULLEXCEPTION: PROC

.STACK 4096
.CONST
	L1 byte 'Строка из функции Fstr',0
	L2 sdword 5
	L3 sdword 27
	L4 byte 1
	L5 sdword 4
	L6 byte 'Mainstr',0
	L7 sdword 2
	L8 sdword 0
	L9 sdword 65
	L10 sdword 50
	L11 byte 'ABCDe',0
	L12 sdword 17
.DATA
	Fstrstr dword ?
	Fmathrezult sdword 0
	mainbl byte 0
	maina sdword 0
	mainb sdword 0
	mainc dword ?
	maink sdword 0
	mains dword ?
.CODE
Fstr PROC Fstrs: dword
	LEA EAX, L1
	push EAX
	pop EAX
	MOV Fstrstr, EAX
	MOV EAX, 0
	MOV EAX, Fstrs
	push EAX
	call ToConsoleSTR

	MOV EAX, 0
	MOV EAX, Fstrstr
	push EAX
	pop EAX
	ret
Fstr ENDP

Fmath PROC Fmathx: sdword, Fmathy: sdword
	MOV EAX, Fmathy
	push EAX
	MOV EAX, Fmathx
	push EAX
	call powb
	push EAX

	push 5
	MOV EAX, 0
	pop EDX
	pop EAX
	SUB EAX, EDX
	push EAX


	push 5
	MOV EAX, 0
	pop EDX
	pop EAX
	IMUL EAX, EDX
	push EAX


	pop EAX
	cmp EAX,127
	jg OVERF10
	cmp EAX,-128
	jl OVERF10
	jmp OVERFEND10
		OVERF10:
	push 10
	call OVERFLOWEXCEPTION
	push 0
	call ExitProcess
		OVERFEND10:
	MOV Fmathrezult, EAX
	push 27
	call negative
	push EAX

	call ToConsoleNUM

	MOV EAX, 0
	MOV EAX, Fmathrezult
	push EAX
	pop EAX
	ret
Fmath ENDP

main PROC
	push 1
	pop EAX
	MOV mainbl, AL
	push 4
	pop EAX
	cmp EAX,127
	jg OVERF19
	cmp EAX,-128
	jl OVERF19
	jmp OVERFEND19
		OVERF19:
	push 19
	call OVERFLOWEXCEPTION
	push 0
	call ExitProcess
		OVERFEND19:
	MOV maina, EAX
	LEA EAX, L6
	push EAX
	pop EAX
	MOV mainc, EAX
	push 2
	MOV EAX, maina
	push EAX
	call Fmath
	push EAX

	pop EAX
	cmp EAX,127
	jg OVERF22
	cmp EAX,-128
	jl OVERF22
	jmp OVERFEND22
		OVERF22:
	push 22
	call OVERFLOWEXCEPTION
	push 0
	call ExitProcess
		OVERFEND22:
	MOV mainb, EAX
	push 0
	push 65
	MOV EAX, 0
	pop EDX
	pop EAX
	SUB EAX, EDX
	push EAX


	pop EAX
	cmp EAX,127
	jg OVERF23
	cmp EAX,-128
	jl OVERF23
	jmp OVERFEND23
		OVERF23:
	push 23
	call OVERFLOWEXCEPTION
	push 0
	call ExitProcess
		OVERFEND23:
	MOV maink, EAX
	MOV EAX, 0
	MOV EAX, mainb
	push EAX
	push 50
	pop EBX
	mov EDX,0
	pop EAX
	cmp EBX,0
	jz nullExcept0
	jmp last0
	nullExcept0:
	push 24
	call NULLEXCEPTION
	push 0
	call ExitProcess
	last0:
	idiv EBX
	push EDX

	push 65

	pop EBX
	pop EAX
	cmp EAX, EBX
	jne ELSEPOINT24
	push 1
	jmp ENDPOINT24
	ELSEPOINT24:
	push 0
	ENDPOINT24:

	pop EAX
	cmp EAX, 1
	jne ELSE24
	LEA EAX, L11
	push EAX
	call ToConsoleSTR

	ELSE24:
	MOV EAX, 0
	MOV AL, mainbl
	push EAX
	push 1

	pop EBX
	pop EAX
	cmp EAX, EBX
	jne ELSEPOINT28
	push 1
	jmp ENDPOINT28
	ELSEPOINT28:
	push 0
	ENDPOINT28:

	pop EAX
	cmp EAX, 1
	jne ELSE28
	MOV EAX, 0
	MOV EAX, mainb
	push EAX
	push 17
	pop EBX
	mov EDX,0
	pop EAX
	cmp EBX,0
	jz nullExcept1
	jmp last1
	nullExcept1:
	push 30
	call NULLEXCEPTION
	push 0
	call ExitProcess
	last1:
	idiv EBX
	push EDX

	call ToConsoleNUM

	ELSE28:
	MOV EAX, mainc
	push EAX
	call Fstr
	push EAX

	pop EAX
	MOV mains, EAX
	MOV EAX, 0
	MOV EAX, mains
	push EAX
	call ToConsoleSTR

	push 0
	call ExitProcess
main ENDP
end main
