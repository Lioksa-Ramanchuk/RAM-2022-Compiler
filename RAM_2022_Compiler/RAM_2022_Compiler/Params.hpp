#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"

namespace RAM_2022_Params			// обработка входных параметров
{
	constexpr uint32_t PARAM_MAX_LENGTH = 300;				// максимальная длина строки параметра
	constexpr uint32_t PARAMS_NUMBER = 3;

	const std::array<std::wstring, PARAMS_NUMBER> OPTIONS{ L"-in:", L"-out:", L"-log:" };
	const std::array<std::wstring, PARAMS_NUMBER> OPTION_DEFAULT_EXTENSIONS{ L"", L".asm", L".log" };

	struct Params		// входные параметры
	{
		std::wstring in = L"";	// -in:		имя файла исходного кода
		std::wstring out = L"";	// -out:	имя файла объектного кода
		std::wstring log = L"";	// -log:	имя файла протокола
	};

	void setParams(Params& params, std::span<wchar_t*> mainArgs, RAM_2022_ClMsg::ClMsgArrs& clMsgArrs);
}