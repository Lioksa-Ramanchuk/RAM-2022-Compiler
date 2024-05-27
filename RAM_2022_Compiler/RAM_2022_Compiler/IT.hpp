#pragma once
#include "stdafx.h"

namespace RAM_2022_IT
{
	constexpr uint32_t ID_MAX_SIZE = 50;
	constexpr uint16_t IT_MAX_SIZE = 10000;
	constexpr int32_t IT_INT_DEFAULT = 0;
	constexpr int32_t IT_STR_DEFAULT_LENGTH = 0;
	constexpr char IT_STR_DEFALUT_SYMBOL = '\0';
	constexpr uint16_t ID_LT_NULL_IDX = UINT16_MAX;
	constexpr uint32_t IT_MAX_STR_LENGTH = 255;

	enum class IdType
	{
		NONE,
		VAR,
		FUNC,
		PARAM,
		LITERAL,
	};

	enum class IdDataType
	{
		NONE,
		INT,
		STR,
		VOID,
	};

	struct IdScope;

	struct IdInfo
	{
		uint16_t iFirstLexLT = ID_LT_NULL_IDX;
		std::string id = "";
		IdType type = IdType::NONE;
		IdDataType dataType = IdDataType::NONE;
		std::variant<int32_t, std::string> value;
		std::weak_ptr<IdScope> pScope;

		std::string getFullId() const;
	};

	struct IdScope : std::enable_shared_from_this<IdScope>
	{
		uint32_t scopeId = 0;

		std::vector<std::shared_ptr<IdInfo>> ids;
		std::vector<std::shared_ptr<IdScope>> innerScopes;
		std::weak_ptr<IdScope> pOuterScope;

		IdScope(uint32_t scopeId = 0, std::weak_ptr<IdScope> pOuterScope = std::weak_ptr<IdScope>());

		void addInnerScope();
		bool contains(std::string_view id) const;
		bool isExtern(std::string_view id, std::vector<std::shared_ptr<IdInfo>>& externs);
		std::weak_ptr<IdInfo> find(std::string_view id);
	};

	struct IdTable
	{
		uint16_t maxSize;
		uint16_t size = 0;
		std::shared_ptr<IdScope> scope = std::make_shared<IdScope>();
		std::vector<std::shared_ptr<IdInfo>> literals;
		std::vector<std::shared_ptr<IdInfo>> externs;

		explicit IdTable(uint16_t maxSize = IT_MAX_SIZE);
		
		void addIdI(const IdInfo& ID_I, std::weak_ptr<IdScope> idScope);
		void addExtern(const IdInfo& ID_I, std::weak_ptr<IdScope> pScope);
		std::weak_ptr<IdInfo> findOrAddExtern(const IdInfo& ID_I);
		std::weak_ptr<IdInfo> findOrAddLiteral(int32_t val, uint16_t iLT);
		std::weak_ptr<IdInfo> findOrAddLiteral(std::string_view val, uint16_t iLT);
	};
}