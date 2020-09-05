#pragma once
#include "Error.h"

typedef short GRBALPHABET;	// символы алфавита грамматики терминалы > 0,нетерминалы < 0
namespace GRB
{
	struct Rule //правило в грамматике 
	{
		GRBALPHABET nn;//алфавит
		int iderror;
		short size;

		struct Chain //цепочка
		{
			short size;
			GRBALPHABET* nt;

			Chain()
			{
				this->size = 0;
				this->nt = 0;
			};

			Chain(short symbolCount, GRBALPHABET s, ...);//количество символов в цепочке символы

			char* getCChain(char* b);		//получить правую сторону правила
			static GRBALPHABET T(char t) { return GRBALPHABET(t); }
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }
			static bool isT(GRBALPHABET s) { return s > 0; }
			static bool isN(GRBALPHABET s) { return s < 0; }
			static char alphabet_to_char(GRBALPHABET s) { return (isT(s) ? char(s) : char(-s)); }
		}*chains;

		Rule()
		{
			this->nn = 0;
			this->size = 0;
			this->chains = nullptr;
			this->iderror = -1;
		}
		Rule(GRBALPHABET nTerm, int iderror, short rules_count, Chain c, ...);

		char* getCRule(char* b, short nchain);
		short getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j);
	};

	struct Greibach // грамматика Грейбах
	{
		short size;
		GRBALPHABET startN;		//стартовый символ
		GRBALPHABET stbottomT;  //дно стека
		Rule* rules;

		Greibach()
		{
			this->size = 0;
			this->startN = 0;
			this->stbottomT = 0;
			this->rules = 0;
		};
		Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short rulesCount, Rule r, ...);

		short getRule(GRBALPHABET nTerm, Rule& rule);
		Rule getRule(short n);
	};

	Greibach getGreibach();
}
