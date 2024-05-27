#pragma once
#include "stdafx.h"

namespace RAM_2022_ClMsg
{
	constexpr uint32_t MAX_ERRORS_NUMBER = 15;
	constexpr uint32_t MAX_WARNINGS_NUMBER = SIZE_MAX;
	constexpr uint32_t GRB_ERROR_SERIES = 600;

	// серии ошибок:   0 -  99 - ошибки системы
	//				 100 - 109 - ошибки параметров программы
	//				 110 - 119 - ошибки открытия и чтения файлов
	//				 200 - 299 - лексические ошибки
	//				 300 - 399 - синтаксические ошибки
	//				 400 - 499 - семантические ошибки
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
		{ ClMsgType::INVALID_ERROR_ID,					"Недопустимый код ошибки" },
		{ ClMsgType::SYSTEM_FAILURE,					"Системный сбой" },
		{ ClMsgType::MEMORY_ALLOCATION_FAILURE,			"Ошибка выделения памяти" },
		{ ClMsgType::LT_MAX_SIZE_EXCEEDED,				"Превышен максимальный допустимый размер таблицы лексем" },
		{ ClMsgType::IT_MAX_SIZE_EXCEEDED,				"Превышен максимальный допустимый размер таблицы идентификаторов" },
		{ ClMsgType::INVALID_LT_LEXEME_IDX,				"Некорректный индекс лексемы в таблице лексем" },
		{ ClMsgType::MISSING_PARAMETER_IN,				"Параметр -in должен быть задан" },
		{ ClMsgType::PARAMETER_MAX_LENGTH_EXCEEDED,		"Превышена максимальная допустимая длина входного параметра" },
		{ ClMsgType::FSTREAM_OPENING_FAILURE,			"Ошибка при открытии файлового потока" },
		{ ClMsgType::CLOSED_STREAM_WRITING_FAILURE,		"Ошибка при записи в поток: поток закрыт" },
		{ ClMsgType::CODE_MAX_LENGTH_EXCEEDED,			"Превышен максимальный допустимый размер исходного кода" },
		{ ClMsgType::UNKNOWN_LEXEME,					"Нераспознанная лексема" },
		{ ClMsgType::FORBIDDEN_CHARACTER_FOUND,			"Встречен недопустимый символ" },
		{ ClMsgType::IGNORED_CHARACTER_FOUND,			"Встречен игнорируемый символ" },
		{ ClMsgType::INVALID_SPECIAL_CHAR,				"Строковый литерал содержит неизвестную управляющую последовательность" },
		{ ClMsgType::STR_LITERAL_IS_NOT_CLOSED,			"Строковый литерал не закрыт" },
		{ ClMsgType::COMMENT_IS_NOT_CLOSED,				"Комментарий не закрыт" },
		{ ClMsgType::ID_REDECLARATION,					"Повторное объявление идентификатора" },
		{ ClMsgType::INT_IS_OUT_OF_RANGE,				"Целочисленное значение не входит в допустимый диапазон" },
		{ ClMsgType::STR_MAX_LENGTH_EXCEEDED,			"Превышена максимальная допустимая длина строкового литерала" },
		{ ClMsgType::ID_MAX_LENGTH_EXCEEDED,			"Превышена максимальная допустимая длина идентификатора" },
		{ ClMsgType::UNKNOWN_ID,						"Неизвестный идентификатор" },
		{ ClMsgType::MAIN_NOT_FOUND,					"Не найдена конструкция main" },
		{ ClMsgType::MAIN_REDEFINITION,					"Повторное объявление конструкции main" },
		{ ClMsgType::MAIN_IS_NOT_GLOBAL,				"Конструкция main должна быть в глобальной области видимости" },
		{ ClMsgType::WRONG_PROGRAM_STRUCTURE,			"Неверная структура программы" },
		{ ClMsgType::WRONG_OPERATOR,					"Ошибочный оператор" },
		{ ClMsgType::WRONG_EXPRESSION,					"Ошибка в выражении" },
		{ ClMsgType::WRONG_PARAMETERS,					"Ошибка в параметрах функции (процедуры)" },
		{ ClMsgType::WRONG_CALLED_PARAMETERS,			"Ошибка в параметрах вызываемой функции (процедуры)" },
		{ ClMsgType::WRONG_ID,							"Ожидается идентификатор" },
		{ ClMsgType::WRONG_ELIF,						"Ошибка в альтернативном условии" },
		{ ClMsgType::PARAMS_NUMBER_MISMATCH,			"Неверное количество параметров при вызове функции (процедуры)" },
		{ ClMsgType::PARAM_TYPE_MISMATCH,				"Неверный тип фактического параметра при вызове функции (процедуры)" },
		{ ClMsgType::PARENTHESES_REQUIRED,				"При вызове функции (процедуры) обязательны круглые скобки" },
		{ ClMsgType::VAR_CALL_ERROR,					"Вызвать можно только  функцию (процедуру)" },
		{ ClMsgType::UNUSED_FUNC,						"Функция (процедура) не вызывается в коде" },
		{ ClMsgType::WRONG_EXPR_TYPE,					"Типы операндов выражения должны совпадать с типом переменной, которой оно присваивается" },
		{ ClMsgType::OP_IS_FORBIDDEN_BY_TYPE,			"Оператор недопустим в данном выражении" },
		{ ClMsgType::CONDITION_TYPE_MISMATCH,			"Обе части условия должны быть одного типа" },
		{ ClMsgType::RETURN_TYPE_MISMATCH,				"Возвращаемый функцией тип не совпадает с указанным в объявлении" },
		{ ClMsgType::NULL_DIVISION,						"Деление на ноль" },
		{ ClMsgType::WRONG_LIB_LINK,					"Путь к библиотеке должен быть строковым литералом" },
		{ ClMsgType::MAX_FUNC_PARAMS_NUMBER_EXCEEDED,	"Превышено максимальное допустимое количество параметров функции (процедуры)" },
		{ ClMsgType::STR_FUNC_IN_EXPR_IS_NOT_1ST,		"Вызов функции, возвращающей строковое значение, может быть только первым операндом выражения" },
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