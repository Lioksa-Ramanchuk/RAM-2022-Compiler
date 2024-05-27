#include "stdafx.h"
#include "LexicalAnalysis.hpp"
#include "Log.hpp"
using namespace RAM_2022_Fst;
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_Log;
using namespace RAM_2022_IT;
using namespace RAM_2022_LT;
using namespace std;

using uchar = char8_t;

namespace RAM_2022_LexicalAnalysis
{
    bool Lexer::isFuncDeclaration(uint16_t posIDInLT) const
    {
        using enum RAM_2022_LT::LexemeType;
        return lexTable.size >= posIDInLT
               &&
               (posIDInLT >= 2 &&
                lexTable.table[posIDInLT - 2].type == TYPE &&
                lexTable.table[posIDInLT - 1].type == FUNC
                ||
                posIDInLT >= 1 &&
                lexTable.table[posIDInLT - 1].type == PROC);
    }

    bool Lexer::isExternFuncDeclaration(uint16_t posIDInLT) const
    {
        using enum RAM_2022_LT::LexemeType;
        return lexTable.size >= posIDInLT
               &&
               (posIDInLT >= 3 &&
                lexTable.table[posIDInLT - 3].type == LINK &&
                lexTable.table[posIDInLT - 2].type == TYPE &&
                lexTable.table[posIDInLT - 1].type == FUNC
                ||
                posIDInLT >= 2 &&
                lexTable.table[posIDInLT - 2].type == LINK &&
                lexTable.table[posIDInLT - 1].type == PROC);
    }

    bool Lexer::isParamDeclaration(uint16_t posIDInLT) const
    {
        return lexTable.size >= posIDInLT
               &&
               ((posIDInLT >= 5 &&
                 lexTable.table[posIDInLT - 5].type == LexemeType::TYPE &&
                 lexTable.table[posIDInLT - 4].type == LexemeType::FUNC
                 ||
                 posIDInLT >= 4 &&
                 lexTable.table[posIDInLT - 4].type == LexemeType::PROC) &&
                lexTable.table[posIDInLT - 3].type == LexemeType::ID &&
                lexTable.table[posIDInLT - 3].pIdI.lock() != weak_ptr<IdInfo>().lock() &&
                lexTable.table[posIDInLT - 3].pIdI.lock()->type == IdType::FUNC &&
                lexTable.table[posIDInLT - 2].type == LexemeType::LEFT_PARENTHESIS &&
                lexTable.table[posIDInLT - 1].type == LexemeType::TYPE
                ||
                lexTable.size >= 3 &&
                lexTable.table[posIDInLT - 3].type == LexemeType::ID &&
                lexTable.table[posIDInLT - 3].pIdI.lock() != weak_ptr<IdInfo>().lock() &&
                lexTable.table[posIDInLT - 3].pIdI.lock()->type == IdType::PARAM &&
                lexTable.table[posIDInLT - 2].type == LexemeType::COMMA &&
                lexTable.table[posIDInLT - 1].type == LexemeType::TYPE);
    }

    void Lexer::fillLexTableAndIdTable(ClMsgArrs& clMsgArrs)
    {
        using enum RAM_2022_LT::LexemeType;

        weak_ptr<IdScope> pCurrentScope = idTable.scope;

        Fst lexemTypeFst;
        LexemeInfo lexeme;

        IdDataType currentDataType = IdDataType::NONE;
        bool repeat = false;
        bool mainFound = false;
        bool closeScopeAfterParamsList = false;
        bool createNewScopeIfLB = true;
        uint32_t anonymousScopeCounter = 0;

        for (uint16_t iWord = 0, nWords = (uint16_t)wordTable.size(); iWord < nWords || repeat; iWord++)
        {
            if (repeat)
            {
                iWord--;
                repeat = false;
            }

            lexeme.type = LexemeType::NONE;
            lexeme.pos.line = wordTable[iWord].pos.line;
            lexeme.pos.posInLine = wordTable[iWord].pos.posInLine;
            lexeme.pWORD = &wordTable[iWord];

            for (const auto& [LEXEM_TYPE, LEXEM_TYPE_FST] : LEXEM_TYPES_FSTS)
            {
                lexemTypeFst = LEXEM_TYPE_FST;
                lexemTypeFst.set(wordTable[iWord].word.c_str());

                if (lexemTypeFst.execute())
                {
                    lexeme.type = LEXEM_TYPE;
                    break;
                }
            }

            switch (lexeme.type)
            {
            case ASSIGN:
            case ARITHMETIC_BIN_OP:
            case ARITHMETIC_UN_OP:
            case CMP_BIN_OP:
            case COMMA:
            case WRITE:
            case READ:
            case FUNC:
            case LINK:
            case IF:
            case ELSE:
            case ELIF:
            case WHILE:
            case RETURN:
            case SEMICOLON:
            case LEFT_PARENTHESIS:
                break;
            case ID:
            {
                bool thisIsExternFuncDeclaration = isExternFuncDeclaration(lexTable.size);
                
                if (!thisIsExternFuncDeclaration && 
                    wordTable[iWord].word.length() > ID_MAX_SIZE &&
                    !pCurrentScope.lock()->isExtern(wordTable[iWord].word, idTable.externs))
                {
                    clMsgArrs.push(Warning(ClMsgType::ID_MAX_LENGTH_EXCEEDED, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine));
                    wordTable[iWord].word.resize(ID_MAX_SIZE);
                    repeat = true;
                    continue;
                }

                bool newIdIsExpected = false;
                bool createNewScope = false;
                IdType currentIdType = IdType::NONE;

                if (isFuncDeclaration(lexTable.size))
                {
                    if (thisIsExternFuncDeclaration) {
                        closeScopeAfterParamsList = true;
                    }
                    else {
                        createNewScopeIfLB = false;
                    }

                    newIdIsExpected = true;
                    currentIdType = IdType::FUNC;
                    createNewScope = true;
                }
                else if (isParamDeclaration(lexTable.size))
                {
                    newIdIsExpected = true;
                    currentIdType = IdType::PARAM;
                }
                else if (lexTable.size >= 1 &&
                         lexTable.table[lexTable.size - 1].type == LexemeType::TYPE)
                {
                    newIdIsExpected = true;
                    currentIdType = IdType::VAR;
                }

                if (newIdIsExpected)
                {
                    if (pCurrentScope.lock()->contains(wordTable[iWord].word))
                    {
                        if (!clMsgArrs.push(Error(ClMsgType::ID_REDECLARATION, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine))) {
                            return;
                        }
                    }
                    else if (thisIsExternFuncDeclaration)
                    {
                        idTable.addExtern(
                            IdInfo{
                                lexTable.size,
                                wordTable[iWord].word,
                                currentIdType,
                                currentDataType,
                            },
                            pCurrentScope);
                    }
                    else
                    {
                        idTable.addIdI(
                            IdInfo{
                                lexTable.size,
                                wordTable[iWord].word,
                                currentIdType,
                                currentDataType,
                            },
                            pCurrentScope);
                    }
                }

                lexeme.pIdI = pCurrentScope.lock()->find(wordTable[iWord].word);
                if (lexeme.pIdI.lock() == weak_ptr<IdInfo>().lock() &&
                    !clMsgArrs.push(Error(ClMsgType::UNKNOWN_ID, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine)))
                {
                    return;
                }

                if (createNewScope)
                {
                    pCurrentScope.lock()->addInnerScope();
                    pCurrentScope = pCurrentScope.lock()->innerScopes.back();
                }
            } break;
            case LEFT_BRACE:
            {
                if (createNewScopeIfLB)
                {
                    anonymousScopeCounter++;
                    pCurrentScope.lock()->addInnerScope();
                    pCurrentScope = pCurrentScope.lock()->innerScopes.back();
                }
                else {
                    createNewScopeIfLB = true;
                }
            } break;
            case LITERAL:
            {
                if (wordTable[iWord].word[0] == '\"')
                {
                    string val = wordTable[iWord].word.substr(1, wordTable[iWord].word.length() - 2);
                    if (val.length() > IT_MAX_STR_LENGTH)
                    {
                        if (!clMsgArrs.push(Warning(ClMsgType::STR_MAX_LENGTH_EXCEEDED, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine))) {
                            return;
                        }
                        val = val.substr(0, IT_MAX_STR_LENGTH);
                    }
                    lexeme.pIdI = idTable.findOrAddLiteral(val, iWord);
                }
                else
                {
                    int32_t val = IT_INT_DEFAULT;
                    try
                    {
                        if (wordTable[iWord].word.back() == 'o') {
                            val = stoi(wordTable[iWord].word.substr(0, wordTable[iWord].word.length() - 1), nullptr, 8);
                        }
                        else {
                            val = stoi(wordTable[iWord].word);
                        }
                    }
                    catch (const out_of_range&) {
                        if (!clMsgArrs.push(Warning(ClMsgType::INT_IS_OUT_OF_RANGE, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine))) {
                            return;
                        }
                    }
                    lexeme.pIdI = idTable.findOrAddLiteral(val, iWord);
                }
            } break;
            case MAIN:
            {
                if (mainFound &&
                    !clMsgArrs.push(Error(ClMsgType::MAIN_REDEFINITION, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine)))
                {
                    return;
                }
                mainFound = true;

                if ((pCurrentScope.lock() != idTable.scope) &&
                    !clMsgArrs.push(Error(ClMsgType::MAIN_IS_NOT_GLOBAL, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine)))
                {
                    return;
                }

                pCurrentScope.lock()->addInnerScope();
                pCurrentScope = pCurrentScope.lock()->innerScopes.back();
                createNewScopeIfLB = false;
            } break;
            case RIGHT_BRACE:
            {
                pCurrentScope = pCurrentScope.lock()->pOuterScope;
            } break;
            case RIGHT_PARENTHESIS:
            {
                if (closeScopeAfterParamsList)
                {
                    pCurrentScope = pCurrentScope.lock()->pOuterScope;
                    closeScopeAfterParamsList = false;
                }
            } break;
            case PROC:
            {
                currentDataType = IdDataType::VOID;
            } break;
            case TYPE:
            {
                if (wordTable[iWord].word == "str") {
                    currentDataType = IdDataType::STR;
                }
                else if (wordTable[iWord].word == "int") {
                    currentDataType = IdDataType::INT;
                }
            } break;
            default:
            {
                if (!clMsgArrs.push(Error(ClMsgType::UNKNOWN_LEXEME, wordTable[iWord].pos.line, wordTable[iWord].pos.posInLine))) {
                    return;
                }
            } break;
            }

            lexTable.add(lexeme);
        }

        if (!mainFound &&
            !clMsgArrs.push(Error(ClMsgType::MAIN_NOT_FOUND)))
        {
            return;
        }
    }

    void Lexer::printTables(ostream& stream) const
    {
        if (wordTable.empty()) {
            return;
        }

        stream << "=============================================\n";
        stream << " ÒÀÁËÈÖÀ ÏÐÅÄËÅÊÑÅÌ, ËÅÊÑÅÌ, ÈÄÅÍÒÈÔÈÊÀÒÎÐÎÂ \n";
        stream << "=============================================\n";
        stream << format(" {: ^6} | {: ^10} | {: ^10} | {: ^7} | {: ^10} | {: ^6} | {: ^10} | {: ^10} | {: ^10}\n",
                           "¹", "Ïîçèöèÿ", "Ïðåäëåêñåìà", "Ëåêñåìà", "ID", "¹ â ÒË", "Òèï ID", "Òèï äàííûõ", "Çíà÷åíèå");
        stream << format("{0:->8}+{0:->12}+{0:->12}+{0:->9}+{0:->12}+{0:->8}+{0:->12}+{0:->12}+{0:->12}\n", '-');
        
        for (uint16_t i = 0; i < lexTable.size; i++)
        {
            stream << format(" {: >6} |", i);
            stream << format(" {: >6}:{: >3} |", lexTable.table[i].pos.line, lexTable.table[i].pos.posInLine);
            stream << format(" {: <10} |", lexTable.table[i].pWORD ? lexTable.table[i].pWORD->word : "");
            stream << format(" {: ^7} |", (char)lexTable.table[i].type);
            
            if (lexTable.table[i].type == LexemeType::ID ||
                lexTable.table[i].type == LexemeType::LITERAL)
            {
                stream << format(" {: <10} |", lexTable.table[i].pIdI.lock()->getFullId());
                stream << format(" {: >6} |", lexTable.table[i].pIdI.lock()->iFirstLexLT);
                switch (lexTable.table[i].pIdI.lock()->type)
                {
                    using enum IdType;
                case VAR:
                    stream << format(" {: ^10} |", "ïåðåìåííàÿ");
                    break;
                case FUNC:
                    if (lexTable.table[i].pIdI.lock()->dataType == IdDataType::VOID) {
                        stream << format(" {: ^10} |", "ïðîöåäóðà");
                    }
                    else {
                        stream << format(" {: ^10} |", "ôóíêöèÿ");
                    }
                    break;
                case PARAM:
                    stream << format(" {: ^10} |", "ïàðàìåòð");
                    break;
                case LITERAL:
                    stream << format(" {: ^10} |", "ëèòåðàë");
                    break;
                default:
                    stream << format(" {: ^10} |", "?");
                    break;
                }

                switch (lexTable.table[i].pIdI.lock()->dataType)
                {
                    using enum IdDataType;
                case INT:
                    stream << format(" {: ^10} |", "int");                    
                    break;
                case STR:
                    stream << format(" {: ^10} |", "string");
                    break;
                case VOID:
                    stream << format(" {: ^10} |", "-");
                    break;
                default:
                    stream << format(" {: ^10} |", "?");
                    break;
                }

                switch (lexTable.table[i].pIdI.lock()->type)
                {
                    using enum IdType;
                case VAR:
                case PARAM:
                case LITERAL:
                {
                    switch (lexTable.table[i].pIdI.lock()->dataType)
                    {
                        using enum IdDataType;
                    case INT:
                        stream << " " << get<int32_t>(lexTable.table[i].pIdI.lock()->value);
                        break;
                    case STR:
                        stream << "\"" << get<string>(lexTable.table[i].pIdI.lock()->value) << "\"";
                        break;
                    default:
                        stream << " ?";
                        break;
                    }
                }
                default:
                    break;
                }
            }
            else {
                stream << format("{0: >12}|{0: >8}|{0: >12}|{0: >12}|", ' ');
            }

            stream << endl;
        }
    }
}