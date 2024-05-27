#include "stdafx.h"
#include "FST.hpp"
using namespace std;

namespace
{
	bool step(RAM_2022_Fst::Fst& fst, std::vector<int32_t>& tempStates)
	{
		bool stepped = false;
		std::swap(tempStates, fst.currStates);

		for (short iState = 0; iState < fst.nStates; iState++)
		{
			if (tempStates[iState] == fst.position)
			{
				for (short iRelation = 0; iRelation < fst.nodes->at(iState).nRelations; iRelation++)
				{
					if (fst.nodes->at(iState).relations[iRelation].symbol == fst.chain[fst.position])
					{
						fst.currStates[fst.nodes->at(iState).relations[iRelation].nodeNumber] = fst.position + 1;
						stepped = true;
					}
				}
			}
		}

		return stepped;
	}
}

namespace RAM_2022_Fst
{
	Relation::Relation(char symbol, uint16_t nodeNumber)
	{
		this->symbol = symbol;
		this->nodeNumber = nodeNumber;
	}
	Node::Node() {}
	Fst::Fst() {}

	void Fst::set(string_view newChain)
	{
		reset();
		chain = newChain;
	}
	void Fst::reset()
	{
		currStates.assign(nStates, -1);
		currStates[0] = 0;
		position = 0;
	}
	bool Fst::execute()
	{
		vector<int32_t> tempStates(nStates, -1);

		uint16_t chainLength = (uint16_t)chain.length();
		bool stepped = true;

		for (size_t i = 0; i < chainLength && stepped; i++)
		{
			stepped = step(*this, tempStates);
			position++;
		}

		return stepped ? (currStates[nStates - 1] == chainLength) : stepped;
	}
}