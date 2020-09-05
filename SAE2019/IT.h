#pragma once
#define ID_MAXSIZE		10			// максимальное количество символов в идентификаторе
#define TI_MAXSIZE		4096		// максимальное количество строк в таблице идентификаторов
#define TI_BYTE_DEFAULT	0x00000000	// значение по умолчанию дл€ типа byte
#define TI_STR_DEFAULT	0x00		// значение по умолчанию дл€ типа string
#define TI_BOOL_DEFAULT	0			// значение по умолчанию дл€ типа bool
#define TI_NULLIDX		0xffffffff	// нет элeмента таблицы идентификаторов
#define TI_STR_MAXSIZE	127			 
#include "Log.h"

namespace IT			// таблица идентификаторов
{
	enum IDDATATYPE { BOOL = 1, BYTE = 2, STR = 3 };			// типы данных идентификаторов: integer, string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5 };	// типы идентификаторов: переменна€, функци€, параметр, литерал, оператор
	//queue<IDDATATYPE> funparametrs;
	struct Entry	// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		unsigned char	id[ID_MAXSIZE];		// индентификатор (автоматически усекаетс€ до ID_MAXSIZE)
		unsigned char visibility[ID_MAXSIZE];
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE		idtype;				// тип идентификатора
		union
		{
			int vbyte=0;					// значение i
			struct
			{
				int len;						// количество символов в string
				unsigned char str[TI_STR_MAXSIZE - 1];	// символы string
			} vstr;				// значение string
			bool vbool;
		} value;		// значение идентификатора
		struct funparms
		{
			int countparm = 0;//количество параметров 
			IDDATATYPE parmtypes[10];//массив типов параметров
		}funparm;
	};

	struct IdTable				// экземпл€р таблицы идентификаторов
	{
		int maxsize;			// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;				// текущий размер таблицы идентификаторов < maxsize
		Entry* table;			// массив строк таблицы идентификаторов
	};

	IdTable Create(				// создать таблицу идентификаторов
		int size				// емкость таблицы идентификаторов < TI_MAXSIZE
	);

	void Add(				// добавить строку в таблицу идентификаторов
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		Entry entry			// строка таблицы идентификаторов
	);

	Entry GetEntry(			// получить строку таблицы идентификаторов
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		int n				// номер получаемой строки
	);

	int IsId(				// возврат: номер строки (если есть), TI_NULLIDX (если нет)
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		unsigned char id[ID_MAXSIZE]	// идентификатор
	);

	void Delete(IdTable& idtable);	// удалить таблицу идентификаторов (освободить пам€ть)
	void showTable(IdTable& idtable, Log::LOG& log);  // вывод таблицы идентификаторов
};