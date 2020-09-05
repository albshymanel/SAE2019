#include "stdafx.h"

namespace LT
{
	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE) throw ERROR_THROW(40);
		LexTable Table;
		Table.maxsize = size;
		Table.size = 0;
		Table.table = new Entry[size];//массив строк
		return Table;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size > lextable.maxsize) throw ERROR_THROW(41);
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}

	Entry writeEntry(Entry& entry, unsigned char lexema, int indx, int line)
	{
		if (lexema == LEX_LEFTTHESIS || lexema == LEX_RIGHTTHESIS) {
			entry.priority = 1;
		}
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		return entry;
	}
	Entry writeEntry(Entry& entry, unsigned char lexema, int indx, int line, unsigned char* op)
	{
		if (op[0] == EQUALSYING )
		{
			entry.priority = 2;
		}
		if (op[0] == PLUS || op[0] == MINUS)
		{
			entry.priority = 3;
		}
		else if (op[0] == STAR || op[0] == DIRSLASH || op[0] == REMAINDER)
		{
			entry.priority = 4;
		}
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		entry.oper = *op;
		return entry;
	}

	void showTable(LexTable lextable, Log::LOG& log)		// вывод таблицы лексем
	{
		int number = 1;
		*log.stream << "01 ";
		for (int i = 0; i < lextable.size; i++)
		{

			if (lextable.table[i].sn != number)
			{
				while (lextable.table[i].sn - number > 1)	// пока строки не станут равны
					number++;
				if (number < 9)
					* log.stream << std::endl << '0' << lextable.table[i].sn << std::ends;
				else
					*log.stream << std::endl << lextable.table[i].sn << std::ends;
				number++;
			}
			*log.stream << lextable.table[i].lexema;
			if (lextable.table[i].lexema == LEX_LITERAL) {
				*log.stream << "[" << lextable.table[i].idxTI << "]";
			}
			if (lextable.table[i].lexema == LEX_OPERATOR) {
				*log.stream << "[" << lextable.table[i].oper << "]";
			}

		}
	}
}