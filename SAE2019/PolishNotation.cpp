#include "stdafx.h"

using namespace Lex;

bool Lex::PolishNotation( LT::LexTable& lextable, IT::IdTable& idtable)
{
	bool Fstring = false;//флаг строкового типа
	bool Foper = false;//флаг оператора
	bool CallFun = false;
	for (int lexindx = 0; lexindx < lextable.size; lexindx++)
	{
		if (lextable.table[lexindx-1].lexema == LEX_EQUAL || lextable.table[lexindx - 1].lexema == LEX_PRINT || lextable.table[lexindx - 1].lexema == LEX_RETURN || lextable.table[lexindx - 1].lexema == LEX_IF)
		{
			bool ExpressionWithEqual = false;
			IT::IDDATATYPE type;
			if (lextable.table[lexindx - 1].lexema == LEX_EQUAL)
			{
				type = idtable.table[lextable.table[lexindx - 2].idxTI].iddatatype;
				ExpressionWithEqual = true;
			}
			
			int lextable_positionBuff = lexindx;
			int countlex = 0;
			bool stackEmpty = true;
			bool findFun = false;
			bool TypeError = false;
			Stack* stack = new Stack;
			stack->head = NULL;
			Queue* begin = NULL, * end;
			while (lextable.table[lexindx - 1].lexema != LEX_SEMICOLON )
			{
				if (lextable.table[lexindx - 1].lexema == LEX_ID)
				{
					if (idtable.table[lextable.table[lexindx-1].idxTI].idtype == IT::F)
					{
						findFun = true;
					}
				}
				if (lextable.table[lexindx].lexema == LEX_ID || lextable.table[lexindx].lexema == LEX_LITERAL)
				{
					
					if (idtable.table[lextable.table[lexindx].idxTI].iddatatype == IT::STR && !findFun)
					{
						Fstring = true;
					}
					if (ExpressionWithEqual)
					{
						if (!findFun && type != idtable.table[lextable.table[lexindx].idxTI].iddatatype)////
						{
							TypeError = true;
						}
					}
					Lex::push(&begin, &end, lextable.table[lexindx].lexema, NULL, lextable.table[lexindx].sn, lextable.table[lexindx].idxTI);
				}
				else if (lextable.table[lexindx].lexema == LEX_OPERATOR)
				{
					Foper = true;
					bool put = true;
					do
					{
						if (stack->head == nullptr || lextable.table[lexindx].priority > stack->head->priority || stackEmpty == true)
						{
							Lex::push(stack, lextable.table[lexindx].lexema, lextable.table[lexindx].oper, lextable.table[lexindx].priority, lextable.table[lexindx].sn, NULL);
							put = true;
							stackEmpty = false;
						}
						else
						{
							while (stackEmpty == false && lextable.table[lexindx].priority <= stack->head->priority)
							{
								Lex::push(&begin, &end, stack->head->data, stack->head->op, stack->head->sn, NULL);
								Lex::pop(stack);
								if (!stack->head) {
									stackEmpty = true;
								}
							}
							put = false;
						}
					} while (put == false);
				}
				else if (lextable.table[lexindx].lexema == LEX_LEFTTHESIS)
				{
					Lex::push(stack, lextable.table[lexindx].lexema, NULL, lextable.table[lexindx].priority, lextable.table[lexindx].sn, NULL);
				}
				else if (lextable.table[lexindx].lexema == LEX_RIGHTTHESIS)
				{
					if (findFun)
					{
						Lex::push(&begin, &end, DIV, NULL, NULL, NULL);
					}
					findFun = false;
					while (lextable.table[lexindx].priority < stack->head->priority)//пока не встретим скобку
					{
						if (stack->head->data == LEX_OPERATOR)
						{
							Lex::push(&begin, &end, stack->head->data, stack->head->op, stack->head->sn, NULL);
						}
						else {
							Lex::push(&begin, &end, stack->head->data, NULL, stack->head->sn, NULL);
						}
						Lex::pop(stack);
					}
					Lex::pop(stack);
				}
				else if (lextable.table[lexindx].lexema == LEX_SEMICOLON ) 
				{
					if (Foper && Fstring)
					{
						throw ERROR_THROW_IN(49, lextable.table[lexindx].sn,0);
					}
					while (stack->head)
					{
						if (stack->head->data == LEX_OPERATOR)
						{
							Lex::push(&begin, &end, stack->head->data, stack->head->op, stack->head->sn, NULL);
						}
						else {
							Lex::push(&begin, &end, stack->head->data, NULL, stack->head->sn, NULL);
						}
						Lex::pop(stack);
					}
					Lex::push(&begin, &end, lextable.table[lexindx].lexema, NULL, lextable.table[lexindx].sn, NULL);
				}
				countlex++;
				lexindx++;
			}
			if (TypeError)
			{
				throw ERROR_THROW_IN(47, lextable.table[lexindx].sn, 0);
			}
			delete stack;
			lexindx = lextable_positionBuff;
			Lex::Clean(lextable, lexindx, begin, countlex);
			Fstring = false;//флаг строкового типа
			Foper = false;//флаг оператора
		}
		
	}
	return true;
}
void Lex::push(Stack* stack, unsigned char data, unsigned char op, int priority, int sn, int idxTI) {
	Stack* stack2 = new Stack;
	stack2->data = data;
	stack2->op = op;
	stack2->priority = priority;
	stack2->sn = sn;
	stack2->idxTI = idxTI;
	stack2->next = stack->head;
	stack->head = stack2;
}
void Lex::pop(Stack* stack) {
	if (stack->head == NULL) {
		return;
	}
	else {
		Stack* stack2 = new Stack;
		stack2->head = stack->head->next;
		while (stack->head != NULL) {
			stack->head = stack->head->next;
		}
		stack->head = stack2->head;
		delete stack2;
	}
}
void Lex::push(Queue** begin, Queue** end, unsigned char data, unsigned char op, int sn, int idxTI)
{
	Queue* t = new Queue;
	t->next = NULL;
	t->data = data;
	t->op = op;
	t->sn = sn;
	t->idxTI = idxTI;
	if (*begin == NULL) {
		*begin = *end = t;
	}
	else {
		(*end)->next = t;
		*end = t;
	}
}
void Lex::Clean(LT::LexTable& lextable, int lextable_position, Queue* begin, int lexQuantity)
{
	Lex::Queue* t = begin;
	while (t != NULL)
	{
		if (t->data)
		{
			lextable.table[lextable_position].lexema = t->data;
		}
		if (t->op)
		{
			lextable.table[lextable_position].oper = t->op;
		}
		if (t->sn)
		{
			lextable.table[lextable_position].sn = t->sn;
		}
		if (t->idxTI )
		{
			lextable.table[lextable_position].idxTI = t->idxTI;
		}
		if (t->idxTI == 0 && t->data == LEX_ID)
		{
			lextable.table[lextable_position].idxTI = t->idxTI;

		}
		t = t->next;
		lexQuantity--;
		lextable_position++;
	
	}
	for (int i = 0; i < lexQuantity; i++)
	{
		lextable.table[lextable_position].lexema = '$';
		lextable_position++;
	}
}

