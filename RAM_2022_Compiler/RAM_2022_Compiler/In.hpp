#pragma once
#include "stdafx.h"
#include "ClMsg.hpp"
#include "LexicalAnalysis.hpp"

namespace RAM_2022_In
{
	// �������:
	// T - ����������
	// F - �����������
	// I - ������������
	// N - ������� �� ����� ������: \n
	// S - �����������: ,;(){}+-*/=
	// P - ����������: <space> <tab>
	// Q - ������� ���������� ��������
	// C - ������������� �����������
	// ������ ������ - ������
	enum class CharacterType : uint32_t
	{
		T = 1 << 8,
		F = 1 << 9,
		I = 1 << 10,
		N = 1 << 11,
		S = 1 << 12,
		P = 1 << 13,
		Q = 1 << 14,
		C = 1 << 15
	};

	const std::array<uint32_t, 256> CODE_TABLE
	{
#define CT(x) (uint32_t)CharacterType::x
	/*	\0		SOH		STX		ETX		EOT		ENQ		ACK		\a		\b		\t		\n		\v		\f		\r		SO		SI		*/
		CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(P),	CT(N),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),
	/*	DLE		DC1		DC2		DC3		DC4		NAK		SYN		ETB		CAN		EM		SUB		ESC		FS		GS		RS		US		*/
		CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	CT(I),	
	/*	 		!		"		#		$		%		&		'		(		)		*		+		,		-		.		/		*/
		CT(P),	CT(S),	CT(Q),	CT(T),	CT(T),	CT(S),	CT(T),	CT(T),	CT(S),	CT(S),	CT(S),	CT(S),	CT(S),	CT(S),	CT(T),	CT(S),	
	/*	0		1		2		3		4		5		6		7		8		9		:		;		<		=		>		?		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(S),	CT(S),	CT(S),	CT(S),	CT(T),	
	/*	@		A		B		C		D		E		F		G		H		I		J		K		L		M		N		O		*/
		CT(C),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	P		Q		R		S		T		U		V		W		X		Y		Z		[		\		]		^		_		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	`		a		b		c		d		e		f		g		h		i		j		k		l		m		n		o		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	p		q		r		s		t		u		v		w		x		y		z		{		|		}		~		DEL		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(S),	CT(T),	CT(S),	CT(S),	CT(I),	

	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		 �		�		*/
		CT(P),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(P),	CT(T),	CT(T),	
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	
	/*	�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		�		*/
		CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T),	CT(T)
#undef CT
	};

	constexpr uint32_t MAX_CODE_LENGTH = 1024 * 1024;	// ������������ ������ ��������� ���� = 1MB
	constexpr unsigned char IN_CODE_ENDL = '\n';

	struct In	// �������� ������
	{
		uint32_t length = 0;
		uint32_t nLines = 1;
		uint32_t nIgnored = 0;
		std::string text = "";

		void readFile(const std::wstring& IN_PATH, std::vector<RAM_2022_LT::WordInfo>& wordTable, RAM_2022_ClMsg::ClMsgArrs& clMsgArrs);
	};
}