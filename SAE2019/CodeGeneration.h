#pragma once
#include "LexAnaliz.h"
#include "stdafx.h"
#define ASM_FILE_PATH "asm.asm"
#define ASM_FILE_HEADER ".586\n.MODEL flat, stdcall \n\nincludelib kernel32.lib \nincludelib libucrt.lib\n"	
#define ASM_LIB_PATH "includelib ..\\Debug\\Lib.lib\n"
#define ASM_STACK_BLOCK ".STACK 4096\n"
#define ASM_CONST_BLOCK ".CONST\n"
#define ASM_DATA_BLOCK ".DATA\n"
#define ASM_CODE_BLOCK ".CODE\n"

#define ASM_BYTE_TYPE "sdword"
#define ASM_STR_PTR_TYPE "dword"
#define ASM_STR_TYPE "byte"
#define ASM_BOOL_TYPE "byte"
#define ASM_NO_VALUE '?'

#define ASM_EXTERN "EXTRN"
#define ASM_EXIT_PROC "ExitProcess PROTO: DWORD"

#define ASM_TAB "\t"
#define ASM_SPACE " "
#define ASM_QUOTE "\'"
#define ASM_COMMA "." 

#define FUN_NEGATIVE "negative: PROC"
#define FUN_POWB "powb: PROC"
#define FUN_TOCONSOLE_STR "ToConsoleSTR: PROC"
#define FUN_TOCONSOLE_INT "ToConsoleNUM: PROC"
#define FUN_NULLEXCEPTION "NULLEXCEPTION: PROC"
#define FUN_OVERFLOWEXCEPTION "OVERFLOWEXCEPTION : PROC"
namespace CodeGeneration
{
	void CodeGeneration(Lex::LEX lex, Log::LOG log);
	void WriteFuncBody(int& i, Lex::LEX lex, std::ofstream& asmFile);
	void CalculateExpression(int& i ,Lex::LEX lex, std::ofstream& asmFile);
}