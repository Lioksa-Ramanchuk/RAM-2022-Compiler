#include "stdafx.h"
#include "Out.hpp"
using namespace RAM_2022_ClMsg;
using namespace std;

namespace RAM_2022_Out
{
	void Out::open(const wstring& OUT_PATH)
	{
		path = OUT_PATH;
		stream.open(OUT_PATH);
		if (stream.fail()) {
			throw Error(ClMsgType::FSTREAM_OPENING_FAILURE, "Ошибка при открытии выходного файла");
		}
	}
}