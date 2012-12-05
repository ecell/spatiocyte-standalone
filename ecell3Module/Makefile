LIBS = -L/usr/lib -lgsl -lgslcblas -lm

BOOST_LIBS = -lboost_unit_test_framework

# add path "/libecs"
VPATH = libecs

DMS=\
PeriodicBoundaryDiffusionProcess\
MassActionProcess\
VisualizationLogProcess\
OscillationAnalysisProcess\
SpatiocyteNextReactionProcess\
FluorescentProteinImagingProcess\
PolymerizationParameterProcess\
DiffusionProcess\
SpatiocyteStepper\
PolymerFragmentationProcess\
PolymerizationProcess\
DiffusionInfluencedReactionProcess\
MoleculePopulateProcess\
ReactionProcess\
SpatiocyteProcess

OBJECTS=${DMS:=.o}

LIBECSNAME=\
Process\
VariableReference\
Variable\
System\
Stepper\
Model\
Entity\
FullID\
Exceptions

LIBECSOBJ=${LIBECSNAME:=.o}

# make exe file from all object files
allobject: main.cpp $(OBJECTS) $(LIBECSOBJ)
	g++ -o main $(LIBS) $(BOOST_LIBS) main.cpp $(OBJECTS) $(LIBECSOBJ)

# check exe file
check:
	./main

simple_spatiocyte: main_simple_spatiocyte.cpp $(OBJECTS) $(LIBECSOBJ)
	g++ -o simple_spatiocyte $(LIBS) $(BOOST_LIBS) main_simple_spatiocyte.cpp $(OBJECTS) $(LIBECSOBJ)

simple_compartment: main_simple_compartment.cpp $(OBJECTS) $(LIBECSOBJ)
	g++ -o simple_compartment $(LIBS) $(BOOST_LIBS) main_simple_compartment.cpp $(OBJECTS) $(LIBECSOBJ)

# compile files in Spatiocyte
Spatiocyte: $(OBJECTS)

# compile files in libecs
libecs: $(LIBECSOBJ)

%.o: %.cpp
	g++ -c $<

# clean object files in libecs
cleanlibecs:
	rm -f $(LIBECSOBJ)

# clean object files in Spatiocyte
cleanSpatiocyte:
	rm -f $(OBJECTS)

# clean all object files
clean:
	rm -f $(OBJECTS)
	rm -f $(LIBECSOBJ)

