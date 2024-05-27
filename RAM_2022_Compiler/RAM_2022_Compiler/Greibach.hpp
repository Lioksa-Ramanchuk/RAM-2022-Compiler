#pragma once
#include "stdafx.h"
#include "LT.hpp"
#include "ClMsg.hpp"

namespace RAM_2022_Greibach
{
	using GrbAlphabet = int16_t;

	struct Rule
	{
		struct Chain
		{
			uint16_t length = 0;
			std::vector<GrbAlphabet> chain;
			Chain();
			template<std::same_as<GrbAlphabet> ...Args>
			explicit Chain(Args... symbols)
			{
				length = (uint16_t)sizeof...(Args);
				chain.reserve(length);
				for (GrbAlphabet symbol : { symbols... }) {
					chain.push_back(symbol);
				}
			}
			std::string getStrChain() const;
			static constexpr GrbAlphabet T(char t) { return (GrbAlphabet)t; }
			static constexpr GrbAlphabet N(char n) { return -(GrbAlphabet)n; }
			static constexpr bool isT(GrbAlphabet s) { return s > 0; }
			static constexpr bool isN(GrbAlphabet s) { return !isT(s); }
			static constexpr char grbAlToChar(GrbAlphabet s) { return (char)(isT(s) ? s : -s); }
		};

		GrbAlphabet nonTerm = 0;
		RAM_2022_ClMsg::ClMsgType errorType = RAM_2022_ClMsg::ClMsgType::INVALID_ERROR_ID;
		uint16_t nChains = 0;
		std::vector<Chain> chains;
		Rule();
		template<std::same_as<Chain> ...Args>
		Rule(GrbAlphabet nonTerm, RAM_2022_ClMsg::ClMsgType errorType, const Args&... CHAINS)
		{
			this->nonTerm = nonTerm;
			this->errorType = errorType;
			nChains = (uint16_t)sizeof...(Args);
			chains.reserve(nChains);
			for (const Chain& CHAIN : { CHAINS... }) {
				chains.push_back(CHAIN);
			}
		}
		std::string getStrRule(uint16_t chainNumber) const;
		int32_t getNextChain(GrbAlphabet t, Rule::Chain& chain, uint16_t minChainNumber) const;
	};

	struct Greibach
	{
		GrbAlphabet startN = 0;
		GrbAlphabet stackBottomT = 0;
		uint16_t nRules = 0;
		std::vector<Rule> rules;
		Greibach();
		template<std::same_as<Rule> ...Args>
		Greibach(GrbAlphabet startN, GrbAlphabet stackBottomT, const Args&... RULES)
		{
			this->startN = startN;
			this->stackBottomT = stackBottomT;
			nRules = (uint16_t)sizeof...(Args);
			rules.reserve(this->nRules);
			for (const Rule& RULE : { RULES... }) {
				rules.push_back(RULE);
			}
		}
		int32_t getRule(GrbAlphabet nonTerm, Rule& rule) const;
		Rule getRule(uint16_t ruleNumber) const;
	};

	constexpr GrbAlphabet NS(char n) {
		return Rule::Chain::N(n);
	}
	constexpr GrbAlphabet TS(char n) {
		return Rule::Chain::T(n);
	}

	const Greibach GREIBACH(NS('S'), TS((char)RAM_2022_LT::LexemeType::END_OF_TAPE),
		Rule(NS('S'), RAM_2022_ClMsg::ClMsgType::WRONG_PROGRAM_STRUCTURE,
			Rule::Chain(TS('x'), TS('l'), TS(';')),
			Rule::Chain(TS('x'), TS('l'), TS(';'), NS('S')),
			Rule::Chain(TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('m'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), TS('r'), NS('I'), TS(';'), TS('}')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), TS('r'), NS('I'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(TS('p'), TS('i'), NS('P'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('p'), TS('i'), NS('P'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(TS('x'), TS('t'), TS('f'), TS('i'), NS('P'), TS(';')),
			Rule::Chain(TS('x'), TS('t'), TS('f'), TS('i'), NS('P'), TS(';'), NS('S')),
			Rule::Chain(TS('x'), TS('p'), TS('i'), NS('P'), TS(';')),
			Rule::Chain(TS('x'), TS('p'), TS('i'), NS('P'), TS(';'), NS('S'))
		),
		Rule(NS('I'), RAM_2022_ClMsg::ClMsgType::WRONG_ID,
			Rule::Chain(TS('i')),
			Rule::Chain(TS('l'))
		),
		Rule(NS('P'), RAM_2022_ClMsg::ClMsgType::WRONG_PARAMETERS,
			Rule::Chain(TS('('), TS(')')),
			Rule::Chain(TS('('), NS('F'), TS(')'))
		),
		Rule(NS('F'), RAM_2022_ClMsg::ClMsgType::WRONG_PARAMETERS,
			Rule::Chain(TS('t'), TS('i')),
			Rule::Chain(TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('C'), RAM_2022_ClMsg::ClMsgType::WRONG_CALLED_PARAMETERS,
			Rule::Chain(TS('('), TS(')')),
			Rule::Chain(TS('('), NS('W'), TS(')'))
		),
		Rule(NS('W'), RAM_2022_ClMsg::ClMsgType::WRONG_CALLED_PARAMETERS,
			Rule::Chain(TS('i')),
			Rule::Chain(TS('l')),
			Rule::Chain(TS('i'), TS(','), NS('W')),
			Rule::Chain(TS('l'), TS(','), NS('W'))
		),
		Rule(NS('E'), RAM_2022_ClMsg::ClMsgType::WRONG_EXPRESSION,
			Rule::Chain(TS('i')),
			Rule::Chain(TS('i'), TS('a'), NS('E')),
			Rule::Chain(TS('l')),
			Rule::Chain(TS('l'), TS('a'), NS('E')),
			Rule::Chain(TS('i'), NS('C')),
			Rule::Chain(TS('i'), NS('C'), TS('a'), NS('E')),
			Rule::Chain(TS('('), NS('E'), TS(')')),
			Rule::Chain(TS('('), NS('E'), TS(')'), TS('a'), NS('E')),
			Rule::Chain(TS('u'), NS('E'))
		),
		Rule(NS('A'), RAM_2022_ClMsg::ClMsgType::WRONG_ELIF,
			Rule::Chain(TS('&'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('&'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('A'))
		),
		Rule(NS('N'), RAM_2022_ClMsg::ClMsgType::WRONG_OPERATOR,
			Rule::Chain(TS('t'), TS('i'), TS(';')),
			Rule::Chain(TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(TS('w'), NS('I'), TS(';')),
			Rule::Chain(TS('w'), NS('I'), TS(';'), NS('N')),
			Rule::Chain(TS('g'), TS('i'), TS(';')),
			Rule::Chain(TS('g'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(TS('i'), NS('C'), TS(';')),
			Rule::Chain(TS('i'), NS('C'), TS(';'), NS('N')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), TS('r'), NS('I'), TS(';'), TS('}')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), TS('r'), NS('I'), TS(';'), TS('}'), NS('N')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}')),
			Rule::Chain(TS('t'), TS('f'), TS('i'), NS('P'), TS('{'), NS('N'), TS('r'), NS('I'), TS(';'), TS('}'), NS('N')),
			Rule::Chain(TS('p'), TS('i'), NS('P'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('p'), TS('i'), NS('P'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), TS(':'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), TS(':'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('A')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('A'), NS('N')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('A'), TS(':'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('?'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('A'), TS(':'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(TS('c'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(TS('c'), TS('('), NS('I'), TS('b'), NS('I'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(TS('x'), TS('t'), TS('f'), TS('i'), NS('P'), TS(';')),
			Rule::Chain(TS('x'), TS('t'), TS('f'), TS('i'), NS('P'), TS(';'), NS('N')),
			Rule::Chain(TS('x'), TS('p'), TS('i'), NS('P'), TS(';')),
			Rule::Chain(TS('x'), TS('p'), TS('i'), NS('P'), TS(';'), NS('N'))
		));
}