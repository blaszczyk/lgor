#include "Parseline.h"
#include <iostream>

using namespace std;

parseline::parseline(string input) : line(input.begin(),input.end()), pointer(0)
{
}

parseline::parseline() : pointer(0)
{
}

void parseline::writeLine(string input)
{
	pointer = 0;
	line.clear();
	for( unsigned int i = 0 ; i < input.length(); i++)
		line.push_back(input[i]);
} 
 
 
bool parseline::gotoInt()
{
	while(pointer < line.size() )
	{
		if(line[pointer] >= '0' && line[pointer] <= '9')
			return true;
		pointer ++;
	}
	return false;
}

bool parseline::gotoSignedInt()
{
	while(pointer < line.size() )
	{
		if( (line[pointer] >= '0' && line[pointer] <= '9') || line[pointer] == '-')
			return true;
		pointer ++;
	}
	return false;
}

bool parseline::gotoLetter()
{
	while(pointer < line.size() )
	{
		if(line[pointer] >= 'a' && line[pointer] <= 'z')
			return true;
		if(line[pointer] >= 'A' && line[pointer] <= 'Z')
			return true;
		pointer ++;
	}
	return false;
}

bool parseline::gotoChar(string limiter)
{
	unsigned int i;
	while(pointer < line.size() )
	{
		for( i = 0 ; i < limiter.length(); i++)
			if(line[pointer] == limiter[i])
				return true;
		pointer ++;
	}
	return false;
}

bool parseline::gotoLetterDigit()
{
	while(pointer < line.size() )
	{
		if(line[pointer] >= '0' && line[pointer] <= '9')
			return true;
		if(line[pointer] >= 'a' && line[pointer] <= 'z')
			return true;
		if(line[pointer] >= 'A' && line[pointer] <= 'Z')
			return true;
		pointer ++;
	}
	return false;
}

bool parseline::eof()
{
	if(pointer < line.size())
		return false;
	return true;
}

unsigned int parseline::getInt()
{
	unsigned int result = 0;
	while(pointer < line.size() && line[pointer] >= '0' && line[pointer] <= '9')
	{
		result *= 10;
		result += (unsigned int) line[pointer] - '0';
		pointer ++;
	}
	return result;
}

int parseline::getSignedInt()
{
	bool negative = false;
	int result = 0;
	if(line[pointer] == '-')
	{
		negative = true;
		if(!gotoInt())
			return 0;
	}
	while(pointer < line.size() && line[pointer] >= '0' && line[pointer] <= '9')
	{
		result *= 10;
		result += (int) line[pointer] - '0';
		pointer ++;
	}
	if(negative)
		return -result;
	return result;
}

unsigned int parseline::getTime()
{
	unsigned int time = 0;
	time += getInt();
	if(!gotoInt())
		return 0;
	time *= 60;
	time += getInt();
	if(!gotoInt())
		return 0;
	time *= 60;
	time += getInt();
	return time;
}

string parseline::getString(string limiter)
{
	vector<char> output;
	unsigned int i;
	bool engage = true;
	while(engage)
	{
		for( i = 0 ; i < limiter.length(); i++)
			if(line[pointer] == limiter[i])
				engage = false;
		if(pointer >= line.size())
			engage = false;
		if(engage)
		{
			output.push_back(line[pointer]);
			pointer ++;
		}
	}
	string result(output.begin(), output.end());
	return result;
}

bool parseline::Skip(int steps)
{
	int temp = (int)pointer + steps;
	if(temp < 0 || temp >= (int) line.size())
		return false;
	pointer = (unsigned int) temp;
	return true;
}


bool parseline::jumpTo(unsigned int position)
{
	if( position >= line.size() )
		return false;
	pointer = position;
	return true;
}

char parseline::here()
{
	return line[pointer];
}
