#include "globals.h"

// -------------------------------------------------
bool Same(const std::string& a, const std::string& b)
{
	return _stricmp(a.c_str(), b.c_str()) == 0;
}

bool Same(const char* a, const std::string& b)
{
	return _stricmp(a, b.c_str()) == 0;
}

bool Same(const std::string& a, const char* b)
{
	return _stricmp(a.c_str(), b) == 0;
}

int Roll(int min, int max)
{
	return (max > 0) ? min + rand() % ((max + 1) - min) : 0;
}

void Tokenize(const std::string& line, std::vector<std::string>& arguments)
{
	const char* str = line.c_str();

	do
	{
		const char *begin = str;

		while(*str != ' ' && *str)
			str++;

		arguments.push_back(std::string(begin, str));
	} while(0 != *str++);
}

void RunEndGameAnimation()
{
    std::cout <<  "\n\n************************************************************\n"
              <<  "\n           CONGRATULATIONS!!!!!!!\n"
              <<  "\n The alien was push to the outer space!!!\n"
              <<  "\n Now you can go back home, even there's no much crew left...\n"
              <<  "************************************************************\n";
    exit(0);
}
