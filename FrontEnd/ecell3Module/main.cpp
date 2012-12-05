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
#define VOXEL_RADIUS	3e-8
#define SEARCH_VACANT	1

// value of Variables in main system
#define TYPE_MAINSYS	0
#define SHAPE		1
#define LENGTHX		0e-6
#define LENGTHY		1e-6
#define LENGTHZ		0e-6
#define SIZE		2.88e-15
#define SURFACEX	0
#define SURFACEY	0
#define SURFACEZ	0
#define ORIGINX		0
#define ORIGINY		0
#define ORIGINZ		0
#define VACANT		0
#define MIND_ATP	500
#define MIND_ADP	500
#define MINE		700

// value of Variables in sub system
#define TYPE_SUBSYS	1
#define LIPID		0
#define MIND_ATP_M	0
#define MIND_ATP_M_MINE	0
#define MIND_ADP_M_MINE	0
#define MINE_M		0

void inputVariable( vector< Variable* > &var, std::string ID, double Value );
void inputVariablesOfMainSystem( vector< Variable* > &var );
void inputVariablesOfSubSystem( vector< Variable* > &subvar );

void inputProcessesToMainSystem();
void inputProcessesToSubSystem();

void checkVariableInStepper();
void checkMainProcessInStepper();
void checkSubProcessInStepper();

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
    long int SearchVacant = SEARCH_VACANT;

    // Input values to SpatiocyteStepper
    stepperData->setID( ID );
    cout << stepperData->getID() << endl;

    stepperData->setVoxelRadius( VoxelRadius );
    cout << stepperData->getVoxelRadius() << endl;

    stepperData->setSearchVacant( SearchVacant );
    cout << stepperData->getSearchVacant() << endl;

    stepperData->setStepperName( "SpatiocyteStepper" );
    cout << stepperData->getStepperName() << endl;

    Model* modelData = new Model();
    //	define mainsystem
    System* sys = new System();
    // define subsystem
    System* subsys = new System();

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

    cout << "StepperID:" << sys->getStepperID() << endl;
    cout << "TYPE:" << sys->getVariable( "TYPE" )->getValue() << endl;
    cout << "SHAPE:" << sys->getVariable( "SHAPE" )->getValue() << endl;
    cout << "LENGTHX:" << sys->getVariable( "LENGTHX" )->getValue() << endl;
    cout << "LENGTHY:" << sys->getVariable( "LENGTHY" )->getValue() << endl;
    cout << "LENGTHZ:" << sys->getVariable( "LENGTHZ" )->getValue() << endl;
    cout << "SIZE:" << sys->getVariable( "SIZE" )->getValue() << endl;
    cout << "SURFACEX:" << sys->getVariable( "SURFACEX" )->getValue() << endl;
    cout << "SURFACEY:" << sys->getVariable( "SURFACEY" )->getValue() << endl;
    cout << "SURFACEZ:" << sys->getVariable( "SURFACEZ" )->getValue() << endl;
    cout << "ORIGINX:" << sys->getVariable( "ORIGINX" )->getValue() << endl;
    cout << "ORIGINY:" << sys->getVariable( "ORIGINY" )->getValue() << endl;
    cout << "ORIGINZ:" << sys->getVariable( "ORIGINZ" )->getValue() << endl;
    cout << "VACANT:" << sys->getVariable( "VACANT" )->getValue() << endl;
    cout << "MinD_ATP:" << sys->getVariable( "MinD_ATP" )->getValue() << endl;
    cout << "MinD_ADP:" << sys->getVariable( "MinD_ADP" )->getValue() << endl;
    cout << "MinE:" << sys->getVariable( "MinE" )->getValue() << endl;

    // Input Variables to subsystem (temporary code)
    vector< Variable* > subvar;
    inputVariablesOfSubSystem( subvar );
    for( int i = 0; i < ( int )subvar.size(); i++ )
        subsys->registerEntity( subvar[i] );

    //  Input StepperID to subystem
    subsys->setStepperID( stepperData->getID() );

    // Input ID to subsystem
    subsys->setID( "/Surface" );

    // Input subsystem to mainsystem
    sys->registerEntity( subsys );

    // Input Model to SpatiocyteStepper
    stepperData->setModel( modelData );

    // Input Processes to SpatiocyteStepper, main system, and sub system
    inputProcessesToMainSystem();
    inputProcessesToSubSystem();

    checkVariableInStepper();

    checkMainProcessInStepper();
    checkSubProcessInStepper();

    stepperData->initialize();

    checkVariableInStepper();

    checkMainProcessInStepper();
    checkSubProcessInStepper();

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
    inputVariable( var, "LENGTHX", LENGTHX );
    inputVariable( var, "LENGTHY", LENGTHY );
    inputVariable( var, "LENGTHZ", LENGTHZ );
    inputVariable( var, "SIZE", SIZE );
    inputVariable( var, "SURFACEX", SURFACEX );
    inputVariable( var, "SURFACEY", SURFACEY );
    inputVariable( var, "SURFACEZ", SURFACEZ );
    inputVariable( var, "ORIGINX", ORIGINX );
    inputVariable( var, "ORIGINY", ORIGINY );
    inputVariable( var, "ORIGINZ", ORIGINZ );
    inputVariable( var, "VACANT", VACANT );
    inputVariable( var, "MinD_ATP", MIND_ATP );
    inputVariable( var, "MinD_ADP", MIND_ADP );
    inputVariable( var, "MinE", MINE );
}

void inputVariablesOfSubSystem( vector< Variable* > &subvar )
{
    inputVariable( subvar, "TYPE", TYPE_SUBSYS );
    inputVariable( subvar, "LIPID", LIPID );
    inputVariable( subvar, "MinD_ATP_m", MIND_ATP_M );
    inputVariable( subvar, "MinD_ATP_m_MinE", MIND_ATP_M_MINE );
    inputVariable( subvar, "MinD_ADP_m_MinE", MIND_ADP_M_MINE );
    inputVariable( subvar, "MinE_m", MINE_M );
}

void inputProcessesToMainSystem()
{
    vector< VariableInProcess > varsInProcArray;
    vector< ParameterInProcess > paramsInProcArray;

    varsInProcArray.push_back( VariableInProcess( "MinD_ATP", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinD_ADP", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinE", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 2.5e-12 ) );
    ProcessSetting( "/", "DiffusionProcess", "diffuse", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ADP_m_MinE", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinE_m", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "LogInterval", 0.05 ) );
    paramsInProcArray.push_back( StringParameterInProcess( "FileName", "visualLog0.dat" ) );
    ProcessSetting( "/", "VisualizationLogProcess", "loggerMean", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "MinD_ADP", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinD_ATP", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 6 ) );
    ProcessSetting( "/", "SpatiocyteNextReactionProcess", "MinD_ADPtoMinD_ATP", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "MinD_ATP", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinD_ADP", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinE", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ADP_m_MinE", 0, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinE_m", 0, true ) );
    ProcessSetting( "/", "MoleculePopulateProcess", "populate", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
}

void inputProcessesToSubSystem()
{
    vector< VariableInProcess > varsInProcArray;
    vector< ParameterInProcess > paramsInProcArray;

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 1e-14 ) );
    ProcessSetting( "/Surface", "DiffusionProcess", "diffuseS", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinE_m", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 1e-14 ) );
    ProcessSetting( "/Surface", "DiffusionProcess", "diffusee", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 1e-14 ) );
    ProcessSetting( "/Surface", "DiffusionProcess", "diffuseSss", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ADP_m_MinE", 0, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "D", 1e-14 ) );
    ProcessSetting( "/Surface", "DiffusionProcess", "diffuseSs", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:LIPID", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinD_ATP", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 0.0125e-6 ) );
    ProcessSetting( "/Surface", "DiffusionInfluencedReactionProcess", "MinD_ATP_2_MinD_ATP_m", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "MinD_ATP", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", 1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 1.49448498e-20 ) );
    ProcessSetting( "/Surface", "DiffusionInfluencedReactionProcess", "MinD_ATP_mtoMinD_ATP_m_MinE", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinE", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 9.23259608e-20 ) );
    ProcessSetting( "/Surface", "DiffusionInfluencedReactionProcess", "MinD_ATP_m_MinEtoMinD_ATP_m_MinE", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinD_ADP", 1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinE", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 0.5 ) );
    ProcessSetting( "/Surface", "SpatiocyteNextReactionProcess", "MinD_ATP_m_MinEtoMinD_ADP_MinE", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ATP_m_MinE", 1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ADP_m_MinE", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 9.23259608e-15 ) );
    ProcessSetting( "/Surface", "DiffusionInfluencedReactionProcess", "polymerize2s", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinD_ADP_m_MinE", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "Surface:MinE_m", 1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinD_ADP", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 0.1 ) );
    ProcessSetting( "/Surface", "SpatiocyteNextReactionProcess", "MinD_ATP_mp22s", varsInProcArray, paramsInProcArray );
    varsInProcArray.clear();
    paramsInProcArray.clear();

    varsInProcArray.push_back( VariableInProcess( "Surface:MinE_m", -1, true ) );
    varsInProcArray.push_back( VariableInProcess( "MinE", 1, true ) );
    paramsInProcArray.push_back( DoubleParameterInProcess( "k", 0.1 ) );
    ProcessSetting( "/Surface", "SpatiocyteNextReactionProcess", "MinD_ATP_mp2s", varsInProcArray, paramsInProcArray );
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

        // pick up sub system from main system
        System::Systems::const_iterator i = aRootSystem->getSystems().begin();
        System* aSubSystem( i->second );

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
            // If ID doesn't contains ":", input Variable in main system to process.
            if( ID.find( ":", 0 ) == string::npos )
                proc->registerVariableReference( name, aRootSystem->getVariable( ID ), varsInProcArray[i].getCoefficient(), varsInProcArray[i].getAccessor() );
            // Else if ID contains ":", input Variable in sub system to process.
            else
            {
                ID.erase( 0, ID.find( ":", 0 ) + 1 );
                proc->registerVariableReference( name, aSubSystem->getVariable( ID ), varsInProcArray[i].getCoefficient(), varsInProcArray[i].getAccessor() );
            }
        }

        proc->setStepper( stepperData );
        proc->setSuperSystem( aRootSystem );
        proc->preinitialize();
        proc->initialize();

        stepperData->registerProcess( proc );
        if( systemID == "/" )
            aRootSystem->registerEntity( proc );
        else
            aSubSystem->registerEntity( proc );
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

    // pick up sub system from main system
    System::Systems::const_iterator i = aRootSystem->getSystems().begin();
    System* aSubSystem( i->second );

    ofstream fout( "out.txt" );

    if( !fout )
    {
        cout << "Cannot open file" << endl;
        return;
    }

    fout << "CurrentTime" << "\t";
    fout << "MinD_ATP" << "\t";
    fout << "MinD_ADP" << "\t";
    fout << "MinE" << "\t";
    fout << "Surface:MinD_ATP_m" << "\t";
    fout << "Surface:MinD_ATP_m_MinE" << "\t";
    fout << "Surface:MinD_ADP_m_MinE" << "\t";
    fout << "Surface:MinE_m" << endl;

    for( int i = 0; i < 48000; i++ )
    {
        stepperData->step();

        fout << stepperData->getCurrentTime() << "\t";
        fout << aRootSystem->getVariable("MinD_ATP")->getValue() << "\t";
        fout << aRootSystem->getVariable("MinD_ADP")->getValue() << "\t";
        fout << aRootSystem->getVariable("MinE")->getValue() << "\t";
        fout << aSubSystem->getVariable("MinD_ATP_m")->getValue() << "\t";
        fout << aSubSystem->getVariable("MinD_ATP_m_MinE")->getValue() << "\t";
        fout << aSubSystem->getVariable("MinD_ADP_m_MinE")->getValue() << "\t";
        fout << aSubSystem->getVariable("MinE_m")->getValue() << endl;

        if( i == 47999 )
        {
            cout << "CurrentTime:" << stepperData->getCurrentTime() << "\t";
            cout << "MinD_ADP_m_MinE:" << aSubSystem->getVariable("MinD_ADP_m_MinE")->getValue() << endl;
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
    BOOST_CHECK( aRootSystem->getVariable( "LENGTHX" )->getValue() == LENGTHX );
    BOOST_CHECK( aRootSystem->getVariable( "LENGTHY" )->getValue() == LENGTHY );
    BOOST_CHECK( aRootSystem->getVariable( "LENGTHZ" )->getValue() == LENGTHZ );
    BOOST_CHECK( aRootSystem->getVariable( "SIZE" )->getValue() == SIZE );
    BOOST_CHECK( aRootSystem->getVariable( "SURFACEX" )->getValue() == SURFACEX );
    BOOST_CHECK( aRootSystem->getVariable( "SURFACEY" )->getValue() == SURFACEY );
    BOOST_CHECK( aRootSystem->getVariable( "SURFACEZ" )->getValue() == SURFACEZ );
    BOOST_CHECK( aRootSystem->getVariable( "ORIGINX" )->getValue() == ORIGINX );
    BOOST_CHECK( aRootSystem->getVariable( "ORIGINY" )->getValue() == ORIGINY );
    BOOST_CHECK( aRootSystem->getVariable( "ORIGINZ" )->getValue() == ORIGINZ );
    BOOST_CHECK( aRootSystem->getVariable( "VACANT" )->getValue() == VACANT );
    BOOST_CHECK( aRootSystem->getVariable( "MinD_ATP" )->getValue() == MIND_ATP );
    BOOST_CHECK( aRootSystem->getVariable( "MinD_ADP" )->getValue() == MIND_ADP );
    BOOST_CHECK( aRootSystem->getVariable( "MinE" )->getValue() == MINE );

    // pick up sub system from main system
    System::Systems::const_iterator i = aRootSystem->getSystems().begin();
    System* aSubSystem( i->second );

    // check value of Variables in sub system
    BOOST_CHECK( aSubSystem->getVariable( "TYPE" )->getValue() == TYPE_SUBSYS );
    BOOST_CHECK( aSubSystem->getVariable( "LIPID" )->getValue() == LIPID );
    BOOST_CHECK( aSubSystem->getVariable( "MinD_ATP_m" )->getValue() == MIND_ATP_M );
    BOOST_CHECK( aSubSystem->getVariable( "MinD_ATP_m_MinE" )->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aSubSystem->getVariable( "MinD_ADP_m_MinE" )->getValue() == MIND_ADP_M_MINE );
    BOOST_CHECK( aSubSystem->getVariable( "MinE_m" )->getValue() == MINE_M );
}

void checkMainProcessInStepper()
{
    // pick up main system from SpatioStepper
    System* aRootSystem( stepperData->getModel()->getRootSystem() );

    // check value of Processes in main system
    Process* aProcess( aRootSystem->getProcess( "diffuse" ) );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 2.5e-12 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ADP );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MINE );

    aProcess = aRootSystem->getProcess( "loggerMean" );
    BOOST_CHECK( aProcess->getProcessName() == "VisualizationLogProcess" );
    BOOST_CHECK( reinterpret_cast< VisualizationLogProcess* >( aProcess )->getLogInterval() == 0.05 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ADP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MINE_M );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getVariable()->getValue() == MIND_ATP_M );

    aProcess = aRootSystem->getProcess( "MinD_ADPtoMinD_ATP" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 6 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ADP );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ATP );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );

    aProcess = aRootSystem->getProcess( "populate" );
    BOOST_CHECK( aProcess->getProcessName() == "MoleculePopulateProcess" );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ADP );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getVariable()->getValue() == MIND_ATP_M );
    BOOST_CHECK( aProcess->getVariableReference( "___04" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___05" ).getVariable()->getValue() == MIND_ADP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___06" ).getVariable()->getValue() == MINE_M );
}

void checkSubProcessInStepper()
{
    // pick up main system from SpatioStepper
    System* aRootSystem( stepperData->getModel()->getRootSystem() );

    // pick up sub system from main system
    System::Systems::const_iterator i = aRootSystem->getSystems().begin();
    System* aSubSystem( i->second );

    // check value of Processes in sub system
    Process* aProcess( aSubSystem->getProcess( "diffuseS" ) );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 1e-14 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP_M );

    aProcess = aSubSystem->getProcess( "diffusee" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 1e-14 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MINE_M );

    aProcess = aSubSystem->getProcess( "diffuseSss" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 1e-14 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP_M_MINE );

    aProcess = aSubSystem->getProcess( "diffuseSs" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionProcess* >( aProcess )->getD() == 1e-14 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ADP_M_MINE );

    aProcess = aSubSystem->getProcess( "MinD_ATP_2_MinD_ATP_m" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionInfluencedReactionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionInfluencedReactionProcess* >( aProcess )->getk() == 0.0125e-6 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == LIPID );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ATP );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MIND_ATP_M );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );

    aProcess = aSubSystem->getProcess( "MinD_ATP_mtoMinD_ATP_m_MinE" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionInfluencedReactionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionInfluencedReactionProcess* >( aProcess )->getk() == 1.49448498e-20 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ATP_M );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MIND_ATP_M );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getVariable()->getValue() == MIND_ATP_M );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getCoefficient() == 1 );

    aProcess = aSubSystem->getProcess( "MinD_ATP_m_MinEtoMinD_ATP_m_MinE" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionInfluencedReactionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionInfluencedReactionProcess* >( aProcess )->getk() == 9.23259608e-20 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP_M );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );

    aProcess = aSubSystem->getProcess( "MinD_ATP_m_MinEtoMinD_ADP_MinE" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 0.5 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ADP );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );

    aProcess = aSubSystem->getProcess( "polymerize2s" );
    BOOST_CHECK( aProcess->getProcessName() == "DiffusionInfluencedReactionProcess" );
    BOOST_CHECK( reinterpret_cast< DiffusionInfluencedReactionProcess* >( aProcess )->getk() == 9.23259608e-15 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MIND_ATP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getVariable()->getValue() == MIND_ADP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );
    BOOST_CHECK( aProcess->getVariableReference( "___03" ).getCoefficient() == 1 );

    aProcess = aSubSystem->getProcess( "MinD_ATP_mp22s" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 0.1 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MIND_ADP_M_MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MINE_M );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getVariable()->getValue() == MIND_ADP );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );
    BOOST_CHECK( aProcess->getVariableReference( "___02" ).getCoefficient() == 1 );

    aProcess = aSubSystem->getProcess( "MinD_ATP_mp2s" );
    BOOST_CHECK( aProcess->getProcessName() == "SpatiocyteNextReactionProcess" );
    BOOST_CHECK( reinterpret_cast< SpatiocyteNextReactionProcess* >( aProcess )->getk() == 0.1 );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getVariable()->getValue() == MINE_M );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getVariable()->getValue() == MINE );
    BOOST_CHECK( aProcess->getVariableReference( "___00" ).getCoefficient() == -1 );
    BOOST_CHECK( aProcess->getVariableReference( "___01" ).getCoefficient() == 1 );
}
