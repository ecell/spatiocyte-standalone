import BaseModel
import ModelData
import FrontEndAdapter
import python.Debug as Debug
import sys
#import numpy
import unittest


MAINSYSTEM = 0

#
#	Create ModelData
#	Sample of workingMinEring
#
modelData = ModelData.ModelData(
	BaseModel.Stepper_Property,
	BaseModel.System_Property,
	BaseModel.System_Variable,
	BaseModel.System_Process
	)

#	modelList is used in Console.py
modelList = [
	modelData
	]


#	Get SubSytem data from BaseModel
for n in range(0,BaseModel.SubSytem_NUM):
	propertySub = "System_Property_Sub_" + str(n)
	variableSub = "System_Variable_Sub_" + str(n)
	processSub = "System_Process_Sub_" + str(n)

	if( BaseModel.isExistVariable(propertySub) == True ):
		prop = BaseModel.getVariableFromName(propertySub)
		var = BaseModel.getVariableFromName(variableSub)
		proc = BaseModel.getVariableFromName(processSub)

		subModel = ModelData.ModelData(BaseModel.Stepper_Property,prop,var,proc)
		modelList.append(subModel)



#	WorkingMinEring etc...
modelList = modelList




def transferSystemValue(systemIndex,systemData):
	'''
	Transer Variable data in PySystem to C++ code.
	:param int systemIndex: PySystem index(0:MainSystem;1,2,...:SubSystem)
	:param PySystem systemData: PySystem data
	'''
	for variable in systemData.variableList:
		FrontEndAdapter.reqAppendVariableToSystem(systemIndex,variable.name,variable.value)

#
#	Convert Processes to C++ Module
#
def transferSystemProcess(systemIndex,systemData):
	'''
	Transfer PyProcess data to C++ code.
	:param int systemIndex: PySystem index(0:MainSystem;1,2,...:SubSystem)
	:param PySystem systemData: PySystem data
	'''
	for process in systemData.processList:
		#
		FrontEndAdapter.reqAppendProcessToSystem(systemIndex,process.ID,process.processName)

		for varRef in process.variableReferenceList:
			FrontEndAdapter.reqAppendVariableReferenceToProcess(systemIndex,process.ID,varRef.name,varRef.value)

		for prop in process.propertyList:
			FrontEndAdapter.reqAppendPropertyToProcess(systemIndex,process.ID,prop.name,prop.value)

		FrontEndAdapter.reqSetFileNameToProcess(systemIndex,process.ID,process.fileName)
		#



class Converter:
	'''
	Convert Python script data to the solver module.
	'''
	#	Initialize Model Data. See ModelData.py
	def setUp(self):
		'''
		Initialize PyStepper data and PySystem data from ModelData.
		'''
		#	Initialize Stepper
		global MAINSYSTEM
		self.stepper = ModelData.PySpatiocyteStepper(modelList[MAINSYSTEM])
		#	Initialize System
		self.SystemList = []
		for model in modelList:
			system = ModelData.PySystem()
			system.initSystem(model)
			#	Set Value to System
			system.setVariableList()
			#	Set Process to System
			system.setProcessList()
			self.SystemList.append(system)

	def printData(self):
		Debug.TRACE("#Stepper ID = " + self.stepper.ID)
		Debug.TRACE("#Stepper voxelRadius = " + str(self.stepper.voxelRadius) )
		Debug.TRACE("#Stepper searchVacant = " + str(self.stepper.searchVacant) )

		for system in self.SystemList:
			Debug.TRACE("///////////////////System = " + system.ID + "///////////////////" )

			for var in system.variableList:
				Debug.TRACE( "\t#Variable ID = " + var.name + " , VALUE = " + str(var.value) )

			for proc in system.processList:
				proc.printData()

	def createTemporaryData(self):
		self.fp = open("temp.dat","w")
		self.fp.write( "#SpatiocyteStepper\n" )
		self.fp.write( str(self.stepper.ID) + "\n" )
		self.fp.write( str(self.stepper.voxelRadius) + "\n" )
		self.fp.write( str(self.stepper.searchVacant) + "\n" )

		for system in self.SystemList:
			self.fp.write("#System\n")
			self.fp.write(system.ID + "\n")
			self.fp.write(system.StepperID + "\n")
			for proc in system.processList:
				self.fp.write("#Process\n")
				self.fp.write(proc.processName + "\n")

				self.fp.write("#VariableReference\n")
				for val in proc.variableReferenceList:
					self.fp.write(val.name + "," + str(val.value) + "\n" )

				self.fp.write("#Property\n")
				for prop in proc.propertyList:
					self.fp.write(prop.name + "," + str(prop.value) + "\n" )

				self.fp.write("#FileName\n")
				self.fp.write(proc.fileName + "\n" );

		self.fp.close()

	def transferToCppCode(self):
		'''
		Transfer Python data to C++ as intermediate data.
		'''
		#	Convert to Core Program
		FrontEndAdapter.reqAppendStepperData(self.stepper.ID,self.stepper.voxelRadius,self.stepper.searchVacant,self.stepper.stepperName,self.stepper.rngSeed)

		for n in range(0,len(self.SystemList)):
			FrontEndAdapter.reqAppendSystem(self.SystemList[n].ID,self.SystemList[n].StepperID)
			transferSystemValue(n,self.SystemList[n])
			transferSystemProcess(n,self.SystemList[n])

	def convertToCore(self):
		'''
		SetUp the solver module.
		'''
		FrontEndAdapter.convertToCore()

	def calculate(self):
		'''
		Start the calculation.
		'''
		#	Start Main Program
		FrontEndAdapter.startMain(BaseModel.Calculation_Span)

	def output(self):
		'''
		Output the result of the calculation.
		'''
		FrontEndAdapter.output()


#
#	Unit Test
#
class TestModule(unittest.TestCase):
	def setUp(self):
		self.converter = Converter()
		self.converter.setUp()

	def testPrintData(self):
		self.converter.printData()

	def testCalculate(self):
		self.converter.transferToCppCode()
		self.converter.convertToCore()
		self.converter.calculate()
		self.converter.output()


def main():
	converter = Converter()

	argvs = sys.argv
	argc = len(argvs)

	if(argc == 1):
		print "Usage: python  ./Console.py  [option]"
		print "Options:"
		print "\t-all\t,Execute all operations and output the result to \"output.eds\"."
		print "\t-calc\t,Calculate the model data without outputting to a file."
		print "\t(None),Execute unittest."

		unittest.main()
	else:
		if(argvs[1] == "-all"):
			converter.setUp()
			converter.transferToCppCode()
			converter.convertToCore()
			converter.calculate()
			converter.output()
		elif(argvs[1] == "-calc"):
			converter.setUp()
			converter.transferToCppCode()
			converter.convertToCore()
			converter.calculate()


#
#	Call MainFunc
#
if __name__ == '__main__':
	#unittest.main()
	main()


