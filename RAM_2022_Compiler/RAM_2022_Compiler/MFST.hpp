#pragma once
#include "stdafx.h"
#include "Greibach.hpp"
#include "LT.hpp"

constexpr size_t MFST_DIAGN_NUMBER = 3;

namespace RAM_2022_Mfst
{
	using MfstStack = std::stack<RAM_2022_Greibach::GrbAlphabet>;

	struct MfstState
	{
		uint16_t tapePos = 0;
		uint16_t ruleNumber = 0;
		uint16_t ruleChainNumber = 0;
		MfstStack st;
		MfstState();
		MfstState(uint16_t tapePos, const MfstStack& ST, uint16_t ruleChainNumber);
		MfstState(uint16_t tapePos, const MfstStack& ST, uint16_t ruleNumber, uint16_t ruleChainNumber);
	};

	using MfstStatesStack = std::stack<MfstState>;

	struct Mfst
	{
		enum class StepResult
		{
			NS_OK,
			NS_NO_RULE,
			NS_NO_RULE_CHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOT_OK,
			TAPE_END,
			SURPRISE
		};

		struct MfstDiagnosis
		{
			int32_t tapePos = -1;
			StepResult stepResult = StepResult::SURPRISE;
			int32_t ruleNumber = -1;
			int32_t ruleChainNumber = -1;
			MfstDiagnosis();
			MfstDiagnosis(int32_t tapePos, StepResult stepResult, int32_t ruleNumber, int32_t ruleChainNumber);
		};

		struct Deducation
		{
			size_t nSteps = 0;
			std::vector<uint16_t> ruleNumbers;
			std::vector<uint16_t> ruleChainNumbers;
		};

		std::array<MfstDiagnosis, MFST_DIAGN_NUMBER> diagnoses;
		std::vector<RAM_2022_Greibach::GrbAlphabet> tape;
		uint16_t tapeLength = 0;
		uint16_t tapePos = 0;
		int32_t ruleNumber = -1;
		int32_t ruleChainNumber = -1;
		const RAM_2022_Greibach::Greibach GREIBACH = RAM_2022_Greibach::GREIBACH;
		const RAM_2022_LT::LexTable* pLEX_TABLE;
		MfstStack st;
		MfstStatesStack states;
		Deducation deducation;
		std::ostream* pLogStream = &std::cout;
		int32_t mfstTraceN = -1;

		explicit Mfst(const RAM_2022_LT::LexTable& LEX_TABLE, std::ostream& logStream);
		
		std::string getStrStack() const;
		std::string getStrTape(uint16_t pos, uint16_t n = 25);
		void logDiagnoses();
		bool saveState();
		bool restoreState();
		void pushChain(const RAM_2022_Greibach::Rule::Chain& CHAIN);
		StepResult step();
		bool start();
		void saveDiagnosis(StepResult stepResult);
		void logRules();
		void saveDeducation();
		void trace1(RAM_2022_Greibach::Rule& rule);
		void trace2();
		void trace3();
		void trace4(std::string_view s);
		void trace5(std::string_view s);
		void trace6(std::string_view s, size_t k);
		void trace7(const MfstState& STATE, const RAM_2022_Greibach::Rule& RULE);
	};
}