import python.Debug as Debug
import BaseModel



class ModelData:
	'''
	Model data for calculation.

	:param string   SpatiocyteStepper_ID: ID of SpatiocyteStepper
	:param double   SpatiocyteStepper_voxelRadius: voxelRadius
	:param long int SpatiocyteStepper_searchVacant: searchVacant
	:param string   SpatiocyteStepper_stepperName: Name of SpatiocyteStepper
	:param string   System_ID: System ID
	:param string   System_StepperID: Stepper
	:param list     System_Variable: Variable list
	:param list     System_Process: Process list
	'''
	def __init__(self,stepperProperty,systemProperty,variables,processList):
		'''
		Create ModelData instance from BaseModel.
		'''
		#	Initialize Stepper Data
		self.SpatiocyteStepper_ID           = stepperProperty[0]
		self.SpatiocyteStepper_voxelRadius  = stepperProperty[1]
		self.SpatiocyteStepper_searchVacant = stepperProperty[2]
		self.SpatiocyteStepper_stepperName  = stepperProperty[3]
		self.Rng_Seed                       = BaseModel.Rng_Seed
		#	Initialize System /
		self.System_ID                      = systemProperty[0]
		self.System_StepperID               = systemProperty[1]

		#	Initialize System Variable Data ( VariableID , value )
		self.System_Variable = variables

		#	Initialize System Process Data
		#	( ["ProcessName"] , ["ProcessID"] , [(VariableReferenceList)] , [(PropertyList)] , ["FileName"] )
		self.System_Process = processList



#
#	Wrapper Class
#
class PySpatiocyteStepper:
	'''
	Adapter class between Python to C++. The data list in PySpatiocyteStepper is transferred to ConvertedModelData(C++).

	:param string ID: Stepper ID
	:param double voxelRadius: voxelRadius
	:param long int searchVacant: searchVacant
	:param string stepperName: stepperName
	:param string rngSeed: seed of random number
	'''
	def __init__(self,ID,voxelRadius,searchVacant,stepperName,rngSeed):
		self.ID = ID
		self.voxelRadius = voxelRadius
		self.searchVacant = searchVacant
		self.stepperName = stepperName
		self.rngSeed = rngSeed;
	def __init__(self,modelData):
		self.ID           = modelData.SpatiocyteStepper_ID
		self.voxelRadius  = modelData.SpatiocyteStepper_voxelRadius
		self.searchVacant = modelData.SpatiocyteStepper_searchVacant
		self.stepperName  = modelData.SpatiocyteStepper_stepperName
		self.rngSeed = modelData.Rng_Seed

class PySystem:
	'''
	Adapter class between Python to C++. The data list in PySystem is transferred to ConvertedModelData(C++).

	:param string ID: System ID
	:param string StepperID: Stepper ID
	:param Variable[] variableList: list of Variable
	:param Process[] processList: list of Process
	'''
	def __init__(self,ID,StepperID):
		self.ID = ID
		self.StepperID = StepperID
		self.variableList = []
		self.processList = []
	def __init__(self):
		self.variableList = []
		self.processList = []
	def initSystem(self,modelData):
		'''
		Read PySystem data from ModelData.

		:param ModelData modelData: Instance of ModelData.
		'''
		self.ID          = modelData.System_ID
		self.StepperID   = modelData.System_StepperID
		self.modelData   = modelData

	def appendVariable(self,ID,value):
		'''
		Append PyVariable data to list.

		:param string ID: Variable ID
		:param double value: Variable value
		'''
		self.variable = PyVariable(ID,value)
		self.variableList.append(self.variable)

	def appendProcess(self,process):
		'''
		Append PyProcess data to list.

		:param PyProcess process: Instance of PyProcess
		'''
		self.processList.append(process)

	def setVariableList(self):
		'''
		Create PyVariable list from ModelData
		'''
		self.listLength = len(self.modelData.System_Variable)
		for n in range(0,self.listLength):
			self.appendVariable( self.modelData.System_Variable[n][0] , self.modelData.System_Variable[n][1] )

	def setProcessList(self):
		'''
		Create PyProcess list from ModelData
		'''
		self.listLength = len(self.modelData.System_Process)
		for n in range(0,self.listLength):
			process = PyProcess( self.modelData.System_Process[n][0][0] , self.modelData.System_Process[n][1][0] )

			self.refLength = len(self.modelData.System_Process[n][2])

			for m in range(0,self.refLength):
				process.appendVariableReference( self.modelData.System_Process[n][2][m][0] , self.modelData.System_Process[n][2][m][1] )

			self.propertyLength = len(self.modelData.System_Process[n][3])
			for j in range(0,self.propertyLength):
				process.appendProperty( self.modelData.System_Process[n][3][j][0]  , self.modelData.System_Process[n][3][j][1] )

			process.setFileName(self.modelData.System_Process[n][4][0])

			self.appendProcess(process)


class PyVariable:
	'''
	Adapter Class between Python to C++.
	The data list in PyVariable is transferred to WrapperVariables.

	:param string name: Name of Variable
	:param double value: Value of Variable
	'''
	def __init__(self,name,value):
		self.name = name
		self.value = value

class PyProcess:
	'''
	Adapter Class between Python to C++.
	Data lists in PyProcess are transferred to WrapperProcess.

	:param string processName: Name of Process
	:param string ID: Process ID
	:param Variable[] variableReferenceList: list of VariableReference
	:param Variable[] propertyList: list of other properties.(For example,D or k,etc...)
	:param string fileName: FileName(Only visualizationLogProcess needs this data.)
	'''
	def __init__(self,processName,ID):
		self.processName = processName
		self.ID = ID
		self.variableReferenceList = []
		self.propertyList = []
		self.fileName = ""

	def appendVariableReference(self,ID,value):
		'''
		Add a VariableReference to the list.

		:param string ID: Variable ID
		:param long int value: aCoefficient
		'''
		self.variable = PyVariable(ID,value)
		self.variableReferenceList.append(self.variable)
	def appendProperty(self,ID,value):
		'''
		Add a property to the list.

		:param string ID: ID
		:param double value: value of property
		'''
		self.prop = PyVariable(ID,value)
		self.propertyList.append(self.prop)
	def setFileName(self,fileName):
		'''
		Set a file name to the Process.
		'''
		self.fileName = fileName
	def printData(self):
		Debug.TRACE("\t#ProcessName = "+self.processName)
		for val in self.variableReferenceList:
			Debug.TRACE("\t\t#VariableReference ID = "+val.name+" , VALUE = " + str(val.value) )

		for prop in self.propertyList:
			Debug.TRACE("\t\t#Property ID = " + prop.name + " , VALUE = " + str(prop.value) )

		Debug.TRACE("\t\t#File Name = " +self.fileName );



