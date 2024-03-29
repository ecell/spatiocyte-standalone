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
// modified for stand-alone (2011/01/17)
// Real -> double (2011/01/17)

#ifndef __PolymerizationProcess_hpp
#define __PolymerizationProcess_hpp

#include "DiffusionInfluencedReactionProcess.hpp"
#include "PolymerFragmentationProcess.hpp"
#include "SpatiocytePolymer.hpp"

// change code (2010/12/22)
class PolymerizationProcess : public DiffusionInfluencedReactionProcess
{ 
// LIBECS_DM_CLASS(PolymerizationProcess, DiffusionInfluencedReactionProcess)
// { 
public:
// comment out (2010/12/22)
//   LIBECS_DM_OBJECT(PolymerizationProcess, Process)
//     {
//       INHERIT_PROPERTIES(DiffusionInfluencedReactionProcess);
//       PROPERTYSLOT_SET_GET(Real, BendAngle);
//       PROPERTYSLOT_SET_GET(Real, CylinderYaw);
//       PROPERTYSLOT_SET_GET(Real, SphereYaw);
//     }
  // change code (2010/12/22)
  void setBendAngle( double &value )
    {
      BendAngle = value;
    }
  const double getBendAngle() const
    {
      return BendAngle;
    }
//  SIMPLE_SET_GET_METHOD(Real, BendAngle);

  // change code (2010/12/22)
  void setCylinderYaw( double &value )
    {
      CylinderYaw = value;
    }
  const double getCylinderYaw() const
    {
      return CylinderYaw;
    }
//  SIMPLE_SET_GET_METHOD(Real, CylinderYaw);

  // change code (2010/12/22)
  void setSphereYaw( double &value )
    {
      SphereYaw = value;
    }
  const double getSphereYaw() const
    {
      return SphereYaw;
    }
//  SIMPLE_SET_GET_METHOD(Real, SphereYaw);
  PolymerizationProcess():
    BendAngle(0),
    CylinderYaw(0),
    SphereYaw(0),
    theMonomerLength(1) {}
  virtual ~PolymerizationProcess() {}
  virtual void initialize()
    {
      DiffusionInfluencedReactionProcess::initialize();
    }
  virtual void initializeThird();
  virtual bool react(Voxel*, Voxel**);
  virtual bool isInterrupting(Process*);
  virtual void finalizeReaction();
  void resetSubunit(Subunit*);
  void removeContPoint(Subunit*,  Point*);

protected:
  void initSubunits(Species*);
  void initSubunit(Voxel*, Species*);
  void initJoinSubunit(Voxel*, Species*, Subunit*);
  void addContPoint(Subunit*,  Point*);
  void updateSharedLipidsID(Voxel*);
  void removeLipid(Subunit*, unsigned int);
  double setImmediateTargetVoxel(Subunit*, unsigned int);
  bool setExtendedTargetVoxel(Subunit*, unsigned int, double);
  int setExistingTargetVoxel(Subunit*, unsigned int);
  bool setTargetVoxels(Subunit*);
  void removeUnboundTargetVoxels(Subunit*);
  Voxel* setNewTargetVoxel(Subunit*, unsigned int);
  unsigned int sortNearestTargetVoxels(Subunit*, unsigned int, vector<Voxel*>&,
                                       vector<double>&, vector<int>&);
  int getLocation(double x)
    {
      if(x > theMinX && x < theMaxX)
        {
          return CYLINDRICAL;
        }
      return SPHERICAL;
    }
  virtual void pushNewBend(Subunit*, double);
  virtual void pushJoinBend(Subunit*, Subunit*, unsigned int);
  virtual Bend* getNewReverseBend(Point*, Point*, Bend*);
  virtual Bend* getReverseBend(Bend*);
  virtual Point getNextPoint(Point*, Bend*); 
  virtual void getCylinderDcm(double, double, double*);
  virtual void getSphereDcm(double, double, double*);
  virtual void initSphereDcm();
  virtual void pinStep(double*, double*, double*, double*);
protected:
  unsigned int theBendIndexA;
  unsigned int theBendIndexB;
  double BendAngle;
  double CylinderYaw;
  double SphereYaw;
  double theInitSphereDcm[9];
  double theRadius;
  double theMinX;
  double theMaxX;
  double theOriY;
  double theOriZ;
  double theMonomerLength;
};

#endif /* __PolymerizationProcess_hpp */
