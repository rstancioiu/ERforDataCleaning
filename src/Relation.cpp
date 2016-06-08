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


#ifndef NDEBUG
#define TRACE_RELATION
#endif

#include <iostream>
//#include "cfunException.h"
#include "ReadTextFile.h"
#include "PartitionCFUN.h"
#include "LevelCFUN.h"
#include "Relation.h"

using namespace std;


//#define __USES_MAP__

#ifdef __USES_MAP__
	#include <map>
	typedef map < string, unsigned int > MAP_DOMAIN;
#else
	#if defined (_MSC_VER) && (_MSC_VER >= 1020)
		#include <hash_map>
		typedef hash_map < std::string, unsigned int > MAP_DOMAIN;
	#else
		//#include <unordered_map>
		//#include <hash_map>
		//typedef __gnu_cxx::hash_map < std::string, unsigned int > MAP_DOMAIN;
		#include <map>
		typedef map < string, unsigned int > MAP_DOMAIN;
	#endif
#endif


Relation::Relation( )
	:  _uNbCol( 0 ), _uNbTuples( 0 )
{
	_attrRelation.init();
}

bool Relation::loadRelation( const char * szFileName, char cDelim )
{
	_fileNameOfRelation = szFileName;

	vector < vector < unsigned int > > vvIndex;
	vector < vector < unsigned int > > vvHisto;

	if ( ! loadRelation( szFileName, cDelim, vvIndex, vvHisto ) ) {
		return false;
	}
	initAttrRelation( );

		// Allocation des buffers temporaire utilises
	PartitionCFUN::AllocTmpListeElem( getNbTuples() );


	buildPartitions( vvIndex, vvHisto );

	return true;
}

Relation::~Relation()
{
	PartitionCFUN::FreeTmpListeElem( );
}

void Relation::initAttrRelation( )
{
   _attrRelation.clear();
   for ( unsigned int col = 0; col < getNbCol(); ++col ) {
      _attrRelation.insert( col );
   }
}

bool Relation::loadRelation( const char * szFileName, char cDelim,
				   vector < vector < unsigned int > > &vvIndex,
				   vector < vector < unsigned int > > &vvHisto )
{
	ReadTextFile textFile( cDelim );

	if ( ! textFile.openTextFile( szFileName, _vsColName ) )
		return false;

	_uNbTuples = (unsigned int) textFile.getNbLine( );
	_uNbCol = (unsigned int) textFile.getNbCol( );

	vector < MAP_DOMAIN > vMapDomain;			// Correspondance ValeurDomaine Identifiant en map puis en le stocke en vecteurs
	vMapDomain.resize( getNbCol() );

	_vDomainName.resize( getNbCol() );

	vvHisto.resize( getNbCol() );
	vvIndex.resize( getNbCol() );

	for ( unsigned int col = 0; col < getNbCol(); ++col )
	{
			// pour [1..n] et non [0..n-1]
		vvIndex[col].push_back( 0 );
		vvHisto[col].push_back( 0 );
	}

	vector<string> vstring;
	while ( textFile.getline( vstring ) )
	{
		for ( size_t col = 0; col < getNbCol(); ++col )
		{
			pair<MAP_DOMAIN::iterator, bool> pairRes = vMapDomain[col].insert( make_pair( vstring[col], vMapDomain[col].size()+1 ) );
			if ( pairRes.second == true ) // Si nouvel element
			{
				vvIndex[col].push_back( (unsigned int) vMapDomain[col].size() );
				vvHisto[col].push_back( 1 );
			}
			else	// Sinon, elem deja present
			{
				unsigned int uVal = (static_cast<MAP_DOMAIN::iterator>(pairRes.first))->second;
				vvIndex[col].push_back( uVal );
				vvHisto[col][uVal]++;
			}
		}

		_vvsRelationInit.push_back( vstring );
	}

	for ( unsigned int col = 0; col < vMapDomain.size( ); ++col ) {
		_vDomainName[col].resize( vMapDomain[col].size() + 1 ); // +1 car on commence tjrs a 1 et non a 0
		for ( MAP_DOMAIN::const_iterator it = vMapDomain[col].begin(); it != vMapDomain[col].end(); ++it ) {
			_vDomainName[col][it->second] = it->first;
		}
	}


	return true;
}

void Relation::buildPartitions( vector < vector < unsigned int > > &vvIndex,
					  vector < vector < unsigned int > > &vvHisto )
{
    unsigned int *pListeElem = PartitionCFUN::GetTmpListeElemRes( );
	unsigned int *pIndexCopy = new unsigned int[getNbTuples() + 1]; // [0..nbTuples]

	_vPartitionsMonoDim.resize( getNbCol() );

	for ( size_t col = 0; col < getNbCol(); ++col )	{
		unsigned int uCumul = 0;
		unsigned int uNbPart = vvHisto[col].size() - 1;

		::memset( pIndexCopy, 0, (getNbTuples() + 1) * sizeof ( *pIndexCopy ) );
		for ( size_t j = 1; j < vvHisto[col].size(); ++j )
		{
			if ( vvHisto[col][j] < PartitionCFUN::m_uMinSup ) {
				vvHisto[col][j] = 0;
				--uNbPart;
			} else {
				pIndexCopy[j] = uCumul;
				uCumul += vvHisto[col][j];
			}
		}

			// Recopie du resultat
		unsigned int uNbTuples = 0;
		for ( size_t j = 1; j < vvIndex[col].size(); ++j ) {
			unsigned int uNumClass = vvIndex[col][j];

			switch ( vvHisto[col][uNumClass] )
			{
			case 0:
				break;

			case 1:
				pListeElem[pIndexCopy[uNumClass]] = (unsigned int) (j | endmark);
				vvHisto[col][uNumClass] = 0;
				++uNbTuples;
				break;

			default:
				pListeElem[pIndexCopy[uNumClass]] = (unsigned int) j;
				pIndexCopy[uNumClass]++;
				vvHisto[col][uNumClass]--;
				++uNbTuples;
				break;
			}
		}

		_vPartitionsMonoDim[col].init( uNbTuples, uNbPart );
		::memcpy( _vPartitionsMonoDim[col].m_pListeElem, pListeElem,
					uNbTuples * sizeof ( unsigned int ) );
		_vPartitionsMonoDim[col].m_Attr.insert( (unsigned int) col );
	}
	delete [] pIndexCopy;
}

void Relation::getInitVal( unsigned int uNumTuple, const Bitset &attr, string &sValue ) const
{
   sValue.clear();
   for ( unsigned int col = 0; col < getNbCol(); ++col ) {
      if ( attr.contain( col ) ) {
         sValue += '.' + getInitVal( uNumTuple, col ) + '.';
      }
   }
}
