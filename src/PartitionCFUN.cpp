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


#include <cstring>

//#include "cfunException.h"
#include "PartitionCFUN.h"
#include "Trace.h"



unsigned int *PartitionCFUN::m_pEC                = NULL;
unsigned int *PartitionCFUN::m_pTmpListeElemRes   = NULL;

unsigned int *PartitionCFUN::m_pAssociation       = NULL;
unsigned int *PartitionCFUN::m_pTmpCountInClass   = NULL;
unsigned int  PartitionCFUN::m_uMinSup            = cst_uDefaultMinSup;

unsigned int  PartitionCFUN::m_uNbPartitionProduct = 0;
unsigned int  PartitionCFUN::m_uNbElemUsedPartitionProduct = 0;

unsigned int  PartitionCFUN::m_uNbTuple = 0;



static inline unsigned int min( const unsigned int a, const unsigned int b )
{
    return a < b ? a : b;
}

PartitionCFUN::PartitionCFUN( )
    : m_uNbElem( 0 ), m_pListeElem( NULL ), m_uNbPart( 0 ),
    m_Attr( ), m_uCount( 0 ),
	m_uNbSubset( 0 ), m_pListeSubset( NULL ),
	m_pAttrQuasiClosureCFD( NULL ), m_pAttrClosureCFD( NULL ),
	m_bFreeSet( true )
{
    init( m_uNbElem, m_uNbPart );
}

PartitionCFUN::PartitionCFUN( unsigned int uNbElem, unsigned int uNbPart )
	: m_uNbSubset( 0 ), m_pListeSubset( NULL ),
	m_pAttrQuasiClosureCFD( NULL ), m_pAttrClosureCFD( NULL ),
	m_bFreeSet( true )
{
    init( uNbElem, uNbPart );
}


PartitionCFUN::~PartitionCFUN()
{
    deleteListe( );

	if ( m_pListeSubset )
	{
		delete [] m_pListeSubset;
		m_pListeSubset = NULL;
	}

	if ( m_pAttrQuasiClosureCFD != NULL ) {
		delete [] m_pAttrQuasiClosureCFD;
		m_pAttrQuasiClosureCFD = NULL;
	}
	if ( m_pAttrClosureCFD != NULL ) {
		delete [] m_pAttrClosureCFD;
		m_pAttrClosureCFD = NULL;
	}
}

unsigned int *PartitionCFUN::GetTmpListeElemRes( )
{
    assert( m_pTmpListeElemRes != NULL );
    return m_pTmpListeElemRes;
}

void PartitionCFUN::AllocTmpListeElem( unsigned int nNbTuple )
{
		// On ne doit le faire qu'une fois...
	assert( m_pEC == NULL );

    // On fait +1 car les numeros d'enreg commence a 1 => evite
    //  de faire -1 dans les indexations
    unsigned int size = nNbTuple + 1;
    
	m_uNbTuple = nNbTuple;

    m_pEC = new unsigned int[size];
    ::memset( m_pEC, 0, sizeof ( unsigned int ) * size );

    m_pTmpListeElemRes = new unsigned int[size];

    m_pAssociation = new unsigned int[size];
    ::memset( m_pAssociation, 0, sizeof ( unsigned int ) * size );

    m_pTmpCountInClass = new unsigned int[size];
    ::memset( m_pTmpCountInClass, 0, sizeof ( unsigned int ) * size );

}

void PartitionCFUN::FreeTmpListeElem( )
{
    if ( m_pEC )
    {
        delete [] m_pEC;
        m_pEC = NULL;
    }

    if ( m_pTmpListeElemRes )
    {
        delete [] m_pTmpListeElemRes;
        m_pTmpListeElemRes = NULL;
    }

    if ( m_pAssociation )
    {
        delete [] m_pAssociation;
        m_pAssociation = NULL;
    }

    if ( m_pTmpCountInClass )
    {
        delete [] m_pTmpCountInClass;
        m_pTmpCountInClass = NULL;
    }

}

void PartitionCFUN::initEC( unsigned int *pEC )
{
    ::memset( pEC, 0, sizeof ( unsigned int ) * (m_uNbTuple+1) );
}

void PartitionCFUN::initCount( unsigned int uNbElem, unsigned int uNbPart )
{
    m_uNbElem = uNbElem;
    m_uNbPart = uNbPart;

    m_uCount  = m_uNbPart;
}

void PartitionCFUN::initMem( unsigned int uNbElem, unsigned int uNbPart )
{
    // Si l'init a deja ete fait au moins une fois
    if ( ! m_pListeElem )
        m_Attr.clear( );

    initCount( uNbElem, uNbPart );

    if ( m_uNbElem )
    {
        // Si l'init a deja ete fait au moins une fois
        if ( ! m_pListeElem ) {
            m_pListeElem  = new unsigned int[m_uNbElem];
        }
    }
}

void PartitionCFUN::initForPruningTechnics( )
{
    m_bFreeSet = true;
}

void PartitionCFUN::initSubset( unsigned int uNbSubset, PartitionCFUN **pListeSubset )
{
	if ( m_pListeSubset != NULL ) {
		delete [] m_pListeSubset;
		m_pListeSubset = NULL;
	}

	m_uNbSubset = uNbSubset;
    m_pListeSubset = new PartitionCFUN *[uNbSubset];
    ::memcpy( m_pListeSubset, pListeSubset, sizeof ( PartitionCFUN *) * uNbSubset );
}

void PartitionCFUN::init( unsigned int uNbElem, unsigned int uNbPart )
{
	initMem( uNbElem, uNbPart );

	initForPruningTechnics( );
}


void PartitionCFUN::init0( )
{
    init( m_uNbTuple, 1 );
    unsigned int i;
    for ( i = 0; i < m_uNbElem -1 ; ++i )
        m_pListeElem[i] = i+1;
    m_pListeElem[i] = (i+1) | endmark;
}

void PartitionCFUN::initQuasiAndClosureCFD( const Bitset &attr, unsigned int uNbPart )
{
	if ( uNbPart != 0 ) {
		// Vector of QClosure
		if ( m_pAttrQuasiClosureCFD != NULL ) {
			delete [] m_pAttrQuasiClosureCFD;
			m_pAttrQuasiClosureCFD = NULL;
		}
			// Vector of Closure
		if ( m_pAttrClosureCFD != NULL ) {
			delete [] m_pAttrClosureCFD;
			m_pAttrClosureCFD = NULL;
		}

        m_pAttrQuasiClosureCFD = new Bitset[m_uNbPart];
		m_pAttrClosureCFD = new Bitset[m_uNbPart];
		for ( unsigned int i = 0; i < uNbPart; ++i ) {
			m_pAttrQuasiClosureCFD[i] = attr;
			m_pAttrClosureCFD[i]      = attr;
		}
	}
}



void PartitionCFUN::deleteListe( )
{
    if ( m_pListeElem )
    {
        delete [] m_pListeElem;
        m_pListeElem = NULL;
    }

    m_uNbElem = 0;
    m_uNbPart = 0;
}

unsigned int PartitionCFUN::nextEC( unsigned int currentIndice ) const
{
    while ( ! IS_END_MARK( m_pListeElem[currentIndice++] ) ) {
    }

    return currentIndice;
}

const unsigned int *PartitionCFUN::nextEC( const unsigned int *pCurrentElem ) const
{
    while ( ! IS_END_MARK( *pCurrentElem++ ) ) {
    }

    return pCurrentElem;
}

void PartitionCFUN::toEC( unsigned int *pEC ) const
{
    ::memset( pEC, 0, sizeof ( unsigned int ) * (m_uNbTuple+1) );
    unsigned int *pL = m_pListeElem;
    for ( unsigned int i = 1; i <= m_uNbPart; ++i ) {
        while ( !(*pL & endmark) ) {
            pEC[*pL++] = i;
        }
        pEC[*pL++ & ~endmark] = i;
    }
}

void PartitionCFUN::product( const PartitionCFUN &Liste1, const PartitionCFUN &Liste2 )
{
    ++m_uNbPartitionProduct;
    m_uNbElemUsedPartitionProduct += Liste1.m_uNbElem + Liste2.m_uNbElem;

    unsigned int uNbElem = 0;
    unsigned int uNbPart = 0;

    if ( (Liste1.m_pListeElem != NULL) && (Liste2.m_pListeElem != NULL) &&
        (Liste1.m_uNbElem != 0) && (Liste2.m_uNbElem != 0) )
    {
        const unsigned int *pL1 = Liste1.m_pListeElem;
        // Calcul suivant le nouvel algo
        unsigned int uM    = 1;
        unsigned int uMini = uM;

        Liste2.toEC( m_pEC );
        ::memset( m_pTmpCountInClass, 0, sizeof ( unsigned int ) * (m_uNbTuple+1) );
        ::memset( m_pAssociation    , 0, sizeof ( unsigned int ) * (m_uNbTuple+1) );
        ::memset( m_pTmpListeElemRes, 0, sizeof ( unsigned int ) * (m_uNbTuple+1) );
        unsigned int i;
        for ( i = 1; i <= Liste1.m_uNbPart; i++ ) {
            uMini = uM;
            while ( !(*pL1 & endmark) ) {
                if ( m_pEC[*pL1] != 0 ) { // Cas ou l'elem est dans les 2 partitions sinon on saute
                    if ( m_pAssociation[m_pEC[*pL1]] < uMini ) {
                        m_pAssociation[m_pEC[*pL1]] = uM;
                        uM++;
                    }
                    m_pEC[*pL1] = m_pAssociation[m_pEC[*pL1]];
                    m_pTmpCountInClass[m_pEC[*pL1]]++;
                }

                ++pL1;
            }

            if ( m_pEC[*pL1 & ~endmark] != 0 ) { // Cas ou l'elem est dans les 2 partitions sinon on saute
                if ( m_pAssociation[m_pEC[*pL1 & ~endmark]] < uMini ) {
                    m_pAssociation[m_pEC[*pL1 & ~endmark]] = uM;
                    uM++;
                }
                m_pEC[*pL1 & ~endmark] = m_pAssociation[m_pEC[*pL1 & ~endmark]];
                m_pTmpCountInClass[m_pEC[*pL1 & ~endmark]]++;
            }

            ++pL1;
        }

        // Elagage du resultat avant la recopie
        for ( size_t j = 1; j < uM; ++j ) {
            // Regle de pruning
            if ( m_pTmpCountInClass[j] < m_uMinSup ) {
                m_pAssociation[j] = 0;
                m_pTmpCountInClass[j] = 0;
            } else {
                m_pAssociation[j] = uNbElem;
                uNbElem += m_pTmpCountInClass[j];

                uNbPart++;
            }
        }

        // Recopie du resultat
        pL1 = Liste1.m_pListeElem;
        for ( size_t i = 0; i < Liste1.m_uNbElem; ++i ) {
            unsigned int j = *pL1++ & ~endmark;
            unsigned int uNumClass = m_pEC[j];

            switch ( m_pTmpCountInClass[uNumClass] )
            {
            case 0:
                break;

            case 1:
                m_pTmpListeElemRes[m_pAssociation[uNumClass]] = j | endmark;
                m_pTmpCountInClass[uNumClass] = 0;
                break;

            default:
                m_pTmpListeElemRes[m_pAssociation[uNumClass]] = j;
                m_pAssociation[uNumClass]++;
                m_pTmpCountInClass[uNumClass]--;
                break;
            }
        }

    }

    init( uNbElem, uNbPart );
    if ( uNbElem != 0 )
        ::memcpy( m_pListeElem, m_pTmpListeElemRes, uNbElem * sizeof ( unsigned int) );

    m_Attr = Liste1.m_Attr | Liste2.m_Attr;
}


PartitionCFUN &PartitionCFUN::operator = ( const PartitionCFUN &lp )
{
	deleteListe( );
    init( lp.m_uNbElem, lp.m_uNbPart );
	if ( m_uNbElem != 0 ) {
		::memcpy( m_pListeElem, lp.m_pListeElem, lp.m_uNbElem * sizeof ( *lp.m_pListeElem ) );
	}
    m_Attr = lp.m_Attr;
    initQuasiAndClosureCFD( m_Attr, m_uNbPart );
	initSubset( lp.m_uNbSubset, lp.m_pListeSubset );

	return *this;
}


/* EOF */
