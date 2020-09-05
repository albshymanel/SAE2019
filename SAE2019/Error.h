#pragma once
#define ERROR_THROW(id) Error::GetError(id);					// сформировать ERROR для ERROR_THROW;		//	throw	ERROR_THROW(id)
#define ERROR_THROW_IN(id, l, c) Error::GetErrorIn(id,l,c);		//	throw	ERROR_THROW(id, строка, позиция в строке)
#define ERROR_ENTRY(id, m) {id, m, {-1, -1}}					//	элемент таблицы ошибок
#define ERROR_MAXSIZE_MESSAGE 200								//	максимальная длина сообщения об ошибке
#define ERROR_ENTRY_NODEF(id)		ERROR_ENTRY(-id, "Неопределенная ошибка")	// 1 неопределенный элемент таблицы ошибок
// ERROR_ENTRY_NODEF10(id) - 10 неопределенных элементов таблицы ошибок
#define ERROR_ENTRY_NODEF10(id)	   ERROR_ENTRY_NODEF(id + 0), ERROR_ENTRY_NODEF(id + 1), ERROR_ENTRY_NODEF(id + 2), ERROR_ENTRY_NODEF(id + 3), \
		ERROR_ENTRY_NODEF(id + 4), ERROR_ENTRY_NODEF(id + 5), ERROR_ENTRY_NODEF(id + 6), ERROR_ENTRY_NODEF(id + 7), \
		ERROR_ENTRY_NODEF(id + 8), ERROR_ENTRY_NODEF(id + 9)
#define ERROR_MAX_ENTRY 100			//	количество элементов в таблице ошибок

namespace Error
{
	struct ERROR	// тип исключения для throw ERROR_THROW |ERROR_THROW_IN и catch(ERROR)
	{
		int id;									// код ошибки
		char message[ERROR_MAXSIZE_MESSAGE];	// сообщение об ошибке
		struct IN								// расширение для ошибок при обработке входных данных
		{
			short line;							// номер строки (0, 1, 2, ...)
			short column;							// номер позиции в строке (0, 1, 2, ...)
		} inext;
	};
	ERROR GetError(short id);							// сформировать ERROR для ERROR_THROW
	ERROR GetErrorIn(short id, short line, short column);	// сформировать ERROR для ERROR_THROW_IN
}