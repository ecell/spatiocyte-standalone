#include "WrapperProcess.h"



WrapperProcess::WrapperProcess(const std::string procID,const std::string procName)
{
	processId = procID;
	processName = procName;
	fileName = "";
}


WrapperProcess::~WrapperProcess()
{

	TRACE0("\t\tRemove WrapperProcess#variableReferenceList...");
	int refLength = getVariableReferenceListLength();
	for(int n=refLength-1;n>=0;n--) {
		WrapperVariables *refVal = (WrapperVariables*)getVariableReferenceAt(n);
		SAFE_DELETE(refVal);
	}

	TRACE0("\t\tRemove WrapperProcess#propertyList...");
	int propLength = getPropertyListLength();
	for(int n=propLength-1;n>=0;n--) {
		WrapperVariables *prop = (WrapperVariables*)getPropertyAt(n);
		SAFE_DELETE(prop);
	}

}


void WrapperProcess::addVariableReference(std::string varName,double varValue)
{
	WrapperVariables *value = new WrapperVariables(varName,varValue);
	variableReferenceList.push_back(value);
}


void WrapperProcess::addProperty(std::string propertyName,double propertyValue)
{
	WrapperVariables *value = new WrapperVariables(propertyName,propertyValue);
	propertyList.push_back(value);
}


void WrapperProcess::setFileName(std::string filename)
{
	this->fileName = filename;
}


/**
 * Get Length of variableReferenceList
 */
int WrapperProcess::getVariableReferenceListLength()
{
	return variableReferenceList.size();
}

WrapperVariables *WrapperProcess::getVariableReferenceAt(int index)
{
	return ((WrapperVariables*)variableReferenceList[index]);
}

int WrapperProcess::getPropertyListLength()
{
	return propertyList.size();
}

WrapperVariables *WrapperProcess::getPropertyAt(int index)
{
	return ((WrapperVariables*)propertyList[index]);
}

std::string WrapperProcess::getProcessName()
{
	return processName;
}

std::string WrapperProcess::getProcessID()
{
	return processId;
}


std::string WrapperProcess::getFileName(){
	return fileName;
}


void WrapperProcess::print(){
	cout << "\tPROCESS ID = " << processId << endl;
	cout << "\tPROCESS NAME = " << processName << endl;

	int length = getVariableReferenceListLength();
	for(int n=0;n<length;n++){
		( (WrapperVariables*) getVariableReferenceAt(n) )->print(2);
	}

	length = getPropertyListLength();
	for(int n=0;n<length;n++){
		( (WrapperVariables*)getPropertyAt(n) )->print(2);
	}

}


