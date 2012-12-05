#ifndef _WRAPPERPROCESS_H_
#define _WRAPPERPROCESS_H_

#include "WrapperVariables.h"
#include "../intermediate/util/Macro.h"
#include "../../ecell3Module/libecs/Process.hpp"

class WrapperProcess{
public:
	WrapperProcess(const std::string procID,const std::string procName);
	~WrapperProcess();

	void addVariableReference(std::string varName,double varValue);

	void addProperty(std::string propertyName,double propertyValue);

	void setFileName(std::string filename);

	/**
	 * Get Length of variableReferenceList
	 */
	int getVariableReferenceListLength();

	WrapperVariables *getVariableReferenceAt(int index);

	int getPropertyListLength();
	WrapperVariables *getPropertyAt(int index);

	std::string getProcessName();
	std::string getProcessID();

	std::string getFileName();

	void print();

protected:
private:
	std::string processId;
	std::string processName;

	//
	//	Note:variableReferenceList only describes names of Variable
	//	variableReferenceList can't resolve address of Variable...
	//
	std::vector<WrapperVariables*> variableReferenceList;
	std::vector<WrapperVariables*> propertyList;

	std::string fileName;

};
#endif

