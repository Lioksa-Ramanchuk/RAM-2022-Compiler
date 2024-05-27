#include "stdafx.h"
#include "ClMsg.hpp"
#include "LexicalAnalysis.hpp"
#include "SemanticAnalysis.hpp"
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_IT;
using namespace RAM_2022_LT;
using namespace RAM_2022_LexicalAnalysis;
using namespace std;

namespace RAM_2022_SemanticAnalysis
{
	void checkSemantics(Lexer& lexer, ClMsgArrs& clMsgArrs)
	{
		auto& [WT, LT, IT, FT] = lexer;
		// проверка на невызов переменных (параметров), вызов функций (процедур),
		// типов в выражени€х при присваивании,
		// совпадени€ типов в услови€х,
		// деление на 0,
		// использование унарных арифметических операторов только с числами
		// типа литерала при подключении внешней библиотеки,
		for (uint16_t i = 0; i < LT.size; i++)
		{
			switch (LT.table[i].type)
			{
				using enum LexemeType;
			case ID:
			{
				switch (LT.table[i].pIdI.lock()->type)
				{
				case IdType::VAR:
				case IdType::PARAM:
				{
					if (LT.table[i + 1].type == LEFT_PARENTHESIS &&
						!clMsgArrs.push(Error(ClMsgType::VAR_CALL_ERROR, LT.table[i + 1].pos.line, LT.table[i + 1].pos.posInLine))) {
						return;
					}
				} break;
				case IdType::FUNC:
				{
					if (LT.table[i].pIdI.lock()->iFirstLexLT != i &&
						LT.table[i + 1].type != LEFT_PARENTHESIS &&
						!clMsgArrs.push(Error(ClMsgType::PARENTHESES_REQUIRED, LT.table[i + 1].pos.line, LT.table[i + 1].pos.posInLine))) {
						return;
					}
				} break;
				default:
					break;
				}
			} break;
			case ASSIGN:
			{
				IdDataType lvalueType = LT.table[i - 1].pIdI.lock()->dataType;
				bool firstOperand = true;
				for (uint16_t j = i + 1; LT.table[j].type != SEMICOLON; j++)
				{
					if (LT.table[j].type == ARITHMETIC_BIN_OP &&
						lvalueType == IdDataType::STR &&
						LT.table[j].pWORD->word != "+" &&
						!clMsgArrs.push(Error(ClMsgType::OP_IS_FORBIDDEN_BY_TYPE, LT.table[j].pos.line, LT.table[j].pos.posInLine))){
						return;
					}

					if ((LT.table[j].type == ID || LT.table[j].type == LITERAL) &&
						LT.table[j].pIdI.lock()->dataType != lvalueType &&
						!clMsgArrs.push(Error(ClMsgType::WRONG_EXPR_TYPE, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
						return;
					}

					if (lvalueType == IdDataType::STR &&
						LT.table[j].type == LEFT_PARENTHESIS &&
						!(LT.table[j - 1].type == ID || LT.table[j - 1].type == LITERAL) &&
						!clMsgArrs.push(Error(ClMsgType::OP_IS_FORBIDDEN_BY_TYPE, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
						return;
					}

					if (LT.table[j].type == ID &&
						LT.table[j + 1].type == LEFT_PARENTHESIS)
					{
						if (LT.table[j].pIdI.lock()->dataType == IdDataType::STR &&
							!firstOperand &&
							!clMsgArrs.push(Error(ClMsgType::STR_FUNC_IN_EXPR_IS_NOT_1ST, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
							return;
						}
						uint16_t parenthesesCounter = 1;
						for (j += 2; parenthesesCounter; j++)
						{
							if (LT.table[j].type == LEFT_PARENTHESIS) {
								parenthesesCounter++;
							}
							else if (LT.table[j].type == RIGHT_PARENTHESIS) {
								parenthesesCounter--;
							}
						}
						j--;
					}

					if (LT.table[j].type == ID ||
						LT.table[j].type == LITERAL)
					{
						firstOperand = false;
					}
				}
			} break;
			case CMP_BIN_OP:
			{
				IdDataType leftType = LT.table[i - 1].pIdI.lock()->dataType;
				IdDataType rightType = LT.table[i + 1].pIdI.lock()->dataType;

				if (leftType != rightType &&
					!clMsgArrs.push(Error(ClMsgType::CONDITION_TYPE_MISMATCH, LT.table[i + 1].pos.line, LT.table[i + 1].pos.posInLine))) {
					return;
				}
			} break;
			case ARITHMETIC_BIN_OP:
			{
				if ((LT.table[i].pWORD->word == "/" || LT.table[i].pWORD->word == "%") &&
					LT.table[i + 1].type == LITERAL &&
					LT.table[i + 1].pIdI.lock()->dataType == IdDataType::INT &&
					get<int32_t>(LT.table[i + 1].pIdI.lock()->value) == 0 &&
					!clMsgArrs.push(Error(ClMsgType::NULL_DIVISION, LT.table[i + 1].pos.line, LT.table[i + 1].pos.posInLine))) {
					return;
				}
			} break;
			case ARITHMETIC_UN_OP:
			{
				uint16_t nextIdPos = i + 1;
				while (LT.table[nextIdPos].type != ID && LT.table[nextIdPos].type != LITERAL) {
					nextIdPos++;
				}

				if (LT.table[nextIdPos].pIdI.lock()->dataType != IdDataType::INT &&
					!clMsgArrs.push(Error(ClMsgType::OP_IS_FORBIDDEN_BY_TYPE, LT.table[i].pos.line, LT.table[i].pos.posInLine))) {
					return;
				}
			} break;
			case LINK:
			{
				if (LT.table[i + 1].type == LITERAL &&
					LT.table[i + 1].pIdI.lock()->dataType != IdDataType::STR &&
					!clMsgArrs.push(Error(ClMsgType::WRONG_LIB_LINK, LT.table[i + 1].pos.line, LT.table[i + 1].pos.posInLine))) {
					return;
				}
			} break;
			default:
				break;
			}
		}

		// проверка на соответствие кол-ва передаваемых параметров, их типов, типа возвращаемого значени€
		map<const IdInfo*, bool> funcsCalled;
		for (uint16_t i = 0; i < LT.size; i++)
		{
			using enum LexemeType;
			if (LT.table[i].type == ID &&
				LT.table[i].pIdI.lock()->type == IdType::FUNC)
			{
				if (lexer.isFuncDeclaration(i))
				{
					FT[LT.table[i].pIdI.lock()->getFullId()] = vector<IdDataType>();
					funcsCalled[LT.table[i].pIdI.lock().get()] = false;
					uint16_t j;
					for (j = i + 2; LT.table[j].type != RIGHT_PARENTHESIS; j += 2)
					{
						if (LT.table[j].type == COMMA) {
							j++;
						}
						if (FT[LT.table[i].pIdI.lock()->getFullId()].size() >= MAX_FUNC_PARAMS_NUMBER)
						{
							if (!clMsgArrs.push(Error(ClMsgType::MAX_FUNC_PARAMS_NUMBER_EXCEEDED, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
								return;
							}
						}
						else {
							FT[LT.table[i].pIdI.lock()->getFullId()].push_back(LT.table[j + 1].pIdI.lock()->dataType);
						}
					}

					// процедура или внешн€€ ф-€/пр-ра не имеют return
					if (LT.table[i].pIdI.lock()->dataType == IdDataType::VOID ||
						LT.table[i].pIdI.lock()->pScope.lock() == weak_ptr<IdScope>().lock())
					{
						continue;
					}

					uint16_t bracesCounter = 0;
					for (j += 2; LT.table[j].type != RETURN || bracesCounter; j++)
					{
						if (LT.table[j].type == LEFT_BRACE) {
							bracesCounter++;
						}
						else if (LT.table[j].type == RIGHT_BRACE) {
							bracesCounter--;
						}
					}
					if (LT.table[j + 1].pIdI.lock()->dataType != LT.table[i].pIdI.lock()->dataType &&
						!clMsgArrs.push(Error(ClMsgType::RETURN_TYPE_MISMATCH, LT.table[j + 1].pos.line, LT.table[j + 1].pos.posInLine))) {
						return;
					}
				}
				else
				{
					funcsCalled[LT.table[i].pIdI.lock().get()] = true;

					if (LT.table[i + 1].type != LEFT_PARENTHESIS) {
						continue;	// обработано ранее
					}

					vector<IdDataType>* pParams = &FT[LT.table[i].pIdI.lock()->getFullId()];
					uint16_t nFormalParams = (uint16_t)pParams->size();
					uint16_t nActualParams = 0;
					uint16_t j;
					for (j = i + 2; LT.table[j].type != RIGHT_PARENTHESIS; j++)
					{
						if (LT.table[j].type == COMMA) {
							continue;
						}

						nActualParams++;
						if (nActualParams > MAX_FUNC_PARAMS_NUMBER) {
							break;
						}

						if (nActualParams == nFormalParams + 1 &&
							!clMsgArrs.push(Error(ClMsgType::PARAMS_NUMBER_MISMATCH, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
							return;
						}

						if (nActualParams <= nFormalParams &&
							LT.table[j].pIdI.lock()->dataType != pParams->at(nActualParams - 1) &&
							!clMsgArrs.push(Error(ClMsgType::PARAM_TYPE_MISMATCH, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
							return;
						}
					}
					if (nActualParams < nFormalParams &&
						!clMsgArrs.push(Error(ClMsgType::PARAMS_NUMBER_MISMATCH, LT.table[j].pos.line, LT.table[j].pos.posInLine))) {
						return;
					}
				}
			}
		}
		
		// проверка на использование объ€вленных функций (процедур)
		for (const auto& [P_FUNC_ID, IS_CALLED] : funcsCalled)
		{
			if (!IS_CALLED &&
				!clMsgArrs.push(Warning(ClMsgType::UNUSED_FUNC,
					LT.table[P_FUNC_ID->iFirstLexLT].pos.line,
					LT.table[P_FUNC_ID->iFirstLexLT].pos.posInLine))) {
				return;
			}
		}
	}
}
