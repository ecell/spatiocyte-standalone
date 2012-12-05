#include "WrapperVariables.h"


WrapperVariables::WrapperVariables(std::string varName,double varValue)
{
	name = varName;
	value = varValue;
}


double WrapperVariables::getValue()
{
	return value;
}

std::string WrapperVariables::getName()
{
	return name;
}

void WrapperVariables::print(int tabNum){
	for(int n=0;n<tabNum;n++) {
		cout << "\t" ;
	}
	cout << "VARIABLE NAME = " << name << " , VALUE = " << value << endl;
}

