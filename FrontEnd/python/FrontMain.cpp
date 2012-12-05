#include "../ecell3Module/libecs/System.hpp"
#include "./intermediate/WrapperSystem.h"
#include "./intermediate/ConvertedModelData.h"


#include <typeinfo>
#include<sstream>

#include <Python.h>


void run(double calculationTime);
void callPythonCode(const char *pythonCode);
void addOutputData(const char *outputData);


ConvertedModelData modelDataContainer;


/**
 * Initialize Stepper with unique Id,voxelRadius and searchVacant.
 * This function is called by Python code before callMainModule()
 */
extern "C" void AppendStepperData(char *Id,double voxelRadius,long int searchVacant,char *stepperName,char *rngSeed)
{
	modelDataContainer.inputStepperData(Id,voxelRadius,searchVacant,stepperName,rngSeed);
}

/**
 * Initialize System
 */
extern "C" void AppendSystem(char *sysId,char *propertyName)
{
	modelDataContainer.addSystemData(sysId,propertyName);
}

extern "C" void AppendVariableToSystem(int systemIndex,char *varName,double varValue)
{
	WrapperSystem *data = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(systemIndex);
	data->addVariable(varName,varValue);
}


extern "C" void AppendProcessToSystem(int systemIndex,char *processId,char *processName)
{
	WrapperSystem *data = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(systemIndex);
	data->addProcess( new WrapperProcess(processId,processName) );
}


extern "C" void AppendVariableReferenceToProcess(int systemIndex,char *processId,char *varRefName,double varValue)
{
	WrapperSystem *data = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(systemIndex);
	data->getProcessFromId(processId)->addVariableReference(varRefName,varValue);
}


extern "C" void AppendPropertyToProcess(int systemIndex,char *processId,char *propertyName,double propertyValue)
{
	WrapperSystem *data = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(systemIndex);
	data->getProcessFromId(processId)->addProperty(propertyName,propertyValue);
}


extern "C" void SetFileNameToProcess(int systemIndex,char *processId,char *fileName)
{
	WrapperSystem *data = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(systemIndex);
	data->getProcessFromId(processId)->setFileName(fileName);
}

/**
 * SetUp SpatiocyteStepper
 */
extern "C" void CommitConvert()
{
	// Input values to SpatiocyteStepper
	modelDataContainer.convertStepperData();
	// Create SystemList
	modelDataContainer.initializeSystemList();
	// Convert WrapperSystem to System
	modelDataContainer.convertSystemList();
	// Input Model to SpatiocyteStepper
	modelDataContainer.stepperData->setModel( modelDataContainer.modelData );
	// Convert WrapperProcess to Process
	modelDataContainer.convertProcessList();

	// Initialize SpatiocyteStepper
	modelDataContainer.stepperData->initialize();
}


extern "C" void callMainModule(double calculationTime)
{
	// Start Output
	Py_SetProgramName("Console.py");
	callPythonCode("import OutputData");
	callPythonCode("output = OutputData.OutputData()");
	run(calculationTime);
}


extern "C" void outputData()
{
	callPythonCode("output.outputData()");
}


void callPythonCode(const char *pythonCode)
{
    PyRun_SimpleString(pythonCode);
}

void addOutputDataLn(const char *outputData)
{

	std::string work = "output.addData(\"";
	work +=  outputData;
	work += "\")";
	callPythonCode(work.c_str());

}

void run(double calculationTime)
{
    // pick up main system from SpatioStepper
    System* aRootSystem( modelDataContainer.stepperData->getModel()->getRootSystem() );

    // pick up sub system from main system
    System::Systems::const_iterator i = aRootSystem->getSystems().begin();
    System* aSubSystem( i->second );

    int sytemLength = modelDataContainer.getWrapSystemListLength();
    std::string caption = "CurrentTime\t";
    for(int n=0;n<sytemLength;n++) {
        WrapperSystem *wrapSystem = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(n);

        int wvalLength = wrapSystem->getVariableLength();
        for(int m=0;m<wvalLength;m++) {

            WrapperVariables* wval = (WrapperVariables*)wrapSystem->getVariableAt(m);
            caption += wrapSystem->getSystemId().substr(1);

            std::string work = wrapSystem->getSystemId().substr(1);
            // If variable of subsystem,put ":" as a token.
            if(work.size() >= 1) {
                caption += ":";
            }

            caption += wval->getName();
            caption += "\t";
        }
    }

    addOutputDataLn(caption.c_str());


    double theTime = 0.0;

    while(theTime < calculationTime) {

        modelDataContainer.stepperData->step();

        char work[512];
        theTime = modelDataContainer.stepperData->getCurrentTime();

        sprintf(work,"%f\t",theTime);
        for(int n=0;n<sytemLength;n++) {
            WrapperSystem *wrapSystem = (WrapperSystem*)modelDataContainer.getWrapperSystemAt(n);
            System* sys = (System*)modelDataContainer.getConvertedSystemAt(n);

            int wvalLength = wrapSystem->getVariableLength();
            for(int m=0;m<wvalLength;m++) {
                WrapperVariables* wval = (WrapperVariables*)wrapSystem->getVariableAt(m);
                double data = sys->getVariable(wval->getName())->getValue();
                char tmp[20];
                sprintf(tmp,"%f\t",data);
                strcat(work,tmp);
            }

        }

        addOutputDataLn(work);

    }

}


