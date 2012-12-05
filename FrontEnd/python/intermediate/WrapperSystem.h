#ifndef _WRAPPERSYSTEM_H_
#define _WRAPPERSYSTEM_H_

#include "WrapperVariables.h"
#include "WrapperProcess.h"
#include "../intermediate/util/Macro.h"


class WrapperSystem{
public:
	WrapperSystem(const std::string sysId,const std::string propertyName);
	~WrapperSystem();

	void addVariable(std::string varName,double varValue);
	void addProcess(WrapperProcess *process);

	std::string getSystemId();

	/**
	 * Get Length of variableList
	 */
	int getVariableLength();

	/**
	 * Get Length of processList
	 */
	int getProcessLength();

	/**
	 * Get variable
	 */
	WrapperVariables *getVariableAt(int index);

	/**
	 * Get process
	 */
	WrapperProcess *getProcessAt(int index);

	/**
	 * Get Process from ID
	 */
	WrapperProcess *getProcessFromId(std::string Id);

	void print();

private:
	std::string systemId;
	std::string systemName;

	std::vector<WrapperVariables*> variableList;
	std::vector<WrapperProcess*> processList;

};

#endif

