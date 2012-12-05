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
		return globals()[varName]
	else:
		return None


################################################################################
#		Definition of caluculation span
################################################################################


Calculation_Span = 7.0	#	[sec]
'''Calculation_Span defines total caluculation time of ECell module'''

Output_VariableList = [
	"Cell0:A",
	"Cell0:B",
	"Cell0:C",
	"Cell1:A",
	"Cell1:B",
	"Cell1:C"
]
'''Output_VariableList makes a Variable list for output.If an empty list is specified,Console outputs all Variable data anyway.'''


################################################################################
#				simple_compartment
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
	(  "LENGTHX"          ,  1e-05      ),
	(  "LENGTHY"          ,  5e-06      ),
	(  "LENGTHZ"          ,  5e-06      ),
	(  "SURFACEX"         ,  0         ),
	(  "SURFACEY"         ,  0         ),
	(  "SURFACEZ"         ,  0         ),
	(  "ORIGINX"          ,  0         ),
	(  "ORIGINY"          ,  0         ),
	(  "ORIGINZ"          ,  0         ),
	(  "N_cells"         ,  2       ),
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
	 [ ["Cell0:SOURCE", 0] , ["Cell0:A", 0] , ["Cell0:B", 0] , ["Cell0:C", 0] , ["Cell1:SOURCE", 0] , 
          ["Cell1:A", 0] , ["Cell1:B", 0] , ["Cell1:C", 0] ] ,
	 [ ] ,
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
SubSytem_NUM = 2
'''
Number of SubSystem
'''

System_Property_Sub_0 = [
	"/Cell0",	#	ID
	"SS"		#	StepperID
	]
'''
System_Property_Sub_0 defines property of SubSystem.
Suffix "_Sub_*" is needed in SubSystem.

:param string argv[0]: SubSystem ID
:param string argv[1]: StepperID
'''

System_Variable_Sub_0 = [
	(  "TYPE"             ,  0         ),
	(  "SHAPE"            ,  3         ),
	(  "LENGTHX"       ,  5e-06         ),
	(  "LENGTHY"  ,  5e-06         ),
	(  "LENGTHZ"  ,  5e-06         ),
	(  "SURFACEX"           ,  0         ),
	(  "SURFACEY"           ,  0         ),
	(  "SURFACEZ"           ,  0         ),
	(  "ORIGINX"          ,  -0.5         ),
	(  "ORIGINY"          ,  0.0         ),
	(  "ORIGINZ"          ,  0.0         ),
	(  "xL_pos"          ,  0.25         ),
	(  "SOURCE"          ,  1         ),
	(  "VACANT"          ,  0         ),
	(  "A"          ,  1000         ),
	(  "B"          ,  3000         ),
	(  "C"          ,  2000         ),
	]
'''
System_Variable_Sub_* defines Variable of SubSystem.
Suffix "_Sub_*" is needed in SubSystem.

:param string argv[n][0]: Variable ID
:param double argv[n][1]: Variable value
'''


System_Process_Sub_0 = [
	(
	 [ "DiffusionProcess" ] , 
	 [ "diffusion_A" ] , 
	 [ ["Cell0:A",0 ] ] ,
	 [ ["D",0.1e-12] ] ,
	 [ "" ]
	)

	,

	(
	 [ "DiffusionProcess" ] ,
	 [ "diffusion_B" ] ,
	 [ ["Cell0:B", 0] ] ,
	 [ ["D",1.0e-12] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["synthesis_A"] ,
	 [ ["Cell0:SOURCE",-1] , ["Cell0:SOURCE",1] , ["Cell0:A",1] ] ,
	 [ ["k",2.0] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["degradation_A"] ,
	 [ ["Cell0:A",-1] , ["Cell0:VACANT",1] ] ,
	 [ ["k",0.056] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["degradation_B"] ,
	 [ ["Cell0:B",-1] , ["Cell0:VACANT",1] ] ,
	 [ ["k",0.002] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["reaction_AB"] ,
	 [ ["Cell0:A",-1] , ["Cell0:B",-1] , ["Cell0:C",1] ] ,
	 [ ["k",1.0e-20] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["decay_C"] ,
	 [ ["Cell0:C",-1] , ["Cell0:A",1] , ["Cell0:B",1] ] ,
	 [ ["k",0.5] ] ,
	 [ "" ]
	)

	]
'''
System_Process_Sub_* defines ProcessList of RootSystem.
Suffix "_Sub_*" is needed in SubSystem.

:param string argv[n][0]: Process Name
:param string argv[n][1]: Process ID
:param list   argv[n][2]: VariableReference List(VariableID,aCoefficient)
:param list   argv[n][3]: Other property list
:param string argv[n][4]: FileName(If you declare VisualizationLogProcess,you can set logfile name.)
'''


System_Property_Sub_1 = [
	"/Cell1",	#	ID
	"SS"		#	StepperID
	]


System_Variable_Sub_1 = [
	(  "TYPE"             ,  0         ),
	(  "SHAPE"            ,  3         ),
	(  "LENGTHX"       ,  5e-06         ),
	(  "LENGTHY"  ,  5e-06         ),
	(  "LENGTHZ"  ,  5e-06         ),
	(  "SURFACEX"           ,  0         ),
	(  "SURFACEY"           ,  0         ),
	(  "SURFACEZ"           ,  0         ),
	(  "ORIGINX"          ,  0.5         ),
	(  "ORIGINY"          ,  0.0         ),
	(  "ORIGINZ"          ,  0.0         ),
	(  "xL_pos"          ,  0.75         ),
	(  "SOURCE"          ,  1         ),
	(  "VACANT"          ,  0         ),
	(  "A"          ,  3000         ),
	(  "B"          ,  1000         ),
	(  "C"          ,  2000         ),
	]


System_Process_Sub_1 = [
	(
	 [ "DiffusionProcess" ] , 
	 [ "diffusion_A" ] , 
	 [ ["Cell1:A",0 ] ] ,
	 [ ["D",1.0e-12] ] ,
	 [ "" ]
	)

	,

	(
	 [ "DiffusionProcess" ] ,
	 [ "diffusion_B" ] ,
	 [ ["Cell1:B", 0] ] ,
	 [ ["D",0.1e-12] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["degradation_A"] ,
	 [ ["Cell1:A",-1] , ["Cell1:VACANT",1] ] ,
	 [ ["k",0.05] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["degradation_B"] ,
	 [ ["Cell1:B",-1] , ["Cell1:VACANT",1] ] ,
	 [ ["k",0.03] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["reaction_AB"] ,
	 [ ["Cell1:A",-1] , ["Cell1:B",-1] , ["Cell1:C",1] ] ,
	 [ ["k",1.5e-20] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["decay_C"] ,
	 [ ["Cell1:C",-1] , ["Cell1:A",1] , ["Cell1:B",1] ] ,
	 [ ["k",0.5] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["synthesis_B"] ,
	 [ ["Cell1:SOURCE",-1] , ["Cell1:SOURCE",1] , ["Cell1:B",1] ] ,
	 [ ["k",10.25] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["forward_A"] ,
	 [ ["Cell0:A",-1] , ["Cell1:A",1] ] ,
	 [ ["k",0.3] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["backward_A"] ,
	 [ ["Cell0:A",1] , ["Cell1:A",-1] ] ,
	 [ ["k",0.4] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["forward_B"] ,
	 [ ["Cell0:B",-1] , ["Cell1:B",1] ] ,
	 [ ["k",0.4] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["backward_B"] ,
	 [ ["Cell0:B",1] , ["Cell1:B",-1] ] ,
	 [ ["k",0.3] ] ,
	 [ "" ]
	)


	]




################################################################################


