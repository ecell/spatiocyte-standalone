//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of E-Cell Simulation Environment package
//
//                Copyright (C) 2006-2009 Keio University
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//
// E-Cell is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// E-Cell is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public
// License along with E-Cell -- see the file COPYING.
// If not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
//END_HEADER
//
// written by Satya Arjunan <satya.arjunan@gmail.com>
// E-Cell Project, Institute for Advanced Biosciences, Keio University.
//
// modified for stand-alone (2011/02/14)
// Integer -> long int (2011/01/17)
// Real -> double (2011/01/17)
// Time -> double (2011/02/03)

#ifndef __SpatiocyteStepper_hpp
#define __SpatiocyteStepper_hpp

#include "libecs/Stepper.hpp"// change code (2010/12/24)
// #include <Stepper.hpp>
#include "SpatiocyteCommon.hpp"

class SpatiocyteStepper : public Stepper// change code (2010/12/22)
{
//LIBECS_DM_CLASS(SpatiocyteStepper, Stepper)
//{ 
public: 
//  LIBECS_DM_OBJECT(SpatiocyteStepper, Stepper)
//    {
//      INHERIT_PROPERTIES(Stepper);
//      PROPERTYSLOT_SET_GET(Real, VoxelRadius);
//      PROPERTYSLOT_SET_GET(Integer, SearchVacant);
//    }
    // change code (2010/12/22)
    void setVoxelRadius( double &value )
      {
        VoxelRadius = value;
      }
    const double getVoxelRadius() const
      {
        return VoxelRadius;
      }
//  SIMPLE_SET_GET_METHOD(Real, VoxelRadius); 

    // change code (2010/12/22)
    void setSearchVacant( long int &value )
      {
        SearchVacant = value;
      }
    const long int getSearchVacant() const
      {
        return SearchVacant;
      }
//  SIMPLE_SET_GET_METHOD(Integer, SearchVacant); 
  SpatiocyteStepper():
    isInitialized(false),
    isPeriodicEdge(false),
    SearchVacant(true),
    VoxelRadius(0.5),
    theNormalizedVoxelRadius(0.5) {}
  virtual ~SpatiocyteStepper() {}
  virtual void initialize();
  // need to check interrupt when we suddenly stop the simulation, do we
  // need to update the priority queue?
  virtual void interrupt(double) {}
  virtual void step();
  void createSpecies();
  Species* addSpecies(Variable*);
  Species* getSpecies(Variable*);
  vector<Species*> getSpecies();
  Point coord2point(unsigned int);
  void optimizeSurfaceVoxel(Voxel*, Compartment*);
  void setSurfaceSubunit(Voxel*, Compartment*);
  Species* id2species(unsigned short);
  Compartment* id2compartment(unsigned short);
  Voxel* coord2voxel(unsigned int);
  Compartment* system2compartment(System*);
  bool isBoundaryCoord(unsigned int, bool);
  Voxel* getPeriodicVoxel(unsigned int, bool, Origin*);
  Point getPeriodicPoint(unsigned int, bool, Origin*);
  void checkLattice();
  void setPeriodicEdge();
//  void reset(int); // comment out for test (2011/02/14)
  unsigned int getStartCoord();
  unsigned int getRowSize();
  unsigned int getLayerSize();
  unsigned int getColSize();
  unsigned int getLatticeSize();
  Point getCenterPoint();
  double getNormalizedVoxelRadius();
  Voxel* point2voxel(Point);
private:
  void removeSurfaces(Compartment*);
  void setReactiveCompartments(Compartment*);
  void printProcessParameters();
  void checkSurfaceCompartment();
  void shuffleAdjoiningVoxels();
  void setLatticeProperties();
  void initPriorityQueue();
  void initProcessSecond();
  void initProcessThird();
  void initProcessFourth();
  void initProcessLastOnce();
  void storeSimulationParameters();
  void printSimulationParameters();
  void setSurfaceVoxelProperties();
  void initSpecies();
  void readjustSurfaceBoundarySizes();
  void constructLattice();
  void compartmentalizeLattice();
  void concatenatePeriodicSurfaces();
  void registerCompartments();
  void setCompartmentsProperties();
  void populateCompartments();
  void clearCompartments();
  void clearCompartment(Compartment*);
  void populateCompartment(Compartment*);
  void populateCompartmentUniformly(Compartment*);
  void registerCompartmentSpecies(Compartment*);
  void setCompartmentProperties(Compartment*);
  void removePeriodicEdgeVoxels(Compartment*);
  void populateCompartmentUniformly(Compartment*, unsigned int);
  void concatenateVoxel(Voxel*, unsigned int, unsigned int, unsigned int);
  void concatenateLayers(Voxel*, unsigned int, unsigned int, unsigned int);
  void concatenateRows(Voxel*, unsigned int, unsigned int, unsigned int);
  void concatenateCols(Voxel*, unsigned int, unsigned int, unsigned int);
  void coord2global(unsigned int, unsigned int*, unsigned int*, unsigned int*);
  void replaceVoxel(Voxel*, Voxel*);
  void replaceUniVoxel(Voxel*, Voxel*);
  bool isRemovableEdgeCoord(unsigned int, Compartment*);
  bool isReactiveCompartment(Compartment*, Compartment*);
  bool isInsideCoord(unsigned int, Compartment*, double);
  bool isPeriodicEdgeCoord(unsigned int, Compartment*);
  bool isSurfaceVoxel(Voxel*, Compartment*);
  bool compartmentalizeVoxel(Voxel*, Compartment*);
  unsigned int coord2row(unsigned int);
  unsigned int coord2col(unsigned int);
  unsigned int coord2layer(unsigned int);
  Compartment* registerCompartment(System*, vector<Compartment*>*);
private:
  bool isInitialized;
  bool isPeriodicEdge;
  bool SearchVacant;
  unsigned short theNullID;
  unsigned int theCellShape;
  unsigned int theStartCoord;
  unsigned int theRowSize;
  unsigned int theColSize;
  unsigned int theLayerSize;
  unsigned int theBioSpeciesSize;
  double VoxelRadius; //r_v
  double theNormalizedVoxelRadius;
  double theHCPk;
  double theHCPh;
  double theHCPl;
  Point theCenterPoint;
  ProcessPriorityQueue thePriorityQueue; 
  vector<Species*>::iterator variable2species(Variable*);
  vector<Species*> theSpecies;
  vector<Compartment*> theCompartments;
  vector<Voxel> theLattice;
};

#endif /* __SpatiocyteStepper_hpp */

