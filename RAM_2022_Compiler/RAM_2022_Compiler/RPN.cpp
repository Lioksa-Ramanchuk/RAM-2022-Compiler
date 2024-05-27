#include "stdafx.h"
#include "RPN.hpp"
#include "LexicalAnalysis.hpp"
#include "IT.hpp"
#include "LT.hpp"
using namespace RAM_2022_IT;
using namespace RAM_2022_LexicalAnalysis;
using namespace RAM_2022_LT;
using namespace std;

namespace RAM_2022_ReversedPolishNotation
{
	namespace
	{
		uint16_t getPriority(const LexemeInfo& LI)
		{
			switch (LI.type)
			{
				using enum LexemeType;
			case LEFT_PARENTHESIS:
			case RIGHT_PARENTHESIS:
				return 0;
			case CMP_BIN_OP:
				return 1;
			case ARITHMETIC_BIN_OP:
			{
				if (LI.pWORD->word == "+" || LI.pWORD->word == "-") {
					return 2;
				}
				else /*if (word == "*" || word == "/" || word == "%")*/ {
					return 3;
				}
			}
			case ARITHMETIC_UN_OP:
				return 4;
			default:
				return 5;
			}
		}

		uint16_t exprToRPN(uint16_t exprPosInLT, Lexer& lexer)
		{
			queue<LexemeInfo> rpnExpr;
			stack<LexemeInfo> operators;
			stack<LexemeInfo> funcTemp;

			uint16_t nLexemes = 0;
			bool exprEnded = false;
			bool funcCall = false;

			for (uint16_t i = exprPosInLT; !exprEnded; i++)
			{
				if (funcCall)
				{
					switch (lexer.lexTable.table[i].type)
					{
						using enum LexemeType;
					case RIGHT_PARENTHESIS:
					{
						while (!funcTemp.empty())
						{
							rpnExpr.push(funcTemp.top());
							funcTemp.pop();
						}
						funcCall = false;
					} break;
					case ID:
					case LITERAL:
					{
						funcTemp.push(lexer.lexTable.table[i]);
					} break;
					default:
						break;
					}
				}
				else
				{
					switch (lexer.lexTable.table[i].type)
					{
						using enum LexemeType;
					case SEMICOLON:
					case LEFT_BRACE:
						exprEnded = true;
						continue;
					case ID:
					{
						if (lexer.lexTable.table[i].pIdI.lock()->type == IdType::FUNC)
						{
							operators.push(lexer.lexTable.table[i]);
							funcCall = true;
						}
						else {
							rpnExpr.push(lexer.lexTable.table[i]);
						}
					} break;
					case LITERAL:
					{
						rpnExpr.push(lexer.lexTable.table[i]);
					} break;
					case LEFT_PARENTHESIS:
					{
						operators.push(lexer.lexTable.table[i]);
					} break;
					case RIGHT_PARENTHESIS:
					{
						while (!operators.empty() && operators.top().type != LEFT_PARENTHESIS)
						{
							rpnExpr.push(operators.top());
							operators.pop();
						}

						if (operators.empty())
						{
							exprEnded = true;
							break;
						}
						operators.pop();
					} break;
					case ARITHMETIC_UN_OP:
					case ARITHMETIC_BIN_OP:
					case CMP_BIN_OP:
					{
						uint16_t thisPriority = getPriority(lexer.lexTable.table[i]);

						while (!operators.empty() && thisPriority <= getPriority(operators.top()))
						{
							rpnExpr.push(operators.top());
							operators.pop();
						}
						operators.push(lexer.lexTable.table[i]);
					} break;
					default:
						break;
					}
				}

				nLexemes++;
			}

			while (!operators.empty())
			{
				rpnExpr.push(operators.top());
				operators.pop();
			}

			for (uint16_t i = 0; i < nLexemes; i++)
			{
				if (rpnExpr.empty())
				{
					lexer.lexTable.table[exprPosInLT + i] = LexemeInfo();
					continue;
				}

				switch (rpnExpr.front().type)
				{
					using enum LexemeType;
				case ID:
				case LITERAL:
				{
					if (exprPosInLT <= rpnExpr.front().pIdI.lock()->iFirstLexLT &&
						rpnExpr.front().pIdI.lock()->iFirstLexLT < exprPosInLT + nLexemes)
					{
						rpnExpr.front().pIdI.lock()->iFirstLexLT = exprPosInLT + i;
					}
					lexer.lexTable.table[exprPosInLT + i] = rpnExpr.front();
					rpnExpr.pop();
				} break;
				default:
				{
					lexer.lexTable.table[exprPosInLT + i] = rpnExpr.front();
					rpnExpr.pop();
				} break;
				}
			}

			return exprPosInLT + nLexemes;
		}
	}

	void toRPN(Lexer& lexer)
	{
		for (uint16_t i = 0; i < lexer.lexTable.size; )
		{
			if (lexer.lexTable.table[i].type == LexemeType::ASSIGN ||
				lexer.lexTable.table[i].type == LexemeType::IF ||
				lexer.lexTable.table[i].type == LexemeType::ELIF ||
				lexer.lexTable.table[i].type == LexemeType::WHILE) {
				i = exprToRPN(i + 1, lexer);
			}
			else if (lexer.lexTable.table[i].type == LexemeType::ID &&
						lexer.lexTable.table[i].pIdI.lock()->type == IdType::FUNC &&
						!lexer.isFuncDeclaration(i))
			{
				i = exprToRPN(i, lexer);
			}
			else {
				i++;
			}
		}
	}
}