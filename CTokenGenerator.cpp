/******************************************************************************************************************************
File      CTokenGenerator.cpp
Author    Chris De Pasquale
Created   Oct 2017
Notes     Implementation of unique token generator
******************************************************************************************************************************/

#include "CTokenGenerator.h"
#include <cstring>
 
bool CTokenGenerator::NextNumber(int& n) {
	// Linear congruent generator
	n = (m_iLastGeneratedNumber + m_iIncrement) % 97335; 

	// Fail on start of full cycle
	if (m_bCycleFail && n == m_iInitialGeneratedNumber) {
		return false;
	}

	m_iLastGeneratedNumber = n;
	return true;
}

bool CTokenGenerator::GetTokenFromNumber(int iTokenNumber, char*& pToken) {
	if (!pToken || iTokenNumber < 0 || iTokenNumber >= m_iModulo)
		return false;



	for (int i = 0; i < m_iTokenLength; i++) {
		pToken[i] = m_szAlphabet[ (int)(iTokenNumber / IntPow(m_iCharListLength, i)) % m_iCharListLength ];
	}
	return true;
}

CTokenGenerator::CTokenGenerator(int iSeed, bool bCycleFail) {
	m_bCycleFail = bCycleFail;
	m_iTokenLength = SERVER_TOKEN_LENGTH;
	m_iCharListLength = strlen(TOKEN_CHARS_ALLOWED);
	m_iIncrement = TOKEN_INCREMENT;

	// Number of different permutations
	m_iModulo = IntPow(m_iCharListLength, m_iTokenLength) - 1;

	// Seed the PRNG
	m_iLastGeneratedNumber = (iSeed < 0 ? -1*iSeed : iSeed) % m_iModulo;

	// First number
	m_iInitialGeneratedNumber = (m_iLastGeneratedNumber + m_iIncrement) % 97335;	
}

bool CTokenGenerator::GetNextToken(char*& token) {
	int n;

	if (!NextNumber(n)) { 
		return false;
	}

	return GetTokenFromNumber(n, token);
}
