#include <fstream>
#include <string>
#include <iostream>
#include "FirstPass.h"
#include "Context.h"
#include "AbstractExpression.h"
#include "SecondPass.h"
#include "Utils.h"

class InstructionDescriptor;
class ProcessorDescriptor;

const std::string tooLittleArguments = "Too little arguments!";

int main(int argc, char *argv[])
{

	if (argc < 3) std::cout<< tooLittleArguments;

	Context context;

	std::ifstream myfile(argv[1]);

	if (myfile.is_open())
	{
		FirstPass firstPass(context, myfile);
		firstPass.doPass();
		firstPass.calculateSymbolValues();

		myfile.close();
	}

	std::ofstream outputFile(argv[2], std::ofstream::out);

	if (outputFile.is_open()) {
		SecondPass secondPass(context, outputFile);
		secondPass.doPass();
		outputFile.close();
	}

	return 0;


}
