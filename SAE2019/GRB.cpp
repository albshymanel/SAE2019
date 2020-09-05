#include "stdafx.h"
#define GRB_ERROR_SERIES 80

#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

namespace GRB
{
	Rule::Chain::Chain(short symbolCount, GRBALPHABET s, ...)
	{
		this->nt = new GRBALPHABET[this->size = symbolCount];

		int* ptr = (int*)& s;
		for (int i = 0; i < symbolCount; i++)
			this->nt[i] = (short)ptr[i];
	}

	Rule::Rule(GRBALPHABET nTerm, int iderror, short rules_count, Chain c, ...)
	{
		this->nn = nTerm;
		this->iderror = iderror;
		this->chains = new Chain[this->size = rules_count];

		Chain* ptr = &c;
		for (int i = 0; i < rules_count; i++)
			this->chains[i] = ptr[i];
	}

	Greibach::Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short rulesCount, Rule r, ...)
	{
		this->startN = startN;
		this->stbottomT = stbottomT;
		this->rules = new Rule[this->size = rulesCount];
		Rule* ptr = &r;
		for (int i = 0; i < rulesCount; i++)
			rules[i] = ptr[i];
	}

	short Greibach::getRule(GRBALPHABET nTerm, Rule& rule)
	{
		short rc = -1, k = 0;

		while (k < this->size && rules[k].nn != nTerm)//пока нетерминал другой переходим к след номеру правила
			k++;

		if (k < this->size)//если нашелс€ нетерминал то выбираем правила с этим нетерминалом и возвращаем номер этого нетерминала
			rule = rules[rc = k];

		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < this->size)
			rc = rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		this->chains[nchain].getCChain(buf);
		strcat_s(b, sizeof(buf) + 5, buf);

		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;

		while (j < this->size && this->chains[j].nt[0] != t)// пока не встретим такой терминал как в ленте переходим к следующей цепочке
			j++;

		rc = (j < this->size ? j : -1);//если нашли цепочку
		if (rc >= 0)
			pchain = chains[rc];//копируем в наш буфер эту цепочку
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
			b[i] = Chain::alphabet_to_char(this->nt[i]);
		b[this->size] = 0;
		return b;
	}

	Greibach greibach(NS('S'), TS('$'), 7,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, 2,	//Ќеверна€ структура программы
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),//global var
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1, 10,	//ошибка в теле функции
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),

			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),

			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),// - =
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),

			Rule::Chain(2, TS('c'), NS('C')),
			Rule::Chain(3, TS('c'), NS('C'), NS('N')),

			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 2, 8,	//выражение =print
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(3, TS('i'), TS('v'), NS('E')),
			Rule::Chain(3, TS('l'), TS('v'), NS('E')),
			Rule::Chain(5, TS('('), NS('E'), TS(')'), TS('v'), NS('E')),
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS('v'), NS('E'))
		),

		Rule(NS('C'), GRB_ERROR_SERIES + 5, 1,	//условие -(
			Rule::Chain(9, TS('('), NS('E'), TS('v'), NS('E'), TS(')'), TS(';'), TS('['), NS('N'), TS(']') )
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 3, 2,	// параметры функции при определении(формальные) -type
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 4, 4, //параметры вызываемой ф-ции(фактические) вместо , =
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);
	Greibach getGreibach() { return greibach; }
}

