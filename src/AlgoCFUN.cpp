/***************************************************************************
 *   Copyright (C) 2000-2010 by Noel Novelli                               *
 *   novelli@lif.univ-mrs.fr                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#include <psapi.h>  //  Add Psapi.lib in VCProject or in LDFLAGS
#else
#include <malloc.h>
#endif
//
#include "AlgoCFUN.h"
#include "Trace.h"
#include "LevelCFUN.h"
#include "PartitionCFUN.h"
#include "Relation.h"
//

extern string gbl_sOutputFileName;

///////////////////////////////////////////////////////////////////
//
//                          C F U N
//
//
//  L_0 := < \emptyset, Count( \emptyset ), \emptyset, \emptyset >
//  L_1 := { < A, Count( A ), A, A > | A \in R }
//
//  for ( k := 1; L_k \not = \emptyset; k++ )
//      Closure( L_k, L_{k-1} )
//      QuasiClosure( L_{k-1}, L_k )
//      DisplayCFD( L_{k-1} )
//      Prune( L_k )
//      L_{k+1} := GenerateNextLevel( L_k )
//  end for
//  Closure( L_k, L_{k-1} )
//  DisplayCFD( L_{k-1} )
//
///////////////////////////////////////////////////////////////////

vector<CFD> cfun( const Relation &relation,
				unsigned int uMaxLevel,
				bool bPrune)
{

    if ( ! LevelCFUN::InitClass( relation.getNbCol() ) ) {
		cerr << "CLevel::InitClass deja  realisee" << endl;
		exit( -2 );
	}
	
    if ( uMaxLevel < 2 ) {
       uMaxLevel = relation.getNbCol() + 2;  // All in RAM [0..#attrbiut+1]
    }
    LevelCFUN *levels = new LevelCFUN[uMaxLevel];

	    //  L_0 := < \emptyset, 0, \emptyset, \emptyset >
	LevelCFUN &level0 = levels[0];
    level0.m_uNbAttr = 1;
    if ( ! level0.Init( level0.m_uNbAttr ) ) {
		cerr << "Allocation error CListePartition.m_pListeAttr" << endl;
		exit( -5 );
	}
    level0.m_pListeAttr[0].init0( );
	level0.m_pListeAttr[0].initQuasiAndClosureCFD( level0.m_pListeAttr[0].m_Attr, level0.m_uNbAttr );
    level0.m_pListeAttr[0].m_uCount = 1;
    level0.m_pListeAttr[0].m_bFreeSet = true;
    
		//  L_1 := { < A, Count( A ), A, A > | A \in R }
	LevelCFUN &level1 = levels[1];
	level1.m_uNbAttr = relation.getNbCol();
    if ( ! level1.Init( level1.m_uNbAttr ) ) {
		cerr << "Allocation error CListePartition.m_pListeAttr" << endl;
		exit( -5 );
	}
    level1.GetAllPartitionFromRelation( relation );

        //  for ( k := 1; L_k \not = \emptyset; k++ )
    unsigned int k;
    
    for ( k = 1; levels[k % uMaxLevel].m_uNbAttr > 0; k++ ) {

        levels[(k-1) % uMaxLevel].closure( levels[k % uMaxLevel], relation );

        levels[k % uMaxLevel].quasiClosure( levels[(k-1) % uMaxLevel] );
		
		levels[(k-1) % uMaxLevel].displayCFD( );

		if ( bPrune ) {
			levels[k % uMaxLevel].PurePrune( );
		}

        if ( ! levels[(k+1) % uMaxLevel].Init( 10 ) ) {
			cerr << "Allocation error CListePartition.m_pListeAttr" << endl;
			exit( -5 );
		}
        levels[(k+1) % uMaxLevel].GenerateNextLevel( levels[k % uMaxLevel], bPrune );
    }
	levels[(k-1) % uMaxLevel].closure( levels[k % uMaxLevel], relation );

	levels[(k-1) % uMaxLevel].displayCFD( );

	vector<CFD> foundCFDs;
	for(uint32_t i=0;i<uMaxLevel;++i)
	{
		vector<CFD> new_array = levels[i].FoundCFDs();
		foundCFDs.insert(foundCFDs.end(),new_array.begin(), new_array.end());
	}

    delete [] levels;

    LevelCFUN::FreeAlloc( );
    return foundCFDs;
}
