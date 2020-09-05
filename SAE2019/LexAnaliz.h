#pragma once
#include "IT.h"
#include "LT.h"
#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define REMAINDER	'%' 
#define EQUAL '='
#define EQUALSYING '?'
namespace Lex
{
	struct LEX
	{
		IT::IdTable idtable;;
		LT::LexTable lextable;
	};
	LEX LexAnaliz(Log::LOG log, In::IN in);

	bool PolishNotation(LT::LexTable& lextable, IT::IdTable& idtable);

	struct Stack {
		unsigned char data;
		int priority;
		unsigned char op;
		int sn;
		int idxTI;
		Stack* head;
		Stack* next;
	};
	void push(Stack* stack, unsigned char data, unsigned char op, int priority, int sn, int idxTI);
	void pop(Stack* stack);
	struct Queue 
	{
		unsigned char data;
		unsigned char op;
		int sn;
		int idxTI;
		Queue* next;
	};
	void push(Queue** begin, Queue** end, unsigned char data, unsigned char op, int sn, int idxTI);
	void Clean(LT::LexTable& lextable, int lextable_position, Queue* begin, int lexQuantity);
}