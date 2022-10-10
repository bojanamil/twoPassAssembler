## Description

In the first pass (`FirstPass.cpp`) information was collected about symbols and segments from the input file, and storing them in the table of symbols. Information about symbol and segment is stored in classes Symbol and Segment respectively.  By storing this data, it was avoided to parse the input file multiple times, and the input file is parsed only once.

In the step in between, that is represented by the function `calculateSymbolValues()` in the class `FirstPass` symbol values were calculated.

In the second pass (`SecondPass.cpp`), symbol table, and also the reallocation table and segment content was written.

Support for uppercase and lowercase letters is supported. Names of instructions and registers can be written in uppercase or in lowercase letters, as well as directives `end`, `global`, `org`, segment names, and also data size tags (`DD` and `dd`).

Also, in the class `AbstractExpression.cpp` there are functions that parse various expressions.

Exception handling for most of the unexpected situations was implemented.

## Compiling and running

Program is compiled with g++ compiler on a Linux machine with the following command:

`g++ -std=c++0x -o asembler *.cpp`

Program is executed by passing two arguments: first argument is the path to the input file, and the second argument is the desired path of the output file, for example:

`./asembler ./test1.txt ./izlaz.txt`

