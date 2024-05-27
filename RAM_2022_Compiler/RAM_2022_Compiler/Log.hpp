#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"
#include "In.hpp"
#include "Params.hpp"

namespace RAM_2022_Log		// работа с протоколом
{
	struct Log		// протокол
	{
		std::wstring path = L"";
		std::ofstream stream;				// выходной поток протокола

		void open(const std::wstring& LOG_PATH, RAM_2022_ClMsg::ClMsgArrs& clMsgArrs);
		template<std::convertible_to<std::string>... Args>
		void writeLine(const Args&... ARGS)
		{
			if (!stream.is_open()) {
				throw RAM_2022_ClMsg::Error(RAM_2022_ClMsg::ClMsgType::CLOSED_STREAM_WRITING_FAILURE, "Ошибка при записи в протокол: поток закрыт.");
			}

			((stream << ARGS), ...);
			stream << std::endl;
		}
		void writeTitle();
	};

	void logInfo(std::ostream& logStream, const RAM_2022_ClMsg::Warning& WARNING);
	void logInfo(std::ostream& logStream, const RAM_2022_ClMsg::Error& ERROR);
	void logInfo(std::ostream& logStream, RAM_2022_ClMsg::ClMsgArrs& clMsgArrs, const std::wstring& IN_PATH);
	void logInfo(std::ostream& logStream, const RAM_2022_In::In& IN);
	void logInfo(std::ostream& logStream, const RAM_2022_Params::Params& PARAMS);
}