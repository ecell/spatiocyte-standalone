#ifndef _CONVERTEDMODELDATA_H_
#define _CONVERTEDMODELDATA_H_

#include <typeinfo>
#include "boost/lexical_cast.hpp"

#include "../intermediate/util/Macro.h"

#include "WrapperSystem.h"
#include "../../ecell3Module/SpatiocyteStepper.hpp"
#include "../../ecell3Module/libecs/System.hpp"
#include "../../ecell3Module/libecs/Process.hpp"
#include "../../ecell3Module/DiffusionProcess.hpp"
#include "../../ecell3Module/PeriodicBoundaryDiffusionProcess.hpp"
#include "../../ecell3Module/MassActionProcess.hpp"
#include "../../ecell3Module/VisualizationLogProcess.hpp"
#include "../../ecell3Module/OscillationAnalysisProcess.hpp"
#include "../../ecell3Module/SpatiocyteNextReactionProcess.hpp"
#include "../../ecell3Module/FluorescentProteinImagingProcess.hpp"
#include "../../ecell3Module/PolymerizationParameterProcess.hpp"
#include "../../ecell3Module/SpatiocyteStepper.hpp"
#include "../../ecell3Module/PolymerFragmentationProcess.hpp"
#include "../../ecell3Module/PolymerizationProcess.hpp"
#include "../../ecell3Module/DiffusionInfluencedReactionProcess.hpp"
#include "../../ecell3Module/MoleculePopulateProcess.hpp"
#include "../../ecell3Module/ReactionProcess.hpp"
#include "../../ecell3Module/SpatiocyteProcess.hpp"

class ConvertedModelData{
public:
	ConvertedModelData();
	~ConvertedModelData();

	void initialize();
	void destroy();

	/** Stepper Data */
	SpatiocyteStepper *stepperData;
	/** Model Data */
	Model* modelData;

	/**
	 * Get WrapperSystem
	 */
	WrapperSystem *getWrapperSystemAt(int index);

	/**
	 * Get length of wrapSystemList
	 */
	int getWrapSystemListLength();

	/**
	 * Get System
	 */
	System *getConvertedSystemAt(int index);

	/**
	 * Get length of convertedSystemList
	 */
	int getConvertedSystemListLength();


	/**
	 * Set Stepper Data to buffer
	 */
	void inputStepperData(std::string Id,double voxelRadius,long int searchVacant,std::string stepperName,std::string rngSeed);

	/**
	 * Convert WrapperStepper Data to SpatiocyteStepper Data
	 */
	void convertStepperData();

	/**
	 * Create WrapperSystem Data and add to list
	 */
	void addSystemData(std::string sysId,std::string propertyName);

	/**
	 * Initialize Converted SystemList
	 */
	void initializeSystemList();

	/**
	 * Convert Wrapper System Data to System Data
	 */
	void convertSystemList();

	/**
	 * Convert Wrapper Process Data to Process Data
	 */
	void convertProcessList();

private:

	/** WrapperSystemList */
	std::vector<WrapperSystem*> wrapSystemList;

	/** Converted SystemList */
	std::vector<System*> convertedSystemList;

	/**
	 * converted Variable list
	 */
	std::vector<Variable*> convertedVariables;
	std::vector<std::string*> variablesRelation;	//	Backup relation between Variable and SystemID


	/**
	 * Stepper Data
	 */
	std::string ID;
	double voxelRadius;
	long int searchVacant;
	std::string stepperName;
	std::string rngSeed;

	enum{
		MAIN_SYSTEM = 0,
		SUB_SYSTEM = 1
	};


	/**
	 * Create Process from processName
	 */
	static Process *createProcess(std::string processName);

	/**
	 * Create Variable Reference Name
	 */
	static std::string createVariableReferenceName(int index);

};

#endif

