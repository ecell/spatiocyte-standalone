#ifndef _WRAPPERVARIABLES_H_
#define _WRAPPERVARIABLES_H_

#include "../intermediate/util/Macro.h"

class WrapperVariables{
public:
	WrapperVariables(std::string varName,double varValue);

	void print(int tabNum);

	double getValue();
	std::string getName();
protected:
private:
	std::string name;
	double value;
};

#endif

