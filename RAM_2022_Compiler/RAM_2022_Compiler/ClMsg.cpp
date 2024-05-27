#include "stdafx.h"
#include "ClMsg.hpp"
using std::string_view;

using uint = uint32_t;

namespace RAM_2022_ClMsg
{
	ClMsg::ClMsg(ClMsgType id, uint line, uint col)
	{
		this->id = id;
		this->message = DEFAULT_CL_MESSAGES.contains(id) ? DEFAULT_CL_MESSAGES.at(id) : "Неопределённое сообщение";
		this->pos.line = line;
		this->pos.posInLine = col;
	}

	ClMsg::ClMsg(ClMsgType id, string_view message, uint line, uint col) {
		this->id = id;
		this->message = message;
		this->pos.line = line;
		this->pos.posInLine = col;
	}

	bool ClMsgArrs::push(const Warning& WARNING)
	{
		if (warnings.size() >= MAX_WARNINGS_NUMBER) {
			return false;
		}

		warnings.push_back(WARNING);
		return true;
	}

	bool ClMsgArrs::push(const Error& ERROR)
	{
		if (errors.size() >= MAX_ERRORS_NUMBER) {
			return false;
		}

		errors.push_back(ERROR);
		return true;
	}

	void ClMsgArrs::clearAll()
	{
		warnings.clear();
		errors.clear();
	}
}