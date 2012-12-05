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
Calculation_Span = 12.0	#	[sec]
'''Calculation_Span defines the total caluculation span of solver module.'''

Output_VariableList = [
	"MinD_ATP",
	"MinD_ADP",
	"MinE",
	"Surface:MinD_ATP_m",
	"Surface:MinD_ATP_m_MinE",
	"Surface:MinD_ADP_m_MinE",
	"Surface:MinE_m"
]
'''Output_VariableList makes a Variable list for output.If an empty list is specified,Console outputs all Variable data anyway.'''

################################################################################
#			Sample of workingMinEring
################################################################################

Stepper_Property = [
	"SS",			#	StepperID
	3e-8,			#	VoxelRadius
	1,			#	SearchVacant
	"SpatiocyteStepper"	#	StepperName
	]
'''
Stepper_Property defines the SpatiocyteStepper data.

:param string   argv[0]: ID
:param double   argv[1]: VoxelRadius
:param long int argv[2]: SearchVacant
:param string   argv[3]: StepperName
'''

Rng_Seed = "10"
'''
Seed of random number.
If Rng_Seed is "TIME",seed is made by timer data.
If Rng_Seed is "DEFAULT",default seed is made.
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


System_Variable = [
	(  "TYPE"             ,  0         ),
	(  "SHAPE"            ,  1         ),
	(  "LENGTHX"          ,  0e-6      ),
	(  "LENGTHY"          ,  1e-6      ),
	(  "LENGTHZ"          ,  0e-6      ),
	(  "SIZE"             ,  2.88e-15  ),
	(  "SURFACEX"         ,  0         ),
	(  "SURFACEY"         ,  0         ),
	(  "SURFACEZ"         ,  0         ),
	(  "ORIGINX"          ,  0         ),
	(  "ORIGINY"          ,  0         ),
	(  "ORIGINZ"          ,  0         ),
	(  "VACANT"           ,  0         ),
	(  "MinD_ATP"         ,  500       ),
	(  "MinD_ADP"         ,  500       ),
	(  "MinE"             ,  700       )
	]
'''
System_Variable defines Variable of RootSystem

:param string argv[n][0]: Variable ID
:param double argv[n][1]: Variable value
'''


#	( ["ProcessName"] , ["ProcessID"] , [(VariableReferenceList)] , [(PropertyList)] , ["FileName"] )
System_Process = [
	(
	 [ "DiffusionProcess" ],					#	ProcessName
	 [ "diffuse" ],						#	ProcessID
	 [ ["MinD_ATP",0] , ["MinD_ADP",0] , ["MinE",0]  ] ,	#	VariableReferenceList
	 [ ["D",2.5e-12] ] , 					#	PropertyList
	 [ "" ] 							#	FileName
	)

	,

	(
	 [ "VisualizationLogProcess" ],
	 [ "loggerMean" ],
	 [ ["Surface:MinD_ATP_m_MinE", 0] , ["Surface:MinD_ADP_m_MinE",0]  , ["Surface:MinE_m",0]  ] ,
	 [ ["LogInterval",0.05] ],
	 [ "visualLog0.dat" ]
	)

	,

	(
	 [ "SpatiocyteNextReactionProcess" ],
	 [ "MinD_ADPtoMinD_ATP" ],
	 [ ["MinD_ADP",-1] , ["MinD_ATP",1] ] ,
	 [ ["k",6] ] ,
	 [ "" ]
	)

	,

	(
	 [ "MoleculePopulateProcess" ],
	 [ "populate" ] ,
	 [ ["MinD_ATP", 0] , ["MinD_ADP", 0] , ["MinE", 0] , ["Surface:MinD_ATP_m", 0] , ["Surface:MinD_ATP_m_MinE", 0] , 
          ["Surface:MinD_ADP_m_MinE", 0] , ["Surface:MinE_m", 0] ] ,
	 [ ] ,
	 [ "" ]
	)

	]
'''
System_Process defines ProcessList of RootSystem

:param string argv[n][0]: Process name
:param string argv[n][1]: Process ID
:param list   argv[n][2]: VariableReference list
:param string argv[n][2][0]: VariableID(in VariableReference)
:param string argv[n][2][1]: aCoefficient(in VariableReference)
:param list   argv[n][3]: Other property list
:param string argv[n][4]: FileName
'''


#
#	Set number of SubSystem
#
SubSytem_NUM = 1
'''
Number of SubSystem
'''

System_Property_Sub_0 = [
	"/Surface",	#	ID
	"SS"		#	StepperID
	]
'''
System_Property_Sub_0 defines property of SubSystem.

Suffix  _Sub_\*  is needed in SubSystem.

:param string argv[0]: SubSystem ID
:param string argv[1]: StepperID
'''

System_Variable_Sub_0 = [
	(  "TYPE"             ,  1         ),
	(  "LIPID"            ,  0         ),
	(  "MinD_ATP_m"       ,  0         ),
	(  "MinD_ATP_m_MinE"  ,  0         ),
	(  "MinD_ADP_m_MinE"  ,  0         ),
	(  "MinE_m"           ,  0         )
	]
'''
System_Variable_Sub_* defines Variable of SubSystem.

Suffix  _Sub_*  is needed in SubSystem.

:param string argv[n][0]: Variable ID
:param double argv[n][1]: Variable value
'''


#	( ["ProcessName"] , ["ProcessID"] , [(VariableReferenceList)] , [(PropertyList)] , ["FileName"] )
System_Process_Sub_0 = [
	(
	 [ "DiffusionProcess" ] , 
	 [ "diffuseS" ] , 
	 [ ["Surface:MinD_ATP_m",0] ] ,
	 [ ["D",1e-14] ] ,
	 [ "" ]
	)

	,

	(
	 [ "DiffusionProcess" ] ,
	 [ "diffusee" ] ,
	 [ ["Surface:MinE_m", 0] ] ,
	 [ ["D",1e-14] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionProcess"] ,
	 ["diffuseSss"] ,
	 [ ["Surface:MinD_ATP_m_MinE", 0] ] , 
	 [ ["D",1e-14] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionProcess"] ,
	 ["diffuseSs"] ,
	 [ ["Surface:MinD_ADP_m_MinE", 0] ] ,
	 [ ["D", 1e-14] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["MinD_ATP_2_MinD_ATP_m"] ,
	 [ ["Surface:LIPID",-1] , ["MinD_ATP",-1] , ["Surface:MinD_ATP_m",1] ] ,
	 [ ["k",0.0125e-6] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["MinD_ATP_mtoMinD_ATP_m_MinE"] ,
	 [ ["MinD_ATP",-1] , ["Surface:MinD_ATP_m",-1] , ["Surface:MinD_ATP_m",1] , ["Surface:MinD_ATP_m",1] ] ,
	 [ ["k",1.49448498e-20] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["MinD_ATP_m_MinEtoMinD_ATP_m_MinE"] ,
	 [ ["Surface:MinD_ATP_m",-1] , ["MinE",-1] , ["Surface:MinD_ATP_m_MinE",1] ] ,
	 [ ["k",9.23259608e-20] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["MinD_ATP_m_MinEtoMinD_ADP_MinE"] ,
	 [ ["Surface:MinD_ATP_m_MinE",-1] , ["MinD_ADP",1] , ["MinE",1] ] ,
	 [ ["k",0.5] ] ,
	 [ "" ]
	)

	,

	(
	 ["DiffusionInfluencedReactionProcess"] ,
	 ["polymerize2s"] ,
	 [ ["Surface:MinD_ATP_m_MinE",-1] , ["Surface:MinD_ATP_m_MinE",-1] , ["Surface:MinD_ATP_m_MinE",1] ,
          ["Surface:MinD_ADP_m_MinE",1] ] ,
	 [ ["k",9.23259608e-15] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["MinD_ATP_mp22s"] ,
	 [ ["Surface:MinD_ADP_m_MinE",-1] , ["Surface:MinE_m",1] , ["MinD_ADP",1] ] ,
	 [ ["k",0.1] ] ,
	 [ "" ]
	)

	,

	(
	 ["SpatiocyteNextReactionProcess"] ,
	 ["MinD_ATP_mp2s"] ,
	 [ ["Surface:MinE_m",-1] , ["MinE",1] ] ,
	 [ ["k",0.1] ] ,
	 [ "" ]
	)

	]
'''
System_Process_Sub_* defines ProcessList of RootSystem.

Suffix  _Sub_*  is needed in SubSystem.

:param string argv[n][0]: Process name
:param string argv[n][1]: Process ID
:param list   argv[n][2]: VariableReference list
:param string argv[n][2][0]: Variable ID(in VariableReference)
:param string argv[n][2][1]: aCoefficient(in VariableReference)
:param list   argv[n][3]: Other property list
:param string argv[n][4]: FileName
'''



################################################################################


