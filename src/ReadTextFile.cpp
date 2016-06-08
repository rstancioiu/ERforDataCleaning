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


#include <string>
#include "ReadTextFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReadTextFile::ReadTextFile( char cDelim  )
    : _cDelim( cDelim ), _uNbCol( 0 ), _uNbLine( 0 )
{
}

ReadTextFile::~ReadTextFile()
{
    closeTextFile( );
}

inline bool ReadTextFile::emptyLine( const string &str )
{
    for ( string::const_iterator it = str.begin(); it != str.end(); ++it )
        if ( *it >= ' ' )
            return false;

    return true;
}


inline void ReadTextFile::splitline( const string &str, char cDelim, vector<string> &vstring ) const
{
    vstring.clear();
        // StringTokenizer...
    for ( size_t posBegin = 0, posEnd = 0; posEnd != string::npos; posBegin = posEnd+1 )
    {
        posEnd = str.find_first_of( cDelim, posBegin );
        while ( str[posBegin] == ' ' ) {
            ++posBegin;
        }

        size_t posEnd2 = posEnd == string::npos ? str.length()-1 : posEnd-1;

        while ( posEnd2 > posBegin && str[posEnd2] == ' ' ) {
            --posEnd2;
        }
        vstring.push_back( str.substr( posBegin, posEnd2-posBegin+1 ) );
    }
}

inline void ReadTextFile::computeNbLine( const char *szFileName )
{
    _uNbLine = 0;

    string str;
    ifstream ifs;
    ifs.open( szFileName );
    if ( !_ifs.good( ) ) {
        return;
    }
    while ( std::getline( ifs, str ) != NULL ) {
        if ( !emptyLine( str ) ) {
            ++_uNbLine;
        }
    }
    
    if ( _uNbLine > 0 ) {
        --_uNbLine; // on ne compte pas la ligne des noms de colonnes
    }
}

inline bool ReadTextFile::readline( string &str )
{
    str.clear();

    while ( true ) {
        if ( !std::getline( _ifs, str ) ) {
            return false;
        }

        size_t lenInit = str.length();
        size_t lenCur = lenInit;
        while ( lenCur > 0 && (str[lenCur-1] == '\n'  ||
                                str[lenCur-1] == '\r' || 
                                str[lenCur-1] == ' ') ) {
            --lenCur;
        }
        if ( lenInit > lenCur )
            str = str.substr( 0, lenCur );

        if ( !emptyLine( str ) ) {
            return true;
        }
    }

    return false;
}


bool ReadTextFile::openTextFile( const char *szFileName, vector<string> &vstring )
{
        // Au cas ou le fichier serait ouvert
    closeTextFile( );

    _ifs.open( szFileName );
    if ( !_ifs.good( ) ) {
        return false;
    }

    // Le delim est connu
    if ( '\0' != getDelim() ) {
        // lecture pas strick la premiere fois, on ne sait pas encore combien de colonnes on a...
        getline( vstring, false );
    } else {
            // On recherche le delimiteur
        string line;
        if ( readline( line ) ) {
            char cDelimTest[] = { ',', ';', '\t' };
            unsigned int delimTested = 0;
            unsigned int bestScore = 0;
            unsigned int bestScoreDelimTested = 0;

            for ( delimTested = 0; delimTested < sizeof cDelimTest; ++delimTested ) {
                splitline( line, cDelimTest[delimTested], vstring );
                if ( vstring.size() > bestScore ) {
                    bestScore = (unsigned int) vstring.size( );
                    bestScoreDelimTested = delimTested;
                }
            }
            if ( 0 == bestScore ) {
                return false;
            }

            setDelim( cDelimTest[bestScoreDelimTested] );
            splitline( line, cDelimTest[bestScoreDelimTested], vstring );
        }
    }

    computeNbLine( szFileName );
    setNbCol( vstring.size( ) );

    return true;
}

void ReadTextFile::closeTextFile()
{
    _uNbLine = 0;

    if ( _ifs.is_open( ) )
        _ifs.close( );
}

bool ReadTextFile::getline( vector<string> &vstring, bool strick /*= true*/ )
{
    string str;

    if ( !readline( str ) ) {
        return false;
    }
    splitline( str, getDelim(), vstring );

    return !strick || (vstring.size() == getNbCol( ) );
}
