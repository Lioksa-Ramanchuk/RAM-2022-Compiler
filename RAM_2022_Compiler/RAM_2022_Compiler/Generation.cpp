#include "stdafx.h"
#include "LexicalAnalysis.hpp"
using namespace RAM_2022_LexicalAnalysis;
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_LT;
using namespace RAM_2022_IT;
using namespace std;

namespace RAM_2022_Generation
{
	namespace
	{
		enum class eContext
		{
			MAIN,
			FUNC,
			IF,
			ELSE,
			ELIF,
			WHILE
		};

		void translateExpr(const Lexer& LEXER, string& code, uint16_t& i, uint16_t& cmpCounter, uint16_t& overflowCounter, IdDataType type)
		{
			using enum LexemeType;
			for (; LEXER.lexTable.table[i].type != SEMICOLON && LEXER.lexTable.table[i].type != LEFT_BRACE; i++)
			{
				switch (LEXER.lexTable.table[i].type)
				{
				case LITERAL:
				case ID:
				{
					if (LEXER.lexTable.table[i].pIdI.lock()->type == IdType::FUNC)
					{
						code += "\tcall ";
						code += LEXER.lexTable.table[i].pIdI.lock()->getFullId();
						code += "\n\tadd ESP, ";
						code += to_string(LEXER.funcsParamTypes.at(LEXER.lexTable.table[i].pIdI.lock()->getFullId()).size() * 4);
						code += "\n";
						code += "\tpush EAX\n";
					}
					else
					{
						code += "\tpush ";
						if (LEXER.lexTable.table[i].pIdI.lock()->dataType == IdDataType::STR &&
							LEXER.lexTable.table[i].pIdI.lock()->type != IdType::PARAM)
						{
							code += "OFFSET ";
						}
						code += LEXER.lexTable.table[i].pIdI.lock()->getFullId();
						code += "\n";
					}
				} break;
				case ARITHMETIC_BIN_OP:
				{
					code += "\tpop EBX\n";
					code += "\tpop EAX\n";
					if (LEXER.lexTable.table[i].pWORD->word == "+")
					{
						if (type == IdDataType::STR) {
							code += "\tINVOKE _ram2022_strcat, OFFSET _ram2022_buf, EAX, EBX\n";
						}
						else {
							code += "\tadd EAX, EBX\n";
						}
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "-") {
						code += "\tsub EAX, EBX\n";
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "*")
					{
						code += "\tcdq\n";
						code += "\timul EBX\n";
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "/")
					{
						code += "\t.IF EBX == 0\n";
						code += "\tINVOKE _ram2022_catchError, OFFSET _ram2022_DIV_BY_NULL_ERR_MSG, ";
						code += to_string(LEXER.lexTable.table[i].pos.line);
						code += ", ";
						code += to_string(LEXER.lexTable.table[i].pos.posInLine);
						code += "\n\t.ENDIF\n";
						code += "\tcdq\n";
						code += "\tidiv EBX\n";
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "%")
					{
						code += "\t.IF EBX == 0\n";
						code += "\tINVOKE _ram2022_catchError, OFFSET _ram2022_DIV_BY_NULL_ERR_MSG, ";
						code += to_string(LEXER.lexTable.table[i].pos.line);
						code += ", ";
						code += to_string(LEXER.lexTable.table[i].pos.posInLine);
						code += "\n\t.ENDIF\n";
						code += "\tcdq\n";
						code += "\tidiv EBX\n";
						code += "\tmov EAX, EDX\n";
					}

					if (type == IdDataType::STR) {
						code += "\tpush OFFSET _ram2022_buf\n";
					}
					else
					{
						overflowCounter++;
						code += "\tjno ov"; code += to_string(overflowCounter); code += "\n";
						code += "\tINVOKE _ram2022_catchError, OFFSET _ram2022_INT_OVERFLOW_ERR_MSG, ";
						code += to_string(LEXER.lexTable.table[i].pos.line);
						code += ", ";
						code += to_string(LEXER.lexTable.table[i].pos.posInLine);
						code += "\nov"; code += to_string(overflowCounter); code += ":\n";
						code += "\tpush EAX\n";
					}
				} break;
				case ARITHMETIC_UN_OP:
				{
					code += "\tpop EAX\n";
					if (LEXER.lexTable.table[i].pWORD->word == "#+") {
						//
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "#-") {
						code += "\tneg EAX\n";
					}
					code += "\tpush EAX\n";
				} break;
				case CMP_BIN_OP:
				{
					cmpCounter++;
					code += "\tpop EBX\n";
					code += "\tpop EAX\n";
					if (type == IdDataType::STR)
					{
						code += "\tINVOKE _ram2022_strcmp, EAX, EBX\n";
						code += "\tcmp EAX, 0\n";
					}
					else {
						code += "\tcmp EAX, EBX\n";
					}

					if (LEXER.lexTable.table[i].pWORD->word == "~") {
						code += "\tje cmp";
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "!") {
						code += "\tjne cmp";
					}
					else if (LEXER.lexTable.table[i].pWORD->word == ">") {
						code += "\tjg cmp";
					}
					else if (LEXER.lexTable.table[i].pWORD->word == "<") {
						code += "\tjl cmp";
					}
					code += to_string(cmpCounter); code += "_t\n";
					code += "\tpush 0\n";
					code += "\tjmp cmp"; code += to_string(cmpCounter); code += "\n";

					code += "cmp"; code += to_string(cmpCounter); code += "_t:\n";
					code += "\tpush 1\n";
					code += "cmp"; code += to_string(cmpCounter); code += ":\n";
				} break;
				default:
					break;
				}
			}
		};
	}

	void generateMasm(const Lexer& LEXER, ostream& out)
	{
		auto typeLexToStr = [&](const LexemeInfo& LI)
		{
			if (LI.pWORD->word == "int") {
				return "SDWORD";
			}
			else /*if (LI.pWORD->word == "str")*/ {
				return "DWORD";
			}
		};

		string strHeader =
R"(.586P
.MODEL FLAT, C

INCLUDELIB kernel32.lib
INCLUDELIB libucrt.lib
ExitProcess			PROTO STDCALL	:DWORD
SetConsoleOutputCP	PROTO STDCALL	:DWORD
SetConsoleCP		PROTO STDCALL	:DWORD
atoi				PROTO			:DWORD

INCLUDELIB "..\Debug\RAM_2022_ClLib.lib"
_ram2022_strcat		PROTO	:DWORD,	:DWORD,	:DWORD
_ram2022_strcpy		PROTO	:DWORD,	:DWORD
_ram2022_strcmp		PROTO	:DWORD,	:DWORD
_ram2022_readStr	PROTO	:DWORD
_ram2022_writeInt	PROTO	:SDWORD
_ram2022_writeStr	PROTO	:DWORD
_ram2022_catchError	PROTO	:DWORD, :SDWORD, :SDWORD

)";
		string strLibs = "";
		string strExterns = "";
		string strProtos = "";

		string strStack = "\n.STACK 4096\n";
		string strConst =
R"(
.CONST
_ram2022_NEW_LINE				BYTE 10, 0
_ram2022_ERR_LINE_MSG			BYTE ", строка ", 0
_ram2022_ERR_POS_MSG			BYTE ", позиция ", 0
_ram2022_DIV_BY_NULL_ERR_MSG	BYTE 10, "ОШИБКА ВРЕМЕНИ ВЫПОЛНЕНИЯ: Деление на ноль", 0
_ram2022_INT_OVERFLOW_ERR_MSG	BYTE 10, "ОШИБКА ВРЕМЕНИ ВЫПОЛНЕНИЯ: Переполнение значения типа int", 0
)";

		string strData = "\n.DATA\n";
		strData += "_ram2022_buf	BYTE ";
		strData += to_string(IT_MAX_STR_LENGTH + 1);
		strData += " dup(0)\n";

		string strCode =
R"(
.CODE
)";
		vector<string> funcs;
		funcs.emplace_back(
R"(
_ram2022_catchError PROC,
		_ram2022_str	:DWORD,
		_ram2022_line	:SDWORD,
		_ram2022_pos	:SDWORD
	INVOKE _ram2022_writeStr, _ram2022_str
	INVOKE _ram2022_writeStr, OFFSET _ram2022_ERR_LINE_MSG
	INVOKE _ram2022_writeInt, _ram2022_line
	INVOKE _ram2022_writeStr, OFFSET _ram2022_ERR_POS_MSG
	INVOKE _ram2022_writeInt, _ram2022_pos
	INVOKE _ram2022_writeStr, OFFSET _ram2022_NEW_LINE
	INVOKE ExitProcess, -1
	ret
_ram2022_catchError ENDP

)"
		);

		string strEnd =
R"(
END main)";

		for (const auto& P_EXTERN : LEXER.idTable.externs)
		{
			strExterns += "EXTERN\t";
			strExterns += P_EXTERN->getFullId();
			strExterns += "\t:PROC\n";
		}
		for (const auto& P_LITERAL : LEXER.idTable.literals)
		{
			if (LEXER.lexTable.table[P_LITERAL->iFirstLexLT - 1].type == LexemeType::LINK) {
				continue;
			}

			strConst += P_LITERAL->getFullId();
			strConst += "\t";
			switch (P_LITERAL->dataType)
			{
			case IdDataType::INT:
			{
				strConst += "SDWORD\t";
				strConst += to_string(get<int32_t>(P_LITERAL->value));
				strConst += "\n";
			} break;
			case IdDataType::STR:
			{
				strConst += "BYTE\t";
				bool started = false;

				for (const char C : get<string>(P_LITERAL->value))
				{
					switch (C)
					{
					case '\n':
					case '\"':
					{
						if (started) {
							strConst += "\", ";
						}
						started = false;
						strConst += to_string((int)C);
						strConst += ", ";
					} break;
					default:
					{
						if (!started) {
							strConst += "\"";
						}
						started = true;
						strConst += C;
					} break;
					}
				}
				if (started) strConst += "\", ";
				strConst += "0\n";
			} break;
			default:
				break;
			}
		}

		queue<weak_ptr<IdScope>> scopes;
		scopes.push(LEXER.idTable.scope);
		while (!scopes.empty())
		{
			for (const auto& pID_I : scopes.front().lock()->ids)
			{
				if (pID_I->type == IdType::VAR)
				{
					strData += pID_I->getFullId();
					strData += "\t";
					switch (pID_I->dataType)
					{
					case IdDataType::INT:
						strData += "SDWORD ";
						strData += to_string(IT_INT_DEFAULT);
						break;
					case IdDataType::STR:
						strData += "BYTE 256 dup(0)";
					default:
						break;
					}
					strData += "\n";
				}
			}
			for (const auto& pINNER_SCOPE : scopes.front().lock()->innerScopes) {
				scopes.push(pINNER_SCOPE);
			}
			scopes.pop();
		}

		stack<string> funcBodiesToAdd;
		stack<eContext> currentContext;
		uint16_t cmpCounter = 0;
		uint16_t ovCounter = 0;

		for (uint16_t i = 0; i < LEXER.lexTable.size; i++)
		{
			switch (LEXER.lexTable.table[i].type)
			{
				using enum LexemeType;
			case TYPE:
			{
				if (LEXER.lexTable.table[i + 1].type == ID &&
					LEXER.lexTable.table[i + 1].pIdI.lock()->type == IdType::VAR) {
					if (LEXER.lexTable.table[i + 2].type == SEMICOLON) {
						string code = "";
						switch (LEXER.lexTable.table[i + 1].pIdI.lock()->dataType)
						{
						case IdDataType::INT:
						{
							code += "\tmov ";
							code += LEXER.lexTable.table[i + 1].pIdI.lock()->getFullId();
							code += ", ";
							code += to_string(IT_INT_DEFAULT);
							code += "\n";
						} break;
						case IdDataType::STR:
						{
							code += "\tmov ";
							code += LEXER.lexTable.table[i + 1].pIdI.lock()->getFullId();
							code += ", 0\n";
						} break;
						default:
							break;
						}
						if (funcBodiesToAdd.empty()) {
							strCode += code;
						}
						else {
							funcBodiesToAdd.top() += code;
						}
					}
					else {
						i++;
					}
				}
			} break;
			case MAIN:
			{
				strCode +=
R"(main PROC
	INVOKE SetConsoleOutputCP, 1251
	INVOKE SetConsoleCP, 1251
)";
				currentContext.push(eContext::MAIN);
			} break;
			case LINK:
			{
				if (LEXER.lexTable.table[i + 1].type == LITERAL)
				{
					i++;
					strLibs += string("INCLUDELIB \"");
					strLibs += get<string>(LEXER.lexTable.table[i].pIdI.lock()->value);
					strLibs += "\"\n";
				}
			} break;
			case FUNC:
			case PROC:
			{
				if (LEXER.isExternFuncDeclaration(i + 1))
				{
					while (LEXER.lexTable.table[i].type != SEMICOLON) {
						i++;
					}
					break;
				}

				strProtos += LEXER.lexTable.table[i + 1].pIdI.lock()->getFullId();
				strProtos += "\tPROTO";
				for (uint16_t j = i + 3; LEXER.lexTable.table[j].type != RIGHT_PARENTHESIS; j++)
				{
					if (LEXER.lexTable.table[j].type == TYPE)
					{
						if (j != i + 3) {
							strProtos += ",";
						}
						strProtos += "\t:";
						strProtos += typeLexToStr(LEXER.lexTable.table[j]);
					}
				}
				strProtos += "\n";

				string newFunc = "";
				newFunc += LEXER.lexTable.table[i + 1].pIdI.lock()->getFullId();
				newFunc += " PROC";

				for (i += 3; LEXER.lexTable.table[i].type != RIGHT_PARENTHESIS; i++)
				{
					if (LEXER.lexTable.table[i].type == ID)
					{
						newFunc += ",\n\t";
						newFunc += LEXER.lexTable.table[i].pIdI.lock()->getFullId();
						newFunc += "\t:";

						switch (LEXER.lexTable.table[i].pIdI.lock()->dataType)
						{
						case IdDataType::INT:
							newFunc += "SDWORD";
							break;
						case IdDataType::STR:
							newFunc += "DWORD";
							break;
						default:
							break;
						}
					}
				}
				newFunc += "\n";
				funcBodiesToAdd.push(newFunc);
				currentContext.push(eContext::FUNC);
			} break;
			case IF:
			{
				currentContext.push(eContext::IF);
				string code = "";
				i++;
				translateExpr(LEXER, code, i, cmpCounter, ovCounter, LEXER.lexTable.table[i].pIdI.lock()->dataType);
				code += "\tpop EAX\n";
				code += "\t.IF EAX == 1\n";

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			case ELIF:
			{
				currentContext.push(eContext::ELIF);
				string code = "\t.ELSE\n";
				i++;
				translateExpr(LEXER, code, i, cmpCounter, ovCounter, LEXER.lexTable.table[i].pIdI.lock()->dataType);
				code += "\tpop EAX\n";
				code += "\t.IF EAX == 1\n";

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			case ELSE:
			{
				currentContext.push(eContext::ELSE);
				string code = "\t.ELSE\n";

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			case WHILE:
			{
				currentContext.push(eContext::WHILE);
				string code = "";
				code += "\t.WHILE EAX == EAX\n";
				i++;
				translateExpr(LEXER, code, i, cmpCounter, ovCounter, LEXER.lexTable.table[i].pIdI.lock()->dataType);
				code += "\tpop EAX\n";
				code += "\t.IF EAX == 0\n";
				code += "\t.BREAK\n";
				code += "\t.ENDIF\n";

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			case ID:
			case LITERAL:
			{
				if (LEXER.lexTable.table[i + 1].type == ASSIGN) {
					break;
				}
				string code = "";
				translateExpr(LEXER, code, i, cmpCounter, ovCounter, LEXER.lexTable.table[i].pIdI.lock()->dataType);
				code += "\tpop EAX\n";

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
				break;
			}
			case ASSIGN:
			{
				uint16_t iIdTo = i - 1;
				string code = "";
				i++;
				if (LEXER.lexTable.table[iIdTo].pIdI.lock()->dataType == IdDataType::STR) {
					code += "\tmov _ram2022_buf[0], 0\n";
				}
				translateExpr(LEXER, code, i, cmpCounter, ovCounter, LEXER.lexTable.table[i - 2].pIdI.lock()->dataType);
				if (LEXER.lexTable.table[iIdTo].pIdI.lock()->dataType == IdDataType::STR)
				{
					code += "\tpop EAX\n";
					code += "\n\tINVOKE _ram2022_strcpy, ";
					if (LEXER.lexTable.table[iIdTo].pIdI.lock()->type != IdType::PARAM) {
						code += "OFFSET ";
					}
					code += LEXER.lexTable.table[iIdTo].pIdI.lock()->getFullId();
					code += ", EAX\n";
					code += "\tmov _ram2022_buf[0], 0\n";
				}
				else
				{
					code += "\tpop EAX\n";
					code += "\tmov ";
					code += LEXER.lexTable.table[iIdTo].pIdI.lock()->getFullId();
					code += ", EAX\n";
				}

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			case RETURN:
			{
				string code = "";
				i++;
				code += "\tmov EAX, ";
				if (LEXER.lexTable.table[i].pIdI.lock()->dataType == IdDataType::STR &&
					LEXER.lexTable.table[i].pIdI.lock()->type != IdType::PARAM)
				{
					code += "OFFSET ";
				}
				code += LEXER.lexTable.table[i].pIdI.lock()->getFullId();
				code += "\n";
				funcBodiesToAdd.top() += code;
			} break;
			case RIGHT_BRACE:
			{
				if (currentContext.top() == eContext::MAIN)
				{
					strCode +=
R"(	
	INVOKE ExitProcess, 0
main ENDP
)";
					currentContext.pop();
				}
				else if (currentContext.top() == eContext::FUNC)
				{
					funcBodiesToAdd.top() += "\tret\n";
					funcBodiesToAdd.top() += funcBodiesToAdd.top().substr(0, funcBodiesToAdd.top().find_first_of(" \t"));
					funcBodiesToAdd.top() += " ENDP\n\n";
					funcs.push_back(funcBodiesToAdd.top());
					funcBodiesToAdd.pop();
					currentContext.pop();
				}
				else if (currentContext.top() == eContext::IF)
				{
					if (i + 1 < LEXER.lexTable.size &&
						LEXER.lexTable.table[i + 1].type != ELSE &&
						LEXER.lexTable.table[i + 1].type != ELIF)
					{
						if (funcBodiesToAdd.empty()) {
							strCode += "\t.ENDIF\n";
						}
						else {
							funcBodiesToAdd.top() += "\t.ENDIF\n";
						}
					}
					currentContext.pop();
				}
				else if (currentContext.top() == eContext::ELIF &&
						 i + 1 < LEXER.lexTable.size &&
						 LEXER.lexTable.table[i + 1].type != ELSE &&
						 LEXER.lexTable.table[i + 1].type != ELIF)
				{
					string code = "\t.ENDIF\n";
					do
					{
						code += "\t.ENDIF\n";
						currentContext.pop();
					} while (currentContext.top() == eContext::ELIF);

					if (funcBodiesToAdd.empty()) {
						strCode += code;
					}
					else {
						funcBodiesToAdd.top() += code;
					}
				}
				else if (currentContext.top() == eContext::ELSE)
				{
					string code = "";
					do
					{
						code += "\t.ENDIF\n";
						currentContext.pop();
					} while (currentContext.top() == eContext::ELIF);

					if (funcBodiesToAdd.empty()) {
						strCode += code;
					}
					else {
						funcBodiesToAdd.top() += code;
					}
				}
				else if (currentContext.top() == eContext::WHILE)
				{
					if (funcBodiesToAdd.empty()) {
						strCode += "\t.ENDW\n";
					}
					else {
						funcBodiesToAdd.top() += "\t.ENDW\n";
					}
					currentContext.pop();
				}
			} break;
			case READ:
			{
				string code = "";
				switch (LEXER.lexTable.table[i + 1].pIdI.lock()->dataType)
				{
				case IdDataType::INT:
					code += "\tINVOKE _ram2022_readStr, OFFSET _ram2022_buf\n";
					code += "\tINVOKE atoi, OFFSET _ram2022_buf\n";
					code += "\tmov ";
					code += LEXER.lexTable.table[i + 1].pIdI.lock()->getFullId();
					code += ", EAX\n";
					break;
				case IdDataType::STR:
					code += "\tINVOKE _ram2022_readStr, OFFSET _ram2022_buf\n";
					code += "\n\tINVOKE _ram2022_strcpy, ";
					if (LEXER.lexTable.table[i + 1].pIdI.lock()->type != IdType::PARAM) {
						code += "OFFSET ";
					}
					code += LEXER.lexTable.table[i + 1].pIdI.lock()->getFullId();
					code += ", OFFSET _ram2022_buf";
					break;
				default:
					break;
				}
				i++;

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			case WRITE:
			{
				string code = "";
				i++;
				switch (LEXER.lexTable.table[i].pIdI.lock()->dataType)
				{
				case IdDataType::STR:
					code += "\tINVOKE _ram2022_writeStr, ";
					if (LEXER.lexTable.table[i].pIdI.lock()->type != IdType::PARAM) {
						code += "OFFSET ";
					}
					code += LEXER.lexTable.table[i].pIdI.lock()->getFullId();
					code += "\n";
					break;
				case IdDataType::INT:
					code += "\tINVOKE _ram2022_writeInt, ";
					code += LEXER.lexTable.table[i].pIdI.lock()->getFullId();
					code += "\n";
					break;
				default:
					break;
				}

				if (funcBodiesToAdd.empty()) {
					strCode += code;
				}
				else {
					funcBodiesToAdd.top() += code;
				}
			} break;
			default:
				break;
			}
		}

		out <<
			strHeader <<
			strLibs <<
			strExterns <<
			strProtos <<
			strStack <<
			strConst <<
			strData <<
			strCode;

		for (const string& STR_FUNC : funcs) {
			out << STR_FUNC;
		}

		out << strEnd;
	}
}