#include "stdafx.h"
#include "Log.hpp"
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_In;
using namespace RAM_2022_Params;
using namespace std;
using chrono::current_zone;
using chrono::system_clock;

using uint = uint32_t;

namespace RAM_2022_Log
{
	void Log::open(const wstring& LOG_PATH, RAM_2022_ClMsg::ClMsgArrs& clMsgArrs)
	{
		path = LOG_PATH;
		stream.open(LOG_PATH);
		if (stream.fail())
		{
			clMsgArrs.push(Warning(ClMsgType::FSTREAM_OPENING_FAILURE, "Ошибка при открытии файла протокола"));
			stream.set_rdbuf(cout.rdbuf());
		}
	}
	void Log::writeTitle()
	{
		if (!stream.is_open()) {
			throw Error(ClMsgType::CLOSED_STREAM_WRITING_FAILURE, "Ошибка при записи в протокол: поток закрыт");
		}

		stream << format("---- Протокол ---- {:%Y-%m-%d %X} ----", current_zone()->to_local(system_clock::now()));
		stream << endl;
	}

	void logInfo(ostream& logStream, const Warning& WARNING)
	{
		logStream << "Предупреждение " << (uint32_t)WARNING.id << ": " << WARNING.message;
		if (WARNING.pos.line)
		{
			logStream << ", строка " << WARNING.pos.line;
			if (WARNING.pos.posInLine) {
				logStream << ", позиция " << WARNING.pos.posInLine;
			}
		}
		logStream << endl;
	}
	void logInfo(ostream& logStream, const Error& ERROR)
	{
		logStream << "Ошибка " << (uint32_t)ERROR.id << ": " << ERROR.message;
		if (ERROR.pos.line)
		{
			logStream << ", строка " << ERROR.pos.line;
			if (ERROR.pos.posInLine) {
				logStream << ", позиция " << ERROR.pos.posInLine;
			}
		}
		logStream << endl;
	}
	void logInfo(std::ostream& logStream, ClMsgArrs& clMsgArrs, const wstring& IN_PATH)
	{
		if (clMsgArrs.errors.empty() && clMsgArrs.warnings.empty()) {
			return;
		}

		ifstream ifile;
		ifile.open(IN_PATH);
		bool inIsOpened = ifile.is_open();
		uint currentLineNumber = 0;
		string currentLine;

		auto cmpClMsgs = [](const ClMsg& Msg1, const ClMsg& Msg2) {
			return Msg1.pos.line < Msg2.pos.line ||
				   Msg1.pos.line == Msg2.pos.line && Msg1.pos.posInLine <= Msg2.pos.posInLine;
		};

		ranges::sort(clMsgArrs.errors, cmpClMsgs);
		for (const Error& ERR : clMsgArrs.errors)
		{
			logInfo(logStream, ERR);
			if (inIsOpened && ERR.pos.line)
			{
				while (currentLineNumber < ERR.pos.line)
				{
					getline(ifile, currentLine);
					currentLineNumber++;
				}

				logStream << currentLine << endl;
			}
		}

		if (inIsOpened)
		{
			ifile.clear();
			ifile.seekg(0, ios::beg);
			currentLineNumber = 0;
		}

		ranges::sort(clMsgArrs.warnings, cmpClMsgs);
		for (const Warning& WARN : clMsgArrs.warnings) {
			logInfo(logStream, WARN);
			if (inIsOpened && WARN.pos.line)
			{
				while (currentLineNumber < WARN.pos.line)
				{
					getline(ifile, currentLine);
					currentLineNumber++;
				}

				logStream << currentLine << endl;
			}
		}
	}
	void logInfo(ostream& logStream, const In& IN)
	{
		logStream << "---- Исходные данные ----\n";
		logStream << "Количество символов: " << IN.length << endl;
		logStream << "Проигнорировано:     " << IN.nIgnored << endl;
		logStream << "Количество строк:    " << IN.nLines << endl;
	}
	void logInfo(ostream& logStream, const Params& PARAMS)
	{
		static auto&& wstrToStr = [](const wstring& WSTR)
		{
			const size_t WSTR_LENGTH = WSTR.length();
			char* buf = new char[WSTR_LENGTH + 1];
			wcstombs_s(nullptr, buf, WSTR_LENGTH + 1, WSTR.c_str(), WSTR_LENGTH + 1);
			string str(buf);
			delete[] buf;
			return str;
		};

		logStream << "---- Параметры ----\n";
		logStream << "-log: " << wstrToStr(PARAMS.log) << endl;
		logStream << "-out: " << wstrToStr(PARAMS.out) << endl;
		logStream << "-in:  " << wstrToStr(PARAMS.in) << endl;
	}
}