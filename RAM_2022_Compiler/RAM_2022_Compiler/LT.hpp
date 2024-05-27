#pragma once
#include "stdafx.h"
#include "IT.hpp"

namespace RAM_2022_LT
{
	constexpr uint16_t LT_MAX_SIZE = 10000;

	enum class LexemeType : char
	{
		NONE				= ' ',
		TYPE				= 't',
		ID					= 'i',
		LITERAL				= 'l',
		LINK				= 'x',
		PROC				= 'p',
		FUNC				= 'f',
		RETURN				= 'r',
		MAIN				= 'm',
		WRITE				= 'w',
		READ				= 'g',
		IF					= '?',
		ELSE				= ':',
		ELIF				= '&',
		WHILE				= 'c',
		SEMICOLON			= ';',
		COMMA				= ',',
		LEFT_BRACE			= '{',
		RIGHT_BRACE			= '}',
		LEFT_PARENTHESIS	= '(',
		RIGHT_PARENTHESIS	= ')',
		ARITHMETIC_BIN_OP	= 'a',
		ARITHMETIC_UN_OP	= 'u',
		CMP_BIN_OP			= 'b',
		ASSIGN				= '=',
		END_OF_TAPE			= '$',
	};

	struct WordInfo
	{
		std::string word = "";
		struct {
			uint32_t line = 0;
			uint32_t posInLine = 0;
		} pos;

		WordInfo(std::string_view word, uint32_t line = 0, uint32_t posInLine = 0);
	};
	
	struct LexemeInfo
	{
		LexemeType type = LexemeType::NONE;
		struct {
			uint32_t line = 0;
			uint32_t posInLine = 0;
		} pos;
		std::weak_ptr<RAM_2022_IT::IdInfo> pIdI;
		const WordInfo* pWORD = nullptr;
	};

	struct LexTable
	{
		uint16_t maxSize = LT_MAX_SIZE;
		uint16_t size = 0;
		std::vector<LexemeInfo> table;

		explicit LexTable(uint16_t maxSize = LT_MAX_SIZE);
		void add(const LexemeInfo& LEXEME_I);
	};
}