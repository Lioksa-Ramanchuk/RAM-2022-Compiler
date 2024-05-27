#include "stdafx.h"
#include "MFST.hpp"
#include "Log.hpp"
using namespace RAM_2022_ClMsg;
using namespace RAM_2022_LT;
using namespace RAM_2022_Greibach;
using namespace RAM_2022_Log;
using namespace std;
using uint = uint32_t;

namespace RAM_2022_Mfst
{
	void Mfst::trace1(Rule& rule)
	{
		mfstTraceN++;
		*pLogStream << setw( 4) << left << mfstTraceN << ": "
					<< setw(20) << left << rule.getStrRule((uint16_t)ruleChainNumber)	// Приведение безопасно
					<< setw(30) << left << getStrTape(tapePos)
					<< setw(20) << left << getStrStack() << endl;
	}
	void Mfst::trace2()
	{
		*pLogStream << setw( 4) << left << mfstTraceN << ": "
					<< setw(20) << left << " "
					<< setw(30) << left << getStrTape(tapePos)
					<< setw(20) << left << getStrStack() << endl;
	}
	void Mfst::trace3()
	{
		mfstTraceN++;
		*pLogStream << setw( 4) << left << mfstTraceN << ": "
					<< setw(20) << left << " "
					<< setw(30) << left << getStrTape(tapePos)
					<< setw(20) << left << getStrStack() << endl;
	}
	void Mfst::trace4(string_view s)
	{
		mfstTraceN++;
		*pLogStream << setw( 4) << left << mfstTraceN << ": "
					<< setw(20) << left << s << endl;
	}
	void Mfst::trace5(string_view s)
	{
		*pLogStream << setw( 4) << left << mfstTraceN << ": "
					<< setw(20) << left << s << endl;
	}
	void Mfst::trace6(string_view s, size_t k)
	{
		*pLogStream << setw( 4) << left << mfstTraceN << ": "
					<< setw(20) << left << s << k << endl;
	}
	void Mfst::trace7(const MfstState& STATE, const Rule& RULE)
	{
		*pLogStream << setw( 4) << left << STATE.tapePos << ": "
					<< setw(20) << left << RULE.getStrRule(STATE.ruleChainNumber) << endl;
	}

	MfstState::MfstState() {}
	MfstState::MfstState(uint16_t tapePos, const MfstStack& ST, uint16_t ruleChainNumber)
	{
		this->tapePos = tapePos;
		this->st = ST;
		this->ruleChainNumber = ruleChainNumber;
	}
	MfstState::MfstState(uint16_t tapePos, const MfstStack& ST, uint16_t ruleNumber, uint16_t ruleChainNumber)
		: MfstState(tapePos, ST, ruleChainNumber)
	{
		this->ruleNumber = ruleNumber;
	}

	Mfst::MfstDiagnosis::MfstDiagnosis() {}
	Mfst::MfstDiagnosis::MfstDiagnosis(int32_t tapePos, StepResult stepResult, int32_t ruleNumber, int32_t ruleChainNumber)
	{
		this->tapePos = tapePos;
		this->stepResult = stepResult;
		this->ruleNumber = ruleNumber;
		this->ruleChainNumber = ruleChainNumber;
	}

	Mfst::Mfst(const LexTable& LEX_TABLE, ostream& logStream)
	{
		pLEX_TABLE = &LEX_TABLE;
		tapeLength = pLEX_TABLE->size;
		tape.reserve(tapeLength);
		for (const LexemeInfo& LI : pLEX_TABLE->table) {
			tape.push_back(TS((char)LI.type));
		}
		st.push(GREIBACH.stackBottomT);
		st.push(GREIBACH.startN);
		pLogStream = &logStream;
	}

	Mfst::StepResult Mfst::step()
	{
		using enum RAM_2022_Mfst::Mfst::StepResult;
		StepResult result = SURPRISE;
		if (tapePos < tapeLength)
		{
			if (Rule::Chain::isN(st.top()))
			{
				Rule rule;
				ruleNumber = GREIBACH.getRule(st.top(), rule);
				if (ruleNumber >= 0)
				{
					Rule::Chain chain;
					ruleChainNumber = rule.getNextChain(tape[tapePos], chain, (uint16_t)(ruleChainNumber + 1));
					if (ruleChainNumber >= 0)
					{
						trace1(rule);
						saveState();
						st.pop();
						pushChain(chain);
						result = NS_OK;
						trace2();
					}
					else
					{
						trace4("TNS_NORULECHAIN/NS_NORULE");
						saveDiagnosis(NS_NO_RULE_CHAIN);
						result = restoreState() ? NS_NO_RULE_CHAIN : NS_NO_RULE;
					}
				}
				else {
					result = NS_ERROR;
				}
			}
			else if (st.top() == tape[tapePos])
			{
				tapePos++;
				st.pop();
				ruleChainNumber = -1;
				result = TS_OK;
				trace3();
			}
			else
			{
				trace4("TS_NOK/NS_NORULECHAIN");
				result = restoreState() ? TS_NOT_OK : NS_NO_RULE_CHAIN;
			}
		}
		else
		{
			result = TAPE_END;
			trace4("TAPE_END");
		}
		return result;
	}

	void Mfst::pushChain(const Rule::Chain& CHAIN)
	{
		for (uint16_t i = CHAIN.length; i-- > 0; ) {
			st.push(CHAIN.chain[i]);
		}
	}
	bool Mfst::saveState()
	{
		states.emplace(tapePos, st, (uint16_t)ruleNumber, (uint16_t)ruleChainNumber);	// Приведение безопасно
		trace6("SAVESTATE:", states.size());
		return true;
	}
	bool Mfst::restoreState()
	{
		if (states.empty()) {
			return false;
		}

		MfstState state = states.top();
		states.pop();
		tapePos = state.tapePos;
		st = state.st;
		ruleNumber = state.ruleNumber;
		ruleChainNumber = state.ruleChainNumber;
		trace5("RESSTATE");
		trace2();

		return true;
	}
	void Mfst::saveDiagnosis(StepResult stepResult)
	{
		size_t i = 0;
		while (i < MFST_DIAGN_NUMBER && tapePos <= diagnoses[i].tapePos) {
			i++;
		}
		if (i < MFST_DIAGN_NUMBER)
		{
			diagnoses[i] = MfstDiagnosis(tapePos, stepResult, ruleNumber, ruleChainNumber);
			for (size_t j = i + 1; j < MFST_DIAGN_NUMBER; j++) {
				diagnoses[j].tapePos = -1;
			}
		}
	}

	bool Mfst::start()
	{
		using enum RAM_2022_Mfst::Mfst::StepResult;

		bool errorsFound = true;

		*pLogStream << setw( 4) << left << "Шаг" << "|"
					<< setw(20) << left << " Правило"
					<< setw(30) << left << " Входная лента"
					<< setw(20) << left << " Стек" << endl;
		*pLogStream << "--------------------------------------------------------------------------------" << endl;

		StepResult stepResult;
		do {
			stepResult = step();
		} while(stepResult == NS_OK ||
			    stepResult == NS_NO_RULE_CHAIN ||
			    stepResult == TS_OK ||
			    stepResult == TS_NOT_OK);

		switch (stepResult)
		{
		case TAPE_END:
			trace4("------>TAPE_END");
			errorsFound = false;
			break;
		case NS_NO_RULE:
			trace4("------>NS_NORULE");
			logDiagnoses();
			break;
		case NS_ERROR:
			trace4("------>NS_ERROR");
			break;
		case SURPRISE:
			trace4("------>SURPRISE");
			break;
		default:
			break;
		}
		return !errorsFound;
	}
	string Mfst::getStrStack() const
	{
		string strStack = "";
		strStack.reserve(st.size());
		for (size_t i = st.size(); i-- > 0; ) {
			strStack += Rule::Chain::grbAlToChar(st._Get_container()[i]);
		}
		return strStack;
	}
	string Mfst::getStrTape(uint16_t pos, uint16_t n)
	{
		string strTape = "";
		int16_t upperBound = (pos + n <= tapeLength) ? pos + n : tapeLength;

		if (pos < upperBound) {
			strTape.reserve(upperBound - pos);
		}
		for (uint16_t i = pos; i < upperBound; i++) {
			strTape += Rule::Chain::grbAlToChar(tape[i]);
		}
		return strTape;
	}
	void Mfst::logDiagnoses()
	{
		for (const MfstDiagnosis& DIAGNOSIS : diagnoses)
		{
			if (DIAGNOSIS.tapePos < 0) {
				continue;
			}

			logInfo(*pLogStream, Error(
				GREIBACH.getRule((uint16_t)DIAGNOSIS.ruleNumber).errorType,	// Приведение безопасно
				pLEX_TABLE->table[DIAGNOSIS.tapePos].pos.line,
				pLEX_TABLE->table[DIAGNOSIS.tapePos].pos.posInLine));
		}
	}
	void Mfst::logRules()
	{
		*pLogStream << "Правила вывода:" << endl;
		MfstState state;
		for (size_t i = 0; i < states.size(); i++)
		{
			state = states._Get_container()[i];			
			trace7(state, GREIBACH.getRule(state.ruleNumber));
		}
	}
	void Mfst::saveDeducation()
	{
		deducation.nSteps = states.size();
		deducation.ruleNumbers.reserve(deducation.nSteps);
		deducation.ruleChainNumbers.reserve(deducation.nSteps);
		for (size_t i = 0; i < deducation.nSteps; i++)
		{
			deducation.ruleNumbers.push_back(states._Get_container()[i].ruleNumber);
			deducation.ruleChainNumbers.push_back(states._Get_container()[i].ruleChainNumber);
		}
	}
}