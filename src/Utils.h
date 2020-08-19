#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

std::string trim(const std::string& input);

std::string toLowerFromUpper(std::string param);


std::vector<std::string> outputBytes(int content, bool isBackwards, int numBytes);

#endif