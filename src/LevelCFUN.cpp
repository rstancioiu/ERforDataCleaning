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


#include <cassert>
#include <iostream>
#include "LevelCFUN.h"
#include "Trace.h"
#include "Relation.h"


PartitionCFUN **LevelCFUN::m_pTmpListeSubset = NULL;
unsigned int LevelCFUN::m_uTmpNbSubset = (unsigned int) -1;

unsigned int **LevelCFUN::m_ppECFromMonoAttr = NULL;


// Stat
unsigned int LevelCFUN::_nbSrcOfCFD = 0;
unsigned int LevelCFUN::_nbCFD      = 0;
bool         LevelCFUN::_bStdOutput = false;

unsigned int LevelCFUN::m_uMinSupCFD = cst_uDefaultMinSupCFD;

////////////////////////////////////////////////////////////////////////
//
//                           C F U N


///////////////////////////////
// STATIC methods
bool LevelCFUN::InitClass( unsigned int uNbAttr )
{
	if ( m_pTmpListeSubset != NULL )
		return false;

	// Init du tampon memoire de taille max des subset d'un attribut
	m_uTmpNbSubset = uNbAttr;
    m_pTmpListeSubset = new PartitionCFUN *[m_uTmpNbSubset];
    ::memset( m_pTmpListeSubset, 0, sizeof ( *m_pTmpListeSubset ) * m_uTmpNbSubset );

	return true;
}

void LevelCFUN::FreeAlloc( )
{
	if ( m_pTmpListeSubset ) {
		delete [] m_pTmpListeSubset;
		m_pTmpListeSubset = NULL;
	}
}


///////////////////////////////
// PRIVATE methods

bool LevelCFUN::Add( )
{
	if ( m_uNbAttr == m_uNbElemAlloc )
	{
		m_uNbElemAlloc += 10;
		m_pListeAttr = (PartitionCFUN *) ::realloc( m_pListeAttr,
			m_uNbElemAlloc * sizeof ( *m_pListeAttr ) );
		if ( !m_pListeAttr )
			return false;

		::memset( m_pListeAttr + m_uNbAttr, 0, sizeof ( *m_pListeAttr ) * 10 );
	}

	return true;
}

int LevelCFUN::findAttr( const Bitset& attr ) const
{
    for ( unsigned int i = 0; i < m_uNbAttr; ++i ) {
        if ( attr == m_pListeAttr[i].m_Attr ) {
            return i;
        }
    }
    return -1;
}

void LevelCFUN::strippedPartitionCFD( PartitionCFUN &XA, unsigned int *pTmpStripEC )
{
    unsigned int lecElem = 0;
    unsigned int ecrElem = 0;
    unsigned int lecPart = 0;
    unsigned int ecrPart = 0;

    for ( unsigned int uPart = 0; uPart < XA.m_uNbPart; ++uPart ) {
        unsigned int currentElem = XA.m_pListeElem[lecElem] & ~endmark;
        if ( pTmpStripEC[currentElem] == 1 ) {
            while ( !IS_END_MARK( XA.m_pListeElem[lecElem] ) ) {
                ++lecElem;
            }
            ++lecElem;
            ++lecPart;
        } else {
            while ( !IS_END_MARK( XA.m_pListeElem[lecElem] ) ) {
                XA.m_pListeElem[ecrElem] =  XA.m_pListeElem[lecElem];
                ++ecrElem;
                ++lecElem;
            }
            XA.m_pListeElem[ecrElem] =  XA.m_pListeElem[lecElem];
            ++ecrElem;
            ++lecElem;
            XA.m_pAttrQuasiClosureCFD[ecrPart] = XA.m_pAttrQuasiClosureCFD[lecPart];
            ++lecPart;
            ++ecrPart;
        }
    }

    XA.initCount( ecrElem, ecrPart );
}

void LevelCFUN::markAllCEUsedForCFD( const PartitionCFUN &X, unsigned int n,
                                     unsigned int *pTmpStripEC )
{
    for ( unsigned int i = n; i < X.m_uNbElem; ++i ) {
        pTmpStripEC[X.m_pListeElem[i] & ~endmark] = 1;
        if ( IS_END_MARK( X.m_pListeElem[i] ) ) {
            break;
        }
    }
}

unsigned int LevelCFUN::computeClosureCFD( const PartitionCFUN &X,
    unsigned int numColTarget, unsigned int *pListeElemRes ) const
{
    if ( X.m_uNbElem == 0 ) {
        return 0;
    }

    const unsigned int * const pEC_TARGET = m_ppECFromMonoAttr[numColTarget];
    unsigned int nbElemRes = 0;
    const unsigned int *pLX = X.m_pListeElem;
    for ( unsigned int numPart = 0; numPart < X.m_uNbPart; ++numPart ) {
        if ( X.m_pAttrQuasiClosureCFD[numPart].contain( numColTarget ) ) {
            assert( X.m_pAttrClosureCFD[numPart].contain( numColTarget ) );
            pLX = X.nextEC( pLX );
            continue;
        }

        unsigned int currentElem = *pLX & ~endmark;
        unsigned int currentEC = pEC_TARGET[currentElem];
        const unsigned int * const pOldLX = pLX;
		bool cfdOK = true;

		do {
            currentElem = *pLX & ~endmark;
            if ( (pEC_TARGET[currentElem] == 0) || (pEC_TARGET[currentElem] != currentEC) ) {
				cfdOK = false;
                break;
            }
        } while ( !IS_END_MARK( *pLX++ ) );

        if ( cfdOK ) { // CFD discorered!
			unsigned int supportCFD = pLX - pOldLX;
            if ( supportCFD >= LevelCFUN::m_uMinSupCFD ) { // Support OK
                X.m_pAttrClosureCFD[numPart].insert( numColTarget );
				pListeElemRes[nbElemRes++] = pOldLX - X.m_pListeElem;
				LevelCFUN::_nbCFD++;
            }
        } else {
            pLX = X.nextEC( pLX );
        }
    }

    return nbElemRes;
}

CFD LevelCFUN::displayFoundCFD( const Relation &relation, const PartitionCFUN &X,
								unsigned int numCol, unsigned int nbElem ) const
{
    CFD cfd;
    vector<bool> ls = Bitset(X.m_Attr).tab;
    vector<bool> rs = Bitset(numCol).tab;
    cfd.left_side = ls;   
    cfd.right_side= rs;
	string sValueSrc;
	for ( unsigned int n = 0; n < nbElem; ++n ) {

        unsigned int uNumTuple = (X.m_pListeElem[PartitionCFUN::m_pTmpListeElemRes[n]] & ~endmark) -1;

        vector<string> ls;
        vector<string> rs;
        for ( unsigned int col = 0; col < relation.getNbCol(); ++col ) {
            if ( X.m_Attr.contain( col ) ) {
                ls.push_back(relation.getInitVal( uNumTuple, col ) );
            }
        }
        string str = relation.getInitVal( (X.m_pListeElem[PartitionCFUN::m_pTmpListeElemRes[n]] & ~endmark) -1, numCol );
        rs.push_back(str);

		relation.getInitVal( uNumTuple ,X.m_Attr, sValueSrc );

        cfd.pattern_left_side.push_back(ls);
        cfd.pattern_right_side.push_back(rs);        
	}

    return cfd;
}

void LevelCFUN::updateClosureCFD( const PartitionCFUN &X, PartitionCFUN &XA,
                                  unsigned int *pListeElemRes, unsigned int nbElem )
{
	PartitionCFUN::initEC( PartitionCFUN::m_pTmpCountInClass );
	for ( unsigned int n = 0; n < nbElem; ++n ) {
		markAllCEUsedForCFD( X, pListeElemRes[n],
								PartitionCFUN::m_pTmpCountInClass );
	}
    strippedPartitionCFD( XA, PartitionCFUN::m_pTmpCountInClass );
}

void LevelCFUN::closureCFD( const LevelCFUN &level, PartitionCFUN &X, const Relation &relation )
{
    bool cfdFound = false;
    Bitset attrUnion;

    Bitset attrTarget( relation.getAttrRelation() );

    if(X.m_Attr.tab.size()==0)
        X.m_Attr.init();

    attrTarget -= X.m_Attr;
    for ( unsigned int numCol = 0; numCol < relation.getNbCol(); numCol++ ) {
		if ( attrTarget.contain( numCol ) ) {
            attrUnion = X.m_Attr;
            attrUnion.insert( numCol );

			unsigned int nbElem = computeClosureCFD( X, numCol,
													PartitionCFUN::m_pTmpListeElemRes );

            if ( nbElem != 0 ) {
                cfdFound = true;
				int indiceAttr = level.findAttr( attrUnion );
				if ( indiceAttr >= 0 ) { // No stripped candidate
					updateClosureCFD( X, level.m_pListeAttr[indiceAttr],
                                      PartitionCFUN::m_pTmpListeElemRes, nbElem );
				}

				//if ( _bStdOutput ) {
					CFD foundCFD = displayFoundCFD( relation, X, numCol, nbElem );
                    foundCFDs.push_back(foundCFD);
				//}
            }
        }
    }

    if ( cfdFound ) {
        LevelCFUN::_nbSrcOfCFD++;
    }
}

void LevelCFUN::updateQuasiClosureCFD( PartitionCFUN &X, const PartitionCFUN &S ) const
{
    if ( (X.m_uNbElem == 0) || (X.m_uNbPart == 0) ) {
        return;
    }

    unsigned int *pEC_X = PartitionCFUN::m_pEC;
	X.toEC( pEC_X );

    const unsigned int *pLS = S.m_pListeElem;
    unsigned int uCurrentPart = 1;
    for ( unsigned int i = 0; i < S.m_uNbElem; ++i ) {
        unsigned int currentElem = *pLS & ~endmark;
        unsigned int currentEC = pEC_X[currentElem];

        if ( currentEC != 0 ) {
            X.m_pAttrQuasiClosureCFD[currentEC-1] |= S.m_pAttrClosureCFD[uCurrentPart-1];
        }
        if ( IS_END_MARK( *pLS ) ) {
            ++uCurrentPart;
        }
        ++pLS;
    }

    PartitionCFUN::initEC( PartitionCFUN::m_pEC );
}

void LevelCFUN::DeleteListe( )
{
	for ( unsigned int i = 0; i < m_uNbAttr; i++ )
		m_pListeAttr[i].deleteListe( );

	m_uNbAttr = 0;
}

///////////////////////////////
// PUBLIC methods

bool LevelCFUN::Init( unsigned int uNbElemAlloc )
{
	if ( m_pListeAttr ) {
		DeleteListe( );
		if ( uNbElemAlloc <= m_uNbElemAlloc ) {
			::memset( m_pListeAttr, 0, sizeof ( *m_pListeAttr ) * m_uNbElemAlloc );
			return true;
		}
		::free( m_pListeAttr );
	}

	m_uNbElemAlloc = uNbElemAlloc;
	m_pListeAttr = (PartitionCFUN *) ::malloc(
		sizeof ( *m_pListeAttr ) * m_uNbElemAlloc );
	if ( !m_pListeAttr )
		return false;

	::memset( m_pListeAttr, 0, sizeof ( *m_pListeAttr ) * m_uNbElemAlloc );

	return true;
}

void LevelCFUN::GetAllPartitionFromRelation( const Relation &relation )
{
    assert( m_uNbAttr == relation.getNbCol() );

    for ( unsigned int i = 0; i < m_uNbAttr; ++i ) {
        m_pListeAttr[i] = relation.getPartitionsMonoDim(i);
    }

    if ( m_ppECFromMonoAttr != NULL ) {
        for ( unsigned int i = 0; i < m_uNbAttr; ++i ) {
            if ( m_ppECFromMonoAttr[i] != NULL ) {
                delete m_ppECFromMonoAttr[i];
                m_ppECFromMonoAttr[i] = NULL;
            }
        }
        delete [] m_ppECFromMonoAttr;
        m_ppECFromMonoAttr = NULL;
    }
    m_ppECFromMonoAttr = new unsigned int *[m_uNbAttr];
    for ( unsigned int i = 0; i < m_uNbAttr; ++i ) {
        m_ppECFromMonoAttr[i] = new unsigned int[relation.getNbTuples() + 1];
    }
    for ( unsigned int i = 0; i < m_uNbAttr; ++i ) {
		m_pListeAttr[i].toEC( m_ppECFromMonoAttr[i] );
    }
}

void LevelCFUN::GenerateNextLevel( const LevelCFUN &Partition, bool bPrune )
{
    Bitset attrUnion;

    m_uNbAttr = 0;

    for ( unsigned int i = 0; i < Partition.m_uNbAttr - 1; i++ ) {
        if ( bPrune && !Partition.m_pListeAttr[i].m_bFreeSet ) {
            continue;
        }

        for ( unsigned int j = i + 1; j < Partition.m_uNbAttr; j++ ) {
            if ( bPrune && !Partition.m_pListeAttr[j].m_bFreeSet ) {
                continue;
            }

			if ( !Partition.m_pListeAttr[i].m_Attr.has_same_begin(
                Partition.m_pListeAttr[j].m_Attr ) )
                break;

            attrUnion = Partition.m_pListeAttr[i].m_Attr
                | Partition.m_pListeAttr[j].m_Attr;

            unsigned int uMaxNbSubset = attrUnion.size( );
            unsigned int uNbSubset = 0;

            for ( unsigned int s = 0; ( (s < Partition.m_uNbAttr) && (uNbSubset < uMaxNbSubset) ); s++ ) {
                if ( Partition.m_pListeAttr[s].m_bFreeSet && 
                    (Partition.m_pListeAttr[s].m_Attr <= attrUnion) ) {
                        m_pTmpListeSubset[uNbSubset] = &Partition.m_pListeAttr[s];
                        ++uNbSubset;
                }
            }
            if ( uNbSubset != uMaxNbSubset ) {
                continue;
            }

            if ( ! Add( ) ) {
				cerr << "Erreur de reallocation CLevel.m_pListeAttr" << endl;
				exit( -3 );
			}

			m_pListeAttr[m_uNbAttr].initSubset( uNbSubset, m_pTmpListeSubset );
            m_pListeAttr[m_uNbAttr].product( Partition.m_pListeAttr[i], Partition.m_pListeAttr[j] );
            m_pListeAttr[m_uNbAttr].initQuasiAndClosureCFD( m_pListeAttr[m_uNbAttr].m_Attr, m_pListeAttr[m_uNbAttr].m_uNbPart );

            m_uNbAttr++;
        }
    }

}   // GenerateNextLevel( )

void LevelCFUN::closure( const LevelCFUN &level, const Relation &relation )
{
    for ( unsigned int i = 0; i < m_uNbAttr; i++ ) {
        if ( ! m_pListeAttr[i].m_bFreeSet ) {
            continue;
        }

        PartitionCFUN &X = m_pListeAttr[i];
        for ( unsigned int uPart = 0; uPart < X.m_uNbPart; ++uPart ) {
            X.m_pAttrClosureCFD[uPart] = X.m_pAttrQuasiClosureCFD[uPart];
        }
		closureCFD( level, X, relation );
	}
}   // Closure( )

void LevelCFUN::quasiClosure( const LevelCFUN & )
{ // level is not used, I use subsets of each elem
    for ( unsigned int i = 0; i < m_uNbAttr; i++ ) {
        PartitionCFUN &X = m_pListeAttr[i];
        for ( unsigned int s = 0; s < X.m_uNbSubset; s++ ) {
            updateQuasiClosureCFD( X, *m_pListeAttr[i].m_pListeSubset[s] );
        }
    }
}   // QuasiClosure( )

void LevelCFUN::PurePrune( )
{
    for ( unsigned int i = 0; i < m_uNbAttr; i++ ) {
        //m_pListeAttr[i].m_bFreeSet = (m_pListeAttr[i].m_uNbElem != 0);
        m_pListeAttr[i].m_bFreeSet = (m_pListeAttr[i].m_uNbElem >= LevelCFUN::m_uMinSupCFD);
    }
}   // PurePrune( )

vector<CFD> LevelCFUN::FoundCFDs(){
    return foundCFDs;
}

void LevelCFUN::displayCFD( ostream &os ) const
{
    Bitset attrDif;

    for ( unsigned int a = 0; a < m_uNbAttr; a++ ) {
        PartitionCFUN &X = m_pListeAttr[a];
        unsigned int nbCFDFromAttr = 0;
        for ( unsigned int i = 0; i < X.m_uNbPart; ++i ) {
            attrDif = X.m_pAttrClosureCFD[i];
            attrDif -= X.m_pAttrQuasiClosureCFD[i];
            if ( ! attrDif.empty( ) ) {
                ++nbCFDFromAttr;
            }
        }
    }

}   // displayCDF( )

