#ifndef _REGISTER_DESCRIPTOR_H
#define _REGISTER_DESCRIPTOR_H

#include <string>


class RegisterDescriptor {
public:
	enum OPCODE { r0 = 0x0, r1, r2, r3, r4, r5, r6, r7, r8,r9, r10, r11, r12, r13, r14, r15, sp=0x10, pc=0x11};
private:
	std::string m_nameOfRegister;
	OPCODE m_registerId;
public:
	RegisterDescriptor(std::string _nameOfRegister, OPCODE _opcode): m_nameOfRegister (_nameOfRegister), m_registerId (_opcode) {};
	~RegisterDescriptor() {}
	std::string getNameOfRegister() const { return m_nameOfRegister; };
	OPCODE getOpcode() const { return m_registerId; };
};

#endif