#include "Utils.h"
#include <vector>
#include <sstream>
#include <iomanip>

std::string trim(const std::string& input)
{
	size_t first = input.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return input;
	}
	size_t last = input.find_last_not_of(' ');
	return input.substr(first, (last - first + 1));
}

std::string toLowerFromUpper(std::string param)
{
	std::string changedString= param;
	for (size_t i = 0; i < param.size(); i++) {
		if (isupper(param[i]))
			changedString[i] = tolower(param[i]);
	}
	return changedString;
	
}


std::vector<std::string> outputBytes(int content, bool isBackwards, int numBytes) 
{	
	std::vector<std::string> vectorOfStrings;
	unsigned char *myBytePtr = (unsigned char *)&content;
	std::ostringstream out;
	if (isBackwards) {
		for (int i = 0; i < numBytes; i++) {

			out << "0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (unsigned)(myBytePtr[numBytes-i-1]);
			vectorOfStrings.push_back(out.str());

			out.str("");

		}
	}
	else {
		for (int i = 0; i < numBytes; i++) {
			out << "0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase<< (unsigned)(myBytePtr[i]);

			vectorOfStrings.push_back(out.str());

			out.str("");
		}
	}
	return vectorOfStrings;
}