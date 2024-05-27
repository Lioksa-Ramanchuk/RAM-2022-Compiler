#pragma once
#include "stdafx.h"

namespace RAM_2022_ClMsg
{
	constexpr uint32_t MAX_ERRORS_NUMBER = 15;
	constexpr uint32_t MAX_WARNINGS_NUMBER = SIZE_MAX;
	constexpr uint32_t GRB_ERROR_SERIES = 600;

	// ����� ������:   0 -  99 - ������ �������
	//				 100 - 109 - ������ ���������� ���������
	//				 110 - 119 - ������ �������� � ������ ������
	//				 200 - 299 - ����������� ������
	//				 300 - 399 - �������������� ������
	//				 400 - 499 - ������������� ������
	enum class ClMsgType : uint32_t
	{
		INVALID_ERROR_ID				= 0,
		SYSTEM_FAILURE					= 1,
		MEMORY_ALLOCATION_FAILURE		= 2,
		LT_MAX_SIZE_EXCEEDED			= 3,
		IT_MAX_SIZE_EXCEEDED			= 4,
		INVALID_LT_LEXEME_IDX			= 5,

		MISSING_PARAMETER_IN			= 100,
		PARAMETER_MAX_LENGTH_EXCEEDED	= 101,

		FSTREAM_OPENING_FAILURE			= 110,
		CLOSED_STREAM_WRITING_FAILURE	= 111,
		CODE_MAX_LENGTH_EXCEEDED		= 112,

		UNKNOWN_LEXEME					= 200,
		FORBIDDEN_CHARACTER_FOUND		= 201,
		IGNORED_CHARACTER_FOUND			= 202,
		INVALID_SPECIAL_CHAR			= 203,
		STR_LITERAL_IS_NOT_CLOSED		= 204,
		COMMENT_IS_NOT_CLOSED			= 205,

		WRONG_PROGRAM_STRUCTURE			= 300,
		WRONG_OPERATOR					= 301,
		WRONG_EXPRESSION				= 302,
		WRONG_PARAMETERS				= 303,
		WRONG_CALLED_PARAMETERS			= 304,
		WRONG_ID						= 305,
		WRONG_ELIF						= 306,

		ID_REDECLARATION				= 401,
		INT_IS_OUT_OF_RANGE				= 402,
		STR_MAX_LENGTH_EXCEEDED			= 403,
		ID_MAX_LENGTH_EXCEEDED			= 404,
		UNKNOWN_ID						= 405,
		MAIN_NOT_FOUND					= 406,
		MAIN_REDEFINITION				= 407,
		MAIN_IS_NOT_GLOBAL				= 408,
		PARAMS_NUMBER_MISMATCH			= 409,
		PARAM_TYPE_MISMATCH				= 410,
		PARENTHESES_REQUIRED			= 411,
		VAR_CALL_ERROR					= 412,
		UNUSED_FUNC						= 413,
		WRONG_EXPR_TYPE					= 414,
		OP_IS_FORBIDDEN_BY_TYPE			= 415,
		CONDITION_TYPE_MISMATCH			= 416,
		RETURN_TYPE_MISMATCH			= 417,
		NULL_DIVISION					= 418,
		MAX_FUNC_PARAMS_NUMBER_EXCEEDED	= 419,
		WRONG_LIB_LINK					= 420,
		STR_FUNC_IN_EXPR_IS_NOT_1ST		= 421,
	};

	const std::map<ClMsgType, std::string> DEFAULT_CL_MESSAGES
	{
		{ ClMsgType::INVALID_ERROR_ID,					"������������ ��� ������" },
		{ ClMsgType::SYSTEM_FAILURE,					"��������� ����" },
		{ ClMsgType::MEMORY_ALLOCATION_FAILURE,			"������ ��������� ������" },
		{ ClMsgType::LT_MAX_SIZE_EXCEEDED,				"�������� ������������ ���������� ������ ������� ������" },
		{ ClMsgType::IT_MAX_SIZE_EXCEEDED,				"�������� ������������ ���������� ������ ������� ���������������" },
		{ ClMsgType::INVALID_LT_LEXEME_IDX,				"������������ ������ ������� � ������� ������" },
		{ ClMsgType::MISSING_PARAMETER_IN,				"�������� -in ������ ���� �����" },
		{ ClMsgType::PARAMETER_MAX_LENGTH_EXCEEDED,		"��������� ������������ ���������� ����� �������� ���������" },
		{ ClMsgType::FSTREAM_OPENING_FAILURE,			"������ ��� �������� ��������� ������" },
		{ ClMsgType::CLOSED_STREAM_WRITING_FAILURE,		"������ ��� ������ � �����: ����� ������" },
		{ ClMsgType::CODE_MAX_LENGTH_EXCEEDED,			"�������� ������������ ���������� ������ ��������� ����" },
		{ ClMsgType::UNKNOWN_LEXEME,					"�������������� �������" },
		{ ClMsgType::FORBIDDEN_CHARACTER_FOUND,			"�������� ������������ ������" },
		{ ClMsgType::IGNORED_CHARACTER_FOUND,			"�������� ������������ ������" },
		{ ClMsgType::INVALID_SPECIAL_CHAR,				"��������� ������� �������� ����������� ����������� ������������������" },
		{ ClMsgType::STR_LITERAL_IS_NOT_CLOSED,			"��������� ������� �� ������" },
		{ ClMsgType::COMMENT_IS_NOT_CLOSED,				"����������� �� ������" },
		{ ClMsgType::ID_REDECLARATION,					"��������� ���������� ��������������" },
		{ ClMsgType::INT_IS_OUT_OF_RANGE,				"������������� �������� �� ������ � ���������� ��������" },
		{ ClMsgType::STR_MAX_LENGTH_EXCEEDED,			"��������� ������������ ���������� ����� ���������� ��������" },
		{ ClMsgType::ID_MAX_LENGTH_EXCEEDED,			"��������� ������������ ���������� ����� ��������������" },
		{ ClMsgType::UNKNOWN_ID,						"����������� �������������" },
		{ ClMsgType::MAIN_NOT_FOUND,					"�� ������� ����������� main" },
		{ ClMsgType::MAIN_REDEFINITION,					"��������� ���������� ����������� main" },
		{ ClMsgType::MAIN_IS_NOT_GLOBAL,				"����������� main ������ ���� � ���������� ������� ���������" },
		{ ClMsgType::WRONG_PROGRAM_STRUCTURE,			"�������� ��������� ���������" },
		{ ClMsgType::WRONG_OPERATOR,					"��������� ��������" },
		{ ClMsgType::WRONG_EXPRESSION,					"������ � ���������" },
		{ ClMsgType::WRONG_PARAMETERS,					"������ � ���������� ������� (���������)" },
		{ ClMsgType::WRONG_CALLED_PARAMETERS,			"������ � ���������� ���������� ������� (���������)" },
		{ ClMsgType::WRONG_ID,							"��������� �������������" },
		{ ClMsgType::WRONG_ELIF,						"������ � �������������� �������" },
		{ ClMsgType::PARAMS_NUMBER_MISMATCH,			"�������� ���������� ���������� ��� ������ ������� (���������)" },
		{ ClMsgType::PARAM_TYPE_MISMATCH,				"�������� ��� ������������ ��������� ��� ������ ������� (���������)" },
		{ ClMsgType::PARENTHESES_REQUIRED,				"��� ������ ������� (���������) ����������� ������� ������" },
		{ ClMsgType::VAR_CALL_ERROR,					"������� ����� ������  ������� (���������)" },
		{ ClMsgType::UNUSED_FUNC,						"������� (���������) �� ���������� � ����" },
		{ ClMsgType::WRONG_EXPR_TYPE,					"���� ��������� ��������� ������ ��������� � ����� ����������, ������� ��� �������������" },
		{ ClMsgType::OP_IS_FORBIDDEN_BY_TYPE,			"�������� ���������� � ������ ���������" },
		{ ClMsgType::CONDITION_TYPE_MISMATCH,			"��� ����� ������� ������ ���� ������ ����" },
		{ ClMsgType::RETURN_TYPE_MISMATCH,				"������������ �������� ��� �� ��������� � ��������� � ����������" },
		{ ClMsgType::NULL_DIVISION,						"������� �� ����" },
		{ ClMsgType::WRONG_LIB_LINK,					"���� � ���������� ������ ���� ��������� ���������" },
		{ ClMsgType::MAX_FUNC_PARAMS_NUMBER_EXCEEDED,	"��������� ������������ ���������� ���������� ���������� ������� (���������)" },
		{ ClMsgType::STR_FUNC_IN_EXPR_IS_NOT_1ST,		"����� �������, ������������ ��������� ��������, ����� ���� ������ ������ ��������� ���������" },
	};

	struct ClMsg
	{
		ClMsgType id = ClMsgType::INVALID_ERROR_ID;
		std::string message = "";
		struct
		{
			uint32_t line = 0;
			uint32_t posInLine = 0;
		} pos;

		ClMsg(ClMsgType id, uint32_t line = 0, uint32_t col = 0);
		ClMsg(ClMsgType id, std::string_view message, uint32_t line = 0, uint32_t col = 0);
	};

	struct Error : ClMsg
	{
		using ClMsg::ClMsg;
	};

	struct Warning : ClMsg
	{
		using ClMsg::ClMsg;
	};

	struct ClMsgArrs
	{
		std::vector<Warning> warnings;
		std::vector<Error> errors;

		bool push(const Error& ERROR);
		bool push(const Warning& WARNING);
		void clearAll();
	};
}