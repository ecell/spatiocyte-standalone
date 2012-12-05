//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//       This file is part of the E-Cell System
//
//       Copyright (C) 1996-2010 Keio University
//       Copyright (C) 2005-2009 The Molecular Sciences Institute
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//
// E-Cell System is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// E-Cell System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public
// License along with E-Cell System -- see the file COPYING.
// If not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
//END_HEADER
//
// written by Koichi Takahashi <shafi@e-cell.org>,
// E-Cell Project.
//
// modified for stand-alone (2011/02/01)
// Real -> double (2011/01/17)

#ifdef HAVE_CONFIG_H
#include "ecell_config.h"
#endif /* HAVE_CONFIG_H */

#include <algorithm>

// #include "Util.hpp" // comment out (2011/01/11)
#include "System.hpp"
#include "FullID.hpp"
#include "Model.hpp"
// #include "EntityType.hpp" // comment out (2011/01/11)

#include "Variable.hpp"

// comment out (2011/01/11)
//namespace libecs
//{
//
//LIBECS_DM_INIT_STATIC( Variable, Variable );


Variable::Variable()
    : 
    theValue( 0.0 ),    
    theLastTime( 0.0 ),
    theFixed( false ),
    theVariableName( "Variable" ) // add (2011/02/01)
{
    ; // do nothing
} 


Variable::~Variable()
{
}

// Delete because no file in spatiocyte use this function. (2011/01/12)
//void Variable::dispose()
//{
//    clearInterpolantVector();
//}

void Variable::preinitialize()
{
}

// Delete because no file in spatiocyte use this function. (2011/01/12)
//void Variable::initialize()
//{
//    clearInterpolantVector();
//}

// change code (2011/01/11)
void Variable::loadNumberConc( const double &value )
// LOAD_METHOD_DEF( Real, NumberConc, Variable )
{
    // Find the SIZE Variable by own.
    // Here, it assumes that System::findSizeVariable() of the supersystem 
    // of this Variable works correctly even at this stage of the model
    // loading.    In other words, properties of Entities of the model should be
    // loaded in the order from the root (/) to leaves, AND Value property
    // of the found SIZE Variable is already set.
    Variable const* aSizeVariable( getSuperSystem()->findSizeVariable() );
    
    setValue( value * aSizeVariable->getValue() );
}

// add from Entity (2011/02/01)
std::string Variable::asString() const
{
    return getVariableName() + "["
            + ( getSuperSystem() ? getFullID().asString():
                                 "unbound: " + getID() ) + "]";
}

// Delete because no file in Spatiocyte use this function (2011/01/11)
//void Variable::clearInterpolantVector()
//{
//    std::for_each( theInterpolantVector.begin(),
//                   theInterpolantVector.end(),
//                   DeletePtr< Interpolant >() );
//    theInterpolantVector.clear();
//}

// Delete because no file in Spatiocyte use this function (2011/01/11)
//void Variable::registerInterpolant( Interpolant* anInterpolantPtr )
//{
//    theInterpolantVector.push_back( anInterpolantPtr );
//}

// Delete because no file in Spatiocyte use this function (2011/01/11)
//void Variable::detach()
//{
//    clearInterpolantVector();
//    Entity::detach();
//}


// } // namespace libecs // comment out (2011/01/11)
