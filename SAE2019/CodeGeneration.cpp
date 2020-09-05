#include "stdafx.h"

using namespace std;
void CodeGeneration::CodeGeneration(Lex::LEX lex, Log::LOG log)
{
	ofstream asmFile(ASM_FILE_PATH, ios_base::trunc);//очистили
	if (!asmFile.is_open())
	{
		throw ERROR_THROW(33);
	}
	asmFile << ASM_FILE_HEADER;
	asmFile << ASM_LIB_PATH << endl;
	asmFile << ASM_EXIT_PROC << endl;
	asmFile << ASM_EXTERN << ASM_SPACE << FUN_TOCONSOLE_STR << endl;
	asmFile << ASM_EXTERN << ASM_SPACE << FUN_TOCONSOLE_INT << endl;
	asmFile << ASM_EXTERN << ASM_SPACE << FUN_NEGATIVE << endl;
	asmFile << ASM_EXTERN << ASM_SPACE << FUN_POWB << endl;
	asmFile << ASM_EXTERN << ASM_SPACE << FUN_OVERFLOWEXCEPTION << endl;
	asmFile << ASM_EXTERN << ASM_SPACE << FUN_NULLEXCEPTION << endl << endl;
	//библиотечные функции
	asmFile << ASM_STACK_BLOCK;
	asmFile << ASM_CONST_BLOCK;
	for (int i = 0; i < lex.idtable.size; i++)
	{
		if ( lex.idtable.table[i].idtype == IT::L)
		{
			switch (lex.idtable.table[i].iddatatype)
			{
			case IT::BYTE:
				asmFile << ASM_TAB << lex.idtable.table[i].id << ASM_SPACE <<ASM_BYTE_TYPE << ASM_SPACE << lex.idtable.table[i].value.vbyte;
				break;
			case IT::STR:
				asmFile << ASM_TAB << lex.idtable.table[i].id << ASM_SPACE << ASM_STR_TYPE << ASM_SPACE << ASM_QUOTE << lex.idtable.table[i].value.vstr.str << ASM_QUOTE << LEX_COMMA << '0';
				break;
			case IT::BOOL:
				asmFile << ASM_TAB << lex.idtable.table[i].id << ASM_SPACE << ASM_BOOL_TYPE << ASM_SPACE << (int)lex.idtable.table[i].value.vbool;
				break;
			}
			asmFile << endl;
		}
	}
	asmFile << ASM_DATA_BLOCK;
	for (int i = 0; i < lex.idtable.size; i++)
	{
			if (lex.idtable.table[i].idtype == IT::V)
			{
				asmFile << ASM_TAB << lex.idtable.table[i].id << ASM_SPACE;
				switch (lex.idtable.table[i].iddatatype)
				{
				case IT::BYTE:
					asmFile << ASM_BYTE_TYPE << ASM_SPACE << lex.idtable.table[i].value.vbyte;
					break;
				case IT::STR:
					asmFile << ASM_STR_PTR_TYPE << ASM_SPACE << ASM_NO_VALUE;
					break;
				case IT::BOOL:
					asmFile << ASM_BOOL_TYPE << ASM_SPACE << (int)lex.idtable.table[i].value.vbool;
					break;
				}
				asmFile << endl;
			}
	}
	asmFile << ASM_CODE_BLOCK;
	for (int i = 0; i < lex.lextable.size; i++)
	{
		switch (lex.lextable.table[i].lexema)
		{
		case LEX_TYPE:
			{
				if(lex.lextable.table[i + 1].lexema == LEX_FUNCTION)//вывод сигнатур
				{
					string funcName = string((char*)lex.idtable.table[lex.lextable.table[i + 2].idxTI].id);
					asmFile << funcName << " PROC ";//?запятая
					for (i += 3; lex.lextable.table[i].lexema != LEX_LEFTBRACE; i++)
					{
						if (lex.lextable.table[i].lexema == LEX_ID)
						{
							if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::P)
							{
								asmFile << lex.idtable.table[lex.lextable.table[i].idxTI].id << ": ";
								switch (lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype)
								{
								case IT::BYTE:
									asmFile << ASM_BYTE_TYPE;
									break;
								case IT::STR:
									asmFile << ASM_STR_PTR_TYPE;
									break;
								case IT::BOOL:
									asmFile << ASM_BOOL_TYPE;
									break;
								}
								if (lex.lextable.table[i + 1].lexema != LEX_RIGHTTHESIS)
									asmFile << LEX_COMMA << ASM_SPACE;
							}
						}
					}
					asmFile << endl;
					i++;//-{
					WriteFuncBody(i, lex, asmFile);//==========================================
					asmFile << funcName << " ENDP" << endl << endl;
				}
				break;
			}
		case LEX_MAIN:
		{
			asmFile << "main PROC" << endl;
			WriteFuncBody(++i,lex, asmFile);
			asmFile << ASM_TAB << "push 0" << endl;
			asmFile << ASM_TAB << "call ExitProcess" << endl;
			asmFile << "main" << " ENDP" << endl;
			asmFile << "end main" << endl;
			break;
		}
		}
	}
	asmFile.close();
}
void CodeGeneration::WriteFuncBody(int& i, Lex::LEX lex, std::ofstream& asmFile)
{
	bool nesting = false;
	IT::IdTable idtable = lex.idtable;
	LT::LexTable lextable = lex.lextable;
	for (; lextable.table[i].lexema != LEX_BRACELET &&!nesting; i++)
	{
		switch (lextable.table[i].lexema)
		{
		case LEX_RETURN:
		{
			CalculateExpression(i, lex, asmFile);
			asmFile << ASM_TAB << "pop EAX" << endl;
			asmFile << ASM_TAB << "ret" << endl;
			break;
		}
		case LEX_PRINT:
		{

			IT::IDDATATYPE type = idtable.table[lextable.table[i + 1].idxTI].iddatatype;
			CalculateExpression(i,lex, asmFile);

			if (type != IT::STR)
				asmFile << ASM_TAB << "call ToConsoleNUM" << endl << endl;
			else
				asmFile << ASM_TAB << "call ToConsoleSTR" << endl << endl;
			break;
		}
		case LEX_EQUAL:
		{
			int currentId = lextable.table[i-1].idxTI;
			string idName = string((char*)idtable.table[currentId].id);
			CalculateExpression(i, lex, asmFile);			

			asmFile << ASM_TAB << "pop EAX" << endl;
			if (idtable.table[currentId].iddatatype == IT::BYTE)
			{
				asmFile << ASM_TAB << "cmp EAX,127" << endl;
				asmFile << ASM_TAB << "jg OVERF" << lextable.table[i].sn << endl;//-128 127
				asmFile << ASM_TAB << "cmp EAX,-128" << endl;
				asmFile << ASM_TAB << "jl OVERF" << lextable.table[i].sn << endl;
				asmFile << ASM_TAB << "jmp OVERFEND" << lextable.table[i].sn << endl;
				asmFile << ASM_TAB << "	OVERF" << lextable.table[i].sn << ":" << endl;
				asmFile << ASM_TAB << "push " << lextable.table[i].sn << endl;
				asmFile << ASM_TAB << "call OVERFLOWEXCEPTION" << endl;
				asmFile << ASM_TAB << "push 0" << endl;
				asmFile << ASM_TAB << "call ExitProcess" << endl;
				asmFile << ASM_TAB << "	OVERFEND" << lextable.table[i].sn << ":" << endl;
				asmFile << ASM_TAB << "MOV " << idName << ", EAX" << endl;
			}
			if (idtable.table[currentId].iddatatype == IT::BOOL)
				asmFile << ASM_TAB << "MOV " << idName << ", AL" << endl;
			if (idtable.table[currentId].iddatatype == IT::STR)
				asmFile << ASM_TAB << "MOV " << idName << ", EAX" << endl;
			break;
		}
		case LEX_IF:
		{
			nesting = true;
			int pointIndex = lextable.table[i].sn;
			CalculateExpression(i, lex, asmFile);
			asmFile << ASM_TAB << "pop EAX" << endl;
			asmFile << ASM_TAB << "cmp EAX, 1" << endl;		//if true
			asmFile << ASM_TAB << "jne ELSE" << pointIndex << endl;
			CodeGeneration::WriteFuncBody(++i,lex, asmFile);
			asmFile << ASM_TAB << "ELSE" << pointIndex << ":" << endl;
			i--;
			nesting = false;
			break;
		}
		case LEX_RIGHTBRACKETS:
		{
			nesting = true;
			break;
		}
		case LEX_ID:
		{
			if (idtable.table[lextable.table[i].idxTI].idtype == IT::F)
			{
				int paramCounter = idtable.table[lextable.table[i].idxTI].funparm.countparm;
				string strToCall = string((char*)idtable.table[lextable.table[i].idxTI].id);
				while (lextable.table[i].lexema != LEX_RIGHTTHESIS)
				{
					i++;
				}
				int a = 1;
				while (paramCounter > 0)
				{
					int currentId = lextable.table[i - a].idxTI;

					if (lextable.table[i - a].lexema == LEX_LITERAL || lextable.table[i - a].lexema == LEX_ID)
					{
						switch (idtable.table[currentId].iddatatype)
						{
						case IT::BYTE:
							if (idtable.table[currentId].idtype == IT::L)
								asmFile << ASM_TAB << "push " << idtable.table[currentId].value.vbyte << endl;
							else
							{
								asmFile << ASM_TAB << "MOV EAX, " << idtable.table[currentId].id << endl;
								asmFile << ASM_TAB << "push EAX" << endl;
							}
							break;
						case IT::STR:
							if (idtable.table[currentId].idtype == IT::P)
								asmFile << ASM_TAB << "MOV EAX, " << idtable.table[currentId].id << endl;
							else
								asmFile << ASM_TAB << "LEA EAX, " << idtable.table[currentId].id << endl;
							asmFile << ASM_TAB << "push EAX" << endl;
							break;
						case IT::BOOL:
							if (idtable.table[currentId].idtype == IT::L)
								asmFile << ASM_TAB << "push " << (int)idtable.table[currentId].value.vbool << endl;
							else
							{
								asmFile << ASM_TAB << "MOV EAX, 0" << endl;
								asmFile << ASM_TAB << "MOV AL, " << idtable.table[currentId].id << endl;
								asmFile << ASM_TAB << "push EAX" << endl;
							}
							break;
						}
						paramCounter--;
					}
					a++;
				}
				asmFile << ASM_TAB << "call " << strToCall << endl;
			}
			
			break;
		}

		}
	}
}
void CodeGeneration::CalculateExpression(int& i, Lex::LEX lex, std::ofstream& asmFile)
{
	IT::IdTable idtable = lex.idtable;
	LT::LexTable lextable = lex.lextable;
	i++;
	static int Metka = 0;
	for (; lextable.table[i].lexema != LEX_SEMICOLON;i++)//добавить для if
	{
		unsigned char sign=NULL;
		switch (lextable.table[i].lexema)
		{
		case LEX_EQUAL:
		{
			sign = LEX_EQUAL;
			break;
		}
		case LEX_LITERAL:
		{
			switch (idtable.table[lextable.table[i].idxTI].iddatatype)
			{
			case IT::BOOL:
			{
				asmFile << ASM_TAB << "push " << (int)idtable.table[lextable.table[i].idxTI].value.vbool << endl;
				break;
			}
			case IT::BYTE:
			{
				asmFile << ASM_TAB << "push " << idtable.table[lextable.table[i].idxTI].value.vbyte << endl;
				break;
			}
			case IT::STR:
			{
				asmFile << ASM_TAB << "LEA EAX, " << idtable.table[lextable.table[i].idxTI].id << endl;
				asmFile << ASM_TAB << "push EAX" << endl;
				break;
			}
			default:
				break;
			}
			break;
		}
		case LEX_ID:
		{
			if (idtable.table[lextable.table[i].idxTI].idtype == IT::F)
			{
				int paramCounter = idtable.table[lextable.table[i].idxTI].funparm.countparm;
				string strToCall = string((char*)idtable.table[lextable.table[i].idxTI].id);
				while (lextable.table[i].lexema != '|'  )
				{
					i++;
				}
				int a = 1;
				while (paramCounter > 0)
				{
					int currentId = lextable.table[i - a].idxTI;
					if (lextable.table[i - a].lexema == LEX_LITERAL || lextable.table[i - a].lexema == LEX_ID)
					{
						switch (idtable.table[currentId].iddatatype)
						{
						case IT::BYTE:
							if (idtable.table[currentId].idtype == IT::L)
								asmFile << ASM_TAB << "push " << idtable.table[currentId].value.vbyte << endl;
							else
							{
								asmFile << ASM_TAB << "MOV EAX, " << idtable.table[currentId].id << endl;
								asmFile << ASM_TAB << "push EAX" << endl;
							}
							break;
						case IT::STR:
							if (idtable.table[currentId].idtype == IT::V)
								asmFile << ASM_TAB << "MOV EAX, " << idtable.table[currentId].id << endl;
							else
								asmFile << ASM_TAB << "LEA EAX, " << idtable.table[currentId].id << endl;
							asmFile << ASM_TAB << "push EAX" << endl;
							break;
						case IT::BOOL:
							if (idtable.table[currentId].idtype == IT::L)
								asmFile << ASM_TAB << "push " << (int)idtable.table[currentId].value.vbool << endl;
							else
							{
								asmFile << ASM_TAB << "MOV EAX, 0" << endl;
								asmFile << ASM_TAB << "MOV AL, " << idtable.table[currentId].id << endl;
								asmFile << ASM_TAB << "push EAX" << endl;
							}
							break;
						}
						paramCounter--;
					}
					a++;
				}
				asmFile << ASM_TAB << "call " << strToCall << endl;
				asmFile << ASM_TAB << "push EAX" << endl << endl;
				break;
			}
			else
			{
				asmFile << ASM_TAB << "MOV EAX, 0" << endl;
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::STR)
					asmFile << ASM_TAB << "MOV EAX, " << idtable.table[lextable.table[i].idxTI].id << endl;
				else if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::BOOL)
					asmFile << ASM_TAB << "MOV AL, " << idtable.table[lextable.table[i].idxTI].id << endl;
				else if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::BYTE)
					asmFile << ASM_TAB << "MOV EAX, " << idtable.table[lextable.table[i].idxTI].id << endl;
				asmFile << ASM_TAB << "push EAX" << endl;
				break;
			}
		}
		case LEX_OPERATOR:
		{
			switch (lextable.table[i].oper)
			{
			case PLUS:
			{
				asmFile << ASM_TAB << "MOV EAX, 0" << endl;
				asmFile << ASM_TAB << "pop EDX" << endl;
				asmFile << ASM_TAB << "pop EAX" << endl;
				asmFile << ASM_TAB << "ADD EAX, EDX" << endl;
				asmFile << ASM_TAB << "push EAX" << endl << endl;
				break;
			}
			case MINUS:
			{
				asmFile << ASM_TAB << "MOV EAX, 0" << endl;
				asmFile << ASM_TAB << "pop EDX" << endl;
				asmFile << ASM_TAB << "pop EAX" << endl;
				asmFile << ASM_TAB << "SUB EAX, EDX" << endl;
				asmFile << ASM_TAB << "push EAX" << endl << endl;
				break;
			}
			case STAR:
			{
				asmFile << ASM_TAB << "MOV EAX, 0" << endl;
				asmFile << ASM_TAB << "pop EDX" << endl;
				asmFile << ASM_TAB << "pop EAX" << endl;
				asmFile << ASM_TAB << "IMUL EAX, EDX" << endl;
				asmFile << ASM_TAB << "push EAX" << endl << endl;
				break;
			}
			case DIRSLASH:
			{
				asmFile << ASM_TAB << "pop EBX" << endl;
				asmFile << ASM_TAB << "pop EAX" << endl;
				//
				asmFile << ASM_TAB << "cmp EBX,0" << endl;
				asmFile << ASM_TAB << "jz nullExcept"<<Metka << endl;
				asmFile << ASM_TAB << "jmp last" <<Metka<< endl;
				asmFile << ASM_TAB << "nullExcept" <<Metka<<":"<< endl;
				asmFile << ASM_TAB << "push " << lextable.table[i].sn << endl;
				asmFile << ASM_TAB << "call NULLEXCEPTION" << endl;
				asmFile << ASM_TAB << "push 0" << endl;
				asmFile << ASM_TAB << "call ExitProcess" << endl;
				asmFile << ASM_TAB << "last" << Metka << ":" << endl;
				//
				asmFile << ASM_TAB << "idiv EBX" << endl;
				asmFile << ASM_TAB << "push EAX" << endl << endl;
				Metka++;
				break;
			}
			case REMAINDER://деление с остатком
			{
				asmFile << ASM_TAB << "pop EBX" << endl;
				asmFile << ASM_TAB << "mov EDX,0" << endl;
				asmFile << ASM_TAB << "pop EAX" << endl;
				//
				asmFile << ASM_TAB << "cmp EBX,0" << endl;
				asmFile << ASM_TAB << "jz nullExcept" << Metka << endl;
				asmFile << ASM_TAB << "jmp last" << Metka << endl;
				asmFile << ASM_TAB << "nullExcept" << Metka << ":" << endl;
				asmFile << ASM_TAB << "push " << lextable.table[i].sn << endl;
				asmFile << ASM_TAB << "call NULLEXCEPTION" << endl;
				asmFile << ASM_TAB << "push 0" << endl;
				asmFile << ASM_TAB << "call ExitProcess" << endl;
				asmFile << ASM_TAB << "last" << Metka << ":" << endl;
				//
				asmFile << ASM_TAB << "idiv EBX" << endl;
				asmFile << ASM_TAB << "push EDX" << endl;
				Metka++;
				break;
			}
			case EQUALSYING:
			{
				sign = EQUALSYING;
				break;
			}
			default:
				break;
			}
			asmFile << endl;
		}
		if (sign != NULL)
		{
			int lineIfOperator = lextable.table[i].sn;
			if (sign == EQUALSYING)//EAX == EBX?
			{
				asmFile << ASM_TAB << "pop EBX" << endl;
				asmFile << ASM_TAB << "pop EAX" << endl;
				asmFile << ASM_TAB << "cmp EAX, EBX" << endl;
				asmFile << ASM_TAB << "jne ELSEPOINT" << lineIfOperator << endl;
				asmFile << ASM_TAB << "push 1" << endl;
				asmFile << ASM_TAB << "jmp ENDPOINT" << lineIfOperator << endl;
				asmFile << ASM_TAB << "ELSEPOINT" << lineIfOperator << ":" << endl;
				asmFile << ASM_TAB << "push 0" << endl;
				asmFile << ASM_TAB << "ENDPOINT" << lineIfOperator << ":" << endl << endl;
				break;
			}
		}
		}	
	}
}