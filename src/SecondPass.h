#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include "Context.h"
#include <iostream>

class SecondPass {

	Context &context;
	std::ostream &outputFile;
	
public:
	SecondPass(Context &_context, std::ostream &_outputFile);

	void setNumbersInSymbolTable();
	void outputSymbolTable();
	void outputReallocations(std::shared_ptr<Segment> segment);
	void outputSegmentContent(std::shared_ptr<Segment> currentSegment);

	void doPass();


};

#endif