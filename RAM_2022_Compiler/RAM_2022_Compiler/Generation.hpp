#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"
#include "LexicalAnalysis.hpp"

namespace RAM_2022_Generation
{
	void generateMasm(const RAM_2022_LexicalAnalysis::Lexer& LEXER, std::ostream& out);
}