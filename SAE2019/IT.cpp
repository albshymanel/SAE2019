
#include "stdafx.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE) throw ERROR_THROW(44);
		IdTable Table;
		Table.maxsize = size;
		Table.size = 0;
		Table.table = new Entry[size];
		return Table;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size > idtable.maxsize) throw ERROR_THROW(43);
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, unsigned char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (_mbscmp(idtable.table[i].id, id) == 0) return i;//если идентификатор есть то вернем номер строки таблицы идентификаторов
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}

	void showTable(IdTable& idtable, Log::LOG& log)
	{
		int i, numberOP = 0, numbLiter = 0;
		*log.stream << std::setfill('-') << std::setw(100) << '-' << std::endl;
		*log.stream << "   №" << " | " << " Идентификатор " << " | " << "Тип данных" << "   | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение    " << std::endl;
		*log.stream << std::setw(100) << '-' << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == OP)
			{
				continue;
			}
			/////
			*log.stream << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			*log.stream << std::setfill(' ') << std::setw(15) << std::left << idtable.table[i].id << " | ";
			///////
			switch (idtable.table[i].iddatatype)
			{
			case BOOL: *log.stream << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else *log.stream << "bool" << " | "; break;
			case BYTE: *log.stream << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else *log.stream << "byte" << " | "; break;
			case STR: *log.stream << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else *log.stream << "string" << " | "; break;
			default: *log.stream << std::setw(10) << std::left << "unknown" << " | "; break;
			}
			/////
			switch (idtable.table[i].idtype)
			{
			case V: *log.stream << std::setw(20) << std::left << "переменная" << " | "; break;
			case F: *log.stream << std::setw(20) << std::left << "функция" << " | "; break;
			case P:*log.stream << std::setw(20) << std::left << "параметр" << " | "; break;
			case L: *log.stream << std::setw(20) << std::left << "литерал" << " | ";
				numbLiter++;
				break;
			case OP: *log.stream << std::setw(20) << std::left << "оператор" << " | ";
				numberOP++;
				break;
			default: *log.stream << std::setw(20) << std::left << "unknown" << " | "; break;
			}
			//////
			*log.stream << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == BOOL && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				* log.stream << std::setw(18) << std::left << idtable.table[i].value.vbool;
			else if (idtable.table[i].iddatatype == BYTE && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				* log.stream << std::setw(18) << std::left << idtable.table[i].value.vbyte;
			else if (idtable.table[i].iddatatype == STR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				* log.stream << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else
				std::cout << "-";
			*log.stream << std::endl;
			//////
		}
		*log.stream << std::setfill('-') << std::setw(100) << '-' << std::endl;
		*log.stream << "Количество идентификаторов: " << i - numberOP - numbLiter << std::endl;
		*log.stream << std::setw(100) << '-' << std::endl;
	}
}