#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include "DiffusionProcess.hpp"
#include "VisualizationLogProcess.hpp"
#include "SpatiocyteNextReactionProcess.hpp"
#include "MoleculePopulateProcess.hpp"
#include "DiffusionInfluencedReactionProcess.hpp"
#include "SpatiocyteStepper.hpp"
#include "libecs/Model.hpp"
#include "libecs/System.hpp"
#include "libecs/Variable.hpp"

#include <iostream>
#include <string>
#include "boost/lexical_cast.hpp"

// value in SpatiocyteStepper
#define STEPPER_ID	"SS"
#define VOXEL_RADIUS	3.5e-8
//#define SEARCH_VACANT	1

// value of Variables in main system
#define TYPE_MAINSYS	0
#define SHAPE		3
#define SIZE		1000e-15
#define LENGTHX		5e-6
#define LENGTHY		5e-6
#define LENGTHZ		5e-6
#define SURFACEX	0
#define SURFACEY	0
#define SURFACEZ	0
#define ORIGINX		0
#define ORIGINY		0
#define ORIGINZ		0
#define SOURCE		1
#define VACANT		0
#define A		200
#define B		100
#define C		300

// value of Variables in sub system
//#define TYPE_SUBSYS	1
//#define LIPID		0
//#define MIND_ATP_M	0
//#define MIND_ATP_M_MINE	0
//#define MIND_ADP_M_MINE	0
//#define MINE_M		0

void inputVariable( vector< Variable* > &var, std::string ID, double Value );
void inputVariablesOfMainSystem( vector< Variable* > &var );
// void inputVariablesOfSubSystem( vector< Variable* > &subvar );

void inputProcessesToMainSystem();
// void inputProcessesToSubSystem();

void checkVariableInStepper();
void checkMainProcessInStepper();
// void checkSubProcessInStepper();

class VariableInProcess
{
    std::string VariableID;
    long int aCoefficient;
    bool isAccessor;
public:
    VariableInProcess( std::string str, long int li, bool b )
    {
        VariableID = str;
        aCoefficient = li;
        isAccessor = b;
    }

    ~VariableInProcess() {}

    const std::string getID() const
    {
        return VariableID;
    }
    const long int getCoefficient() const
    {
        return aCoefficient;
    }
    const bool getAccessor() const
    {
        return isAccessor;
    }
};

class ParameterInProcess
{
protected:
    std::string name;
    double doubleValue;
    std::string stringValue;
public:
    ParameterInProcess() {}

    ~ParameterInProcess() {}

    const std::string getName() const
    {
        return name;
    }
};

class DoubleParameterInProcess : public ParameterInProcess
{
public:
    DoubleParameterInProcess( std::string str, double v )
    {
        name = str;
        doubleValue = v;
    }

    ~DoubleParameterInProcess() {}

    const double getValue() const
    {
        return doubleValue;
    }
};

class StringParameterInProcess : public ParameterInProcess
{
public:
    StringParameterInProcess( std::string str, std::string v )
    {
        name = str;
        stringValue = v;
    }

    ~StringParameterInProcess() {}

    const std::string getValue() const
    {
        return stringValue;
    }
};

int inputOriginalParamsToProcess
( Process* &proc,
  std::string procName,
  vector< ParameterInProcess > paramsInProcArray
);

void ProcessSetting
( std::string systemID,
  std::string procName,
  std::string procID,
  vector< VariableInProcess > varsInProcArray,
  vector< ParameterInProcess > paramsInProcArray
);

void run();

SpatiocyteStepper *stepperData = new SpatiocyteStepper();

BOOST_AUTO_TEST_CASE( SpatiocyteTest )
{
    stepperData->setRngSeed( "10" );

    std::string ID = STEPPER_ID;
    double VoxelRadius = VOXEL_RADIUS;
//    long int SearchVacant = SEARCH_VACANT;

    // Input values to SpatiocyteStepper
    stepperData->setID( ID );
    cout << stepperData->getID() << endl;

    stepperData->setVoxelRadius( VoxelRadius );
    cout << stepperData->getVoxelRadius() << endl;

//    stepperData->setSearchVacant( SearchVacant );
//    cout << stepperData->getSearchVacant() << endl;

    stepperData->setStepperName( "SpatiocyteStepper" );
    cout << stepperData->getStepperName() << endl;

    Model* modelData = new Model();
    //	define mainsystem
    System* sys = new System();
    // define subsystem
    // System* subsys = new System();

    modelData->theRootSystem = sys;
    sys->setStepperID( stepperData->getID() );
    cout << "StepperID in model:" << modelData->getRootSystem()->getStepperID() << endl;

    // Input StepperID to mainsystem
    sys->setStepperID( ID );

    // Input ID to mainsystem
    sys->setID( "/" );

    // Input Variables to mainsystem
    vector< Variable* > var;
    inputVariablesOfMainSystem( var );
    for( int i = 0; i < ( int )var.size(); i++ )
        sys->registerEntity( var[i] );

    sys->setSuperSystem( sys );

//    cout << "StepperID:" << sys->getStepperID() << endl;
//    cout << "TYPE:" << sys->getVariable( "TYPE" )->getValue() << endl;
//    cout << "SHAPE:" << sys->getVariable( "SHAPE" )->getValue() << endl;
//    cout << "LENGTHX:" << sys->getVariable( "LENGTHX" )->getValue() << endl;
//    cout << "LENGTHY:" << sys->getVariable( "LENGTHY" )->getValue() << endl;
//    cout << "LENGTHZ:" << sys->getVariable( "LENGTHZ" )->getValue() << endl;
//    cout << "SIZE:" << sys->getVariable( "SIZE" )->getValue() << endl;
//    cout << "SURFACEX:" << sys->getVariable( "SURFACEX" )->getValue() << endl;
//    cout << "SURFACEY:" << sys->getVariable( "SURFACEY" )->getValue() << endl;
//    cout << "SURFACEZ:" << sys->getVariable( "SURFACEZ" )->getValue() << endl;
//    cout << "ORIGINX:" << sys->getVariable( "ORIGINX" )->getValue() << endl;
//    cout << "ORIGINY:" << sys->getVariable( "ORIGINY" )->getValue() << endl;
//    cout << "ORIGINZ:" << sys->getVariable( "ORIGINZ" )->getValue() << endl;
//    cout << "VACANT:" << sys->getVariable( "VACANT" )->getValue() << endl;
//    cout << "MinD_ATP:" << sys->getVariable( "MinD_ATP" )->getValue() << endl;
//    cout << "MinD_ADP:" << sys->getVariable( "MinD_ADP" )->getValue() << endl;
//    cout << "MinE:" << sys->getVariable( "MinE" )->getValue() << endl;

    // Input Model to SpatiocyteStepper
    stepperData->setModel( modelData );

    // Input Processes to SpatiocyteStepper, main system, and sub system
    inputProcessesToMainSystem();

    checkVariableInStepper();

    checkMainProcessInStepper();

    stepperData->initialize();

    checkVariableInStepper();

    checkMainProcessInStepper();

    run();
}

void inputVariable( vector< Variable* > &var, std::string ID, double Value )
{
    Variable *v = new Variable();
    v->setID( ID );
    v->setValue( Value );
    var.push_back( v );
}

void inputVariablesOfMainSystem( vector< Variable* > &var )
{
    inputVariable( var, "TYPE", TYPE_MAINSYS );
    inputVariable( var, "SHAPE", SHAPE );
    inputVariable( var, "SIZE", SIZE );
    inputVariable( var, "LENGTHX", LENGTHX );
    inputVariable( var, "LENGTHY", LENGTHY );
    inputVariable( var, "LENGTHZ", LENGTHZ );
    inputVariable( var, "SURFACEX", SURFACEX );
    inputVariable( var, "SURFACEY", SURFACEY );
    inputVariable( var, "SURFACEZ", SURFACEZ );
    inputVariable( var, "ORIGINX", ORIGINX );
    inputVariable( var, "ORIGINY", ORIGINY );
    inputVariable( var, "ORIGINZ", ORIGINZ );
    inputVariable( var, "SOURCE", SOURCE );
    inputVariable( var, "VACANT", VACANT );
    inputVariable( var, "A", A );
    inputVariable( var, "B", B );
    inputVariable( var, "C", C );
}

void inputProcessesToMainSystem()
{
    vector< VariableInProcess > varsInProcArray;
    vector< ParameterInProcess > paramsInProcArray;

    varsInProcArray.push_back( VariableInProcess( "SOURCE", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "VACANT", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "A", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "B", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "C", 0, true ) );
    ProcessSetting( "/", "MoleculePopulateProcess", "populate", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "A", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 10e-12 ) );
    ProcessSetting( "/", "DiffusionProcess", "diffusion_A", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "B", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 1.0e-12 ) );
    ProcessSetting( "/", "DiffusionProcess", "diffusion_B", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "C", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 5.0e-12 ) );
    ProcessSetting( "/", "DiffusionProcess", "diffusion_C", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "SOURCE", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "SOURCE", 1, true ) );
    varsInProcArray.push_back( VariableInProcess( "C", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 20.0 ) );
    ProcessSetting( "/", "SpatiocyteNextReactionProcess", "synthesis_C", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "A", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "VACANT", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 1.0e-3 ) );
    ProcessSetting( "/", "SpatiocyteNextReactionProcess", "degradation_A", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "B", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "VACANT", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 1.0e-2 ) );
    ProcessSetting( "/", "SpatiocyteNextReactionProcess", "degradation_B", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "A", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "B", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "C", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 0.5e-20 ) );
    ProcessSetting( "/", "DiffusionInfluencedReactionProcess", "ABCreaction", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "C", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "A", 1, true ) );
    varsInProcArray.push_back( VariableInProcess( "B", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 0.5 ) );
    ProcessSetting( "/", "SpatiocyteNextReactionProcess", "Cdecay", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

}

int inputOriginalParamsToProcess
( Process* &proc,
  std::string procName,
  vector< ParameterInProcess > paramsInProcArray
)
{
    if( procName == "DiffusionProcess" )
    {
        proc = new DiffusionProcess();
        for( int i = 0; i < ( int )paramsInProcArray.size(); i++ )
        {
            if( paramsInProcArray[ i ].getName() == "D" )
            {
                double D = reinterpret_cast< DoubleParameterInProcess* >( &paramsInProcArray[ i ] )->getValue();
                reinterpret_cast< DiffusionProcess* >( proc )->setD( D );
            }
        }
    }
    else if( procName == "DiffusionInfluencedReactionProcess" )
    {
        proc = new DiffusionInfluencedReactionProcess();
        for( int i = 0; i < ( int )paramsInProcArray.size(); i++ )
        {
            if( paramsInProcArray[ i ].getName() == "k" )
            {
                double k = reinterpret_cast< DoubleParameterInProcess* >( &paramsInProcArray[ i ] )->getValue();
                reinterpret_cast< DiffusionInfluencedReactionProcess* >( proc )->setk( k );
            }
        }
    }
    else if( procName == "SpatiocyteNextReactionProcess" )
    {
        proc = new SpatiocyteNextReactionProcess();
        for( int i = 0; i < ( int )paramsInProcArray.size(); i++ )
        {
            if( paramsInProcArray[ i ].getName() == "k" )
            {
                double k = reinterpret_cast< DoubleParameterInProcess* >( &paramsInProcArray[ i ] )->getValue();
                reinterpret_cast< SpatiocyteNextReactionProcess* >( proc )->setk( k );
            }
        }
    }
    else if( procName == "VisualizationLogProcess" )
    {
        proc = new VisualizationLogProcess();
        for( int i = 0; i < ( int )paramsInProcArray.size(); i++ )
        {
            if( paramsInProcArray[ i ].getName() == "LogInterval" )
            {
                double LogInterval = reinterpret_cast< DoubleParameterInProcess* >( &paramsInProcArray[ i ] )->getValue();
                reinterpret_cast< VisualizationLogProcess* >( proc )->setLogInterval( LogInterval );
            }
            else if( paramsInProcArray[ i ].getName() == "FileName" )
            {
                std::string FileName = reinterpret_cast< StringParameterInProcess* >( &paramsInProcArray[ i ] )->getValue();
                reinterpret_cast< VisualizationLogProcess* >( proc )->setFileName( FileName );
            }
        }
    }
    else if( procName == "MoleculePopulateProcess" )
        proc = new MoleculePopulateProcess();
    else
        return 0;

    return 1;
}

void ProcessSetting
( std::string systemID,
  std::string procName,
  std::string procID,
  vector< VariableInProcess > varsInProcArray,
  vector< ParameterInProcess > paramsInProcArray
)
{
    try
    {
        Process* proc;

        // pick up main system from SpatioStepper
        System* aRootSystem( stepperData->getModel()->getRootSystem() );

        // input original parameters to each Process
        if( !inputOriginalParamsToProcess( proc, procName, paramsInProcArray ) )
            throw std::exception();

        // input common parameters to each Process
        proc->setID( procID );
        proc->setProcessName( procName );
        for( int i = 0; i < ( int )varsInProcArray.size(); i++ )
        {
            std::string name;
            // name is "___XXX" (XXX is Variable number with at least two-digit number.).
            if( i < 10 )
                name = "___0" + boost::lexical_cast<std::string>( i );
            else
                name = "___" + boost::lexical_cast<std::string>( i );

            std::string ID = varsInProcArray[i].getID();
            // input Variable in main system to process.
            proc->registerVariableReference( name, aRootSystem->getVariable( ID ), varsInProcArray[i].getCoefficient(), varsInProcArray[i].getAccessor() );
        }

        proc->setStepper( stepperData );
        proc->setSuperSystem( aRootSystem );
        proc->preinitialize();
        proc->initialize();

        stepperData->registerProcess( proc );
        aRootSystem->registerEntity( proc );
    }
    catch( std::exception )
    {
        cout << "Invalid process name was input." << endl;
    }
}

void run()
{
    // pick up main system from SpatioStepper
    System* aRootSystem( stepperData->getModel()->getRootSystem() );

//    // pick up sub system from main system
//    System::Systems::const_iterator i = aRootSystem->getSystems().begin();
//    System* aSubSystem( i->second );

    ofstream fout( "out.txt" );

    if( !fout )
    {
        cout << "Cannot open file" << endl;
        return;
    }

    fout << "CurrentTime" << "\t";
    fout << "A" << "\t";
    fout << "B" << "\t";
    fout << "C" <<  endl;

//    double theTime = 0.0;

    for( int i = 0; i < 65536; i++ )
    {
        stepperData->step();

        fout << stepperData->getCurrentTime() << "\t";
        fout << aRootSystem->getVariable("A")->getValue() << "\t";
        fout << aRootSystem->getVariable("B")->getValue() << "\t";
        fout << aRootSystem->getVariable("C")->getValue() << endl;

        if( i == 65535 )
        {
            cout << "CurrentTime:" << stepperData->getCurrentTime() << "\t";
            cout << "A:" << aRootSystem->getVariable("A")->getValue() << "\t";
            cout << "B:" << aRootSystem->getVariable("B")->getValue() << "\t";
            cout << "C:" << aRootSystem->getVariable("C")->getValue() << endl;
        }
    }

    fout.close();
}

void checkVariableInStepper()
{
    // pick up main system from SpatioStepper
    System* aRootSystem( stepperData->getModel()->getRootSystem() );

    // check value of Variables in main system
    BOOST_CHECK( aRootSystem->getStepperID() == STEPPER_ID );
    BOOST_CHECK( aRootSystem->getVariable( "TYPE" )->getValue() == TYPE_MAINSYS );
    BOOST_CHECK( aRootSystem->getVariable( "SHAPE" )->getValue() == SHAPE );
    BOOST_CHECK( aRootSystem->getVariable( "SIZE" )->getValue() == SIZE );
    BOOST_CHECK( aRootSystem->getVariable( "LENGTHX" )->getValue() == LENGTHX );
    BOOST_CHECK( aRootSystem->getVariable( "LENGTHY" )->getValue() == LENGTHY );
    BOOST_CHECK( aRootSystem->getVariable( "LENGTHZ" )->getValue() == LENGTHZ );
    BOOST_CHECK( aRootSystem->getVariable( "SURFACEX" )->getValue() == SURFACEX );
    BOOST_CHECK( aRootSystem->getVariable( "SURFACEY" )->getValue() == SURFACEY );
    BOOST_CHECK( aRootSystem->getVariable( "SURFACEZ" )->getValue() == SURFACEZ );
    BOOST_CHECK( aRootSystem->getVariable( "ORIGINX" )->getValue() == ORIGINX );
    BOOST_CHECK( aRootSystem->getVariable( "ORIGINY" )->getValue() == ORIGINY );
    BOOST_CHECK( aRootSystem->getVariable( "ORIGINZ" )->getValue() == ORIGINZ );
    BOOST_CHECK( aRootSystem->getVariable( "SOURCE" )->getValue() == SOURCE );
    BOOST_CHECK( aRootSystem->getVariable( "VACANT" )->getValue() == VACANT );
    BOOST_CHECK( aRootSystem->getVariable( "A" )->getValue() == A );
    BOOST_CHECK( aRootSystem->getVariable( "B" )->getValue() == B );
    BOOST_CHECK( aRootSystem->getVariable( "C" )->getValue() == C );
}

void checkMainProcessInStepper()
{
    // pick up main system from SpatioStepper
    System* aRootSystem( stepperData->getModel()->getRootSystem() );

    // check value of Processes in main system
    Process* aProcess( aRootSystem->getProcess( "populate" ) );
    BOOST_CHECK( aProcess->getProcessName() == "MoleculePopulateProcess" );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == SOURCE );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == VACANT );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == A );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getVariable()->getValue() == B );
    BOOST_CHECK( aProcess->getVariableReference( "___04" ).getVariable()->getValue() == C );

    aProcess = aRootSystem->getProcess( "diffusion_A" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 10e-12 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == A );

    aProcess = aRootSystem->getProcess( "diffusion_B" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 1.0e-12 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == B );

    aProcess = aRootSystem->getProcess( "diffusion_C" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 5.0e-12 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == C );

    aProcess = aRootSystem->getProcess( "synthesis_C" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 20.0 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == SOURCE );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == SOURCE );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == C );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );

    aProcess = aRootSystem->getProcess( "degradation_A" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 1.0e-3 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == A );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == VACANT );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );

    aProcess = aRootSystem->getProcess( "degradation_B" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 1.0e-2 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == B );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == VACANT );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );

    aProcess = aRootSystem->getProcess( "ABCreaction" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionInfluencedReactionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionInfluencedReactionProcess* >( aProcess )->getk() == 0.5e-20 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == A );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == B );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == C );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );

    aProcess = aRootSystem->getProcess( "Cdecay" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 0.5 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == C );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == A );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == B );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );
}

