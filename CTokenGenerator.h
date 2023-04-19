/******************************************************************************************************************************
File      CTokenGenerator.h
Author    Chris De Pasquale
Created   Oct 2017
Notes     Header for unique token generator, contains implementation notes. 
 !!__ This isn't finished & seems to have a bug causing duplicate tokens before the end of the cycle __!!
******************************************************************************************************************************/
/*	
	Server Token Generator
	
	Christopher De Pasquale, 2nd Oct 2017
	Generates pseudorandom, unique tokens over a very long period.
	Can be set to fail after a full cycle is completed if required.
	However, this is unlikely for default token length and alphabet, 
	as there are 110,591 tokens in the cycle. The alphabet used for 
	tokens has all vowels and vowel-resembling letters removed to 
	avoid accidentally generating offensive words.
	Uses a Linear Congruent Generator as a PRNG, as it is:
	 - simple	- easy to implement
	 - fast		- a full cycle PRNG
	A random number is chosen and converted to a token using the formula
		char_at_index(number, index) = floor( number / L^index ) % L
	Where:
		number is the random number
		index is the current character index, in range 0 to (Num Token Letters-1)
		L is the length of the alphabet used for token generation
	Usage:
		Create an instance of the token generator
			m_Generator = new CTokenGenerator( rand() );
		Create a buffer for the token
			int tokenLength = m_Generator.GetTokenLength();
			char* pToken = malloc(tokenLength+1);
			// add null terminator
			pToken[tokenLength] = 0;
		Generator a token
			if (m_Generator.GetNextToken(pToken)) {
				// Profit!!
			}
*/
#ifndef __CTOKENGENERATOR_H__
#define __CTOKENGENERATOR_H__

// Length of each token, in chars
#define SERVER_TOKEN_LENGTH 3

// Available token chars, no vowels to avoid 'bad' words (or any words) being generated
// Length 48, generator modulo = 48^3-1 = 110591
#define TOKEN_CHARS_ALLOWED "23456789bcdfghjklnmnpqrstvwxyzBCDFGHJKLMNPQRTWXY"

// Any number coprime with (Num_Chars ^tokenLength)-1 (e.g. 110591 above)
// Must also be smaller than the above value, and greater than 1
#define TOKEN_INCREMENT 883

class CTokenGenerator {
	// Length of the token, in characters 
	int m_iTokenLength = SERVER_TOKEN_LENGTH; 

	// previous, first token numbers generated 
	int m_iLastGeneratedNumber, m_iInitialGeneratedNumber;

	// Number of different allowed token characters 
	int m_iCharListLength; 

	// prime number to offset PRNG, must be both coprime with and smaller than modulo 
	int m_iIncrement = TOKEN_INCREMENT;

	// Maximum value returned by token number generator = number of unique tokens - 1 
	int m_iModulo;

	// Characters used in token
	const char* m_szAlphabet = TOKEN_CHARS_ALLOWED;

	// Fail on full cycle
	bool m_bCycleFail;

	// Each token is a list of characters, from index 0 to index 
	// f(i) = floor( n / L^i ) % L
	// Returns false on invalid input parameters
	bool GetTokenFromNumber(int iTokenNumber, char*& pToken);

	// Select the next pseudorandom number in the sequence
	// Xn+1 = (Xn + Increment) % modulo
	bool NextNumber(int& number);

	// Disable default constructor
	CTokenGenerator() {}

	// Calc integer n^p, p > 0
	static int IntPow(int n, int p) {
		int r = 1;
		
		for (int i = 0; i < p; i++) {
			r *= n;
		}
		return r;
	}

public:

	// Construct generator using a seed
	// if bCycleFail == true, token generator will fail when a 
	// number is reused, and must be explicitly reset
	CTokenGenerator(int iSeed, bool bCycleFail = false);

	// Fills the supplied array with a token
	// Returns boolean indicating success. Failure 
	// indicates all tokens have been used, and reset
	// must be called. occurs on bAutoReset = false.
	bool GetNextToken(char*& __token_out);

	// Reset generator to initial state
	void ResetGenerator();

	// Reset generator to a new, random state
	void ResetGenerator(int iSeed);

	// Get the length of each token, in characters 
	int GetTokenLength() { return m_iTokenLength; }
};



#endif
