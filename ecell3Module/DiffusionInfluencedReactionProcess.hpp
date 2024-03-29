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
// modified for stand-alone (2011/02/01)
// String -> std::string (2011/01/07)

#ifndef __DiffusionInfluencedReactionProcess_hpp
#define __DiffusionInfluencedReactionProcess_hpp

#include "ReactionProcess.hpp"

#include "libecs/Exceptions.hpp" // add (2011/01/11)

// change code (2010/12/22)
class DiffusionInfluencedReactionProcess : public ReactionProcess
{ 
// LIBECS_DM_CLASS(DiffusionInfluencedReactionProcess, ReactionProcess)
// { 
public:
//   LIBECS_DM_OBJECT(DiffusionInfluencedReactionProcess, Process)
//     {
//       INHERIT_PROPERTIES(ReactionProcess);
//     }
  DiffusionInfluencedReactionProcess() {}
  virtual ~DiffusionInfluencedReactionProcess() {}
  virtual void addSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule) {}
  virtual void removeSubstrateInterrupt(Species* aSpecies, Voxel* aMolecule) {}
  virtual void initialize()
    {
      if(isInitialized)
        {
          return;
        }
      ReactionProcess::initialize();
      if(getOrder() != 2)
        {
               // change code (2011/02/01)
               THROW_EXCEPTION(ValueError, 
                               std::string( getProcessName() ) + 
                               "[" + getFullID().asString() + 
//               THROW_EXCEPTION(ValueError, 
//                               std::string(getPropertyInterface().getClassName()) + 
//                               "[" + getFullID().asString() + 
                               "]: Only second order scheme is allowed for "+
                               "diffusion influenced reactions.");
        }
    }
  virtual void initializeSecond();
  virtual void initializeThird();
  virtual bool react(Voxel*, Voxel**);
  virtual void printParameters();
  virtual void finalizeReaction();
protected:
  void calculateReactionProbability();
protected:
  double D_A;
  double D_B;
  double r_v;
  double V;
};

#endif /* __DiffusionInfluencedReactionProcess_hpp */
