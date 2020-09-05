#include "stdafx.h"

namespace In
{
	IN GetIn(wchar_t infile[])
	{
		setlocale(LC_ALL, "rus");
		IN in;
		std::ifstream file;
		int i = 0, column = -1;
		in.size = 0;
		in.ignore = 0;
		in.lines = 1;
		unsigned char c;
		in.text = new unsigned char[IN_MAX_LEN_TEXT];
		int code[256] = IN_CODE_TABLE;
		for (int k = 0; k < 256; k++)
		{
			in.code[k] = code[k];
		}
		file.open(infile);
		if (file.fail())
		{
			throw ERROR_THROW(30);//не открылся файл in
		}
		while (true)
		{
			c = file.get();
			if (file.eof())
			{
				break;
			}
			in.size++; column++;
			if (in.code[(int)c] == in.F) 
			{
				throw ERROR_THROW_IN(31, in.lines - 1, column);//недопустимый символ
			}
			else if (in.code[(int)c] == in.T) 
			{
				in.text[i] = c;
				i++;
			}
			else if (in.code[(int)c] == in.I) 
			{
				in.ignore++;
				in.size--;
			}
			else
			{
				in.text[i] = in.code[(int)c];
				i++;
			}
			if (c == IN_CODE_ENDL)
			{
				in.lines++;
				column = -1;
			}
		}
		in.text[i] = '\0';
		return in;
	}
}