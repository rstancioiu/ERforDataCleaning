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


#ifndef __RELATION_H__
#define __RELATION_H__


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "PartitionCFUN.h"
#include "Bitset.h"
#include "LevelCFUN.h"


using namespace std;



class Relation
{
public:
        Relation( );
        ~Relation( );

		bool loadRelation( const char * szFileName, char cDelim = '\0' );

		std::string getFileNameOfRelation( ) const {
                return _fileNameOfRelation;
        }
        Bitset getAttrRelation( ) const {
                return _attrRelation;
        }
        unsigned int getNbCol( )    const {
                return _uNbCol;
        }
        unsigned int getNbTuples( ) const {
                return _uNbTuples;
        }

        string getNameCol(int i) const {
            return _vsColName[i];
        }

		const PartitionCFUN *getPtrPartitionMonoDim( unsigned int i ) const {
                return &getPartitionsMonoDim( i );
        }

        const PartitionCFUN &getPartitionsMonoDim( unsigned int i ) const {
                return _vPartitionsMonoDim[i]; // Les partitions initiailes mono dimension
        }

        const string &getInitVal( unsigned int uNumTuple, unsigned int uNumDimension ) const {
                assert( uNumTuple < getNbTuples() );
                assert( uNumDimension < getNbCol() );

                return _vvsRelationInit[uNumTuple][uNumDimension];
        }
        
        void getInitVal( unsigned int uNumTuple, const Bitset &attr, std::string &sValue ) const;

        ostream &display( ostream &os = cout ) const;


private:
        ostream &displayDomain( ostream &os = cout ) const;
        ostream &displayPartitionsMonoCol( unsigned int uDim, ostream &os = cout ) const;
        ostream &displayColNames( ostream &os = cout ) const;
        ostream &displayColNames( const Bitset &bs, ostream &os = cout ) const;

        bool loadRelation( const char * szFileName, char cDelim,
                           vector < vector < unsigned int > > &vvIndex,
                           vector < vector < unsigned int > > &vvHisto );

		void stripHisto( vector < vector < unsigned int > > &vvHisto ) const;

        void buildPartitions( vector < vector < unsigned int > > &vvIndex,
                              vector < vector < unsigned int > > &vvHisto );

        void initAttrRelation( );
        ///////////////////////////////////////////////////////////////////////////

        std::string             _fileNameOfRelation; // Nom du fichier de la relation
        Bitset                  _attrRelation; // represente R de la relation
        unsigned int				_uNbCol;			// Nb de colonne de la relation
        unsigned int				_uNbTuples;				// Nb de ligne dans la raletion

        vector < vector < string > >_vvsRelationInit;		// La relation initiale
        vector < string >			_vsColName;				// Liste des noms de dimemsion
        vector < vector < string > >_vDomainName;			// Le vecteur des valeurs des domaines
        vector < PartitionCFUN >	_vPartitionsMonoDim;	// Les partitions initiailes mono dimension
};


#endif   // __RELATION_H__

