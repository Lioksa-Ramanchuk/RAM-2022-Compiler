#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"
#include "FST.hpp"
#include "IT.hpp"
#include "LT.hpp"

namespace RAM_2022_LexicalAnalysis
{
	const std::vector<std::pair<RAM_2022_LT::LexemeType, RAM_2022_Fst::Fst>> LEXEM_TYPES_FSTS
	{
		{
			{
				RAM_2022_LT::LexemeType::TYPE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('n', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('t', 3)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::TYPE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('s', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('t', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('r', 3)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::MAIN,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('m', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('a', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('n', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::LINK,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('l', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('n', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('k', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::FUNC,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('f', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('u', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('n', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('c', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::ARITHMETIC_UN_OP,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('#', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('+', 2), RAM_2022_Fst::Relation('-', 2)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::LITERAL,	// string
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('\"', 1)),
					RAM_2022_Fst::Node(
							RAM_2022_Fst::Relation(' ', 1),  RAM_2022_Fst::Relation('!', 1),  RAM_2022_Fst::Relation('\'', 1), RAM_2022_Fst::Relation('#', 1),  RAM_2022_Fst::Relation('$', 1),
							RAM_2022_Fst::Relation('%', 1),  RAM_2022_Fst::Relation('&', 1),  RAM_2022_Fst::Relation('(', 1),  RAM_2022_Fst::Relation(')', 1),  RAM_2022_Fst::Relation('*', 1),  
							RAM_2022_Fst::Relation('+', 1),  RAM_2022_Fst::Relation(',', 1),  RAM_2022_Fst::Relation('-', 1),  RAM_2022_Fst::Relation('.', 1),  RAM_2022_Fst::Relation('/', 1),
							RAM_2022_Fst::Relation(':', 1),  RAM_2022_Fst::Relation(';', 1),  RAM_2022_Fst::Relation('<', 1),  RAM_2022_Fst::Relation('=', 1),  RAM_2022_Fst::Relation('>', 1),
							RAM_2022_Fst::Relation('?', 1),  RAM_2022_Fst::Relation('@', 1),  RAM_2022_Fst::Relation('[', 1),  RAM_2022_Fst::Relation(']', 1),  RAM_2022_Fst::Relation('^', 1),  
							RAM_2022_Fst::Relation('`', 1),  RAM_2022_Fst::Relation('¼', 1),  RAM_2022_Fst::Relation('½', 1),  RAM_2022_Fst::Relation('¾', 1),  RAM_2022_Fst::Relation('¿', 1),
							RAM_2022_Fst::Relation('{', 1),  RAM_2022_Fst::Relation('|', 1),  RAM_2022_Fst::Relation('}', 1),  RAM_2022_Fst::Relation('~', 1),  RAM_2022_Fst::Relation('', 1),  
							RAM_2022_Fst::Relation('€', 1),  RAM_2022_Fst::Relation('', 1),  RAM_2022_Fst::Relation('‚', 1),  RAM_2022_Fst::Relation('ƒ', 1),  RAM_2022_Fst::Relation('„', 1),  
							RAM_2022_Fst::Relation('…', 1),  RAM_2022_Fst::Relation('†', 1),  RAM_2022_Fst::Relation('‡', 1),  RAM_2022_Fst::Relation('ˆ', 1),  RAM_2022_Fst::Relation('‰', 1),
							RAM_2022_Fst::Relation('Š', 1),  RAM_2022_Fst::Relation('‹', 1),  RAM_2022_Fst::Relation('Œ', 1),  RAM_2022_Fst::Relation('', 1),  RAM_2022_Fst::Relation('Ž', 1), 
							RAM_2022_Fst::Relation('', 1),  RAM_2022_Fst::Relation('', 1),  RAM_2022_Fst::Relation('‘', 1),  RAM_2022_Fst::Relation('’', 1),  RAM_2022_Fst::Relation('“', 1), 
							RAM_2022_Fst::Relation('”', 1),  RAM_2022_Fst::Relation('•', 1),  RAM_2022_Fst::Relation('–', 1),  RAM_2022_Fst::Relation('—', 1),  RAM_2022_Fst::Relation('˜', 1), 
							RAM_2022_Fst::Relation('™', 1),  RAM_2022_Fst::Relation('š', 1),  RAM_2022_Fst::Relation('›', 1),  RAM_2022_Fst::Relation('œ', 1),  RAM_2022_Fst::Relation('', 1), 
							RAM_2022_Fst::Relation('ž', 1),  RAM_2022_Fst::Relation('Ÿ', 1),  RAM_2022_Fst::Relation(' ', 1),  RAM_2022_Fst::Relation('¡', 1),  RAM_2022_Fst::Relation('¢', 1),  
							RAM_2022_Fst::Relation('£', 1),  RAM_2022_Fst::Relation('¤', 1),  RAM_2022_Fst::Relation('¥', 1),  RAM_2022_Fst::Relation('¦', 1),  RAM_2022_Fst::Relation('§', 1),
							RAM_2022_Fst::Relation('¨', 1),  RAM_2022_Fst::Relation('©', 1),  RAM_2022_Fst::Relation('ª', 1),  RAM_2022_Fst::Relation('«', 1),  RAM_2022_Fst::Relation('¬', 1),  
							RAM_2022_Fst::Relation('­', 1),   RAM_2022_Fst::Relation('®', 1),  RAM_2022_Fst::Relation('¯', 1),  RAM_2022_Fst::Relation('°', 1),  RAM_2022_Fst::Relation('±', 1), 
							RAM_2022_Fst::Relation('²', 1),  RAM_2022_Fst::Relation('³', 1),  RAM_2022_Fst::Relation('´', 1),  RAM_2022_Fst::Relation('µ', 1),  RAM_2022_Fst::Relation('¶', 1),  
							RAM_2022_Fst::Relation('·', 1),  RAM_2022_Fst::Relation('¸', 1),  RAM_2022_Fst::Relation('¹', 1),  RAM_2022_Fst::Relation('º', 1),  RAM_2022_Fst::Relation('»', 1),
							RAM_2022_Fst::Relation('_', 1),  RAM_2022_Fst::Relation('\\', 1), RAM_2022_Fst::Relation('\"', 1), RAM_2022_Fst::Relation('\n', 1), RAM_2022_Fst::Relation('\t', 1),

							RAM_2022_Fst::Relation('0', 1),  RAM_2022_Fst::Relation('1', 1),  RAM_2022_Fst::Relation('2', 1),  RAM_2022_Fst::Relation('3', 1),  RAM_2022_Fst::Relation('4', 1), 
							RAM_2022_Fst::Relation('5', 1),  RAM_2022_Fst::Relation('6', 1),  RAM_2022_Fst::Relation('7', 1),  RAM_2022_Fst::Relation('8', 1),  RAM_2022_Fst::Relation('9', 1), 

							RAM_2022_Fst::Relation('A', 1),  RAM_2022_Fst::Relation('B', 1),  RAM_2022_Fst::Relation('C', 1),  RAM_2022_Fst::Relation('D', 1),  RAM_2022_Fst::Relation('E', 1),  
							RAM_2022_Fst::Relation('F', 1),  RAM_2022_Fst::Relation('G', 1),  RAM_2022_Fst::Relation('H', 1),  RAM_2022_Fst::Relation('I', 1),  RAM_2022_Fst::Relation('J', 1),  
							RAM_2022_Fst::Relation('K', 1),  RAM_2022_Fst::Relation('L', 1),  RAM_2022_Fst::Relation('M', 1),  RAM_2022_Fst::Relation('N', 1),  RAM_2022_Fst::Relation('O', 1),  
							RAM_2022_Fst::Relation('P', 1),  RAM_2022_Fst::Relation('Q', 1),  RAM_2022_Fst::Relation('R', 1),  RAM_2022_Fst::Relation('S', 1),  RAM_2022_Fst::Relation('T', 1),  
							RAM_2022_Fst::Relation('U', 1),  RAM_2022_Fst::Relation('V', 1),  RAM_2022_Fst::Relation('W', 1),  RAM_2022_Fst::Relation('X', 1),  RAM_2022_Fst::Relation('Y', 1),
							RAM_2022_Fst::Relation('Z', 1),
							
							RAM_2022_Fst::Relation('a', 1),  RAM_2022_Fst::Relation('b', 1),  RAM_2022_Fst::Relation('c', 1),  RAM_2022_Fst::Relation('d', 1),  RAM_2022_Fst::Relation('e', 1), 
							RAM_2022_Fst::Relation('f', 1),  RAM_2022_Fst::Relation('g', 1),  RAM_2022_Fst::Relation('h', 1),  RAM_2022_Fst::Relation('i', 1),  RAM_2022_Fst::Relation('j', 1), 
							RAM_2022_Fst::Relation('k', 1),  RAM_2022_Fst::Relation('l', 1),  RAM_2022_Fst::Relation('m', 1),  RAM_2022_Fst::Relation('n', 1),  RAM_2022_Fst::Relation('o', 1),  
							RAM_2022_Fst::Relation('p', 1),  RAM_2022_Fst::Relation('q', 1),  RAM_2022_Fst::Relation('r', 1),  RAM_2022_Fst::Relation('s', 1),  RAM_2022_Fst::Relation('t', 1), 
							RAM_2022_Fst::Relation('u', 1),  RAM_2022_Fst::Relation('v', 1),  RAM_2022_Fst::Relation('w', 1),  RAM_2022_Fst::Relation('x', 1),  RAM_2022_Fst::Relation('y', 1),
							RAM_2022_Fst::Relation('z', 1),
							
							RAM_2022_Fst::Relation('À', 1),  RAM_2022_Fst::Relation('Á', 1),  RAM_2022_Fst::Relation('Â', 1),  RAM_2022_Fst::Relation('Ã', 1),  RAM_2022_Fst::Relation('Ä', 1),
							RAM_2022_Fst::Relation('Å', 1),  RAM_2022_Fst::Relation('Æ', 1),  RAM_2022_Fst::Relation('Ç', 1),  RAM_2022_Fst::Relation('È', 1),  RAM_2022_Fst::Relation('É', 1),
							RAM_2022_Fst::Relation('Ê', 1),  RAM_2022_Fst::Relation('Ë', 1),  RAM_2022_Fst::Relation('Ì', 1),  RAM_2022_Fst::Relation('Í', 1),  RAM_2022_Fst::Relation('Î', 1),
							RAM_2022_Fst::Relation('Ï', 1),  RAM_2022_Fst::Relation('Ð', 1),  RAM_2022_Fst::Relation('Ñ', 1),  RAM_2022_Fst::Relation('Ò', 1),  RAM_2022_Fst::Relation('Ó', 1),
							RAM_2022_Fst::Relation('Ô', 1),  RAM_2022_Fst::Relation('Õ', 1),  RAM_2022_Fst::Relation('Ö', 1),  RAM_2022_Fst::Relation('×', 1),  RAM_2022_Fst::Relation('Ø', 1),
							RAM_2022_Fst::Relation('Ù', 1),  RAM_2022_Fst::Relation('Ú', 1),  RAM_2022_Fst::Relation('Û', 1),  RAM_2022_Fst::Relation('Ü', 1),  RAM_2022_Fst::Relation('Ý', 1),
							RAM_2022_Fst::Relation('Þ', 1),  RAM_2022_Fst::Relation('ß', 1),
							
							RAM_2022_Fst::Relation('à', 1),  RAM_2022_Fst::Relation('á', 1),  RAM_2022_Fst::Relation('â', 1),  RAM_2022_Fst::Relation('ã', 1),  RAM_2022_Fst::Relation('ä', 1),  
							RAM_2022_Fst::Relation('å', 1),  RAM_2022_Fst::Relation('æ', 1),  RAM_2022_Fst::Relation('ç', 1),  RAM_2022_Fst::Relation('è', 1),  RAM_2022_Fst::Relation('é', 1),  
							RAM_2022_Fst::Relation('ê', 1),  RAM_2022_Fst::Relation('ë', 1),  RAM_2022_Fst::Relation('ì', 1),  RAM_2022_Fst::Relation('í', 1),  RAM_2022_Fst::Relation('î', 1),  
							RAM_2022_Fst::Relation('ï', 1),  RAM_2022_Fst::Relation('ð', 1),  RAM_2022_Fst::Relation('ñ', 1),  RAM_2022_Fst::Relation('ò', 1),  RAM_2022_Fst::Relation('ó', 1),  
							RAM_2022_Fst::Relation('ô', 1),  RAM_2022_Fst::Relation('õ', 1),  RAM_2022_Fst::Relation('ö', 1),  RAM_2022_Fst::Relation('÷', 1),  RAM_2022_Fst::Relation('ø', 1),  
							RAM_2022_Fst::Relation('ù', 1),  RAM_2022_Fst::Relation('ú', 1),  RAM_2022_Fst::Relation('û', 1),  RAM_2022_Fst::Relation('ü', 1),  RAM_2022_Fst::Relation('ý', 1),  
							RAM_2022_Fst::Relation('þ', 1),  RAM_2022_Fst::Relation('ÿ', 1),

							RAM_2022_Fst::Relation('\"', 2)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::LITERAL,	// dec integer
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('+', 1), RAM_2022_Fst::Relation('-', 1),
						RAM_2022_Fst::Relation('0', 1), RAM_2022_Fst::Relation('1', 1), RAM_2022_Fst::Relation('2', 1), RAM_2022_Fst::Relation('3', 1), RAM_2022_Fst::Relation('4', 1),
						RAM_2022_Fst::Relation('5', 1), RAM_2022_Fst::Relation('6', 1), RAM_2022_Fst::Relation('7', 1), RAM_2022_Fst::Relation('8', 1), RAM_2022_Fst::Relation('9', 1),
						RAM_2022_Fst::Relation('0', 2), RAM_2022_Fst::Relation('1', 2), RAM_2022_Fst::Relation('2', 2), RAM_2022_Fst::Relation('3', 2), RAM_2022_Fst::Relation('4', 2),
						RAM_2022_Fst::Relation('5', 2), RAM_2022_Fst::Relation('6', 2), RAM_2022_Fst::Relation('7', 2), RAM_2022_Fst::Relation('8', 2), RAM_2022_Fst::Relation('9', 2)),
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('0', 1), RAM_2022_Fst::Relation('1', 1), RAM_2022_Fst::Relation('2', 1), RAM_2022_Fst::Relation('3', 1), RAM_2022_Fst::Relation('4', 1),
						RAM_2022_Fst::Relation('5', 1), RAM_2022_Fst::Relation('6', 1), RAM_2022_Fst::Relation('7', 1), RAM_2022_Fst::Relation('8', 1), RAM_2022_Fst::Relation('9', 1),
						RAM_2022_Fst::Relation('0', 2), RAM_2022_Fst::Relation('1', 2), RAM_2022_Fst::Relation('2', 2), RAM_2022_Fst::Relation('3', 2), RAM_2022_Fst::Relation('4', 2),
						RAM_2022_Fst::Relation('5', 2), RAM_2022_Fst::Relation('6', 2), RAM_2022_Fst::Relation('7', 2), RAM_2022_Fst::Relation('8', 2), RAM_2022_Fst::Relation('9', 2)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::LITERAL,	// oct integer
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('+', 1), RAM_2022_Fst::Relation('-', 1),
						RAM_2022_Fst::Relation('0', 1), RAM_2022_Fst::Relation('1', 1), RAM_2022_Fst::Relation('2', 1), RAM_2022_Fst::Relation('3', 1),
						RAM_2022_Fst::Relation('4', 1), RAM_2022_Fst::Relation('5', 1), RAM_2022_Fst::Relation('6', 1), RAM_2022_Fst::Relation('7', 1),
						RAM_2022_Fst::Relation('0', 2), RAM_2022_Fst::Relation('1', 2), RAM_2022_Fst::Relation('2', 2), RAM_2022_Fst::Relation('3', 2),
						RAM_2022_Fst::Relation('4', 2), RAM_2022_Fst::Relation('5', 2), RAM_2022_Fst::Relation('6', 2), RAM_2022_Fst::Relation('7', 2)),
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('0', 1), RAM_2022_Fst::Relation('1', 1), RAM_2022_Fst::Relation('2', 1), RAM_2022_Fst::Relation('3', 1),
						RAM_2022_Fst::Relation('4', 1), RAM_2022_Fst::Relation('5', 1), RAM_2022_Fst::Relation('6', 1), RAM_2022_Fst::Relation('7', 1),
						RAM_2022_Fst::Relation('0', 2), RAM_2022_Fst::Relation('1', 2), RAM_2022_Fst::Relation('2', 2), RAM_2022_Fst::Relation('3', 2),
						RAM_2022_Fst::Relation('4', 2), RAM_2022_Fst::Relation('5', 2), RAM_2022_Fst::Relation('6', 2), RAM_2022_Fst::Relation('7', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('o', 3)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::PROC,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('p', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('r', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('o', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('c', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::RETURN,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('r', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('t', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('u', 4)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('r', 5)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('n', 6)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::WRITE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('w', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('r', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('t', 4)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 5)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::READ,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('r', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('a', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('d', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::IF,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('f', 2)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::ELSE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('l', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('s', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::ELIF,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('l', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('f', 4)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::WHILE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('w', 1)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('h', 2)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('i', 3)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('l', 4)),
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('e', 5)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::SEMICOLON,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation(';', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::COMMA,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation(',', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::ASSIGN,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('=', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::LEFT_BRACE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('{', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::RIGHT_BRACE,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('}', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::LEFT_PARENTHESIS,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation('(', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::RIGHT_PARENTHESIS,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(RAM_2022_Fst::Relation(')', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::ARITHMETIC_BIN_OP,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('+', 1), RAM_2022_Fst::Relation('-', 1),
						RAM_2022_Fst::Relation('*', 1), RAM_2022_Fst::Relation('/', 1), RAM_2022_Fst::Relation('%', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::CMP_BIN_OP,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('>', 1), RAM_2022_Fst::Relation('<', 1),
						RAM_2022_Fst::Relation('~', 1), RAM_2022_Fst::Relation('!', 1)),
					RAM_2022_Fst::Node()
				}
			},
			{
				RAM_2022_LT::LexemeType::ID,
				RAM_2022_Fst::Fst
				{
					{""},
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('a', 1), RAM_2022_Fst::Relation('b', 1), RAM_2022_Fst::Relation('c', 1), RAM_2022_Fst::Relation('d', 1), RAM_2022_Fst::Relation('e', 1),
						RAM_2022_Fst::Relation('f', 1), RAM_2022_Fst::Relation('g', 1), RAM_2022_Fst::Relation('h', 1), RAM_2022_Fst::Relation('i', 1), RAM_2022_Fst::Relation('j', 1),
						RAM_2022_Fst::Relation('k', 1), RAM_2022_Fst::Relation('l', 1), RAM_2022_Fst::Relation('m', 1), RAM_2022_Fst::Relation('n', 1), RAM_2022_Fst::Relation('o', 1),
						RAM_2022_Fst::Relation('p', 1), RAM_2022_Fst::Relation('q', 1), RAM_2022_Fst::Relation('r', 1), RAM_2022_Fst::Relation('s', 1), RAM_2022_Fst::Relation('t', 1),
						RAM_2022_Fst::Relation('u', 1), RAM_2022_Fst::Relation('v', 1), RAM_2022_Fst::Relation('w', 1), RAM_2022_Fst::Relation('x', 1), RAM_2022_Fst::Relation('y', 1), RAM_2022_Fst::Relation('z', 1),
						RAM_2022_Fst::Relation('A', 1), RAM_2022_Fst::Relation('B', 1), RAM_2022_Fst::Relation('C', 1), RAM_2022_Fst::Relation('D', 1), RAM_2022_Fst::Relation('E', 1),
						RAM_2022_Fst::Relation('F', 1), RAM_2022_Fst::Relation('G', 1), RAM_2022_Fst::Relation('H', 1), RAM_2022_Fst::Relation('I', 1), RAM_2022_Fst::Relation('J', 1),
						RAM_2022_Fst::Relation('K', 1), RAM_2022_Fst::Relation('L', 1), RAM_2022_Fst::Relation('M', 1), RAM_2022_Fst::Relation('N', 1), RAM_2022_Fst::Relation('O', 1),
						RAM_2022_Fst::Relation('P', 1), RAM_2022_Fst::Relation('Q', 1), RAM_2022_Fst::Relation('R', 1), RAM_2022_Fst::Relation('S', 1), RAM_2022_Fst::Relation('T', 1),
						RAM_2022_Fst::Relation('U', 1), RAM_2022_Fst::Relation('V', 1), RAM_2022_Fst::Relation('W', 1), RAM_2022_Fst::Relation('X', 1), RAM_2022_Fst::Relation('Y', 1), RAM_2022_Fst::Relation('Z', 1),
						RAM_2022_Fst::Relation('a', 2), RAM_2022_Fst::Relation('b', 2), RAM_2022_Fst::Relation('c', 2), RAM_2022_Fst::Relation('d', 2), RAM_2022_Fst::Relation('e', 2),
						RAM_2022_Fst::Relation('f', 2), RAM_2022_Fst::Relation('g', 2), RAM_2022_Fst::Relation('h', 2), RAM_2022_Fst::Relation('i', 2), RAM_2022_Fst::Relation('j', 2),
						RAM_2022_Fst::Relation('k', 2), RAM_2022_Fst::Relation('l', 2), RAM_2022_Fst::Relation('m', 2), RAM_2022_Fst::Relation('n', 2), RAM_2022_Fst::Relation('o', 2),
						RAM_2022_Fst::Relation('p', 2), RAM_2022_Fst::Relation('q', 2), RAM_2022_Fst::Relation('r', 2), RAM_2022_Fst::Relation('s', 2), RAM_2022_Fst::Relation('t', 2),
						RAM_2022_Fst::Relation('u', 2), RAM_2022_Fst::Relation('v', 2), RAM_2022_Fst::Relation('w', 2), RAM_2022_Fst::Relation('x', 2), RAM_2022_Fst::Relation('y', 2), RAM_2022_Fst::Relation('z', 1),
						RAM_2022_Fst::Relation('A', 2), RAM_2022_Fst::Relation('B', 2), RAM_2022_Fst::Relation('C', 2), RAM_2022_Fst::Relation('D', 2), RAM_2022_Fst::Relation('E', 2),
						RAM_2022_Fst::Relation('F', 2), RAM_2022_Fst::Relation('G', 2), RAM_2022_Fst::Relation('H', 2), RAM_2022_Fst::Relation('I', 2), RAM_2022_Fst::Relation('J', 2),
						RAM_2022_Fst::Relation('K', 2), RAM_2022_Fst::Relation('L', 2), RAM_2022_Fst::Relation('M', 2), RAM_2022_Fst::Relation('N', 2), RAM_2022_Fst::Relation('O', 2),
						RAM_2022_Fst::Relation('P', 2), RAM_2022_Fst::Relation('Q', 2), RAM_2022_Fst::Relation('R', 2), RAM_2022_Fst::Relation('S', 2), RAM_2022_Fst::Relation('T', 2),
						RAM_2022_Fst::Relation('U', 2), RAM_2022_Fst::Relation('V', 2), RAM_2022_Fst::Relation('W', 2), RAM_2022_Fst::Relation('X', 2), RAM_2022_Fst::Relation('Y', 2), RAM_2022_Fst::Relation('Z', 2)),
					RAM_2022_Fst::Node(
						RAM_2022_Fst::Relation('0', 1), RAM_2022_Fst::Relation('1', 1), RAM_2022_Fst::Relation('2', 1), RAM_2022_Fst::Relation('3', 1), RAM_2022_Fst::Relation('4', 1),
						RAM_2022_Fst::Relation('5', 1), RAM_2022_Fst::Relation('6', 1), RAM_2022_Fst::Relation('7', 1), RAM_2022_Fst::Relation('8', 1), RAM_2022_Fst::Relation('9', 1),
						RAM_2022_Fst::Relation('a', 1), RAM_2022_Fst::Relation('b', 1), RAM_2022_Fst::Relation('c', 1), RAM_2022_Fst::Relation('d', 1), RAM_2022_Fst::Relation('e', 1),
						RAM_2022_Fst::Relation('f', 1), RAM_2022_Fst::Relation('g', 1), RAM_2022_Fst::Relation('h', 1), RAM_2022_Fst::Relation('i', 1), RAM_2022_Fst::Relation('j', 1),
						RAM_2022_Fst::Relation('k', 1), RAM_2022_Fst::Relation('l', 1), RAM_2022_Fst::Relation('m', 1), RAM_2022_Fst::Relation('n', 1), RAM_2022_Fst::Relation('o', 1),
						RAM_2022_Fst::Relation('p', 1), RAM_2022_Fst::Relation('q', 1), RAM_2022_Fst::Relation('r', 1), RAM_2022_Fst::Relation('s', 1), RAM_2022_Fst::Relation('t', 1),
						RAM_2022_Fst::Relation('u', 1), RAM_2022_Fst::Relation('v', 1), RAM_2022_Fst::Relation('w', 1), RAM_2022_Fst::Relation('x', 1), RAM_2022_Fst::Relation('y', 1), RAM_2022_Fst::Relation('z', 1),
						RAM_2022_Fst::Relation('A', 1), RAM_2022_Fst::Relation('B', 1), RAM_2022_Fst::Relation('C', 1), RAM_2022_Fst::Relation('D', 1), RAM_2022_Fst::Relation('E', 1),
						RAM_2022_Fst::Relation('F', 1), RAM_2022_Fst::Relation('G', 1), RAM_2022_Fst::Relation('H', 1), RAM_2022_Fst::Relation('I', 1), RAM_2022_Fst::Relation('J', 1),
						RAM_2022_Fst::Relation('K', 1), RAM_2022_Fst::Relation('L', 1), RAM_2022_Fst::Relation('M', 1), RAM_2022_Fst::Relation('N', 1), RAM_2022_Fst::Relation('O', 1),
						RAM_2022_Fst::Relation('P', 1), RAM_2022_Fst::Relation('Q', 1), RAM_2022_Fst::Relation('R', 1), RAM_2022_Fst::Relation('S', 1), RAM_2022_Fst::Relation('T', 1),
						RAM_2022_Fst::Relation('U', 1), RAM_2022_Fst::Relation('V', 1), RAM_2022_Fst::Relation('W', 1), RAM_2022_Fst::Relation('X', 1), RAM_2022_Fst::Relation('Y', 1), RAM_2022_Fst::Relation('Z', 1),
						RAM_2022_Fst::Relation('0', 2), RAM_2022_Fst::Relation('1', 2), RAM_2022_Fst::Relation('2', 2), RAM_2022_Fst::Relation('3', 2), RAM_2022_Fst::Relation('4', 2),
						RAM_2022_Fst::Relation('5', 2), RAM_2022_Fst::Relation('6', 2), RAM_2022_Fst::Relation('7', 2), RAM_2022_Fst::Relation('8', 2), RAM_2022_Fst::Relation('9', 2),
						RAM_2022_Fst::Relation('a', 2), RAM_2022_Fst::Relation('b', 2), RAM_2022_Fst::Relation('c', 2), RAM_2022_Fst::Relation('d', 2), RAM_2022_Fst::Relation('e', 2),
						RAM_2022_Fst::Relation('f', 2), RAM_2022_Fst::Relation('g', 2), RAM_2022_Fst::Relation('h', 2), RAM_2022_Fst::Relation('i', 2), RAM_2022_Fst::Relation('j', 2),
						RAM_2022_Fst::Relation('k', 2), RAM_2022_Fst::Relation('l', 2), RAM_2022_Fst::Relation('m', 2), RAM_2022_Fst::Relation('n', 2), RAM_2022_Fst::Relation('o', 2),
						RAM_2022_Fst::Relation('p', 2), RAM_2022_Fst::Relation('q', 2), RAM_2022_Fst::Relation('r', 2), RAM_2022_Fst::Relation('s', 2), RAM_2022_Fst::Relation('t', 2),
						RAM_2022_Fst::Relation('u', 2), RAM_2022_Fst::Relation('v', 2), RAM_2022_Fst::Relation('w', 2), RAM_2022_Fst::Relation('x', 2), RAM_2022_Fst::Relation('y', 2), RAM_2022_Fst::Relation('z', 1),
						RAM_2022_Fst::Relation('A', 2), RAM_2022_Fst::Relation('B', 2), RAM_2022_Fst::Relation('C', 2), RAM_2022_Fst::Relation('D', 2), RAM_2022_Fst::Relation('E', 2),
						RAM_2022_Fst::Relation('F', 2), RAM_2022_Fst::Relation('G', 2), RAM_2022_Fst::Relation('H', 2), RAM_2022_Fst::Relation('I', 2), RAM_2022_Fst::Relation('J', 2),
						RAM_2022_Fst::Relation('K', 2), RAM_2022_Fst::Relation('L', 2), RAM_2022_Fst::Relation('M', 2), RAM_2022_Fst::Relation('N', 2), RAM_2022_Fst::Relation('O', 2),
						RAM_2022_Fst::Relation('P', 2), RAM_2022_Fst::Relation('Q', 2), RAM_2022_Fst::Relation('R', 2), RAM_2022_Fst::Relation('S', 2), RAM_2022_Fst::Relation('T', 2),
						RAM_2022_Fst::Relation('U', 2), RAM_2022_Fst::Relation('V', 2), RAM_2022_Fst::Relation('W', 2), RAM_2022_Fst::Relation('X', 2), RAM_2022_Fst::Relation('Y', 2), RAM_2022_Fst::Relation('Z', 2)),
					RAM_2022_Fst::Node()
				}
			},
		}
	};
	
	struct Lexer
	{
		std::vector<RAM_2022_LT::WordInfo> wordTable;
		RAM_2022_LT::LexTable lexTable;
		RAM_2022_IT::IdTable idTable;
		std::map<std::string, std::vector<RAM_2022_IT::IdDataType>, std::less<>> funcsParamTypes;

		void fillLexTableAndIdTable(RAM_2022_ClMsg::ClMsgArrs& clMsgArrs);
		bool isExternFuncDeclaration(uint16_t posIDInLT) const;
		bool isFuncDeclaration(uint16_t posIDInLT) const;
		bool isParamDeclaration(uint16_t posIDInLT) const;
		void printTables(std::ostream& stream) const;
	};
}