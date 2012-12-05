#
#	Definintion of model parameter
#

#	Do not edit this function.
def isExistVariable(varName):
	if varName in globals():
		return True
	else:
		return False

#	Do not edit this function.
def getVariableFromName(varName):
	if( isExistVariable(varName) == True ):
		print "ret : " + varName
		return globals()[varName]
	else:
		return None

################################################################################
#		Definition of caluculation span
################################################################################


Calculation_Span = 4.0	#	[sec]
'''Calculation_Span defines total caluculation time of ECell module'''

Output_VariableList = []
'''Output_VariableList makes a Variable list for output.If an empty list is specified,Console outputs all Variable data anyway.'''


################################################################################
#			simple_spatiocyte
################################################################################
Stepper_Property = [
	"SS",			#	StepperID
	3.5e-8,		#	VoxelRadius
	1,			#	SearchVacant
	"SpatiocyteStepper"	#	StepperName
	]
'''
Stepper_Property defines SpatiocyteStepper data.

:param string   argv[0]: ID
:param double   argv[1]: VoxelRadius
:param long int argv[2]: SearchVacant
:param string   argv[3]: StepperName
'''


System_Property = [
	"/" ,		#	ID
	"SS"		#	StepperID
	]
'''
System_Property defines property of RootSystem

:param string argv[0]: RootSystem ID
:param string argv[1]: StepperID
'''

Rng_Seed = "10"
'''
Seed of random number.
If Rng_Seed is "TIME",seed is made by timer data.
If Rng_Seed is "DEFAULT",default seed is made.
'''


System_Variable = [
	(  "TYPE"             ,  0         ),
	(  "SHAPE"            ,  3         ),
	(  "SIZE"            ,  1000e-15         ),
	(  "LENGTHX"          ,  5e-06      ),
	(  "LENGTHY"          ,  5e-06      ),
	(  "LENGTHZ"          ,  5e-06      ),
	(  "SURFACEX"         ,  0         ),
	(  "SURFACEY"         ,  0         ),
	(  "SURFACEZ"         ,  0         ),
	(  "ORIGINX"          ,  0         ),
	(  "ORIGINY"          ,  0         ),
	(  "ORIGINZ"          ,  0         ),
	(  "SOURCE"         ,  1       ),
	(  "VACANT"         ,  0       ),
	(  "A"         ,  200       ),
	(  "B"         ,  100       ),
	(  "C"         ,  300       )
	]
'''
System_Variable defines Variable of RootSystem

:param string argv[n][0]: Variable ID
:param double argv[n][1]: Variable value
'''


System_Process = [
	(
	 [ "MoleculePopulateProcess" ],
	 [ "populate" ] ,
	 [ ["SOURCE", 0] , ["VACANT", 0] , ["A", 0] , ["B", 0] , ["C", 0] ] ,
	 [ ] ,
	 [ "" ]
	)

	,

	(
	 [ "DiffusionProcess" ],
	 [ "diffusion_A" ],
	 [ ["A",0] ] ,
	 [ ["D",10e-12] ] , 
	 [ "" ] 
	)

	,

	(
	 [ "DiffusionProcess" ],
	 [ "diffusion_B" ],
	 [ ["B",0] ] ,
	 [ ["D",1.0e-12] ] , 
	 [ "" ] 
	)

	,

	(
	 [ "DiffusionProcess" ],
	 [ "diffusion_C" ],
	 [ ["C",0] ] ,
	 [ ["D",5.0e-12] ] , 
	 [ "" ] 
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["synthesis_C"] ,
	 [ ["SOURCE",-1] , ["SOURCE",1] , ["C",1] ] ,
	 [ ["k",20.0] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["degradation_A"] ,
	 [ ["A",-1] , ["VACANT",1] ] ,
	 [ ["k",1.0e-3] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["degradation_B"] ,
	 [ ["B",-1] , ["VACANT",1] ] ,
	 [ ["k",1.0e-2] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["ABCreaction"] ,
	 [ ["A",-1] , ["B",-1] , ["C",1] ] ,
	 [ ["k",0.5e-20] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["Cdecay"] ,
	 [ ["C",-1] , ["A",1] , ["B",1] ] ,
	 [ ["k",0.5] ] ,
	 [ "" ]
	)

	]
'''
System_Process defines ProcessList of RootSystem

:param string argv[n][0]: Process Name
:param string argv[n][1]: Process ID
:param list   argv[n][2]: VariableReference List(VariableID,aCoefficient)
:param list   argv[n][3]: Other property list
:param string argv[n][4]: FileName(If you declare VisualizationLogProcess,you can set logfile name.)
'''


#
#	Set number of SubSystem
#
SubSytem_NUM = 0
'''
Number of SubSystem
'''




