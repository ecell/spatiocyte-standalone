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
// modified for stand-alone (2011/01/17)
// String -> std::string (2011/01/11)
// Integer -> long int (2011/01/17)

#ifdef HAVE_CONFIG_H
#include "ecell_config.h"
#endif /* HAVE_CONFIG_H */

// #include "Util.hpp" // comment out (2011/01/11)

#include <boost/lexical_cast.hpp>// add for stringCast (2011/01/11)

#include "VariableReference.hpp"

#include "Exceptions.hpp" // add (2011/01/11)

// comment out (2011/01/11)
//namespace libecs
//{

const std::string VariableReference::ELLIPSIS_PREFIX( "___" );
const std::string VariableReference::DEFAULT_NAME( "_" );

long int VariableReference::getEllipsisNumber() const
{
    if( isEllipsisName() )
    {
        // change code like code in Util.hpp(2011/01/11)
        return boost::lexical_cast<long int>( theName.substr( 3 ) );
//        return stringCast<Integer>( theName.substr( 3 ) );
    }
    else
    {
        THROW_EXCEPTION( ValueError,
                         "VariableReference [" + theName
                         + "] is not an Ellipsis (which starts from '___')" );
    }
}

// } // namespace libecs // comment out (2011/01/11)
