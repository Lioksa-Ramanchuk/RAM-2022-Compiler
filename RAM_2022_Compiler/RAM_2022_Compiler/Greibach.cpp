#include "stdafx.h"
#include "Greibach.hpp"
using namespace RAM_2022_ClMsg;
using namespace std;

namespace RAM_2022_Greibach
{
	Rule::Chain::Chain() {}
	Rule::Rule() {}

	Greibach::Greibach() {}
	
	int32_t Greibach::getRule(GrbAlphabet nonTerm, Rule& rule) const
	{
		int32_t ruleNumber = -1;
		uint16_t k = 0;
		while (k < nRules && rules[k].nonTerm != nonTerm) {
			k++;
		}
		if (k < nRules)
		{
			ruleNumber = k;
			rule = rules[ruleNumber];
		}
		return ruleNumber;
	}

	Rule Greibach::getRule(uint16_t ruleNumber) const
	{
		return (ruleNumber < nRules) ? rules[ruleNumber] : Rule();
	}

	string Rule::getStrRule(uint16_t chainNumber) const
	{
		return string(1, Chain::grbAlToChar(nonTerm)) + "->" + chains[chainNumber].getStrChain();
	}

	int32_t Rule::getNextChain(GrbAlphabet t, Rule::Chain& destChain, uint16_t minChainNumber) const
	{
		int32_t chainNumber = -1;
		while (minChainNumber < nChains && chains[minChainNumber].chain[0] != t) {
			minChainNumber++;
		}
		chainNumber = (minChainNumber < nChains ? minChainNumber : -1);
		if (chainNumber >= 0) {
			destChain = chains[chainNumber];
		}
		return chainNumber;
	}

	string Rule::Chain::getStrChain() const
	{
		string strChain = "";
		strChain.reserve(length);
		for (GrbAlphabet c : chain) {
			strChain.push_back(Chain::grbAlToChar(c));
		}
		return strChain;
	}
}