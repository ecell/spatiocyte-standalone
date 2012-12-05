import python.Debug as Debug
import BaseModel

class OutputData:
	'''
	Output the result of the calculation to the file.
	'''
	def __init__(self):
		self.outputStringList = []
	def addData(self,string):
		'''Add an output data.'''
		self.outputStringList.append(string)
	def outputData(self):
		self.counter = 0

		'''Output the result to the file.'''
		self.fp = open("output.eds","w")
		self.outputIndexList = []
		for data in self.outputStringList:
			work = data.split("\t")

			if(self.counter == 0):
				length = len(work)
				for n in range(0,length):
					if(self.checkOutputVal(work[n])):
						self.outputIndexList.append(n)

			self.counter = self.counter + 1

			for m in range(0,len(self.outputIndexList)):
				self.fp.write(work[self.outputIndexList[m]])
				self.fp.write("\t")

			#self.fp.write(data)
			self.fp.write("\n")
		
		self.fp.close()
	def printData(self):
		for data in self.outputStringList:
			Debug.TRACE(data)

	def checkOutputVal(self,valName):
		if(len(BaseModel.Output_VariableList) == 0):
			return True
		if(valName in BaseModel.Output_VariableList):
			return True
		if(valName == "CurrentTime"):
			return True
		return False




