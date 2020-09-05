#include "pch.h"
#include "framework.h"
#include <iostream>
#include <Windows.h>
#pragma warning(disable: 4996)
extern "C"
{
	void ToConsoleSTR(char* str)
	{ 
		setlocale(LC_ALL, "rus");
		std::cout << str << std::endl;
	}

	void ToConsoleNUM(int num)
	{
		std::cout << num << std::endl;
	}
	void NULLEXCEPTION(int s)
	{
		setlocale(LC_ALL, "rus");
		std::cout << "Ошибка 100: Деление на 0 строка "<<s<< std::endl;
	}
	void OVERFLOWEXCEPTION(int st)
	{
		setlocale(LC_ALL, "rus");
		std::cout << "Ошибка 101: Переполнение строка " << st << std::endl;
	}
	int negative(int i)
	{
		i = -i;
		return i;
	}

	int powb(int x, int y)
	{
		return pow(x, y);
	}
	
}
