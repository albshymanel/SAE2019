#pragma once  
#define IN_MAX_LEN_TEXT 1024*1024		// максимальный размер исходного кода = 1МВ
#define IN_CODE_ENDL '\n'				// символ конца строки

// таблица проверки входной информации, индекс = код (Windows-1251) символа
// значения IN::F - запрещенный символ, IN::T - разршенный символ, IN::I - игнорировать (не вводить),
//		если 0 <= значение < 256 - то вводится данное значение
#define IN_CODE_TABLE {\
		/*0*/  /*1*/  /*2*/  /*3*/  /*4*/  /*5*/  /*6*/  /*7*/  /*8*/  /*9*/  /*10*/ /*11*/ /*12*/ /*13*/ /*14*/ /*15*/																									\
/*0*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*1*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*2*/	IN::T, IN::T, IN::T, IN::F, IN::F, IN::T, IN::F, IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*3*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T, IN::F, IN::T, IN::F, IN::T, \
/*4*/	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*5*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T, IN::F, IN::T, \
/*6*/	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*7*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T, IN::F, IN::F, \
		/*0*/  /*1*/  /*2*/  /*3*/  /*4*/  /*5*/  /*6*/  /*7*/  /*8*/  /*9*/  /*10*/ /*11*/ /*12*/ /*13*/ /*14*/ /*15*/																									\
/*8*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*9*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*10*/	IN::T, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, \
/*11*/	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*12р*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*13*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*14*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*15*/	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
}
namespace In
{
	struct IN			// исходные данные
	{
		enum { T = 1024, F = 2048, I = 4096 };	// Т - допустимый символ, F - недопустимый, I - игнорировать, иначе заменить
		int size;								// размер исходного кода
		int lines;								// количество строк
		int ignore;						// количество проигнорированных символов
		unsigned char* text;					// исходный код (Windows - 1251)
		int code[256];							// таблица проверки: T, F, I новое значение
	};
	static const IN INITIN = { NULL, NULL,NULL,NULL,NULL,NULL };
	IN GetIn(wchar_t infile[]);					// ввести и проверить входной поток
};