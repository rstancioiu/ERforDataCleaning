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


#ifndef __LEVELCFUN_H__
#define __LEVELCFUN_H__

#include <cassert>
#include <cstring>

#include "Bitset.h"
#include "PartitionCFUN.h"
#include "CFD.h"
//#include "cfunException.h"

// Forward declaration
class Relation;


class LevelCFUN
{
public:

	////////////////////////////////////
	// STATIC Properties

	static PartitionCFUN  **m_pTmpListeSubset;
	static unsigned int     m_uTmpNbSubset;


	static unsigned int **m_ppECFromMonoAttr;
	static unsigned int _nbSrcOfCFD;
	static unsigned int _nbCFD;
	static bool         _bStdOutput;

    static unsigned int m_uMinSupCFD;	// threshold valid CFDs (must be at least m_uMinSup)

public:
	////////////////////////////////////
	// STATIC methods

	static bool InitClass( unsigned int uNbAttr );
	static void FreeAlloc( );


private:
	////////////////////////////////////
	// PRIVATE Properties

	unsigned int m_uNbElemAlloc;

public:
	////////////////////////////////////
	// PUBLIC Properties

	unsigned int m_uNbAttr;
	PartitionCFUN *m_pListeAttr;
	vector<CFD> foundCFDs;

public:
	////////////////////////////////////
	// PUBLIC Methods

	LevelCFUN( ) : m_uNbElemAlloc( 0 ), m_uNbAttr( 0 ), m_pListeAttr( NULL )
	{
	}

	~LevelCFUN( )
	{
		DeleteListe( );
		foundCFDs.clear();

		if ( m_pListeAttr ) {
			::free( m_pListeAttr );
		}
	}

	bool Init( unsigned int uNbElemAlloc );
	void GetAllPartitionFromRelation( const Relation &relation );
	void GenerateNextLevel( const LevelCFUN &Partition, bool bPrune );
	void closure( const LevelCFUN &level, const Relation &relation );
	void quasiClosure( const LevelCFUN &level );
	void PurePrune( );
	void displayCFD( std::ostream &os = std::cout ) const;
	void displayDetails( std::ostream &os = std::cerr ) const;
	void displayStep( int nNumStep, std::ostream &os = std::cout ) const;
	vector<CFD> FoundCFDs();

private:
	////////////////////////////////////
	// PRIVATE Methods

	void markAllCEUsedForCFD( const PartitionCFUN &X, unsigned int n, unsigned int *pTmpStripEC );
	void updateClosureCFD( const PartitionCFUN &X, PartitionCFUN &XA,
                           unsigned int *pListeElemRes, unsigned int nbElem );
	CFD displayFoundCFD( const Relation &relation, const PartitionCFUN &X,
						unsigned int numCol, unsigned int nbElem ) const;
	void closureCFD( const LevelCFUN &level, PartitionCFUN &X, const Relation &relation );
	unsigned int computeClosureCFD( const PartitionCFUN &X,
		unsigned int a, unsigned int *pListeElemRes ) const;
	void updateQuasiClosureCFD( PartitionCFUN &X, const PartitionCFUN &S ) const;
	bool Add( );
	void SetSubset( unsigned int uIndex, unsigned int uNbSubset );
	int findAttr( const Bitset& attr ) const;
	void strippedPartitionCFD( PartitionCFUN &XA, unsigned int *pTmpStripEC );
	void DeleteListe( );

};



#endif // __LEVELCFUN_H__
