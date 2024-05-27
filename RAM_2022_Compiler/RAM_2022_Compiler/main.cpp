#include "stdafx.h"
#include "ClMsg.hpp"
#include "FST.hpp"
#include "Generation.hpp"
#include "In.hpp"
#include "IT.hpp"
#include "LexicalAnalysis.hpp"
#include "Log.hpp"
#include "LT.hpp"
#include "MFST.hpp"
#include "Out.hpp"
#include "Params.hpp"
#include "RPN.hpp"
#include "SemanticAnalysis.hpp"
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_Generation;
using namespace RAM_2022_In;
using namespace RAM_2022_LexicalAnalysis;
using namespace RAM_2022_Log;
using namespace RAM_2022_LT;
using namespace RAM_2022_Mfst;
using namespace RAM_2022_Out;
using namespace RAM_2022_Params;
using namespace RAM_2022_ReversedPolishNotation;
using namespace RAM_2022_SemanticAnalysis;

using std::cout;

struct ProgramInfo
{
	ClMsgArrs clMsgArrs;
	Params params;
	Log log;
	In in;
	Out out;
	Lexer lexer;
};

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "rus");

	ProgramInfo programInfo;
	std::string phase;

	auto logClMsgs = [&]
	{
		logInfo(programInfo.log.stream, programInfo.clMsgArrs, programInfo.params.in);
		if (!programInfo.clMsgArrs.errors.empty())
		{
			cout << "���������� ��������. ������ �� �����: " << phase << std::endl;
			return false;
		}
		programInfo.clMsgArrs.clearAll();
		return true;
	};

	try
	{
		phase = "��������� ���������� ���������";
		setParams(programInfo.params, std::span<wchar_t*>(argv, argc), programInfo.clMsgArrs);
		if (!logClMsgs()) return 1;

		phase = "�������� ���������";
		programInfo.log.open(programInfo.params.log, programInfo.clMsgArrs);
		if (!logClMsgs()) return 1;

		phase = "������ � �������� ���������";
		programInfo.log.writeTitle();
		if (!logClMsgs()) return 1;

		phase = "������ � �������� ���������� ���������";
		logInfo(programInfo.log.stream, programInfo.params);
		if (!logClMsgs()) return 1;

		phase = "�������� ��������� �����";
		programInfo.out.open(programInfo.params.out);
		if (!logClMsgs()) return 1;

		phase = "��������� ��������� ���� �� �����������";
		programInfo.in.readFile(programInfo.params.in, programInfo.lexer.wordTable, programInfo.clMsgArrs);
		if (!logClMsgs()) return 1;

		phase = "������ � �������� ������������� ��������� ����";
		logInfo(programInfo.log.stream, programInfo.in);
		if (!logClMsgs()) return 1;
		programInfo.log.writeLine();

		phase = "����������� ������";
		programInfo.lexer.fillLexTableAndIdTable(programInfo.clMsgArrs);
		if (!logClMsgs()) return 1;
		programInfo.log.writeLine("����������� ������ �������� �������.\n");

		phase = "������ � �������� ������� ����������, ������, ���������������";
		programInfo.lexer.printTables(programInfo.log.stream);
		programInfo.log.writeLine();

		programInfo.lexer.lexTable.table.push_back(LexemeInfo(LexemeType::END_OF_TAPE));
		programInfo.lexer.lexTable.size++;

		phase = "�������������� ������";
		Mfst mfst(programInfo.lexer.lexTable, programInfo.log.stream);
		if (!mfst.start())
		{
			cout << " ���������� �������� (������ ��� �������������� �������).\n";
			return 1;
		}
		mfst.saveDeducation();
		programInfo.lexer.lexTable.table.pop_back();
		programInfo.lexer.lexTable.size--;

		programInfo.log.writeLine("\n�������������� ������ �������� �������.\n");
		mfst.logRules();
		programInfo.log.writeLine();

		phase = "������������� ������";
		checkSemantics(programInfo.lexer, programInfo.clMsgArrs);
		if (!logClMsgs()) return 1;
		programInfo.log.writeLine("������������� ������ �������� �������.\n");

		phase = "�������������� ��������� � �����";
		toRPN(programInfo.lexer);
		programInfo.log.writeLine("�������������� ��������� � ����� ��������� �������.\n");

		phase = "������ � �������� ��������������� ������� ����������, ������, ���������������";
		programInfo.lexer.printTables(programInfo.log.stream);
		programInfo.log.writeLine();

		phase = "��������� ���������� ����";
		generateMasm(programInfo.lexer, programInfo.out.stream);
		programInfo.log.writeLine("��������� ���������� ���� ��������� �������.");
	}
	catch (const Error& ERROR)
	{
		programInfo.clMsgArrs.errors.push_back(ERROR);
		if (!logClMsgs()) return 1;
	}

	cout << " ���������� ��������� �������.\n";

	return 0;
}