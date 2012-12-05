#include "ConvertedModelData.h"


ConvertedModelData::ConvertedModelData()
{
	initialize();
}


ConvertedModelData::~ConvertedModelData()
{
	destroy();
}


void ConvertedModelData::initialize()
{
	TRACE0("Creating stepperData...");
	stepperData = new SpatiocyteStepper();
	TRACE0("Creating modelData...");
	modelData = new Model();
}

void ConvertedModelData::destroy()
{
	int length = getWrapSystemListLength();
	TRACE0("Removing wrapSystemList...");
	for(int n=length-1;n>=0;n--) {
		WrapperSystem *wrapSystem = (WrapperSystem*)getWrapperSystemAt(n);
		SAFE_DELETE( wrapSystem );
	}

	length = getConvertedSystemListLength();
	TRACE0("Removing convertedSystemList...");
	for(int n=length-1;n>=0;n--) {
		System *sys = (System*)getConvertedSystemAt(n);
		SAFE_DELETE( sys );
	}

	TRACE0("Remove WrapperSytem#convertedVariables");
	int convartedValLength = convertedVariables.size();
	for(int n=convartedValLength-1;n>=0;n--) {
		Variable *val = (Variable*)convertedVariables[n];
		SAFE_DELETE( val );
	}

	TRACE0("Removing modelData...");
	SAFE_DELETE(modelData);
	TRACE0("Removing stepperData...");
	SAFE_DELETE(stepperData);
}




/**
 * Get WrapperSystem
 */
WrapperSystem *ConvertedModelData::getWrapperSystemAt(int index){
	if(index >= getWrapSystemListLength()){
		cout << "Error! index is wrong @ConvertedModelData::getWrapperSystemAt" << endl;
		return NULL;
	}
	return wrapSystemList[index];
}



/**
 * Set Stepper Data to buffer
 */
void ConvertedModelData::inputStepperData(std::string Id,double voxelRadius,long int searchVacant,std::string stepperName,std::string rngSeed){
	this->ID = Id;
	this->voxelRadius = voxelRadius;
	this->searchVacant = searchVacant;
	this->stepperName = stepperName;
	this->rngSeed = rngSeed;
}



/**
 * Convert WrapperStepper Data to SpatiocyteStepper Data
 */
void ConvertedModelData::convertStepperData(){
	// Input values to SpatiocyteStepper
	stepperData->setID( ID );
	stepperData->setVoxelRadius( voxelRadius );
	stepperData->setSearchVacant( searchVacant );
	stepperData->setStepperName( stepperName );
	stepperData->setRngSeed(rngSeed);
}



/**
 * Create WrapperSystem Data and add to list
 */
void ConvertedModelData::addSystemData(std::string sysId,std::string propertyName){
	WrapperSystem *work = new WrapperSystem(sysId,propertyName);
	//wrapSystemList.addElement(work);
	wrapSystemList.push_back(work);
}



/**
 * Get length of wrapSystemList
 */
int ConvertedModelData::getWrapSystemListLength(){
	return wrapSystemList.size();
}



/**
 * Get System
 */
System *ConvertedModelData::getConvertedSystemAt(int index){
	if(index >= getWrapSystemListLength() ){
		cout << "Error! index is too large.@ConvertedModelData::getConvertedSystemAt" << endl;
		return NULL;
	}
	return convertedSystemList[index];
}



/**
 * Get length of convertedSystemList
 */
int ConvertedModelData::getConvertedSystemListLength(){
	return convertedSystemList.size();
}



/**
 * Initialize Converted SystemList
 */
void ConvertedModelData::initializeSystemList()
{
	//	Create SystemList Instance
	int sytemLength = getWrapSystemListLength();
	for(int n=0;n<sytemLength;n++) {
		System* sys = new System();	//	define main system and subsystem
		//convertedSystemList.addElement(sys);
		convertedSystemList.push_back(sys);
	}
	//	Set Root System
	modelData->theRootSystem = (System*)getConvertedSystemAt(MAIN_SYSTEM);
}


/**
 * Convert Wrapper System Data to System Data
 */
void ConvertedModelData::convertSystemList(){

	int sytemLength = getWrapSystemListLength();
	for(int n=0;n<sytemLength;n++) {
		WrapperSystem *wrapSystem = (WrapperSystem*)getWrapperSystemAt(n);
		// Input StepperID to each system
		System* sys = (System*)getConvertedSystemAt(n);
		sys->setStepperID( stepperData->getID() );
		// Input ID to each system
		sys->setID( wrapSystem->getSystemId() );
		// Input Variables to each System
		int valLength = wrapSystem->getVariableLength();

		for( int n = 0; n < valLength; n++ ){
			Variable *var = new Variable();
			WrapperVariables *data = wrapSystem->getVariableAt(n);
			var->setValue( data->getValue() );
			var->setID( data->getName() );
			sys->registerEntity(var);
			//wrapSystem->convertedVariables.addElement(var);
			convertedVariables.push_back(var);
			variablesRelation.push_back( &(wrapSystem->getSystemId()) );
		}

		//	Set superSystem
		if( n == MAIN_SYSTEM ) {
			sys->setSuperSystem( sys );
		}
		//	Input subSystem to mainSystem
		else {
			System* mainSys = (System*)getConvertedSystemAt(MAIN_SYSTEM);
			mainSys->registerEntity( sys );
		}
	}
}


/**
 * Convert Wrapper Process Data to Process Data
 */
void ConvertedModelData::convertProcessList(){
	int sytemLength = getWrapSystemListLength();

	for(int n=0;n<sytemLength;n++) {
		System* sys = (System*)getConvertedSystemAt(n);
		System* mainSys = (System*)getConvertedSystemAt(MAIN_SYSTEM);
		WrapperSystem *wrapSystem = (WrapperSystem*)getWrapperSystemAt(n);

		// Input ProcessList to System
		int procLength = wrapSystem->getProcessLength();

		// pick up main system from SpatioStepper
		System* aRootSystem( stepperData->getModel()->getRootSystem() );

		// Input Processes to SpatiocyteStepper, main system, and sub system
		for(int n = 0;n<procLength;n++){
			// Generate Core Process
			Process *thisProcess = createProcess(wrapSystem->getProcessAt(n)->getProcessName());

			std::string processID = wrapSystem->getProcessAt(n)->getProcessID();
			int variableReferenceLength = wrapSystem->getProcessAt(n)->getVariableReferenceListLength();
			std::string propertyName;
			double propertyData;

			if(  typeid( *thisProcess ) == typeid( MoleculePopulateProcess )  ) 
			{
				propertyName = "";
				propertyData = 0.0;
			}else{
				propertyName = wrapSystem->getProcessAt(n)->getPropertyAt(0)->getName();
				propertyData = wrapSystem->getProcessAt(n)->getPropertyAt(0)->getValue();
			}

			if( typeid( *thisProcess ) == typeid( DiffusionProcess ) ){
				std::string name = propertyName;
				std::string procName = "DiffusionProcess";
				((DiffusionProcess*)thisProcess)->setD( propertyData );
				((DiffusionProcess*)thisProcess)->setID( processID );
				((DiffusionProcess*)thisProcess)->setProcessName( procName );
			}
			else if( typeid( *thisProcess ) == typeid( VisualizationLogProcess ) ) {
				std::string name = propertyName;
				std::string fileName = wrapSystem->getProcessAt(n)->getFileName();
				std::string procName = "VisualizationLogProcess";
				((VisualizationLogProcess*)thisProcess)->setLogInterval( propertyData );
				((VisualizationLogProcess*)thisProcess)->setFileName( fileName );
				((VisualizationLogProcess*)thisProcess)->setID( processID );
				((VisualizationLogProcess*)thisProcess)->setProcessName( procName );

			}
			else if(  typeid( *thisProcess ) == typeid( SpatiocyteNextReactionProcess )  ) {
				std::string procName = "SpatiocyteNextReactionProcess";
				((SpatiocyteNextReactionProcess*)thisProcess)->setk( propertyData );
				((SpatiocyteNextReactionProcess*)thisProcess)->setID( processID );
				((SpatiocyteNextReactionProcess*)thisProcess)->setProcessName( procName );

			}
			else if(  typeid( *thisProcess ) == typeid( MoleculePopulateProcess )  ) {
				std::string procName = "MoleculePopulateProcess";
				((MoleculePopulateProcess*)thisProcess)->setID( processID );
				((MoleculePopulateProcess*)thisProcess)->setProcessName( procName );

			}
			else if( typeid( *thisProcess ) == typeid( DiffusionInfluencedReactionProcess ) ){
				std::string procName = "DiffusionInfluencedReactionProcess";
				((DiffusionInfluencedReactionProcess*)thisProcess)->setk( propertyData );
				((DiffusionInfluencedReactionProcess*)thisProcess)->setID( processID );
				((DiffusionInfluencedReactionProcess*)thisProcess)->setProcessName( procName );
			}

			int varsNum = variableReferenceLength;

			for( int i = 0; i < varsNum; i++ )
			{
				std::string name = createVariableReferenceName(i);
				std::string reqID = wrapSystem->getProcessAt(n)->getVariableReferenceAt(i)->getName();
				long int aCoefficient = (long int)wrapSystem->getProcessAt(n)->getVariableReferenceAt(i)->getValue();

				if( reqID.find( ":", 0 ) == string::npos ) {
					thisProcess->registerVariableReference( name, aRootSystem->getVariable( reqID ), aCoefficient, true );
				}
				else {
					std::string strData = "/";
					strData += reqID.substr(0 , reqID.find(":",0) );
					std::string varName = reqID;
					varName.erase( 0, varName.find( ":", 0 ) + 1 );

					for(int k=0;k<sytemLength;k++) {
						System* sysWork = (System*)getConvertedSystemAt(k);
						if(sysWork->getID() == strData) {
							System* aSubSystem( aRootSystem->getSystem( sysWork->getID() ) );
							thisProcess->registerVariableReference( name, aSubSystem->getVariable( varName ), aCoefficient, true );
						}
					}

				}

			}


			(thisProcess)->setStepper( stepperData );
			(thisProcess)->setSuperSystem( aRootSystem );
			(thisProcess)->preinitialize();
			(thisProcess)->initialize();

			stepperData->registerProcess( thisProcess );
			sys->registerEntity( thisProcess );
		}

	}


}




/**
 * Create Process from processName
 */
Process *ConvertedModelData::createProcess(std::string processName){
	Process *ret = NULL;
	if(processName == "DiffusionProcess"){
		TRACE0("Create DiffusionProcess");
		ret = new DiffusionProcess();
	}
	else if(processName == "PeriodicBoundaryDiffusionProcess"){
		TRACE0("Create PeriodicBoundaryDiffusionProcess");
		ret = new PeriodicBoundaryDiffusionProcess();
	}
	else if(processName == "MassActionProcess"){
		TRACE0("Create MassActionProcess");
		ret = new MassActionProcess();
	}
	else if(processName == "VisualizationLogProcess"){
		TRACE0("Create VisualizationLogProcess");
		ret = new VisualizationLogProcess();
	}
	else if(processName == "OscillationAnalysisProcess"){
		TRACE0("Create OscillationAnalysisProcess");
		ret = new OscillationAnalysisProcess();
	}
	else if(processName == "SpatiocyteNextReactionProcess"){
		TRACE0("Create SpatiocyteNextReactionProcess");
		ret = new SpatiocyteNextReactionProcess();
	}
	else if(processName == "FluorescentProteinImagingProcess"){
		TRACE0("Create FluorescentProteinImagingProcess");
		ret = new FluorescentProteinImagingProcess();
	}
	else if(processName == "PolymerizationParameterProcess"){
		TRACE0("Create PolymerizationParameterProcess");
		ret = new PolymerizationParameterProcess();
	}
	else if(processName == "PolymerFragmentationProcess"){
		TRACE0("Create PolymerFragmentationProcess");
		ret = new PolymerFragmentationProcess();
	}else if(processName == "PolymerizationProcess"){
		TRACE0("Create PolymerizationProcess");
		ret = new PolymerizationProcess();
	}
	else if(processName == "DiffusionInfluencedReactionProcess"){
		TRACE0("Create DiffusionInfluencedReactionProcess");
		ret = new DiffusionInfluencedReactionProcess();
	}
	else if(processName == "MoleculePopulateProcess"){
		TRACE0("Create MoleculePopulateProcess");
		ret = new MoleculePopulateProcess();
	}
	else if(processName == "ReactionProcess"){
		TRACE0("Create ReactionProcess");
		ret = new ReactionProcess();
	}
	else if(processName == "SpatiocyteProcess"){
		TRACE0("Create SpatiocyteProcess");
		ret = new SpatiocyteProcess();
	}
	else{
		TRACE0("Not Exist such a process.");
	}
	return ret;
}




/**
 * Create Variable Reference Name
 */
std::string ConvertedModelData::createVariableReferenceName(int index){
	std::string name;
	// name is "___XXX" (XXX is Variable number with at least two-digit number.).
	if( index < 10 )
		name = "___0" + boost::lexical_cast<std::string>( index );
	else
		name = "___" + boost::lexical_cast<std::string>( index );

	return name;
}





