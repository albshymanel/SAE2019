#include "stdafx.h"

void clearSpace(unsigned char source[], int size)			// очистка пробельных символов
{
	char SP[] = { " ,;(){}=+-*%/?|" };
	bool findLit = false;
	for (int i = 0; i < size; i++) { // установка сепараторов
		if (source[i] == '\n')
			source[i] = '|';
	}
	for (int i = 0; i < size; i++) 
	{
		if (source[i] == '\"')	// если встречается ковычка
			findLit = !findLit;	// установить флаг в противоположное (false)
		if ( source[i] == ' ' && findLit == false)
		{		// если i-тый символ - пробел и это не литерал
			for (int j = 0; j < sizeof(SP) - 1; j++)
			{
				if (source[i + 1] == SP[j] || source[i - 1] == SP[j] || i == 0)
				{	// если следующий или предыдущий символ - сепараторы
					for (int k = i; k < size; k++)
					{	// сдвиг массива
						source[k] = source[k + 1];
					}
					i--;		// возврат на ту же позицию
					break;
				}
			}
		}
	}
}

unsigned char** divideText(unsigned char source[], int size)		// формирование массива строк
{
	unsigned char** word = new unsigned char* [max_word];	//  массив слов
	for (int i = 0; i < max_word; i++)
		word[i] = new unsigned char[size_word] {NULL};

	bool findSP, findLit = false;	// найден сепаратор, найден литерал
	int j = 0;
	char SP[] = { " ,;(){}=+-*%/?|" };
	for (int i = 0, k = 0; i < size - 1; i++, k++)
	{
		findSP = false;
		if (source[i] == '\"')	// если встречается ковычка
			findLit = !findLit;	// установить флаг в противоположное (false)

		for (int t = 0; t < sizeof(SP) - 1; t++)
		{
			if (source[i] == SP[t] && findLit == false)		// если i-тый - сепаратор и это не литерал
			{
				findSP = true;
				if (word[j][0] != NULL) {  // если в начале слова не null
					word[j++][k] = '\0';	// то записать в конец слова \0
					k = 0;	// начать столбец с первого символа
				}
				if (SP[t] == ' ') {  // если символ - пробел => переход к следующему слову
					k = -1;
					break;
				}
				word[j][k++] = SP[t];	// записать сепаратор
				word[j++][k] = '\0';	// записать в конец слова \0
				k = -1;		// установить в предначальную позицию столбца
				break;
			}
		}
		if (!findSP)	// если не сепаратор
			word[j][k] = source[i];		//записать символ в массив
	}
	word[j] = NULL;		//последующие строки в null
	for (int i = 0; i < j; i++)
		if (!strcmp((char*)word[i], ""))	// если есть пустая строка => вернуть null
			return NULL;
	return word;	// иначе - массив слов
}

