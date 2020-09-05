#pragma once

#define max_word 1000
#define size_word 20

void clearSpace(unsigned char source[], int size);		// функция удаления пробелов (не в литералах)
unsigned char** divideText(unsigned char source[], int size);		// функция разделения потока слов на отдельные обьекты (слова,сепараторы, и т.д.)