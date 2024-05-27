#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"

namespace RAM_2022_Out
{
	struct Out		// выходной файл
	{
		std::wstring path = L"";
		std::ofstream stream;				// выходной поток

		void open(const std::wstring& OUT_PATH);
		template<std::convertible_to<std::string>... Args>
		void writeLine(const Args&... ARGS)
		{
			if (!stream.is_open()) {
				throw RAM_2022_ClMsg::Error(RAM_2022_ClMsg::ClMsgType::CLOSED_STREAM_WRITING_FAILURE, "Ошибка при записи в выходной файл: поток закрыт.");
			}

			(stream << ... << ARGS) << std::endl;
		}
	};
}