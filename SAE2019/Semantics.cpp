#include "Semantics.h"
#include "Error.h"
#include <queue>
void Semantics::SemanticsAnaliz(IT::IdTable& idtable, LT::LexTable& lextable)
{
	/*Semantics::CheckReturnType(idtable, lextable);
	Semantics::CheckOperandType(idtable, lextable);*/
}
void Semantics::CheckOperandType(IT::IdTable& idtable, LT::LexTable& lextable)
{
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == LEX_OPERATOR)
		{
			if (idtable.table[lextable.table[i - 1].idxTI].iddatatype != IT::BYTE || idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::BYTE)
			{
				throw ERROR_THROW_IN(42, lextable.table[i + 1].sn, 0);
			}
		}

	}
}

void Semantics::CheckReturnType(IT::IdTable& idtable, LT::LexTable& lextable)
{
	IT::IDDATATYPE funtype;
	int n=0;
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == LEX_FUNCTION)
		{
			n = lextable.table[i + 1].idxTI;
			funtype = idtable.table[n].iddatatype;
		}
		if (lextable.table[i].lexema == LEX_RETURN )
		{
			n = lextable.table[i + 1].idxTI;
			if (funtype != idtable.table[n].iddatatype)
			{
				throw ERROR_THROW_IN(43,lextable.table[i + 1].sn, 0);
			}
		}
	}
}
void Semantics::CheckFunParm(IT::IdTable& idtable, LT::LexTable& lextable)
{
	IT::IDDATATYPE funtype;
	std::queue<IT::IDDATATYPE> parmtypes[10];
	int n = 0;
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == LEX_FUNCTION && lextable.table[i+1].lexema == LEX_ID && lextable.table[i+2].lexema == '6')
		{
			n = lextable.table[i + 1].idxTI;
			funtype = idtable.table[n].iddatatype;
		}
		if (lextable.table[i].lexema == LEX_RETURN)
		{
			n = lextable.table[i + 1].idxTI;
			if (funtype != idtable.table[n].iddatatype)
			{
				throw ERROR_THROW_IN(43, lextable.table[i + 1].sn, 0);
			}
		}
	}
}