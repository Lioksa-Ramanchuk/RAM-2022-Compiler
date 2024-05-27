#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"
#include "LexicalAnalysis.hpp"

namespace RAM_2022_SemanticAnalysis
{
	constexpr uint16_t MAX_FUNC_PARAMS_NUMBER = 10;

	void checkSemantics(RAM_2022_LexicalAnalysis::Lexer& lexer, RAM_2022_ClMsg::ClMsgArrs& clMsgArrs);
}