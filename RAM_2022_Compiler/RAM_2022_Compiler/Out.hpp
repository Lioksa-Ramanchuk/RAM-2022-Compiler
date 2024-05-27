#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"

namespace RAM_2022_Out
{
	struct Out		// �������� ����
	{
		std::wstring path = L"";
		std::ofstream stream;				// �������� �����

		void open(const std::wstring& OUT_PATH);
		template<std::convertible_to<std::string>... Args>
		void writeLine(const Args&... ARGS)
		{
			if (!stream.is_open()) {
				throw RAM_2022_ClMsg::Error(RAM_2022_ClMsg::ClMsgType::CLOSED_STREAM_WRITING_FAILURE, "������ ��� ������ � �������� ����: ����� ������.");
			}

			(stream << ... << ARGS) << std::endl;
		}
	};
}