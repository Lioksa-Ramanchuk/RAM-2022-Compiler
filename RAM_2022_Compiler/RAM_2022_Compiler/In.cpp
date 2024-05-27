#include "stdafx.h"
#include "In.hpp"
#include "Log.hpp"
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_In;
using namespace RAM_2022_LexicalAnalysis;
using namespace RAM_2022_Log;
using namespace RAM_2022_LT;
using namespace std;

using uchar = char8_t;
using uint = uint32_t;

namespace RAM_2022_In
{
	void In::readFile(const std::wstring& IN_PATH, vector<WordInfo>& wordTable, ClMsgArrs& clMsgArrs)
	{
        ifstream ifile(IN_PATH);
        if (ifile.fail()) {
            throw Error(ClMsgType::FSTREAM_OPENING_FAILURE, "Ошибка при открытии входного файла");
        }

        text.clear();
        try {
            text.reserve(MAX_CODE_LENGTH);
        }
        catch (const bad_alloc&) {
            throw Error(ClMsgType::MEMORY_ALLOCATION_FAILURE, "Ошибка выделения памяти для исходного кода");
        }

        wordTable.clear();
        try {
            wordTable.reserve(LT_MAX_SIZE);
        }
        catch (const bad_alloc&) {
            throw Error(ClMsgType::MEMORY_ALLOCATION_FAILURE, "Ошибка выделения памяти для таблицы предлексем");
        }

        length = 0;
        nLines = 1;
        nIgnored = 0;
        uint posInLine = 1;
        string word = "";
        uint wordPosInLine = 1;
        bool isStringLiteralContext = false;
        bool isSpecialCharContext = false;
        bool isCommentContext = false;

        auto addCharToInText = [&](char c)
        {
            if (length >= MAX_CODE_LENGTH)
            {
                clMsgArrs.push(Warning(ClMsgType::CODE_MAX_LENGTH_EXCEEDED, nLines, posInLine));
                return;
            }

            text += c;
            length++;
        };
        auto addWordToWT = [&]
        {
            if (wordTable.size() >= LT_MAX_SIZE) {
                throw Error(ClMsgType::LT_MAX_SIZE_EXCEEDED, nLines, wordPosInLine);
            }

            wordTable.emplace_back(word, nLines, wordPosInLine);
            word.clear();
        };

        for (uchar c = (uchar)ifile.get();
             !ifile.eof();
             c = (uchar)ifile.get(), posInLine++)
        {
            switch (CODE_TABLE[c])
            {
            default:
                c = (uchar)CODE_TABLE[c];
                [[fallthrough]];
            case (uint32_t)CharacterType::C:
            case (uint32_t)CharacterType::N:
            case (uint32_t)CharacterType::P:
            case (uint32_t)CharacterType::Q:
            case (uint32_t)CharacterType::S:
            case (uint32_t)CharacterType::T:
                addCharToInText(c);
                break;
            case (uint32_t)CharacterType::F:
                clMsgArrs.push(Error(ClMsgType::FORBIDDEN_CHARACTER_FOUND, nLines, posInLine));
                nIgnored++;
                continue;
            case (uint32_t)CharacterType::I:
                clMsgArrs.push(Warning(ClMsgType::IGNORED_CHARACTER_FOUND, nLines, posInLine));
                nIgnored++;
                continue;
            }

            if (isStringLiteralContext)
            {
                if (isSpecialCharContext)
                {
                    switch (c)
                    {
                    case '\\':
                        word += '\\';
                        break;
                    case 'n':
                        word += '\n';
                        break;
                    case 't':
                        word += '\t';
                        break;
                    case '\"':
                        word += '\"';
                        break;
                    default:
                        clMsgArrs.push(Warning(ClMsgType::INVALID_SPECIAL_CHAR, nLines, posInLine - 1));
                        word += c;
                        break;
                    }

                    isSpecialCharContext = false;
                }
                else if (c == '\\') {
                    isSpecialCharContext = true;
                }
                else if (CODE_TABLE[c] & (uint)CharacterType::N)
                {
                    clMsgArrs.push(Error(ClMsgType::STR_LITERAL_IS_NOT_CLOSED, nLines, posInLine));
                    nLines++;
                    posInLine = 0;
                }
                else if (c == '\"')
                {
                    word += c;
                    addWordToWT();
                    isStringLiteralContext = false;
                }
                else {
                    word += c;
                }

                continue;
            }

            if (isCommentContext)
            {
                if (CODE_TABLE[c] & (uint)CharacterType::C) {
                    isCommentContext = false;
                }
                else if (CODE_TABLE[c] & (uint)CharacterType::N)
                {
                    nLines++;
                    posInLine = 0;
                }

                continue;
            }

            switch (CODE_TABLE[c])
            {
            case (uint)CharacterType::S:
            {
                if (!word.empty()) {
                    addWordToWT();
                }

                wordPosInLine = posInLine;
                if (wordTable.size() >= LT_MAX_SIZE) {
                    throw Error(ClMsgType::LT_MAX_SIZE_EXCEEDED, nLines, wordPosInLine);
                }

                if ((c == '+' || c == '-')
                    &&
                    (wordTable.empty()
                     ||
                     wordTable.back().word.back() != '\"' &&
                     !isalpha(wordTable.back().word.back()) &&
                     !isdigit(wordTable.back().word.back()) &&
                     wordTable.back().word != ")"))
                {
                    wordTable.emplace_back(string(1, '#') + string(1, c), nLines, wordPosInLine);
                }
                else {
                    wordTable.emplace_back(string(1, c), nLines, wordPosInLine);
                }
            } break;
            case (uint)CharacterType::Q:
            {
                if (!word.empty()) {
                    addWordToWT();
                }

                wordPosInLine = posInLine;
                word += c;
                isStringLiteralContext = true;
            } break;
            case (uint)CharacterType::C:
            {
                if (!word.empty()) {
                    addWordToWT();
                }
                isCommentContext = true;
            } break;
            case (uint)CharacterType::P:
                if (!word.empty()) {
                    addWordToWT();
                }
                break;
            case (uint)CharacterType::N:
                if (!word.empty()) {
                    addWordToWT();
                }
                nLines++;
                posInLine = 0;
                break;
            default:
            {
                if (word.empty() && isdigit(c) && !wordTable.empty())
                {
                    do
                    {
                        if (wordTable.back().word == "#+") {
                            //
                        }
                        else if (wordTable.back().word == "#-")
                        {
                            if (word.starts_with('-')) {
                                word[0] = '+';
                            }
                            else {
                                word = '-' + word;
                            }
                        }
                        else {
                            break;
                        }

                        wordPosInLine = wordTable.back().pos.posInLine;
                        wordTable.pop_back();
                    } while (!wordTable.empty());
                }

                if (word.empty()) {
                    wordPosInLine = posInLine;
                }
                word += c;
            } break;
            }
        }

        if (isStringLiteralContext) {
            clMsgArrs.push(Error(ClMsgType::STR_LITERAL_IS_NOT_CLOSED, nLines, posInLine));
        }

        if (isCommentContext) {
            clMsgArrs.push(Error(ClMsgType::COMMENT_IS_NOT_CLOSED, nLines, posInLine));
        }

        if (!word.empty()) {
            addWordToWT();
        }
	}
}