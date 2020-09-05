#include "stdafx.h"

int bin_to_dec(char* s)
{
	strrev(s);
	int rezult = 0; // n - количество бит, sum это наше десятичное число которое получится из двоичного
	int counter=1;
	while ( s[counter] )
	{
		if (s[counter] == '1')
			rezult += pow(2, counter - 1);
		counter++;
	}

	return rezult;
}
int oct_to_dec(char* s)
{
	
	strrev(s);
	int rezult = 0; // n - количество бит, sum это наше десятичное число которое получится из двоичного
	int counter = 1;
	char buffer[2];
	buffer[1] = '\0';
	while (s[counter])
	{
		buffer[0] = s[counter];
		rezult += atoi(buffer)*pow(8, counter - 1);
		counter++;
	}

	return rezult;
}

using namespace std;

namespace Lex
{
	LEX LexAnaliz(Log::LOG log, In::IN in)
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		clearSpace(in.text, in.size);	// очистит ненужные пробелы

		unsigned char** word = new unsigned char* [max_word];
		for (int i = 0; i < max_word; i++)
			word[i] = new unsigned char[size_word] {NULL};//все слова null

		do {
			word = divideText(in.text, in.size);	// запишет слова в массив
		} while (word == NULL);		// до тех пор, пока не разберет все слова

		int indxLex = 0;		// индекс лексемы
		int indexID = 0;		// индекс идентификатора
		int clit = 1;			// счетчик литералов для указания в таблице идентификаторов
		int line = 1;			// номер строки
		int position = 0;		// номер позиции в исходном файле
		int defFunindex = NULL; // позиция в тл идентификатора функции (для проверки параметров)
		int callFunindex = NULL;//позиция в тл идентификатора функции при вызове (для проверки параметров)
		int bufferCountFunParm = NULL; //буфер хранящий оставшиеся параметры (которые проверяются)
		int numberparm = NULL; //номер параметра 
		int EqualOperator = NULL;// позиция в тл =(для проверки rvalue)
		int Funindex = NULL;//
		int Returnindex = NULL;// позиции для проверки возвращаемого значения

		unsigned char emptystr[] = "";	// пустая строка
		unsigned char* prefix = new unsigned char[10]{ "" };	// текущий префикс
		unsigned char* bufprefix = new unsigned char[10]{ "" };	// буфер для префикса
		unsigned char* oldprefix = new unsigned char[10]{ "" };	// предыдущий префикс
		unsigned char* L = new unsigned char[2]{ "L" };		// идентификатор литерала
		unsigned char* bufL = new unsigned char[TI_STR_MAXSIZE];	//  буфер для идентификатора литерала
		unsigned char* nameLiteral = new unsigned char[10]{ "" };	// имя литерала
		char* charclit = new char[10]{ "" };		// для строковой записи счетчика

		bool findFunc = false;		// флаг функции
		bool findParm = false;		// флаг параметров
		///
		bool findMain = false;      // флаг main
		bool findEqualID = false;   //флаг одинаковых идентификаторов
		bool findIF = false;
		bool findReturn = false;
		///
		IT::Entry entryIT2;		// экземпляр строки таблицы идентификаторов
		char* negative = (char*)"negative";
		char* pow = (char*)"powb";
		const unsigned char* neg = (const unsigned char*) "negative";
		const unsigned char* Pow = (const unsigned char*)"powb";
				entryIT2.iddatatype = IT::BYTE;
				entryIT2.idtype = IT::F;
				_mbscpy(entryIT2.id, neg);
				entryIT2.funparm.countparm = 1;
				entryIT2.funparm.parmtypes[0] = IT::IDDATATYPE::BYTE;
				IT::Add(idtable, entryIT2);
				
				entryIT2.iddatatype = IT::BYTE;
				entryIT2.idtype = IT::F;
				_mbscpy(entryIT2.id, Pow);
				entryIT2.funparm.countparm = 2;
				entryIT2.funparm.parmtypes[0] = IT::IDDATATYPE::BYTE;
				entryIT2.funparm.parmtypes[1] = IT::IDDATATYPE::BYTE;
				IT::Add(idtable, entryIT2);
				indexID = 2;
				IT::Entry entryIT;		// экземпляр строки таблицы идентификаторов костыль
		for (int i = 0; word[i] != NULL; i++, indxLex++)
		{
			bool findSameID = false;		// флажок одинаковых литералов
			FST::FST fstDefine(word[i], FST_DEFINE);
			if (FST::execute(fstDefine))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_DEF, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstTypeBool(word[i], FST_BOOL);
			if (FST::execute(fstTypeBool))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BOOL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.iddatatype = IT::BOOL;
				continue;
			}
			FST::FST fstTypeByte(word[i], FST_BYTE);
			if (FST::execute(fstTypeByte))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BYTE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.iddatatype = IT::BYTE;
				continue;
			}
			FST::FST fstTypeString(word[i], FST_STRING);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.iddatatype = IT::STR;
				_mbscpy(entryIT.value.vstr.str, emptystr);		// заполнить строку пустой строкой (пока что)
				continue;
			}
			FST::FST fstFunction(word[i], FST_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				findFunc = true;	// установить флажок ф-ции
				continue;
			}
			FST::FST fstReturn(word[i], FST_RETURN);
			if (FST::execute(fstReturn))
			{
				if (findIF)//если внутри условного блока то ошибка
				{
					throw ERROR_THROW_IN(61, line, position);
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findReturn = true;//флаг для проверки есть ли возвращаемое значение
				Returnindex = lextable.size;//индекс для проверки типа возвращаемого значения
				continue;
			}
			FST::FST fstPrint(word[i], FST_PRINT);
			if (FST::execute(fstPrint))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_PRINT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstif(word[i], FST_IF);//флаги условия
			if (FST::execute(fstif))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_IF, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				if (findMain == true)//если main  уже был
				{
					throw  ERROR_THROW_IN(53, line, position);
				}
				else
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
					Funindex = lextable.size;//индекс для проверки возвращаемого значения
					LT::Add(lextable, entryLT);
					_mbscpy(oldprefix, prefix);		// копируем префикс в старый префикс
					_mbscpy(prefix, word[i]);		// копируем идентификатор в префикс
					findMain = true;
					continue;
				}
			}
			FST::FST fstLiteralTrue(word[i], FST_TRUE);//
			if (FST::execute(fstLiteralTrue))
			{
				int value = 1;	// значение литерала
								// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vbool == value && idtable.table[k].iddatatype == IT::IDDATATYPE::BOOL && idtable.table[k].idtype == IT::L)
					{
						if (defFunindex == NULL && callFunindex != NULL)
						{
							if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
							{
								throw ERROR_THROW_IN(51, line, position);
							}
							if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != idtable.table[k].iddatatype)//если типы параметтров не совпадают
							{
								throw ERROR_THROW_IN(50, line, position);
							}
							numberparm++;
							bufferCountFunParm--;
						}
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;	// нашло такой же
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				////
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				////
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::BOOL;
				entryIT.value.vbool = value;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)//число/строка/размер/основание
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				if (defFunindex == NULL && callFunindex != NULL)
				{
					if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
					{
						throw ERROR_THROW_IN(51, line, position);
					}
					if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != entryIT.iddatatype)
					{
						throw ERROR_THROW_IN(50, line, position);
					}
					numberparm++;
					bufferCountFunParm--;
				}

				continue;
			}
			FST::FST fstLiteralFalse(word[i], FST_FALSE);//
			if (FST::execute(fstLiteralFalse))
			{
				int value = 0;	// значение литерала
								// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vbool == value && idtable.table[k].iddatatype == IT::IDDATATYPE::BOOL && idtable.table[k].idtype == IT::L)
					{
						if (defFunindex == NULL && callFunindex != NULL)
						{
							if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
							{
								throw ERROR_THROW_IN(51, line, position);
							}
							if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != idtable.table[k].iddatatype)
							{
								throw ERROR_THROW_IN(50, line, position);
							}
							numberparm++;
							bufferCountFunParm--;
						}
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;	// нашло такой же
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				////
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				////
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::BOOL;
				entryIT.value.vbool = value;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)//число/строка/размер/основание
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				if (defFunindex == NULL && callFunindex != NULL)
				{
					if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
					{
						throw ERROR_THROW_IN(51, line, position);
					}
					if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != entryIT.iddatatype)
					{
						throw ERROR_THROW_IN(50, line, position);
					}
					numberparm++;
					bufferCountFunParm--;
				}

				continue;
			}
			FST::FST fstIdentif(word[i], FST_ID);//
			if (FST::execute(fstIdentif))
			{
				word[i][10] = NULL;//урезание идентификатора до 10 символов
				if (findFunc)	// если функция
				{
					
					int idx = IT::IsId(idtable, word[i]);	// ищем без префикса
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						callFunindex = lextable.size;
						numberparm = 0;
						if (lextable.table[lextable.size - 1].lexema == LEX_FUNCTION)
						{
							throw ERROR_THROW_IN(55, line, position);
						}
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
					if (lextable.table[lextable.size -1].lexema != LEX_FUNCTION)
					{
						throw ERROR_THROW_IN(57, line, position);
					}
					defFunindex = lextable.size;
					Funindex = lextable.size;
					
				}
				else
				{
					int idx = IT::IsId(idtable, word[i]);	// ищем без префикса, а потом с префиксом
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						if (lextable.table[lextable.size - 2].lexema == LEX_DEF)//повторное объявление
						{
							throw ERROR_THROW_IN(54, line, position);
						}

						if (defFunindex == NULL && callFunindex != NULL)//
						{
							if (bufferCountFunParm==0)//если параметров в функции больше нет а аргументы есть
							{
								throw ERROR_THROW_IN(51, line, position);
							}
							if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != entryIT.iddatatype)
							{
								throw ERROR_THROW_IN(50, line, position);
							}
							numberparm++;
							bufferCountFunParm--;
						}

						if (idtable.table[idx].idtype == IT::F)
						{
							callFunindex = lextable.size;
							numberparm = 0;
							bufferCountFunParm = idtable.table[idx].funparm.countparm;
						}


						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}

					_mbscpy(bufprefix, prefix);		// копируем префикс в буфер
					word[i] = _mbscat(bufprefix, word[i]);	// конкатенируем идентификатор с префиксом из буфера
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						if (lextable.table[lextable.size - 2].lexema == LEX_DEF)
						{
							throw ERROR_THROW_IN(54, line, position);
						}

						if (defFunindex == NULL && callFunindex != NULL)
						{
							if (bufferCountFunParm == 0)
							{
								throw ERROR_THROW_IN(51, line, position);
							}
							if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != idtable.table[idx].iddatatype)
							{
								throw ERROR_THROW_IN(50, line, position);
							}
							numberparm++;
							bufferCountFunParm--;
						}


						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
					if (lextable.table[lextable.size - 2].lexema != LEX_DEF && findParm == false) {
						throw ERROR_THROW_IN(57, line, position);
					}
				}

				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);

				if (findParm) {		// если параметр
					_mbscpy(entryIT.visibility, prefix);
					entryIT.idtype = IT::P;
					if (defFunindex != NULL && callFunindex==NULL)
					{
						idtable.table[lextable.table[defFunindex].idxTI].funparm.parmtypes[idtable.table[lextable.table[defFunindex].idxTI].funparm.countparm] = entryIT.iddatatype;
						idtable.table[lextable.table[defFunindex].idxTI].funparm.countparm++;
						if (idtable.table[lextable.table[defFunindex].idxTI].funparm.countparm > 3)
						{
							throw ERROR_THROW_IN(63, line, NULL);
						}
					}
					if (defFunindex == NULL && callFunindex != NULL)
					{
						if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != entryIT.iddatatype)
						{
							throw ERROR_THROW_IN(50, line, position);
						}
						numberparm++;
						bufferCountFunParm--;
					}

				}
				else if (!findFunc) {	// если переменная  
					_mbscpy(entryIT.visibility, prefix);
					entryIT.idtype = IT::V;

					if (entryIT.iddatatype == IT::BYTE)
						entryIT.value.vbyte = TI_BYTE_DEFAULT;	// выделить память на int (пустой)
					if (entryIT.iddatatype == IT::STR) 
					{
						entryIT.value.vstr.len = 0;		// устанавливаем длину строки в 0
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));	// заполнить первый символ-> TI_STR_DEFAULT
					}
					if (entryIT.iddatatype == IT::BOOL)
						entryIT.value.vbool = TI_BOOL_DEFAULT;
				}
				else {		// если функция
					_mbscpy(oldprefix, prefix);		// копируем префикс в старый префикс
					_mbscpy(prefix, word[i]);		// копируем идентификатор в префикс

				}

				entryIT.idxfirstLE = indxLex;	// установить индекс в лт 
				_mbscpy(entryIT.id, word[i]);	// название идентификатора в поле идентификатора экземпляра
				IT::Add(idtable, entryIT);
				findFunc = false;
				continue;
			}
			FST::FST fstLiteralByte(word[i], FST_BYTELIT);
			if (FST::execute(fstLiteralByte))
			{
				int value = 0;	// значение литерала
				int counter = 0;
				if(strstr((const char*)word[i], "b"))
				{
					char c = '2';
					for (int a = 0; a < 8; a++)
					{
						while(word[i][counter]!='b')
						{
							if(word[i][counter] ==c)
							throw  ERROR_THROW_IN(74, line, position);
							counter++;
						}
						counter = 0;
						c++;
					}
					char* buffer = new char[8];
					value =bin_to_dec( strcpy(buffer, (const char*)word[i]) );
					delete[] buffer;
				}
				else if(strstr((const char*)word[i],"o"))
				{
					if (strstr((const char*)word[i], "8") || strstr((const char*)word[i], "9"))
					{
						throw  ERROR_THROW_IN(73, line, position);
					}
					char* buffer = new char[8];
					value = oct_to_dec(strcpy(buffer, (const char*)word[i]));
					delete[] buffer;
				}
				else
				{
					value = atoi((char*)word[i]);
				}
				if (value > 127)
				{
					throw  ERROR_THROW_IN(71, line, position);
				}
													// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vbyte == value && idtable.table[k].iddatatype==IT::IDDATATYPE::BYTE && idtable.table[k].idtype == IT::L)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						if (defFunindex == NULL && callFunindex != NULL)
						{
							if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
							{
								throw ERROR_THROW_IN(51, line, position);
							}
							if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != idtable.table[k].iddatatype)
							{
								throw ERROR_THROW_IN(50, line, position);
							}
							numberparm++;
							bufferCountFunParm--;
						}
						LT::Add(lextable, entryLT);
						findSameID = true;	// нашло такой же
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				////
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				////
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::BYTE;
				entryIT.value.vbyte = value;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)//число/строка/размер/основание
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				if (defFunindex == NULL && callFunindex != NULL)
				{
					if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
					{
						throw ERROR_THROW_IN(51, line, position);
					}
					if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != entryIT.iddatatype)
					{
						throw ERROR_THROW_IN(50, line, position);
					}
					numberparm++;
					bufferCountFunParm--;
				}

				continue;
			}
			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString))
			{
				// запись значения
				int length = _mbslen(word[i]);
				for (int k = 0; k < length; k++)	// перезапись массива, убираем кавычки
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;//отсекаем закр ковычку и дублирующ символ
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (!(_mbscmp(idtable.table[k].value.vstr.str, word[i])))	// поиск таких же строковых литералов 
					{
						findSameID = true;
						if (defFunindex == NULL && callFunindex != NULL)
						{
							if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
							{
								throw ERROR_THROW_IN(51, line, position);
							}
							if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != idtable.table[k].iddatatype)
							{
								throw ERROR_THROW_IN(50, line, position);
							}
							numberparm++;
							bufferCountFunParm--;
						}
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)	// если был найден такой же
				{
					
					continue;
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.value.vstr.str, word[i]);	// запись значиния строкового литерала
				entryIT.value.vstr.len = length - 2;		// запись длины строкового литерала
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				nameLiteral = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала (L + charclit)
				_mbscpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				if (defFunindex == NULL && callFunindex != NULL)
				{
					if (bufferCountFunParm == 0)//если параметров в функции больше нет а аргументы есть
					{
						throw ERROR_THROW_IN(51, line, position);
					}
					if (idtable.table[lextable.table[callFunindex].idxTI].funparm.parmtypes[numberparm] != entryIT.iddatatype)
					{
						throw ERROR_THROW_IN(50, line, position);
					}
					numberparm++;
					bufferCountFunParm--;
				}
				if (idtable.table[idtable.size-1].value.vstr.len > 32)
				{
					throw ERROR_THROW_IN(70, line, position);
				}
				continue;
			}
			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, NULL, line, word[i]);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstSemicolon(word[i], FST_SEMICOLON);
			if (FST::execute(fstSemicolon))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			
				if (EqualOperator!=NULL)//если в операторе было = проверяем на rvalue
				{
					if (lextable.table[EqualOperator - 1].lexema != LEX_ID && idtable.table[lextable.table[EqualOperator - 1].idxTI].idtype != IT::IDTYPE::V)
					{
						throw ERROR_THROW_IN(48, line, position);
					}
					EqualOperator = NULL;
				}
				if (Returnindex != NULL)
				{
					bool Fparm = false;
					if(lextable.table[Funindex].lexema != LEX_MAIN)
					for (int q = Returnindex; q < lextable.size; q++)
					{
						if (lextable.table[q].lexema == LEX_ID || lextable.table[q].lexema == LEX_LITERAL)
						{
						    if (idtable.table[lextable.table[q].idxTI].iddatatype != idtable.table[lextable.table[Funindex].idxTI].iddatatype && Fparm == false)
							{
								throw ERROR_THROW_IN(62, line, position);
							}
							if (idtable.table[lextable.table[q].idxTI].idtype == IT::F)
							{
								Fparm = true;
							}
						}
						if (lextable.table[q].lexema == LEX_RIGHTTHESIS)
						{
							Fparm = false;
						}
					}
					Funindex = NULL;
					Returnindex = NULL;
				}
				continue;
			}
			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}  
			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrace(word[i], FST_RIGHTBRACE);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				if(Funindex)
				if (findReturn == false && lextable.table[Funindex].lexema != LEX_MAIN)
				{
					throw ERROR_THROW_IN(60, line, position);
				}
				LT::Add(lextable, entryLT);
				findReturn = false;
				Funindex = NULL;
				_mbscpy(oldprefix, prefix);		// копируем префикс в старый префикс
				unsigned char* nullprefix = new unsigned char[10]{ "" };
				_mbscpy(prefix, nullprefix);
				continue;
			}
			FST::FST fstLeftBrackets(word[i], FST_LEFTBRACKETS);//
			if (FST::execute(fstLeftBrackets))
			{
				findIF = true;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACKETS, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrackets(word[i], FST_RIGHTBRACKETS);//
			if (FST::execute(fstRightBrackets))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTBRACKETS, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findIF = false;
				continue;
			}
			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				if (lextable.table[lextable.size - 3].lexema == LEX_FUNCTION)//если предыдущая лексема функция то флаг параметр
					findParm = true;
				continue;
			}
			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE)		// если после функции нет {
					_mbscpy(prefix, oldprefix);		// возвращаем предыдущ префикс
				findParm = false;
				if (bufferCountFunParm != 0)//если параметры функции остались
				{
					throw ERROR_THROW_IN(51, line, position);
				}
				numberparm = 0;
				callFunindex = NULL;
				LT::Add(lextable, entryLT);
				defFunindex = NULL;
				continue;
			}
			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				EqualOperator = lextable.size;
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			position += _mbslen(word[i]);
			if (word[i][0] == DIV) {
				line++;
				position = 0;
				indxLex--;
				continue;
			}
			throw ERROR_THROW_IN(56, line, position); //идентификатор задан неверно
		}
		if (findMain == false)
		{
			throw  ERROR_THROW_IN(52, line, 0);// если мэйна нету 
		}
		lex.idtable = idtable;		// записать в ит полученную таблицу
		lex.lextable = lextable;	// записать в лт полученную таблицу
		return lex;
	}
}
