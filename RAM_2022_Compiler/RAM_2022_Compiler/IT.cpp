#include "stdafx.h"
#include "IT.hpp"
#include "ClMsg.hpp"
using namespace RAM_2022_ClMsg;
using namespace std;

namespace RAM_2022_IT
{
	std::string IdInfo::getFullId() const
	{
		string fullId = id;

		weak_ptr<IdScope> pCurrentScope = pScope;
		if (pCurrentScope.lock() != weak_ptr<IdScope>().lock())
		{
			while (pCurrentScope.lock()->pOuterScope.lock() != weak_ptr<IdScope>().lock())
			{
				fullId = to_string(pCurrentScope.lock()->scopeId) + "_" + fullId;
				pCurrentScope = pCurrentScope.lock()->pOuterScope;
			}
			fullId = "g_" + fullId;
		}

		return fullId;
	}

	IdScope::IdScope(uint32_t scopeId, weak_ptr<IdScope> pOuterScope)
	{
		this->scopeId = scopeId;
		this->pOuterScope = pOuterScope;
	}
	void IdScope::addInnerScope()
	{
		innerScopes.push_back(make_shared<IdScope>(innerScopes.size(), weak_from_this()));
	}
	bool IdScope::contains(string_view id) const
	{
		return ranges::any_of(ids, [&id](weak_ptr<IdInfo> pIdI) { return pIdI.lock()->id == id; });
	}
	weak_ptr<IdInfo> IdScope::find(std::string_view id)
	{
		weak_ptr<IdScope> pCurrentScope = weak_from_this();

		while (pCurrentScope.lock() != weak_ptr<IdScope>().lock())
		{
			for (weak_ptr<IdInfo> pIdI : pCurrentScope.lock()->ids)
			{
				if (pIdI.lock()->id == id) {
					return pIdI;
				}
			}

			pCurrentScope = pCurrentScope.lock()->pOuterScope;
		}

		return weak_ptr<IdInfo>();
	}
	bool IdScope::isExtern(string_view id, vector<shared_ptr<IdInfo>>& externs)
	{
		weak_ptr<IdInfo> pThisExtern;
		for (weak_ptr<IdInfo> pExtern : externs)
		{
			if (pExtern.lock()->id == id)
			{
				pThisExtern = pExtern;
				break;
			}
		}

		return pThisExtern.lock() != weak_ptr<IdInfo>().lock() &&
			   pThisExtern.lock() == find(id).lock();
	}
	
	IdTable::IdTable(uint16_t maxSize)
	{
		if (maxSize > IT_MAX_SIZE) {
			throw Error(ClMsgType::IT_MAX_SIZE_EXCEEDED);
		}

		this->maxSize = maxSize;
	}
	void IdTable::addIdI(const IdInfo& ID_I, std::weak_ptr<IdScope> idScope)
	{
		if (size >= maxSize) {
			throw Error(ClMsgType::IT_MAX_SIZE_EXCEEDED);
		}

		idScope.lock()->ids.push_back(make_shared<IdInfo>(ID_I));
		idScope.lock()->ids.back()->pScope = idScope;
		switch (idScope.lock()->ids.back()->dataType)
		{
		using enum RAM_2022_IT::IdDataType;
		case INT:
			idScope.lock()->ids.back()->value = IT_INT_DEFAULT;
			break;
		case STR:
			idScope.lock()->ids.back()->value = string(IT_STR_DEFAULT_LENGTH, IT_STR_DEFALUT_SYMBOL);
			break;
		default:
			break;
		}
		size++;
	}
	void IdTable::addExtern(const IdInfo& ID_I, std::weak_ptr<IdScope> pScope)
	{
		if (size >= maxSize) {
			throw Error(ClMsgType::IT_MAX_SIZE_EXCEEDED);
		}

		weak_ptr<IdInfo> pExtern = findOrAddExtern(ID_I);
		pScope.lock()->ids.push_back(pExtern.lock());
		size++;
	}
	std::weak_ptr<IdInfo> IdTable::findOrAddExtern(const IdInfo& ID_I)
	{
		for (weak_ptr<IdInfo> pExtern : externs)
		{
			if (pExtern.lock()->id == ID_I.id) {
				return pExtern;
			}
		}

		externs.emplace_back(make_shared<IdInfo>(ID_I));
		return externs.back();
	}
	std::weak_ptr<IdInfo> IdTable::findOrAddLiteral(int32_t val, uint16_t iLT)
	{
		for (weak_ptr<IdInfo> pLiteral : literals)
		{
			if (pLiteral.lock()->dataType == IdDataType::INT &&
				get<int32_t>(pLiteral.lock()->value) == val)
			{
				return pLiteral;
			}
		}

		literals.emplace_back(make_shared<IdInfo>(iLT, string("L") + to_string(literals.size() + 1), IdType::LITERAL, IdDataType::INT));
		literals.back()->value = val;
		return literals.back();
	}
	std::weak_ptr<IdInfo> IdTable::findOrAddLiteral(std::string_view val, uint16_t iLT)
	{
		for (weak_ptr<IdInfo> pLiteral : literals)
		{
			if (pLiteral.lock()->dataType == IdDataType::STR &&
				get<string>(pLiteral.lock()->value) == val)
			{
				return pLiteral;
			}
		}

		literals.emplace_back(make_shared<IdInfo>(iLT, string("L") + to_string(literals.size() + 1), IdType::LITERAL, IdDataType::STR));
		literals.back()->value = val.data();
		return literals.back();
	}
}