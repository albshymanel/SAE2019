
#include "stdafx.h"


MFST::MFSTState::MFSTState()
{
	this->lenta_position = 0;
	this->nrulechain = -1;
	this->nrule = -1;
}

MFST::MFSTState::MFSTState(short posInLent, std::stack<short>& a_stack, short currentChain)
{
	this->lenta_position = posInLent;
	this->stack_mfst = a_stack;
	this->nrulechain = currentChain;
}

MFST::MFSTState::MFSTState(short posInLent, std::stack<short>& a_stack, short currentRule, short currentChain)
{
	this->lenta_position = posInLent;
	this->stack_mfst = a_stack;
	this->nrule = currentRule;
	this->nrulechain = currentChain;
}

MFST::MFST::MFST_Diagnosis::MFST_Diagnosis()
{
	this->lenta_position = -1;
	this->rc_step = RC_STEP::SURPRISE;
	this->nrule = -1;
	this->nrulechain = -1;
}

MFST::MFST::MFST_Diagnosis::MFST_Diagnosis(short posInLent, RC_STEP rc_step, short ruleNum, short ruleChainNum)
{
	this->lenta_position = posInLent;
	this->rc_step = rc_step;
	this->nrule = ruleNum;
	this->nrulechain = ruleChainNum;
}

MFST::MFST::MFST()
{
	this->lenta = 0;
	this->lenta_size;
	this->lenta_position = 0;
}

MFST::MFST::MFST(const LT::LexTable& lexTable, GRB::Greibach grebach, Log::LOG log)
{
	this->log = log;
	this->grebach = grebach;//перед записью грамматики грейбах инициализируется grebach и mfst через конструкторы без параметров
	this->lexTable = lexTable;
	this->lenta = new short[this->lenta_size = lexTable.size];

	for (int i = 0; i < this->lenta_size; i++)
		this->lenta[i] = GRB::Rule::Chain::T(lexTable.table[i].lexema);//заполняем ленту из таблицы лексем

	this->lenta_position = 0;
	this->stack_mfst.push(grebach.stbottomT);
	this->stack_mfst.push(grebach.startN);
	this->nrulechain = -1;
}

MFST::MFST::RC_STEP MFST::MFST::step()
{
	RC_STEP rc = SURPRISE;

	if (this->lenta_position < lenta_size)//если лента не разобрана
	{
		if (GRB::Rule::Chain::isN(stack_mfst.top()))//если в стеке нетерминал
		{
			GRB::Rule rule;// создаем буфер для правила вызывая конструктор
			if ((this->nrule = grebach.getRule(stack_mfst.top(), rule)) >= 0)// если нетерминал есть 
			{
				GRB::Rule::Chain chain;//буфер для записи цепочки
				if ((this->nrulechain = rule.getNextChain(lenta[lenta_position], chain, this->nrulechain + 1)) >= 0)//если цепочка начинающаяся на терминал из ленты есть то
				{
					MFST_TRACE1
						savestate();
					stack_mfst.pop();// убрать символ из стека
					push_chain(chain);//добавить цепочку -1 символ
					rc = NS_OK;
					MFST_TRACE2
				}
				else//если цепочки начинающейся на терминал из ленты нет то
				{
					MFST_TRACE4("TNS_NS_NORULECHAIN/NS_NORULE")
						savedDiagnosis(NS_NORULECHAIN); rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;
				}
			}
			else
				rc = NS_ERROR;
		}
		else if (stack_mfst.top() == lenta[lenta_position])//если в стеке терминал
		{
			lenta_position++;
			stack_mfst.pop();
			nrulechain = -1;
			rc = TS_OK;
			MFST_TRACE3
		}
		else// если в стеке терминал не равный входному на ленте то 
		{
			MFST_TRACE4("TNS_NS_NORULECHAIN/NS_NORULE")
				rc = resetstate() ? TS_NOK : NS_NORULECHAIN;
		}
	}
	else// лента разобрана 
	{
		rc = LENTA_END;
		MFST_TRACE4("LENTA_END")
	}

	return rc;
}

bool MFST::MFST::push_chain(GRB::Rule::Chain chain)//добавить в стек правило без симола который убрали
{
	for (int i = chain.size - 1; i >= 0; i--)
		stack_mfst.push(chain.nt[i]);
	return true;
}

bool MFST::MFST::savestate()//сохранение состояния 
{
	storestate.push(MFSTState(lenta_position, stack_mfst, nrule, nrulechain));
	MFST_TRACE6("SAVESTATE:", storestate.size())//вывод сохраненного состояния
		return true;
}

bool MFST::MFST::resetstate()//возврат к предыдущему сохраненному состоянию
{
	bool rc = false;
	MFSTState state;

	if ((rc = storestate.size()) > 0)
	{
		state = storestate.top();
		lenta_position = state.lenta_position;
		stack_mfst = state.stack_mfst;
		nrule = state.nrule;
		nrulechain = state.nrulechain;
		storestate.pop();

		MFST_TRACE5("RESTATE")
			MFST_TRACE2
	}

	return rc;
}

bool MFST::MFST::savedDiagnosis(RC_STEP prc_step)//получить код завершения шага
{
	bool rc = false;
	short k = 0;

	while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		k++;

	if (rc = (k < MFST_DIAGN_NUMBER))
	{
		diagnosis[k] = MFST_Diagnosis(lenta_position, prc_step, nrule, nrulechain);

		for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
			diagnosis[i].lenta_position = -1;
	}

	return rc;
}

bool MFST::MFST::start()
{
	bool rc = false;
	RC_STEP rc_step = SURPRISE;
	char buf[MFST_DIAGN_MAXSIZE]{};
	rc_step = step();
	while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		rc_step = step();

	switch (rc_step)
	{
	case LENTA_END:
	{
		MFST_TRACE4("------>LENTA_END")
			*log.stream << "------------------------------------------------------------------------------------------   ------" << std::endl;
		sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
		*log.stream << std::setw(4) << std::left << 0 << "всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок" << std::endl;
		rc = true;
		break;
	}

	case NS_NORULE:
	{
		MFST_TRACE4("------>NS_NORULE")
			std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
		*log.stream << getDiagnosis(0, buf) << std::endl;
		*log.stream << getDiagnosis(1, buf) << std::endl;
		*log.stream << getDiagnosis(2, buf) << std::endl;
		break;
	}

	case NS_NORULECHAIN: MFST_TRACE4("------>NS_NORULECHAIN") break;
	case NS_ERROR: MFST_TRACE4("------>NS_ERROR") break;
	case SURPRISE: MFST_TRACE4("------>NS_SURPRISE") break;


	}

	return rc;
}

char* MFST::MFST::getCSt(char* buf)
{
	short p;
	for (int i = (signed)stack_mfst.size() - 1; i >= 0; --i)
	{
		p = stack_mfst._Get_container()[i];
		buf[stack_mfst.size() - 1 - i] = GRB::Rule::Chain::alphabet_to_char(p);//переписываем стек
	}

	buf[stack_mfst.size()] = '\0';

	return buf;
}

char* MFST::MFST::getCLenta(char* buf, short pos, short n)
{
	short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;

	for (int i = pos; i < k; i++)
		buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

	buf[i - pos] = '\0';
	return buf;
}

char* MFST::MFST::getDiagnosis(short n, char* buf)
{
	char* rc = new char[200]{};
	int errid = 0;
	int lpos = -1;
	if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
	{
		errid = grebach.getRule(diagnosis[n].nrule).iderror;
		Error::ERROR err = Error::GetError(errid);
		throw ERROR_THROW_IN(errid, lexTable.table[lpos].sn,NULL);
		//sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lexTable.table[lpos].sn, err.message);
		rc = buf;
	}
	return rc;
}

void MFST::MFST::printRules()
{
	MFSTState state;
	GRB::Rule rule;
	for (unsigned short i = 0; i < storestate.size(); i++)
	{
		state = storestate._Get_container()[i];
		rule = grebach.getRule(state.nrule);
		MFST_TRACE7
	}
}

bool MFST::MFST::saveoutputTree()
{
	MFSTState state;
	GRB::Rule rule;
	output.nrules = new short[output.stepscounter = storestate.size()];
	output.nrulechain = new short[output.stepscounter];

	for (unsigned short i = 0; i < storestate.size(); i++)
	{
		state = storestate._Get_container()[i];
		output.nrules[i] = state.nrule;
		output.nrulechain[i] = state.nrulechain;
	}
	return true;
}