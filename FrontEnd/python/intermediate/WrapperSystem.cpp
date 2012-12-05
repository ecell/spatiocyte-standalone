#include "WrapperSystem.h"


WrapperSystem::WrapperSystem(const std::string sysId,const std::string propertyName)
{
	systemId = sysId;
	systemName = propertyName;
}

WrapperSystem::~WrapperSystem(){

	TRACE0("\tRemove WrapperSytem#variableList...");
	int variableLength = getVariableLength();
	for(int n=variableLength-1;n>=0;n--) {
		WrapperVariables *val = (WrapperVariables*)getVariableAt(n);
		SAFE_DELETE( val );
	}

	TRACE0("\tRemove WrapperSytem#processList...");
	int processLength = getProcessLength();
	for(int n=processLength-1;n>=0;n--) {
		WrapperProcess *proc = (WrapperProcess*)getProcessAt(n);
		SAFE_DELETE( proc );
	}

}


void WrapperSystem::addVariable(std::string varName,double varValue)
{
	WrapperVariables *value = new WrapperVariables(varName,varValue);
	variableList.push_back(value);
}

void WrapperSystem::addProcess(WrapperProcess *process)
{
	processList.push_back(process);
}


std::string WrapperSystem::getSystemId(){
	return systemId;
}

/**
 * Get Length of variableList
 */
int WrapperSystem::getVariableLength(){
	return variableList.size();
}

/**
 * Get Length of processList
 */
int WrapperSystem::getProcessLength(){
	return processList.size();
}

/**
 * Get variable
 */
WrapperVariables *WrapperSystem::getVariableAt(int index){
	int length = getVariableLength();
	if(index >= length) {
		std::cout << "@Error! : variableList length = " << length << " , index = " << index << std::endl;
		return NULL;
	}

	return ( (WrapperVariables*)(variableList[index]) );
}

/**
 * Get process
 */
WrapperProcess *WrapperSystem::getProcessAt(int index){
	int length = getProcessLength();
	if(index >= length) {
		std::cout << "@Error! : processList length = " << length << " , index = " << index << std::endl;
		return NULL;
	}

	return ( (WrapperProcess*)processList[index] );
}


/**
 * Get Process from ID
 */
WrapperProcess *WrapperSystem::getProcessFromId(std::string Id){
	int length = getProcessLength();
	for(int n=0;n<length;n++) {
		WrapperProcess *process = (WrapperProcess*)getProcessAt(n);
		if(process->getProcessID() == Id) {
			return process;
		}
	}

	return NULL;
}


void WrapperSystem::print()
{
	cout << "SYSTEM ID = " << systemId << endl;
	cout << "SYSTEM NAME = " << systemName << endl;

	int length = getProcessLength();
	for(int n=0;n<length;n++){
		( (WrapperProcess*)getProcessAt(n) )->print();
	}

	length = getVariableLength();
	for(int n=0;n<length;n++){
		( (WrapperVariables*)getVariableAt(n) )->print(1);
	}

}

