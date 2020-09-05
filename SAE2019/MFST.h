#pragma once
#include "GRB.h"
#include "LT.h"
#include <stack>
#include <iomanip>
#include "Log.h"
#define MFST_DIAGN_MAXSIZE 2 * ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

static int FST_TRACE_n = -1;
static char rbuf[1000]{}, sbuf[205]{}, lbuf[1000]{};
#define MFST_TRACE_START *log.stream<<std::setfill(' ')<<std::setw( 4)<< std::left<<"Шаг"<< ':'		\
								  <<std::setw(20)<< std::left<<" Правило"		\
								  <<std::setw(30)<< std::left<<" Входная лента"	\
								  <<std::setw(20)<< std::left<<" Стек"			\
								  <<std::endl;

#define MFST_TRACE1		*log.stream<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, nrulechain) \
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;
#define MFST_TRACE2		*log.stream<<std::setw(4)<<std::left<<FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<< " "	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;
#define MFST_TRACE3		*log.stream<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "	\
									<<std::setw(20)<<std::left<< " "	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf)	\
									<<std::endl;

#define MFST_TRACE4(c) *log.stream<<std::setw(4)<<std::left<<++FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(c) *log.stream<<std::setw(4)<<std::left<<  FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE6(c,k) *log.stream<<std::setw(4)<<std::left<<  FST_TRACE_n<< ": "<<std::setw(20)<<std::left<<c<<k<<std::endl;
#define	MFST_TRACE7		*log.stream << std::setw(4) << std::left << state.lenta_position<<": "	\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, state.nrulechain)	\
									<<std::endl;


namespace MFST
{
	struct MFSTState	// состояние автомата (для сохранения)
	{
		short lenta_position;
		short nrule;
		short nrulechain;		// номер текущей цепочки, текущего правила
		std::stack<short> stack_mfst;	// стек автомата

		MFSTState();
		MFSTState(short posInLent, std::stack<short>& a_steck, short currentChain);
		MFSTState(short posInLent, std::stack<short>& a_stack, short currentRule, short currentChain);

	};

	struct MFST // магазинный автомат
	{
		enum RC_STEP //код возврата функции step
		{
			NS_OK,			// найдено правило и цепочка, цепочка записана в стек 
			NS_NORULE,		// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,	// не найдена походящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,		// неизвесный нетерминальный символ грамматики
			TS_OK,			// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,			// тек. символ ленты != вершине стека, восстановленно состояние 
			LENTA_END,		// теущая позиция ленты >= lenta_size 
			SURPRISE		// неожиданный код возврата (ошибка в step)
		};

		struct MFST_Diagnosis	// диагностика
		{
			short	lenta_position;		// позиция на ленте 
			RC_STEP	rc_step;			// код завершения шага 
			short	nrule;			// номер правила 
			short	nrulechain;		// номер цепочки правила
			MFST_Diagnosis();
			MFST_Diagnosis(short lenta_position, RC_STEP rc_step, short nrule, short nrulechain);
		} diagnosis[MFST_DIAGN_NUMBER]; // последние самые глубокие сообщения

		GRBALPHABET* lenta;					// перекодированная (TS/NS) лента (из LEX)
		short lenta_position;				// текущая позиция на ленте
		short nrule;					// номер текущего правила
		short nrulechain;				// номер текущей цепочки, текущего правила
		short lenta_size;					// размер ленты
		GRB::Greibach grebach;				// грамматика Грейбах
		LT::LexTable lexTable;
		std::stack<short> stack_mfst;				// стек автомата
		std::stack<MFSTState> storestate;	// стек для сохранения состояний
		Log::LOG log;
		MFST();
		MFST(const LT::LexTable& lexTable, GRB::Greibach grebach, Log::LOG log);

		char* getCSt(char* buf);			//получить содержиое стека
		char* getCLenta(char* buf, short pos, short n = 25);	//лента: n символов, начиная с pos
		char* getDiagnosis(short n, char* buf);		//получить n-ую строку диагностики или '\0'

		bool savestate();	//сохранить состояние автомата
		bool resetstate();	//восстановить состояние автомата
		bool push_chain(GRB::Rule::Chain chain);

		RC_STEP step();		//выполнить шаг автомата
		bool start();		//запустить автомат
		bool savedDiagnosis(RC_STEP prc_step);

		void printRules();	//вывести последовательность правил

		struct Output
		{
			short stepscounter;
			short* nrules;
			short* nrulechain;

			Output()
			{
				this->stepscounter = 0;
				this->nrules = 0;
				this->nrulechain = 0;
			}
		}output;

		bool saveoutputTree();
	};
}

