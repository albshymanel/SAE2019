#include "stdafx.h"      

namespace Error
{

	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),		// код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5), ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY(20, "Параметр -in должен быть задан"),
		ERROR_ENTRY(21, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(22),ERROR_ENTRY_NODEF(23),ERROR_ENTRY_NODEF(24),
		ERROR_ENTRY_NODEF(25),ERROR_ENTRY_NODEF(26),ERROR_ENTRY_NODEF(27),
		ERROR_ENTRY_NODEF(28),ERROR_ENTRY_NODEF(29),
		ERROR_ENTRY(30, "Ошибка при открытии файла с исходным кодом (-in)"),//+
		ERROR_ENTRY(31, "Недопустимый символ в исходном файле (-in)"),//+
		ERROR_ENTRY(32, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(33,"Ошибка открытия выходного asm файла"),
		ERROR_ENTRY_NODEF(34),
		ERROR_ENTRY_NODEF(35),ERROR_ENTRY_NODEF(36),
		ERROR_ENTRY_NODEF(37),ERROR_ENTRY_NODEF(38),ERROR_ENTRY_NODEF(39),
		ERROR_ENTRY(40,"Превышен максимльный размер таблицы лексем"),//+
		ERROR_ENTRY(41,"Таблица лексем переполнена"),//+
		ERROR_ENTRY(42,"Ошибка лексического анализа"),//+
		ERROR_ENTRY(43,"Таблица идентификаторов переполнена"),//+
		ERROR_ENTRY(44,"Превышен максимльный размер таблицы идентификаторов"),//+
		ERROR_ENTRY(45,""),
		ERROR_ENTRY(46,""),
		ERROR_ENTRY(47,"Тип переменной и присваемого значения не совпадают"),//+
		ERROR_ENTRY(48,"Вырожение должно быть допустимым для изменения левосторонним значением"),//5=bl;
		ERROR_ENTRY(49,"Выражение должно относиться к целочисленному типу"),//100b+"string"

		ERROR_ENTRY(50,"Неверный тип передоваемого параметра"),//Fmath(c,2);
		ERROR_ENTRY(51,"Неверное количество параметров"),//Fmath(a);
		ERROR_ENTRY(52,"Функция main не обЪявлена"),//+
		ERROR_ENTRY(53,"Функция main обЪявлена повторно"),//+
		ERROR_ENTRY(54,"Повторное обЪявление переменной"),//+
		ERROR_ENTRY(55,"Повторное обЪявление функции"),//+
		ERROR_ENTRY(56,"Идентификатор задан неверно"),//+
		ERROR_ENTRY(57,"Неизвестный идентификатор"),//+
		ERROR_ENTRY(58,""),//
		ERROR_ENTRY(59,""),

		ERROR_ENTRY(60,"Функция должна возвращать значение"),//+
		ERROR_ENTRY(61,"Оператор return не может находится внутри условного блока"),//+
		ERROR_ENTRY(62,"Неверный возвращаеммый тип"),//+
		ERROR_ENTRY(63,"Функция не может принимать больше 3-x аргументов"),//+
		ERROR_ENTRY(64,""),//+
		ERROR_ENTRY(65,""),//+
		ERROR_ENTRY(66,""),//+
		ERROR_ENTRY(67,""),//+
		ERROR_ENTRY(68,""),//+
		ERROR_ENTRY(69,""),

		ERROR_ENTRY(70,"Длинна строки больше максимально допустимой"),//"Строка из функции Fstrddddddddddddd";
		ERROR_ENTRY(71,"Слишком большой целочисленный литерал"),//+
		ERROR_ENTRY(72,""),//
		ERROR_ENTRY(73,"Запрещенный символ литерале восьмиричной системы счисления"),//+
		ERROR_ENTRY(74,"Запрещенный символ литерале двоичной системы счисления"),//+
		ERROR_ENTRY(75,""),//
		ERROR_ENTRY(76,""),//
		ERROR_ENTRY(77,""),//
		ERROR_ENTRY(78,""),//
		ERROR_ENTRY(79,""),


		ERROR_ENTRY(80,"Неверная структура программы"),//+
		ERROR_ENTRY(81,"Неверная структура функции"),//+
		ERROR_ENTRY(82,"Ошибка в выражении"),//+
		ERROR_ENTRY(83,"Ошибка в формальных параметрах функции"),//+
		ERROR_ENTRY(84,"Ошибка в фактических параметрах  функции"),//+
		ERROR_ENTRY(85,"Ошибка в условном операторе"),//
		ERROR_ENTRY(86,""),//
		ERROR_ENTRY(87,""),//
		ERROR_ENTRY(88,""),//
		ERROR_ENTRY(89,""),

		ERROR_ENTRY_NODEF10(90)
	};

	ERROR GetError(short id)
	{
		ERROR error;
		if (id < 0 || id > ERROR_MAX_ENTRY)
		{
			return errors[0];
		}
		error.id = id;
		strcpy_s(error.message, errors[id].message);
		return error;
	}

	ERROR GetErrorIn(short id, short line = -1, short column = -1)
	{
		ERROR error;
		if (id < 0 || id > ERROR_MAX_ENTRY)
		{
			return errors[0];
		}
		error.id = id;
		error.inext.line = line;
		error.inext.column = column;
		strcpy_s(error.message, errors[id].message);
		return error;
	}
};