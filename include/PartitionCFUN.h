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


#if !defined __PARTITIONCFUN_H__
#define __PARTITIONCFUN_H__

#include <cassert>
#include <cstring>
#include <iostream>

#include "Bitset.h"
#include "Trace.h"


const unsigned int cst_uDefaultMinSup    = 1;
const unsigned int cst_uDefaultMinSupCFD = 1;


const unsigned int endmark = (1U << 31);
#define SET_END_MARK( x )    ((x) | endmark)
#define IS_END_MARK( x )	 ((x) & endmark)


class PartitionCFUN
{
public:
	////////////////////////////////////
	// STATIC Properties

		// Buffers
    static unsigned int *m_pEC;
    static unsigned int *m_pTmpListeElemRes;

    static unsigned int *m_pAssociation;
    static unsigned int *m_pTmpCountInClass;

		// Context
	static unsigned int m_uMinSup;		// threshold stripped partition
    static unsigned int m_uNbTuple;		// In partition

		// Stat
    static unsigned int m_uNbPartitionProduct;
    static unsigned int m_uNbElemUsedPartitionProduct;


public:
	////////////////////////////////////
	// STATIC methods

    static unsigned int *GetTmpListeElemRes( );

    static void AllocTmpListeElem( unsigned int nNbTuple );

    static void FreeTmpListeElem( );
    
    static void initEC( unsigned int *pEC );


public:
	////////////////////////////////////
	// PUBLIC Properties

		// Liste des elements (toutes EC confondues)
    unsigned int   m_uNbElem;
    unsigned int * m_pListeElem;

    unsigned int   m_uNbPart;

    Bitset         m_Attr;
    unsigned int   m_uCount;


	////////////////////////////////////
	//
	unsigned int         m_uNbSubset;
	PartitionCFUN **m_pListeSubset;

	Bitset         *m_pAttrQuasiClosureCFD;
	Bitset         *m_pAttrClosureCFD;

	bool            m_bFreeSet;

public:
	////////////////////////////////////
	// PUBLIC methods

	PartitionCFUN( );

	PartitionCFUN( unsigned int uNbElem, unsigned int uNbPart = 0 );

	~PartitionCFUN();

    unsigned int nextEC( unsigned int currentIndice ) const;
    const unsigned int *nextEC( const unsigned int *pCurrentElem ) const;
    void initCount( unsigned int uNbElem, unsigned int uNbPart );
    void init( unsigned int uNbElem, unsigned int uNbPart );
    void init0( );

    void deleteListe( );

    void toEC( unsigned int *pEC ) const;
    void product( const PartitionCFUN &Liste1, const PartitionCFUN &Liste2 );

    std::ostream &display( std::ostream &os = std::cerr ) const;

	void initQuasiAndClosureCFD( const Bitset &attr, unsigned int uNbPart );
    void initForPruningTechnics( );
	void initSubset( unsigned int uNbSubset, PartitionCFUN **pListeSubset );

	PartitionCFUN &operator = ( const PartitionCFUN &lp );

private:
    void initMem( unsigned int uNbElem, unsigned int uNbPart );
};


#endif // !defined __PARTITIONCFUN_H__

