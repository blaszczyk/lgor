#ifndef __LGOR_PARSELINE__
#define __LGOR_PARSELINE__



#include <vector>
#include <string>

/* ****************************************************************************************************************************************

PARSELINE CLASS

**************************************************************************************************************************************** */

class parseline
{
	public:
		std::vector<char> line;
		unsigned int pointer;

		parseline(std::string);
		parseline();

		void writeLine(std::string);

		bool gotoInt();
		bool gotoSignedInt();
		bool gotoLetter();
		bool gotoLetterDigit();
		bool gotoChar(std::string);
		bool Skip(int);
		bool jumpTo(unsigned int);
		bool eof();
		char here();

		unsigned int getInt();
		int getSignedInt();
		unsigned int getTime();
		std::string getString(std::string);
};

#endif 
