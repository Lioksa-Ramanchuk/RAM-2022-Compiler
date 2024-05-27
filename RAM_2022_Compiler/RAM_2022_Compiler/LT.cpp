#include "stdafx.h"
#include "LT.hpp"
#include "ClMsg.hpp"
using namespace RAM_2022_ClMsg;
using namespace std;

using uint = uint32_t;

namespace RAM_2022_LT
{
	WordInfo::WordInfo(std::string_view word, uint32_t line, uint32_t posInLine)
	{
		this->word = word;
		pos.line = line;
		pos.posInLine = posInLine;
	}

	LexTable::LexTable(uint16_t maxSize)
	{
		if (maxSize > LT_MAX_SIZE) {
			throw Error(ClMsgType::LT_MAX_SIZE_EXCEEDED);
		}

		try {
			this->table.reserve(maxSize);
		}
		catch (const bad_alloc&) {
			throw Error(ClMsgType::MEMORY_ALLOCATION_FAILURE, "Ошибка при выделении памяти для таблицы лексем");
		}
	}
	void LexTable::add(const LexemeInfo& LEXEME_I)
	{
		if (size >= maxSize) {
			throw Error(ClMsgType::LT_MAX_SIZE_EXCEEDED, LEXEME_I.pos.line, LEXEME_I.pos.posInLine);
		}

		table.push_back(LEXEME_I);
		size++;
	}
}