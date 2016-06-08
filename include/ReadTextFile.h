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


#ifndef __READTEXTFILE_H__
#define __READTEXTFILE_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class ReadTextFile
{
private:
	char _cDelim;
    size_t _uNbCol;
    size_t _uNbLine;
    ifstream _ifs;

    void setDelim( const char cDelim )   { _cDelim = cDelim; }
    char getDelim( )				     { return _cDelim; }
    void setNbCol( const size_t nNbCol ) { _uNbCol = nNbCol; }

    void computeNbLine( const char *szFileName );
	bool emptyLine( const string &str );

    void splitline( const string &str, char cDelim, vector<string> &vstring ) const;
    bool readline( string &str );

public:
	ReadTextFile( char cDelim = '\0' );
	~ReadTextFile();

    size_t getNbLine( )				   { return _uNbLine; }
    size_t getNbCol( )				   { return _uNbCol; }

    bool openTextFile( const char* szFileName, vector<string> &vstring );
    void closeTextFile( );

    bool getline( vector<string> &vstring, bool strict = true );
};

#endif // __READTEXTFILE_H__
