#pragma once
#include "stdafx.h"

namespace RAM_2022_Fst
{
	struct Relation
	{
		char symbol = '\0';
		uint32_t nodeNumber = 0;

		Relation(char symbol = '\0', uint16_t nodeNumber = 0);
	};

	struct Node
	{
		uint16_t nRelations = 0;
		std::vector<Relation> relations;

		Node();
		template<std::same_as<Relation> ...Args>
		explicit Node(const Args&... RELATIONS)
		{
			nRelations = (uint16_t)sizeof...(Args);
			relations.reserve(nRelations);
			for (const Relation& RELATION : { RELATIONS... }) {
				relations.push_back(RELATION);
			}
		}
	};

	struct Fst
	{
		std::string chain = "";
		uint16_t position = 0;
		uint16_t nStates = 0;
		std::shared_ptr<std::vector<Node>> nodes = nullptr;
		std::vector<int32_t> currStates;

		Fst();
		template<std::same_as<Node> ...Args>
		Fst(std::string_view s, const Args&... NODES)
		{
			chain = s;
			nStates = (uint16_t)sizeof...(Args);

			nodes = std::make_shared<std::vector<Node>>();
			nodes->reserve(nStates);
			for (const Node& NODE : { NODES... }) {
				nodes->push_back(NODE);
			}

			currStates.assign(nStates, -1);
			currStates[0] = 0;
		}

		void set(std::string_view newChain);
		void reset();
		bool execute();
	};
}