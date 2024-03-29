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
// modified for stand-alone (2011/01/07)
// String -> std::string (2011/01/07)


#include "SpatiocyteProcess.hpp"
#include "SpatiocyteSpecies.hpp"

// LIBECS_DM_INIT(SpatiocyteProcess, Process); // comment out (2010/12/27)

const std::string SpatiocyteProcess::getIDString(Voxel* aVoxel) const
{
  Variable* aVariable(theSpecies[aVoxel->id]->getVariable());
  return "["+aVariable->getSystemPath().asString()+":"+aVariable->getID()+"]";
}

const std::string SpatiocyteProcess::getIDString(Species* aSpecies) const
{
  Variable* aVariable(aSpecies->getVariable());
  return "["+aVariable->getSystemPath().asString()+":"+aVariable->getID()+"]";
}
