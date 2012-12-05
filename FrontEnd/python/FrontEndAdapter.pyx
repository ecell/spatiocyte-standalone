

cdef extern:
	void AppendStepperData(char *Id,double voxelRadius,long int searchVacant,char *stepperName,char *rngSeed)


cdef extern:
	void AppendSystem(char *sysId,char *propertyName)


cdef extern:
	void AppendVariableToSystem(int systemIndex,char *varName,double varValue)


cdef extern:
	void AppendProcessToSystem(int systemIndex,char *processId,char *processName)


cdef extern:
	void AppendVariableReferenceToProcess(int systemIndex,char *processId,char *varRefName,double varValue)


cdef extern:
	void AppendPropertyToProcess(int systemIndex,char *processId,char *propertyName,double propertyValue)


cdef extern:
	void SetFileNameToProcess(int systemIndex,char *processId,char *fileName)


cdef extern:
	void CommitConvert()


cdef extern:
	void callMainModule(double calculationTime)


cdef extern:
	void outputData()


def reqAppendStepperData(Id,voxelRadius,searchVacant,stepperName,rngSeed):
	'''
	Transfer Stepper data to C++ code.
	'''
	AppendStepperData(Id,voxelRadius,searchVacant,stepperName,rngSeed)

def reqAppendSystem(systemId,propertyName):
	'''
	Transfer System data to C++ code.
	'''
	AppendSystem(systemId,propertyName)

def reqAppendVariableToSystem(systemIndex,varName,varValue):
	'''
	Transfer Variable data to the WrapperSystem of C++ code.
	'''
	AppendVariableToSystem(systemIndex,varName,varValue)

def reqAppendProcessToSystem(systemIndex,processId,processName):
	'''
	Transfer Process data to the WrapperSystem of C++ code.
	'''
	AppendProcessToSystem(systemIndex,processId,processName)

def reqAppendVariableReferenceToProcess(systemIndex,processId,varRefName,varValue):
	'''
	Transfer VariableReference data to WrapperProcess of C++ code.
	'''
	AppendVariableReferenceToProcess(systemIndex,processId,varRefName,varValue)

def reqAppendPropertyToProcess(systemIndex,processId,propertyName,propertyValue):
	'''
	Transfer Property data to WrapperProcess of C++ code.
	'''
	AppendPropertyToProcess(systemIndex,processId,propertyName,propertyValue)

def reqSetFileNameToProcess(systemIndex,processId,fileName):
	'''
	Set file name to WrapperProcess of C++ code.
	'''
	SetFileNameToProcess(systemIndex,processId,fileName)

def convertToCore():
	'''
	Convert intermediate data and setup solver module.
	'''
	CommitConvert()

def startMain(calculationTime):
	'''
	Start calculation.
	'''
	#CommitConvert()
	callMainModule(calculationTime)
	#outputData()

def output():
	'''
	Output the result to file.
	'''
	outputData()


