#pragma once
#include "stdafx.h"
#include "In.hpp"
#include "Log.hpp"
#include "Out.hpp"
#include "Params.hpp"

namespace RAM_2022_ProgramInfo
{
	struct ProgramInfo
	{
		struct WordInfo
		{
			std::string word = "";
			uint32_t lineNumber = 0;
		};

		RAM_2022_Params::Params params;
		RAM_2022_Log::Log log;
		RAM_2022_In::In in;
		RAM_2022_Out::Out out;
		std::vector<WordInfo> words;
		
		void parseWords();
	};
}